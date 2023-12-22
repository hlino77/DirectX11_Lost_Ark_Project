#include "Client_Shader_Defines.hlsl"
//#include "Shader_Lighting_Functions.hlsl"

#define MAX_INSTANCE 500

/* 상수테이블. */
//matrix g_ViewMatrix, g_ProjMatrix;
//matrix g_WorldMatrix;

matrix g_ViewProj;
Texture2D g_Texture;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightEmissive = vector(1.f, 1.f, 1.f, 1.f);

vector g_vMtrlEmissive = vector(1.f, 1.f, 0.1f, 1.f);

vector g_vCamPosition;

/* For ParticleSystem */
float3 g_Gravity = { 0.0f, -9.81f, 0.0f };

float3 g_WaveSplashAcc = { 0.0f, -0.5f, 0.0f };

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

struct VS_OUT
{
    float3  vPosition   : POSITION;
    float3  vVelocity   : VELOCITY;
    uint    iType       : TYPE;
};
    
struct VS_WAVE_OUT
{
    float3  vPosition   : POSITION;
    float2  vSize       : SIZE;
    float4  vColor      : COLOR;
    uint    iType       : TYPE;
};

struct VS_FIRE_OUT
{
    float3  vPosition    : POSITION;
    float2  vSize        : SIZE;
    float4  vColor       : COLOR;
    uint    iType        : TYPE;
    uint2   vIndex       : INDEX;
};

struct GS_PARTICLE
{
    float3  vPosition   : POSITION;
    float3  vVelocity   : VELOCITY;
    float2  vSize       : SIZE;
    float   fAge        : AGE;
    uint    iType       : TYPE;
};

VS_OUT VS_RAIN_MAIN(GS_PARTICLE In)
{
    VS_OUT Out = (VS_OUT) 0;

    float t = In.fAge;

	// constant acceleration equation
    Out.vPosition = 0.5f * t * t * g_Gravity + t * In.vVelocity + In.vPosition;

    Out.iType = In.iType;

    return Out;
}

VS_WAVE_OUT VS_WAVE_MAIN(GS_PARTICLE In)
{
    VS_WAVE_OUT Out = (VS_WAVE_OUT) 0;

    float t = In.fAge;

   // constant acceleration equation
    Out.vPosition = 0.5f * t * t * g_WaveSplashAcc + t * In.vVelocity + In.vPosition;

    float opacity = 1.0f - smoothstep(0.0f, 1.0f, t * 0.15f);
    Out.vColor = float4(1.0f, 1.0f, 1.0f, opacity);
    
    Out.vSize = In.vSize;
    Out.iType = In.iType;

    return Out;
}

struct GS_RAIN_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD;
};
    
struct GS_WAVE_OUT
{
    float4 vPosition: SV_POSITION;
    float4 vColor   : COLOR;
    float2 vTexcoord: TEXCOORD;
};

// The draw GS just expands points into lines.
[maxvertexcount(2)]
void GS_RAIN_MAIN(point VS_OUT In[1],
	inout LineStream<GS_RAIN_OUT> OutStream)
{
	// do not draw emitter particles.
    if (In[0].iType != PT_EMITTER)
    {        
		// Slant line in acceleration direction.
        float3 p0 = In[0].vPosition;
        float3 p1 = In[0].vPosition + 0.15f * g_Gravity + In[0].vVelocity;

        GS_RAIN_OUT Out0 = (GS_RAIN_OUT) 0;
        Out0.vPosition = mul(float4(p0, 1.0f), g_ViewProj);
        Out0.vTexcoord = float2(0.0f, 0.0f);
        OutStream.Append(Out0);

        GS_RAIN_OUT Out1 = (GS_RAIN_OUT) 0;
        Out1.vPosition = mul(float4(p1, 1.0f), g_ViewProj);
        Out1.vTexcoord = float2(1.0f, 1.0f);
        OutStream.Append(Out1);
    }
}

