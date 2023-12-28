#ifndef _CLIENT_SHADER_EFFECT_HLSL_
#define _CLIENT_SHADER_EFFECT_HLSL_

cbuffer FX_Variables
{
    float2 vUV_Offset = float2(0.f, 0.f);
    float2 vUV_Direction = float2(1.f, 1.f);
    float2 vUV_TileCount = float2(1.f, 1.f);
    float2 vUV_TileIndex = float2(0.f, 0.f);
    float4 vColor_Offset = float4(0.f, 0.f, 0.f, 0.f);
    //float2 vUV_Speed = float2(0.f, 0.f);
};

cbuffer FX_Intensity
{
    float fIntensity_Bloom = 1.f;
    float fIntensity_Radial = 0.f;
    float2 padding;
};

cbuffer FX_Billboard
{
    bool bBillboard = false;
    float3 padding2;
};

#endif