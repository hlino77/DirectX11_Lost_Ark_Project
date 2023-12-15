#ifndef _CLIENT_SHADER_INOUT_HLSL_
#define _CLIENT_SHADER_INOUT_HLSL_

struct STATIC_IN
{
    float3  vPosition : POSITION;
    float3  vNormal : NORMAL;
    float3  vTangent : TANGENT;
    float2  vTexUV : TEXCOORD0;
};

struct SKELETAL_IN
{
    float3  vPosition : POSITION;
    float3  vNormal : NORMAL;
    float3  vTangent : TANGENT;
    float2  vTexUV : TEXCOORD0;
    uint4   vBlendIndex : BLENDINDEX;
    float4  vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4  vPosition : SV_POSITION;
    float4  vNormal : NORMAL;
    float2  vTexUV : TEXCOORD0;
    float4  vProjPos : TEXCOORD1;
    float3  vTangent : TANGENT;
};

struct PS_OUT_PBR
{
    float4  vDiffuse : SV_TARGET0;
    float4  vNormal : SV_TARGET1;
    float4  vDepth : SV_TARGET2;
    float4  vMetallic : SV_TARGET3;
    float4  vRoughness : SV_TARGET4;
	float4  vEmissive : SV_TARGET5;
};
	
struct PS_OUT_PHONG
{
    float4  vDiffuse : SV_TARGET0;
    float4  vNormal : SV_TARGET1;
    float4  vDepth : SV_TARGET2;
	
	//float4 vEmissive : SV_TARGET5;
};

struct PS_OUT_EFFECT
{
    float4  vDiffuse : SV_TARGET0;
    float4  vBlur : SV_TARGET1;
};

struct PS_OUT_SHADOW
{
    float4  vDepth : SV_TARGET0;
};

#endif