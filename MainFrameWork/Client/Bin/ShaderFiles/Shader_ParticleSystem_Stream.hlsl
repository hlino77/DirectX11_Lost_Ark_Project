#include "Client_Shader_Defines.hlsl"
//#include "Shader_Lighting_Functions.hlsl"

#define MAX_INSTANCE 500

matrix g_WorldMatrix;

/* 상수테이블. */
Texture2D		g_Texture;

vector g_vCamPosition;

/* For ParticleSystem */
float3 g_vEmitPosition;
float3 g_vEmitDirection;

float g_fGameTime;
float g_fTimeStep;

Texture1D g_RandomTexture;

// HELPER FUNCTIONS
float3 RandUnitVec3(float fOffset)
{
	// Use game time plus offset to sample random texture.
    float u = (g_fGameTime + fOffset);

	// coordinates in [-1,1]
    float3 v = g_RandomTexture.SampleLevel(LinearSampler, u, 0).xyz;

	// project onto unit sphere
    return normalize(v);
}

float3 RandVec3(float fOffset)
{
	// Use game time plus offset to sample random texture.
    float u = (g_fGameTime + fOffset);
    
	// coordinates in [-1,1]
    float3 v = g_RandomTexture.SampleLevel(LinearSampler, u, 0).xyz;

    return v;
}

// STREAM-OUT TECH
#define PT_EMITTER 0
#define PT_PARTICLE 1

struct GS_PARTICLE
{	
    float3  vPosition   : POSITION;
    float3  vVelocity   : VELOCITY;
    float2  vSize       : SIZE;
    float   fAge        : AGE;
    uint    iType       : TYPE;
};

GS_PARTICLE VS_MAIN(GS_PARTICLE In)
{
    return In;
}

[maxvertexcount(6)]
void GS_RAIN_MAIN(point GS_PARTICLE In[1], inout PointStream<GS_PARTICLE> OutStream)
{
    In[0].fAge += g_fTimeStep;

    if (In[0].iType == PT_EMITTER)
    {
		// time to emit a new particle?
        if (In[0].fAge > 0.002f)
        {
            for (int i = 0; i < 5; ++i)
            { 
				// Spread rain drops out above the camera.
                float3 vRandom = 35.0f * RandVec3((float) i / 5.0f);
                vRandom.y = 20.0f;

                GS_PARTICLE p;
                p.vPosition = g_vEmitPosition.xyz + vRandom;
                p.vVelocity = float3(1.f, 0.0f, -1.f);
                p.vSize = float2(1.0f, 1.0f);
                p.fAge = 0.0f;
                p.iType = PT_PARTICLE;

                OutStream.Append(p);
            }

			// reset the time to emit
            In[0].fAge = 0.0f;
        }

		// always keep emitters
        OutStream.Append(In[0]);
    }
    else
    {
		// Specify conditions to keep particle; this may vary from system to system.
        if (In[0].fAge <= 2.5f)
            OutStream.Append(In[0]);
    }
}

