#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"

matrix  g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix  g_ViewMatrixInv;
matrix  g_ProjMatrixInv;
matrix  g_CamViewMatrix;
matrix  g_CamProjMatrix;
float4  g_vCamPosition;

Texture2D g_NormalTarget;
Texture2D g_NormalDepthTarget;
Texture2D g_PrePostProcessTarget;
Texture2D g_PropertiesTarget;
Texture2D g_SSRTarget;
Texture2D g_SSROriginalTarget;

cbuffer SSR_Data
{
    float fSSRStep = 0.0175f;
    int iStepCount = 25;
    float2 padding;
}

VS_OUT_TARGET VS_MAIN_SSR(TARGET_IN In)
{
    VS_OUT_TARGET Out = (VS_OUT_TARGET) 0;
	
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

float4 PS_MAIN_SSR(VS_OUT_TARGET In) : SV_TARGET
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vNormal = g_NormalTarget.Sample(LinearSampler, In.vTexcoord);
    float4 vDepth = g_NormalDepthTarget.Sample(LinearSampler, In.vTexcoord);
    float4 vProperties = g_PropertiesTarget.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = vDepth.w * 1200.f;
    
    float4 vWorldPos;

    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vNormal.w;
    vWorldPos.w = 1.0f;
    
    vWorldPos *= fViewZ;
    //vWorldPos = mul(vWorldPos, g_ProjViewMatrixInv);
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float4 vViewDir = normalize(vWorldPos - g_vCamPosition);
    vViewDir.w = 0.f;
    
    float4 vRayOrigin = vWorldPos;
    
    vNormal = float4(vNormal.xyz * 2.f - 1.f, 0.f);
    float4 vRayDir = normalize(reflect(vViewDir, vNormal));
    vRayDir.w = 0.f;
    
    float fStep = fSSRStep;
    matrix matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
    
    float fPixelDepth = 0.f;
    int iStepDistance = 0;
    float2 vRayPixelPos = (float2) 0;
  
    [unroll(10)]
    for (iStepDistance = 1; iStepDistance < iStepCount; ++iStepDistance)
    {
        float4 vDirStep = vRayDir * fStep * iStepDistance;
        vDirStep.w = 0.f;
        float4 vRayWorldPos = vRayOrigin + vDirStep;

        float4 vRayProjPos = mul(vRayWorldPos, matVP);
        vRayProjPos.x = vRayProjPos.x / vRayProjPos.w;
        vRayProjPos.y = vRayProjPos.y / vRayProjPos.w;
      
        vRayPixelPos = float2(vRayProjPos.x * 0.5f + 0.5f, vRayProjPos.y * -0.5f + 0.5f);
        
        clip(vRayPixelPos);
        clip(1.f - vRayPixelPos);
        
        float2 vPixelCoord = float2(0.f, 0.f);
        //vPixelCoord.x = g_NormalTarget.Sample(LinearSampler, vRayPixelPos).w;
        //vPixelCoord.y = g_NormalDepthTarget.Sample(LinearSampler, vRayPixelPos).w;
        
        fPixelDepth = g_NormalTarget.Sample(LinearSampler, vRayPixelPos).w;
        fPixelDepth *= g_NormalDepthTarget.Sample(LinearSampler, vRayPixelPos).w * 1200.f;
        
        float fDiff = vRayProjPos.z - fPixelDepth;
        
        if (fDiff > 0.0f && fDiff < fSSRStep)
            break;
    }
 
    clip(iStepCount - 0.5f - iStepDistance);
    

    return g_PrePostProcessTarget.Sample(LinearSampler, vRayPixelPos) * (1.f - iStepDistance / iStepCount) * (clamp(pow(vProperties.x, 2.2f) / (vProperties.y + EPSILON), 0.01f, 1.f));
    
}

//cbuffer PerFrame
//{
//    float g_fTexelWidth = 1.f / 1600.f;
//    float g_fTexelHeight = 1.f / 900.f;
//    float2 padding0;
//};

//cbuffer BlurWeight
//{
//    float g_fBlurWeight[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };
//    float3 padding3;
//}

//#define GAUSSIAN_RADIUS 4

//float4 PS_MAIN_SSRBLUR(VS_OUT_TARGET In, uniform bool bHorizontal) : SV_Target
//{
//    float4 vProperties = g_PropertiesTarget.Sample(LinearSampler, In.vTexcoord);
//    clip((MT_DYNAMIC - vProperties.z) - EPSILON);
    
//    float4 vFinalPixel = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
//    float2 vTexOffset;
//    if (bHorizontal)
//        vTexOffset = float2(g_fTexelWidth, 0.0f);
//    else
//        vTexOffset = float2(0.0f, g_fTexelHeight);
    
//    float3 color = float3(0.0f, 0.0f, 0.0f);
    
//    for (int i = -GAUSSIAN_RADIUS; i <= GAUSSIAN_RADIUS; ++i)
//    {
//        vFinalPixel.xyz += g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_SSRTarget.Sample(LinearBorderSampler, In.vTexcoord + i * vTexOffset).rgb;
//    }
    
//    float fS = pow(vProperties.y, 8.f);
//    float4 vOriginSSR = g_SSROriginalTarget.Sample(LinearBorderSampler, In.vTexcoord);
    
//    vFinalPixel.x = lerp(vOriginSSR.x, clamp(vFinalPixel.x, 0.f, 1.f), fS);
//    vFinalPixel.y = lerp(vOriginSSR.y, clamp(vFinalPixel.y, 0.f, 1.f), fS);
//    vFinalPixel.z = lerp(vOriginSSR.z, clamp(vFinalPixel.z, 0.f, 1.f), fS);
        
//    return vFinalPixel;
//}

DepthStencilState DSS_DepthFalse_StencilEnable
{
    DepthEnable = false;
    DepthWriteMask = zero;

    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

    FrontFaceStencilFunc = equal;
    FrontFaceStencilPass = keep;
    FrontFaceStencilFail = keep;
};

technique11 DefaultTechnique
{
    pass SSR // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        //SetDepthStencilState(DSS_DepthFalse_StencilEnable, 2);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN_SSR();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SSR();
    }

    //pass SSR_Blur_H // 1
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_None, 0);
    //    SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
    //    VertexShader = compile vs_5_0 VS_MAIN_SSR();
    //    GeometryShader = NULL;
    //    HullShader = NULL;
    //    DomainShader = NULL;
    //    PixelShader = compile ps_5_0 PS_MAIN_SSRBLUR(true);
    //}

    //pass SSR_Blur_V // 2
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_None, 0);
    //    SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
    //    VertexShader = compile vs_5_0 VS_MAIN_SSR();
    //    GeometryShader = NULL;
    //    HullShader = NULL;
    //    DomainShader = NULL;
    //    PixelShader = compile ps_5_0 PS_MAIN_SSRBLUR(false);
    //}
}