[maxvertexcount(4)]
void GS_WAVE_MAIN(point VS_WAVE_OUT In[1],
	inout TriangleStream<GS_WAVE_OUT> OutStream)
{
	// do not draw emitter particles.
    if (In[0].iType != PT_EMITTER)
    {        
		//
		// Compute world matrix so that billboard faces the camera.
		//
        float3 vLook = normalize(g_vCamPosition.xyz - In[0].vPosition);
        float3 vRight = normalize(cross(float3(0, 1, 0), vLook));
        float3 vUp = cross(vLook, vRight);

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
        GS_WAVE_OUT Out;
		[unroll]
        for (int i = 0; i < 4; ++i)
        {
            Out.vPosition = mul(v[i], g_ViewProj);
            Out.vTexcoord = g_QuadTexC[i];
            Out.vColor = In[0].vColor;
            OutStream.Append(Out);
        }
    }
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vDistortion : SV_TARGET1;
};

PS_OUT PS_RAIN_MAIN(GS_RAIN_OUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.a = 0.5f;
    
    return Out;
}

PS_OUT PS_WAVE_MAIN(GS_WAVE_OUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord) * In.vColor;
    if (Out.vColor.r < 0.01f)
        discard;
    
    Out.vColor.rgb *= 1.5f;
    Out.vDistortion = Out.vColor * 0.005f;
    
    return Out;
}

VS_FIRE_OUT VS_FIRETRAIL_MAIN(GS_PARTICLE In)
{
    VS_FIRE_OUT Out = (VS_FIRE_OUT) 0;

    float t = In.fAge;

    //constant acceleration equation
    //Out.vPosition = 0.5f * t * t * g_WaveSplashAcc + t * In.vVelocity + In.vPosition;
    Out.vPosition = t * In.vVelocity + In.vPosition;

    float opacity = 1.0f - smoothstep(0.0f, 1.0f, t * 0.15f);
    Out.vColor = float4(1.0f, 1.0f, 1.0f, opacity);
    
    Out.vSize = In.vSize;
    Out.iType = In.iType;
    Out.vIndex = uint2((uint)In.vPosition.x % 4, (uint)In.vPosition.z % 4);

    return Out;
}

[maxvertexcount(4)]
void GS_FIRETRAIL_MAIN(point VS_FIRE_OUT In[1],
	inout TriangleStream<GS_WAVE_OUT> OutStream)
{
	// do not draw emitter particles.
    if (In[0].iType != PT_EMITTER)
    {
		//
		// Compute world matrix so that billboard faces the camera.
		//
        float3 vLook = normalize(g_vCamPosition.xyz - In[0].vPosition);
        float3 vRight = normalize(cross(float3(0, 1, 0), vLook));
        float3 vUp = cross(vLook, vRight);

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
        GS_WAVE_OUT Out;
		[unroll]
        for (int i = 0; i < 4; ++i)
        {
            Out.vPosition = mul(v[i], g_ViewProj);
            Out.vTexcoord = g_QuadTexC[i];
            Out.vColor = In[0].vColor;
            OutStream.Append(Out);
        }
    }
}

Texture2D g_MaskTexture;

PS_OUT PS_FIRETRAIL_MAIN(GS_WAVE_OUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
    if (Out.vColor.r == 0.f)
        discard;
    
    Out.vColor *= (g_Texture.Sample(LinearSampler, In.vTexcoord) * In.vColor);
    
    //Out.vDistortion = Out.vColor * 0.005f;
    
    return Out;
}

technique11 DrawTech
{
    pass RAINDROP
    {
        SetRasterizerState(RS_Default);
        //SetDepthStencilState(DSS_NoDepthWrite, 0);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_RAIN_MAIN();
        GeometryShader = compile gs_5_0 GS_RAIN_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RAIN_MAIN();
        ComputeShader = NULL;
    }

    pass WAVESPLASH
    {
        SetRasterizerState(RS_Default);
        //SetDepthStencilState(DSS_NoDepthWrite, 0);
        SetDepthStencilState(DSS_Default, 0); 
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_WAVE_MAIN();
        GeometryShader = compile gs_5_0 GS_WAVE_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_WAVE_MAIN();
        ComputeShader = NULL;
    }

    pass WAVEFALL
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_WAVE_MAIN();
        GeometryShader = compile gs_5_0 GS_WAVE_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_WAVE_MAIN();
        ComputeShader = NULL;
    }

    pass FIRETRAIL
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0); 
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_FIRETRAIL_MAIN();
        GeometryShader = compile gs_5_0 GS_FIRETRAIL_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_FIRETRAIL_MAIN();
        ComputeShader = NULL;
    }
}



