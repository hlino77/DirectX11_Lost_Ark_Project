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
    float fSSRStep = 0.005f;
    int iStepCount = 100;
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
    if (0 == iStepCount || EPSILON > fSSRStep)
        //return g_PrePostProcessTarget.Sample(LinearSampler, In.vTexcoord);
        return (float4) 0.f;
    
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vNormal = g_NormalTarget.Sample(LinearSampler, In.vTexcoord);
    float  fDepth = g_NormalDepthTarget.Sample(LinearSampler, In.vTexcoord).w;
    float4 vProperties = g_PropertiesTarget.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = fDepth * g_fFar;
    
    float4 vWorldPos;

    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vNormal.w;
    vWorldPos.w = 1.0f;
    
    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float4 vViewDir = normalize(vWorldPos - g_vCamPosition);
    vViewDir.w = 0.f;
    
    float4 vRayOrigin = vWorldPos;
    
    vNormal = float4(vNormal.xyz * 2.f - 1.f, 0.f);
    float4 vRayDir = normalize(reflect(vViewDir, vNormal));
    vRayDir.w = 0.f;
    
    float4x4 matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
    
    float fPixelDepth = 0.f;
    int iStepDistance = 0;
    float2 vRayPixelPos = (float2) 0;

    [loop]
    for (iStepDistance = 1; iStepDistance < iStepCount; ++iStepDistance)
    {
        float4 vDirStep = vRayDir * fSSRStep * iStepDistance;
        vDirStep.w = 0.f;
        float4 vRayWorldPos = vRayOrigin + vDirStep;

        float4 vRayProjPos = mul(vRayWorldPos, matVP);
        vRayProjPos.x = vRayProjPos.x / vRayProjPos.w;
        vRayProjPos.y = vRayProjPos.y / vRayProjPos.w;
      
        vRayPixelPos = float2(vRayProjPos.x * 0.5f + 0.5f, vRayProjPos.y * -0.5f + 0.5f);
        
        clip(vRayPixelPos);
        clip(1.f - vRayPixelPos);
        
        fPixelDepth = g_NormalTarget.Sample(LinearSampler, vRayPixelPos).w;
        fPixelDepth *= g_NormalDepthTarget.Sample(LinearSampler, vRayPixelPos).w * g_fFar;
        
        float fDiff = vRayProjPos.z - fPixelDepth;
        
        if (fDiff > 0.0f && fDiff < fSSRStep)
            break;
    }
 
    // clip(iStepCount - 0.5f - iStepDistance);
    float fReflectionRatio = iStepDistance / (float) iStepCount;
    float fRoughness = vProperties.y + EPSILON;
    float fMetallic = vProperties.x;
    
    vColor = g_PrePostProcessTarget.Sample(LinearSampler, vRayPixelPos);

    vColor = saturate(vColor * saturate(1.f - fReflectionRatio) * fMetallic / fRoughness);
    return vColor;
}

float4 PS_MAIN_BLENDSSR(VS_OUT_TARGET In) : SV_TARGET
{
    float4 vColor = g_PrePostProcessTarget.Sample(LinearSampler, In.vTexcoord);
    float4 vSSR = g_SSRTarget.Sample(LinearSampler, In.vTexcoord);
    
    if (EPSILON < vSSR.a)
    {
        vColor = float4(vSSR.rgb * vSSR.a + vColor.rgb * (1.f - vSSR.a), 1.f);
    }
    
    return vColor;
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

technique11 DefaultTechnique
{
    pass SSR // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN_SSR();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SSR();
    }

    pass BlendSSR // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN_SSR();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLENDSSR();
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



