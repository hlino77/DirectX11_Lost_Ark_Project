#ifndef _CLIENT_SHADER_EFFECT_HLSL_
#define _CLIENT_SHADER_EFFECT_HLSL_
#include "Client_Shader_Defines.hlsl"

cbuffer FX_Variables
{
    float2 vUV_Offset = float2(0.f, 0.f);
    bool bUV_Wave = false;
    float fUV_WaveSpeed = 1.f;
    float2 vUV_TileCount = float2(1.f, 1.f);
    float2 vUV_TileIndex = float2(0.f, 0.f);
    float4 vColor_Offset = float4(0.f, 0.f, 0.f, 0.f);
    float4 vColor_Clip = float4(0.f, 0.f, 0.f, 0.f);
};

cbuffer FX_Intensity
{
    float fIntensity_Bloom = 1.f;
    float fIntensity_Radial = 0.f;
    //float fDissolve_Amount = 0.f;
    float2 padding;
};

cbuffer FX_Billboard
{
    bool bBillboard = false;
    float3 padding2;
};

cbuffer FX_Particle
{
    float fGameTime = false;
    float3 vEmitPosition;
    float3 vEmitDirection;
    float fTimeStep;
};

Texture1D g_RandomTexture;

// HELPER FUNCTIONS
float3 RandUnitVec3(float fOffset)
{
	// Use game time plus offset to sample random texture.
    float u = (fGameTime + fOffset);

	// coordinates in [-1,1]
    float3 v = g_RandomTexture.SampleLevel(LinearSampler, u, 0).xyz;

	// project onto unit sphere
    return normalize(v);
}

float3 RandVec3(float fOffset)
{
	// Use game time plus offset to sample random texture.
    float u = (fGameTime + fOffset);
    
	// coordinates in [-1,1]
    float3 v = g_RandomTexture.SampleLevel(LinearSampler, u, 0).xyz;

    return v;
}

#endif