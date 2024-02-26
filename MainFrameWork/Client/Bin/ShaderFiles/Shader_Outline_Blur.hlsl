#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_PBR.hlsl"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_BrightTarget;
Texture2D       g_OutBlurTarget;

cbuffer BloomOption
{
    float g_Threshold_Min = EPSILON; //EPSILON;
    float g_Threshold_Max = 10.f;
};

float GetBloomCurve(float x, float threshold)
{
    float result = x;
    //result *= 2.f;
    // Bloom Curve 1
	//result = x * 0.05 + max(0, x - threshold) * 0.5; // default threshold = 1.26

    // Bloom Curve 2
    //result = x * x / 3.2;
    
    // Bloom Curve 3
    result = max(0, x - threshold); // default threshold = 1.0
    result *= result;

    return result * 0.5f;
}

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

float Tonemap_ACES(float x)
{
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

cbuffer PerFrame
{
    float g_fTexelWidth = 1.f / 1600.f;
    float g_fTexelHeight = 1.f / 900.f;
    float2 padding0;
};
	
cbuffer BlurIntensity
{
    float g_fBlurIntensity = 1.0f;
    float3 padding1;
};

cbuffer BlurWeight
{
    float g_fBlurWeight[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };
    
    float3 padding3;
}

#define GAUSSIAN_RADIUS 4

float4 PS_MAIN_OUTBLUR(VS_OUT_TARGET In, uniform bool bHorizontal) : SV_Target
{
    float4 vFinalPixel = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float2 vTexOffset;
    if (bHorizontal)
        vTexOffset = float2(g_fTexelWidth, 0.0f);
    else
        vTexOffset = float2(0.0f, g_fTexelHeight);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = -GAUSSIAN_RADIUS; i <= GAUSSIAN_RADIUS; ++i)
    {
        float2 vTexcoord = In.vTexcoord + i * vTexOffset;
        if (vTexcoord.x > 1.f || vTexcoord.y > 1.f || vTexcoord.x < 0.f || vTexcoord.y < 0.f)
            continue;
            
        float4 vSample = g_OutBlurTarget.Sample(LinearBorderSampler, vTexcoord);
        
        if (vSample.a > 0.03f)
            vFinalPixel += vSample * g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_fBlurIntensity;
        
        vFinalPixel.a += vSample.a * g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_fBlurIntensity;
    }
    
    clip(vFinalPixel.a - 0.03f);
    
    return vFinalPixel;
}

technique11 DefaultTechnique
{
	pass BlurH // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OUTBLUR(true);
    }

	pass BlurV // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_OUTBLUR(false);
    }
}




