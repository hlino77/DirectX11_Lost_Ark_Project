#ifndef _CLIENT_SHADER_EFFECT_HLSL_
#define _CLIENT_SHADER_EFFECT_HLSL_
#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Light.hlsl"

cbuffer FX_Variables
{
    float2 vUV_Offset = float2(0.f, 0.f);
    bool bUV_Wave = false;
    float fUV_WaveSpeed = 1.f;
    float2 vUV_TileCount = float2(1.f, 1.f);
    float2 vUV_TileIndex = float2(0.f, 0.f);
    float4 vColor_Offset = float4(0.f, 0.f, 0.f, 0.f);
    float4 vColor_Clip = float4(0.f, 0.f, 0.f, 0.f);
    float4 vColor_Mul = float4(1.f, 1.f, 1.f, 1.f);
};

cbuffer FX_Intensity
{
    float fIntensity_Bloom = 1.f;
    float fIntensity_Distortion = 0.f;
    float fDissolve_Amount = 0.f;
    bool  bDistortionOnBaseMaterial = false;
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
    
    float3 vRandomMul = float3(1.f, 1.f, 1.f);
    float fSpreadSpeed = 1.f;
    float fEmitTerm = 0.005f;
    float fParticleLifeTime = 1.f;
    float fSequenceTerm = 0.0f;
    int iIsLoop = 0;
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

float4 CalculateEffectColor(in float2 vUV, in float2 vOriginUV, out float fDistortion)
{
    float fMask = 1.f;
    float2 vNewUV = vUV;

    if (EPSILON < NoisMaskEmisDslv.x)   // Noise
    {
        float2 vNoiseUV = g_NoiseTexture.Sample(LinearSampler, vOriginUV).rg;
        vNewUV += (vNoiseUV - 0.5f) * 2.f;
    }
    
    if (EPSILON < NoisMaskEmisDslv.y)   // Mask
    {
        fMask = g_MaskTexture.Sample(LinearSampler, vNewUV).r;
        clip(fMask - 0.01f);
    }

    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, vNewUV);
    
    vColor += vColor_Offset;

    vColor *= vColor_Mul * fMask;

    if (vColor.r + 0.01f < vColor_Clip.r && vColor.g + 0.01f < vColor_Clip.g && vColor.b + 0.01f < vColor_Clip.b)
        discard;

    clip(vColor.a - vColor_Clip.a);

    if (bDistortionOnBaseMaterial)
        fDistortion = fMask * fIntensity_Distortion;
    else
        fDistortion = vColor.r * fIntensity_Distortion;
    
    if (EPSILON < NoisMaskEmisDslv.w)	// Dissolve
    {
        float fDissolve = g_DissolveTexture.Sample(LinearSampler, vNewUV).x;
        
	    //Discard the pixel if the value is below zero
        clip(fDissolve - fDissolve_Amount);
	    //Make the pixel emissive if the value is below ~f
        if (fDissolve - fDissolve_Amount < 0.25f)/*0.08f*/
        {
            vColor.xyz += vColor.xyz * float3(0.3f, 0.3f, 0.3f);
        }
    }
    
    return vColor;
}


float4 CalculateEffectColorClamp(in float2 vUV, in float2 vOriginUV, out float fDistortion)
{
    float fMask = 1.f;
    float2 vNewUV = vUV;

    if (EPSILON < NoisMaskEmisDslv.x)   // Noise
    {
        float2 vNoiseUV = g_NoiseTexture.Sample(LinearClampSampler, vOriginUV).rg;
        vNewUV += (vNoiseUV - 0.5f) * 2.f;
    }

    if (EPSILON < NoisMaskEmisDslv.y)   // Mask
    {
        fMask = g_MaskTexture.Sample(LinearClampSampler, vNewUV).r;
        clip(fMask - 0.01f);
    }

    float4 vColor = g_DiffuseTexture.Sample(LinearClampSampler, vNewUV);

    vColor += vColor_Offset;

    vColor *= vColor_Mul * fMask;

    if (vColor.r + 0.01f < vColor_Clip.r && vColor.g + 0.01f < vColor_Clip.g && vColor.b + 0.01f < vColor_Clip.b)
        discard;

    clip(vColor.a - vColor_Clip.a);

    if (bDistortionOnBaseMaterial)
        fDistortion = fMask * fIntensity_Distortion;
    else
        fDistortion = vColor.r * fIntensity_Distortion;
    
    if (EPSILON < NoisMaskEmisDslv.w)	// Dissolve
    {
        float fDissolve = g_DissolveTexture.Sample(LinearClampSampler, vNewUV).x;

        //Discard the pixel if the value is below zero
        clip(fDissolve - fDissolve_Amount);
        //Make the pixel emissive if the value is below ~f
        if (fDissolve - fDissolve_Amount < 0.25f)/*0.08f*/
        {
            vColor.xyz += vColor.xyz * float3(0.3f, 0.3f, 0.3f);
        }
    }

    return vColor;
}

#endif