[maxvertexcount(6)]
void GS_WAVE_MAIN(point GS_PARTICLE In[1], inout PointStream<GS_PARTICLE> OutStream)
{
    In[0].fAge += g_fTimeStep;

    if (In[0].iType == PT_EMITTER)
    {
		// time to emit a new particle?
        if (In[0].fAge > 0.0015f)
        {
            // Spread rain drops out above the camera.
                float3 vRandom = RandUnitVec3(0.0f);
                vRandom.x *= 3.3f;
                vRandom.y *= 0.5f;
            
                vRandom += 2.7f * g_vEmitDirection;
            
                GS_PARTICLE p;
                p.vPosition = g_vEmitPosition.xyz + 4.9f * vRandom;
                p.vVelocity = 1.23f * (vRandom + g_vEmitDirection);
                p.vSize = float2(5.3f, 5.3f);
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
        if (In[0].fAge <= 2.f)
            OutStream.Append(In[0]);
    }
}

[maxvertexcount(6)]
void GS_WATERFALL_MAIN(point GS_PARTICLE In[1], inout PointStream<GS_PARTICLE> OutStream)
{
    In[0].fAge += g_fTimeStep;

    if (In[0].iType == PT_EMITTER)
    {
		// time to emit a new particle?
        if (In[0].fAge > 0.005f)
        {
            // Spread rain drops out above the camera.
            float3 vRandom = RandUnitVec3(0.0f);
            //vRandom *= 0.5f;
            
            GS_PARTICLE p;
            p.vPosition = g_vEmitPosition.xyz;
            p.vVelocity = (vRandom + 3.7f * g_vEmitDirection);
            p.vSize = float2(5.f, 5.f);
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
        if (In[0].fAge <= 2.f)
            OutStream.Append(In[0]);
    }
}

struct tagKeyframeDesc
{
    int animIndex;
    uint currFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

struct tagTweenFrameDesc
{
    float tweenDuration;
    float tweenRatio;
    float tweenSumTime;
    float padding;
    tagKeyframeDesc curr;
    tagKeyframeDesc next;
};

tagTweenFrameDesc g_Tweenframes;

Texture2DArray g_TransformMap;
int g_iSocketBoneIndex;

Matrix g_matOffsetTop;

matrix GetSocketMatrix(tagTweenFrameDesc tTweenFrames)
{
    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];
    
    animIndex[0] = tTweenFrames.curr.animIndex;
    currFrame[0] = tTweenFrames.curr.currFrame;
    nextFrame[0] = tTweenFrames.curr.nextFrame;
    ratio[0] = tTweenFrames.curr.ratio;
    
    animIndex[1] = tTweenFrames.next.animIndex;
    currFrame[1] = tTweenFrames.next.currFrame;
    nextFrame[1] = tTweenFrames.next.nextFrame;
    ratio[1] = tTweenFrames.next.ratio;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0;
    matrix next = 0;

    c0 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 0, currFrame[0], animIndex[0], 0)); // indices[i] 위치에 SocketBoneNumber를 던지자
    c1 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 1, currFrame[0], animIndex[0], 0)); // for문 순회 필요는 없다. 정점과 달리 소켓은 한뼈에만 영향을 받는다.
    c2 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 2, currFrame[0], animIndex[0], 0));
    c3 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 3, currFrame[0], animIndex[0], 0));
    curr = matrix(c0, c1, c2, c3);
    
    n0 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 0, nextFrame[0], animIndex[0], 0));
    n1 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 1, nextFrame[0], animIndex[0], 0));
    n2 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 2, nextFrame[0], animIndex[0], 0));
    n3 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 3, nextFrame[0], animIndex[0], 0));
    next = matrix(n0, n1, n2, n3);
    
    matrix result = lerp(curr, next, ratio[0]);
    
    if (tTweenFrames.next.animIndex >= 0)
    {
        c0 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 0, currFrame[1], animIndex[1], 0));
        c1 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 1, currFrame[1], animIndex[1], 0));
        c2 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 2, currFrame[1], animIndex[1], 0));
        c3 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 3, currFrame[1], animIndex[1], 0));
        curr = matrix(c0, c1, c2, c3);

        n0 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 0, nextFrame[1], animIndex[1], 0));
        n1 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 1, nextFrame[1], animIndex[1], 0));
        n2 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 2, nextFrame[1], animIndex[1], 0));
        n3 = g_TransformMap.Load(int4(g_iSocketBoneIndex * 4 + 3, nextFrame[1], animIndex[1], 0));
        next = matrix(n0, n1, n2, n3);

        matrix nextResult = lerp(curr, next, ratio[1]);
        result = lerp(result, nextResult, tTweenFrames.tweenRatio);
    }

    return result;
}

[maxvertexcount(6)]
void GS_FIRETRAIL_MAIN(point GS_PARTICLE In[1], inout PointStream<GS_PARTICLE> OutStream)
{
    In[0].fAge += g_fTimeStep;

    if (In[0].iType == PT_EMITTER)
    {
		// time to emit a new particle?
        if (In[0].fAge > 0.001f)
        {
            // Spread rain drops out above the camera.
            float3 vRandom = RandUnitVec3(0.0f);
            
            GS_PARTICLE p;
            
            Matrix m = GetSocketMatrix(g_Tweenframes);
            Matrix mTop = mul(g_matOffsetTop, m);
            //Matrix mBottom = mul(g_matOffsetBottom, m);
            
            p.vPosition = mul(float4(In[0].vPosition, 1.f), mTop);
            p.vPosition = mul(float4(p.vPosition, 1.f), g_WorldMatrix);
            p.vPosition += vRandom;
            p.vVelocity = 2.5f * g_vEmitDirection + vRandom;
            p.vSize = float2(1.f, 1.f);
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
        if (In[0].fAge <= g_fTimeStep + 0.3f)
            OutStream.Append(In[0]);
    }
}

technique11 StreamOutTech
{
    pass RAINDROP // 0
    {
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = ConstructGSWithSO(CompileShader(gs_5_0, GS_RAIN_MAIN()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = NULL;
    }

    pass WAVESPLASH // 1
    {
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = ConstructGSWithSO(CompileShader(gs_5_0, GS_WAVE_MAIN()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = NULL;
    }

    pass WATERFALLSPLASH // 2
    {
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = ConstructGSWithSO(CompileShader(gs_5_0, GS_WATERFALL_MAIN()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = NULL;
    }

    pass FIRETRAIL // 3
    {
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = ConstructGSWithSO(CompileShader(gs_5_0, GS_FIRETRAIL_MAIN()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = NULL;
    }
}




