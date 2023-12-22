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

#endif