#ifndef _CLIENT_SHADER_GLOBAL_HLSL_
#define _CLIENT_SHADER_GLOBAL_HLSL_

cbuffer GlobalBuffer
{
    matrix ViewMatrix;
    matrix ProjMatrix;
    matrix ViewProj;
    matrix ViewInv;
    matrix WorldInvTransposeView;
};

cbuffer TransformBuffer
{
    matrix WorldMatrix;
};

float3 CameraPosition()
{
    return ViewInv._41_42_43;
}

float4 CreateOutline(float4 vertPos, float Outline)
{
    float4x4 scaleMat;
    scaleMat[0][0] = 1.0f + Outline;
    scaleMat[0][1] = 0.0f;
    scaleMat[0][2] = 0.0f;
    scaleMat[0][3] = 0.0f;
    scaleMat[1][0] = 0.0f;
    scaleMat[1][1] = 1.0f + Outline;
    scaleMat[1][2] = 0.0f;
    scaleMat[1][3] = 0.0f;
    scaleMat[2][0] = 0.0f;
    scaleMat[2][1] = 0.0f;
    scaleMat[2][2] = 1.0f + Outline;
    scaleMat[2][3] = 0.0f;
    scaleMat[3][0] = 0.0f;
    scaleMat[3][1] = 0.0f;
    scaleMat[3][2] = 0.0f;
    scaleMat[3][3] = 1.0f;
				
    return mul(scaleMat, vertPos);
}

static const float PI = 3.14159265359f;
static const float EPSILON = 0.000001f;

Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_SpecularTexture;
Texture2D g_EmissiveTexture;
Texture2D g_MRMaskTexture;

Texture2D g_DissolveTexture;
Texture2D g_MaskTexture;
Texture2D g_NoiseTexture;

float4    g_vColor_R;
float4    g_vColor_G;
float4    g_vColor_B;

float     g_fDissolveAmount;

float     g_fOutlineThickness = 0.01f;
float4    g_vOutlineColor = float4(1.f, 1.f, 1.f, 1.f);
float4    g_vOutlineBloom = float4(0.f, 0.f, 0.f, 0.f);
float4    g_vOutlineDivision = float4(1.f, 0.f, 1.f, 1.f);

#define MT_DYNAMIC 1
#define MT_STATIC 0

#endif