#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Effect.hlsl"

/* For ParticleSystem */

// STREAM-OUT TECH
#define PT_EMITTER 0
#define PT_PARTICLE 1

PARTICLE_IN VS_MAIN_STREAM_SMOKE(PARTICLE_IN In)
{
    float4 vRight = WorldMatrix._11_12_13_14;
    float4 vUp = WorldMatrix._21_22_23_24;
    
    In.vSize.x = length(vRight);
    In.vSize.y = length(vUp);
    
    return In;
}

[maxvertexcount(6)]
void GS_STREAM_SMOKE(point PARTICLE_IN In[1], inout PointStream<PARTICLE_IN> OutStream)
{
    In[0].fAge += fTimeStep;

    if (In[0].iType == PT_EMITTER)
    {
		// time to emit a new particle?
        if (In[0].fAge > fEmitTerm)
        {
            // Spread rain drops out above the camera.
            float3 vRandom = RandUnitVec3(0.0f);
            vRandom *= 0.8f;
            
            PARTICLE_IN p;

            p.vPosition = vEmitPosition;
            //p.vPosition += vRandom;
            p.vVelocity = (vEmitDirection + (vRandom * vRandomMul)) * fSpreadSpeed;
            p.vSize = In[0].vSize;
            //p.vSize = float2(1.f, 1.f);
            p.fAge = 0.0f;
            p.iType = PT_PARTICLE;

            OutStream.Append(p);
			// reset the time to emit
            In[0].fAge = 0.0f;
        }

		// always keep emitters
        OutStream.Append(In[0]);
    }
    else
    {
		// Specify conditions to keep particle; this may vary from system to system.
        if (In[0].fAge <= fParticleLifeTime)
            OutStream.Append(In[0]);
    }
}

technique11 StreamOutTech
{
    pass Default // 0
    {
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_STREAM_SMOKE();
        GeometryShader = ConstructGSWithSO(CompileShader(gs_5_0, GS_STREAM_SMOKE()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = NULL;
    }
}

/* For ParticleSystem */
float3 g_Gravity = { 0.0f, -9.81f, 0.0f };

float2 g_QuadTexC[4] =
{
    float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f)
};

// STREAM-OUT TECH
#define PT_EMITTER 0
#define PT_PARTICLE 1

struct VS_OUT_SMOKE
{
    float3 vPosition : POSITION;
    float2 vSize : SIZE;
    float4 vColor : COLOR;
    uint iType : TYPE;
};

struct GS_OUT_SMOKE
{
    float4 vPosition : SV_POSITION;
    float4 vColor : COLOR;
    float2 vTexcoord : TEXCOORD;
};

VS_OUT_SMOKE VS_MAIN_DRAW_SMOKE(PARTICLE_IN In)
{
    VS_OUT_SMOKE Out = (VS_OUT_SMOKE) 0;

    float t = In.fAge;

    //constant acceleration equation
    //Out.vPosition = 0.5f * t * t * g_WaveSplashAcc + t * In.vVelocity + In.vPosition;
    Out.vPosition = t * In.vVelocity + In.vPosition;

    float opacity = 1.f - smoothstep(0.0f, 1.0f, 1.f * t);
    Out.vColor = float4(1.0f, 1.0f, 1.0f, opacity);
    
    Out.vSize = In.vSize * (1.f + t);
    Out.iType = In.iType;

    return Out;
}

[maxvertexcount(4)]
void GS_DRAW_SMOKE(point VS_OUT_SMOKE In[1], inout TriangleStream<GS_OUT_SMOKE> OutStream)
{
	// do not draw emitter particles.
    if (In[0].iType != PT_EMITTER)
    {
		//
		// Compute world matrix so that billboard faces the camera.
		//
        float3 vLook = float3(0.f, 0.f, 0.f);
        float3 vRight = float3(0.f, 0.f, 0.f);
        float3 vUp = float3(0.f, 0.f, 0.f);
        if (bBillboard)
        {
            float3 vCamPos = CameraPosition();
            vLook = normalize(vCamPos - In[0].vPosition);
            vRight = normalize(cross(float3(0, 1, 0), vLook));
            vUp = cross(vLook, vRight);
            //vRight *= length(WorldMatrix._11_12_13);
            //vUp *= length(WorldMatrix._21_22_23);
        }
        else
        {
            vRight = WorldMatrix._11_12_13;
            vUp = WorldMatrix._21_22_23;
        }
		//
		// Compute triangle strip vertices (quad) in world space.
		//
        float fHalfWidth = 0.5f * In[0].vSize.x;
        float fHalfHeight = 0.5f * In[0].vSize.y;

        float4 v[4];
        v[0] = float4(In[0].vPosition + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
        v[1] = float4(In[0].vPosition + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
        v[2] = float4(In[0].vPosition - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
        v[3] = float4(In[0].vPosition - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);

		//
		// Transform quad vertices to world space and output
		// them as a triangle strip.
		//
        GS_OUT_SMOKE Out;
		[unroll]
        for (int i = 0; i < 4; ++i)
        {
            Out.vPosition = mul(v[i], ViewProj);
            Out.vTexcoord = g_QuadTexC[i];
            Out.vColor = In[0].vColor;
            OutStream.Append(Out);
        }
    }
}

PS_OUT_EFFECT PS_DRAW_FXPARTICLE(GS_OUT_SMOKE In, uniform bool bOneBlend)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    float2 vNewUV = float2(0.f, 0.f);
    
    if (!bUV_Wave)
        vNewUV = (In.vTexcoord + vUV_TileIndex) / vUV_TileCount + vUV_Offset;
    else
        vNewUV = ((((In.vTexcoord + vUV_TileIndex) / vUV_TileCount - 0.5f) * 2.f * (1.f + vUV_Offset)) * 0.5f + 0.5f) * fUV_WaveSpeed;
    
    float4 vColor = CalculateEffectColor(vNewUV);
    
    if (bOneBlend)
        Out.vOneBlend = vColor;
    else
        Out.vAlphaBlend = vColor;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        Out.vEmissive = vColor * fIntensity_Bloom;
    }
    
    return Out;
}

technique11 DrawTech
{
    pass OneBlend // 0
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN_DRAW_SMOKE();
        GeometryShader = compile gs_5_0 GS_DRAW_SMOKE();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DRAW_FXPARTICLE(true);
        ComputeShader = NULL;
    }

    pass AlphaBlend // 1
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN_DRAW_SMOKE();
        GeometryShader = compile gs_5_0 GS_DRAW_SMOKE();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DRAW_FXPARTICLE(false);
        ComputeShader = NULL;
    }
}