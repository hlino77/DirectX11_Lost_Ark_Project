#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_PBR.hlsl"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D       g_NormalDepthTarget;

Texture2D       g_PreProcessedTarget;
Texture2D		g_DOFBlurTarget;

VS_OUT_TARGET VS_MAIN(TARGET_IN In)
{
    VS_OUT_TARGET Out = (VS_OUT_TARGET) 0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

cbuffer PerFrame
{
    float g_fTexelWidth = 1.f / 1600.f;
    float g_fTexelHeight = 1.f / 900.f;
    float2 padding0;
};
	
cbuffer BlurIntensity
{
    float g_fBlurIntensity = 1.f;
    float3 padding1;
};

cbuffer BlurWeight
{
    //float g_fBlurWeight[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.0f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
    //float fTotal = 6.2108f;

    //float g_fBlurWeight[9] = { 0.1f, 0.18f, 0.55f, 0.9f, 1.0f, 0.9f, 0.55f, 0.18f, 0.1f };
    //float fTotal = 4.46f;

    float g_fBlurWeight[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };

    float3 padding3;
}

#define GAUSSIAN_RADIUS 4

float4 PS_MAIN_DOFBLUR(VS_OUT_TARGET In, uniform bool bHorizontal) : SV_Target
{
    float4 vFinalPixel = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float2 vTexOffset;
    if (bHorizontal)
        vTexOffset = float2(g_fTexelWidth, 0.0f);
    else
        vTexOffset = float2(0.0f, g_fTexelHeight);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    //float weight = 0.0f;
    
    for (int i = -GAUSSIAN_RADIUS; i <= GAUSSIAN_RADIUS; ++i)
    {
        float2 vTexcoord = In.vTexcoord + i * vTexOffset;
        if (vTexcoord.x > 1.f || vTexcoord.y > 1.f || vTexcoord.x < 0.f || vTexcoord.y < 0.f)
            continue;
            
        float4 vSample = g_DOFBlurTarget.Sample(LinearBorderSampler, vTexcoord);
        
        if (vSample.a > 0.03f)
            vFinalPixel += vSample * g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_fBlurIntensity;
        
        vFinalPixel.a += vSample.a * g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_fBlurIntensity;
    }
    
    clip(vFinalPixel.a - 0.03f);
    
    return vFinalPixel;
}

cbuffer DOFData
{
    float g_fRange = 10.f;
    float g_fFocus = 0.00625f;
    float2 vPadding;
};

float4 PS_MAIN_DEPTHOFFIELD(VS_OUT_TARGET In) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float   fDepth = g_NormalDepthTarget.Sample(LinearSampler, In.vTexcoord).w;
    float4 vSharp = g_PreProcessedTarget.Sample(LinearSampler, In.vTexcoord);
    float4  vBlur = g_DOFBlurTarget.Sample(LinearSampler, In.vTexcoord);
    
    return lerp(vSharp, vBlur, saturate(g_fRange * abs(g_fFocus - fDepth)));
}
    
technique11 DefaultTechnique
{
    pass DOFBlurH // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOFBLUR(true);
    }

    pass DOFBlurV // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOFBLUR(false);
    }

    pass DepthOfField // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEPTHOFFIELD();
    }
}




