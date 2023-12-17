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

// 정점 포맷 전부 옮길 것

float3 CameraPosition()
{
    return ViewInv._41_42_43;
}
// 

#endif