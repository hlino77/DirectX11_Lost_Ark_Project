#ifndef _CLIENT_SHADER_LIGHT_HLSL_
#define _CLIENT_SHADER_LIGHT_HLSL_

#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Global.hlsl"

float4  g_vBloomColor = float4(1.3f, 1.3f, 1.3f, 1.f);
float   g_fRimLight = 0.0f;
bool    g_bReverseDissolve = false;

// Light
struct LightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 direction;
    float padding;
};

cbuffer LightBuffer
{
    LightDesc GlobalLight;
};

cbuffer MaterialFlag
{
    float4 SpecMaskEmisExtr;
};
    
cbuffer EffectMaterialFlag
{
    float4 NoisMaskEmisDslv;
};

// Material

float4 PhongDiffuse(float3 normal, float2 uv)
{
    float4 color = g_DiffuseTexture.Sample(LinearSampler, uv);
    float value = dot(-GlobalLight.direction, normalize(normal));
    return color * value * GlobalLight.diffuse/* * Material.diffuse*/;
}

float4 PhongSpecular(float3 normal, float3 worldPosition)
{
    float3 vReflect = reflect(GlobalLight.direction, normal);
    //float3 vReflect = GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal));
    vReflect = normalize(vReflect);

    float3 cameraPosition = CameraPosition();
    float3 vEye = normalize(cameraPosition - worldPosition);

    float value = saturate(dot(vReflect, vEye)); // clamp(0~1)
    float specular = pow(value, 10);

    return GlobalLight.specular/* * Material.specular*/ * specular;
}

float4 ComputeAmbient(float2 uv)
{
    float4 color = GlobalLight.ambient/* * Material.ambient*/;
    return g_DiffuseTexture.Sample(LinearSampler, uv) * color;
}

float4 ComputeEmissive(float2 uv)
{
    float4 color = GlobalLight.emissive/* * Material.emissive*/;
    return g_EmissiveTexture.Sample(LinearSampler, uv) * color;
}

float4 ComputeForwardLight(float3 normal, float2 uv, float3 worldPosition)
{
    return ComputeAmbient(uv) + PhongDiffuse(normal, uv) + PhongSpecular(normal, worldPosition) + ComputeEmissive(uv);
}

void ComputeNormalMapping(inout float4 normal, float3 tangent, float2 texcoord)
{
	// [0,255] 범위에서 [0,1]로 변환
    float4 map = g_NormalTexture.Sample(LinearSampler, texcoord);
    if (any(map.rgb) == false)
        return;

    float3 N = normalize(normal.xyz); // z
    float3 T = normalize(tangent); // x
    float3 B = normalize(cross(N, T)); // y
    float3x3 TBN = float3x3(T, B, N); // TS -> WS

	// [0,1] 범위에서 [-1,1] 범위로 변환
    float3 tangentSpaceNormal = (map.rgb * 2.0f - 1.0f);
    float3 worldNormal = mul(tangentSpaceNormal, TBN);

    normal = float4(worldNormal, 0.f);
};


void ComputeDissolveColor(inout float4 color, float2 texcoord)
{
    float4 deffuseCol = g_DiffuseTexture.Sample(LinearSampler, texcoord);
    float dissolveSample = g_DissolveTexture.Sample(LinearSampler, texcoord * 3.f).x;
    if (g_bReverseDissolve)
    {
        dissolveSample = 1 - dissolveSample;
    }
    //Discard the pixel if the value is below zero
    clip(dissolveSample - g_fDissolveAmount);
    float4 emissive = { 0.f, 0.f, 0.f, 0.f };
    //Make the pixel emissive if the value is below ~f
    if (dissolveSample - g_fDissolveAmount < 0.1f)/*0.08f*/
    {
        emissive = float4(1.3f, 1.3f, 1.3f, 1.f)* g_vBloomColor;
    }

    color = (color + emissive) * deffuseCol;
};

void ComputeDissolveColorforInstance(inout float4 color, float2 texcoord,float fDissolveAmount)
{
    float4 deffuseCol = g_DiffuseTexture.Sample(LinearSampler, texcoord);
    float dissolveSample = g_DissolveTexture.Sample(LinearSampler, texcoord * 0.5f).x;

    //Discard the pixel if the value is below zero
    clip(dissolveSample - fDissolveAmount);
    float4 emissive = { 0.f, 0.f, 0.f, 0.f };
    //Make the pixel emissive if the value is below ~f
    if (dissolveSample - fDissolveAmount < 0.25f)/*0.08f*/
    {
        emissive = float4(1.3f, 1.3f, 1.3f, 1.f);
    }

    color = (color + emissive) * deffuseCol;
};

void ComputeRimLight(inout float3 color, float3 vNormal, float3 vLook)
{
    float3 E = normalize(-vLook);
    
    float value = saturate(dot(E, vNormal));
    float fEmissive = 1.0f - value;

	// min, max, x
    fEmissive = smoothstep(0.0f, 1.0f, fEmissive);

    fEmissive = pow(fEmissive, 1.5f);
    color *= fEmissive;
};

#endif