#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_PBR.hlsl"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_BrightTarget;
Texture2D		g_BloomBlurTarget;
Texture2D       g_DecalBloomBlurTarget;
Texture2D		g_EffectBloomBlurTarget;
Texture2D		g_DownSampledTarget;

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

float4 PS_MAIN_BRIGHT(VS_OUT_TARGET In) : SV_Target
{   // ¾È¾¸
    float fBrightColor = 0.f;
    float4 vPixelColor = g_BrightTarget.Sample(LinearSampler, In.vTexcoord);
	
    float fBrightness = dot(vPixelColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    if (fBrightness > 0.99f)
        fBrightColor = float4(vPixelColor.rgb, 1.f);
	
    return fBrightColor;
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

float4 PS_DOWNSAMPLE_BRIGHT(VS_OUT_TARGET In) : SV_TARGET
{
    float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int y = -2; y < 3; y++)
    {
        for (int x = -2; x < 3; x++)
        {
            // Compute the sum of color values
            outColor += g_BloomBlurTarget.Sample(LinearBorderSampler, In.vTexcoord, int2(x, y));
            outColor += g_DecalBloomBlurTarget.Sample(LinearBorderSampler, In.vTexcoord, int2(x, y));
            outColor += g_EffectBloomBlurTarget.Sample(LinearBorderSampler, In.vTexcoord, int2(x, y));
        }
    }
    
    //outColor /= 25;
    outColor *= 0.04f;
    
    float intensity = max(dot(outColor.rgb, float3(1.05f, 1.05f, 1.05f)) * outColor.a, EPSILON);
    //float intensity = max(dot(outColor, float3(0.3f, 0.59f, 0.11f)), 0.000001f);
    
    float bloom_intensity = min(GetBloomCurve(intensity, g_Threshold_Min), g_Threshold_Max);
    float3 bloom_color = outColor * bloom_intensity / intensity;
    
    return float4(bloom_color, 1.0f);

    //float fBrightness = dot(outColor, float3(0.2125f, 0.7154f, 0.0721f));
    //outColor *= Tonemap_ACES(fBrightness);
    
    //return float4(outColor, 1.0f);
}

float4 PS_DOWNSAMPLE(VS_OUT_TARGET In) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int y = -2; y < 3; y++)
    {
        for (int x = -2; x < 3; x++)
        {
            // Compute the sum of color values
            outColor += g_BloomBlurTarget.Sample(LinearClampSampler, In.vTexcoord, int2(x, y)).rgb;
        }
    }
    
    //outColor /= 25.f;
    outColor *= 0.04f;
    
    return float4(outColor, 1.0f);
}

float4 PS_UPSAMPLE(VS_OUT_TARGET In) : SV_TARGET
{
    float3 outColor = float3(0.0f, 0.0f, 0.0f);
    
    float3 vDst = g_BloomBlurTarget.Sample(LinearClampSampler, In.vTexcoord).rgb;
    float3 vSrc = g_DownSampledTarget.Sample(LinearClampSampler, In.vTexcoord).rgb;
    
    outColor = pow(pow(abs(vDst), 2.2f) + pow(abs(vSrc), 2.2f), 1.f / 2.2f);
    
    return float4(outColor, 1.0f);
}

cbuffer PerFrame
{
    float g_fTexelWidth = 1.f / 1600.f;
    float g_fTexelHeight = 1.f / 900.f;
    float2 padding0;
};
	
cbuffer BlurIntensity
{
    float g_fBlurIntensity = 1.08f;
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

float4 PS_MAIN_BLOOMBLUR(VS_OUT_TARGET In, uniform bool bHorizontal) : SV_Target
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
            
        float4 vSample = g_BloomBlurTarget.Sample(LinearBorderSampler, vTexcoord);
        
        if (vSample.a > 0.03f)
            vFinalPixel += vSample * g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_fBlurIntensity;
        
        vFinalPixel.a += vSample.a * g_fBlurWeight[i + GAUSSIAN_RADIUS] * g_fBlurIntensity;
    }
    
    clip(vFinalPixel.a - 0.03f);
    
    return vFinalPixel;
    
    ///////////////////////////////
    
    //float4 vFinalPixel = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //float2 vTexOffset;
    //if (bHorizontal)
    //    vTexOffset = float2(g_fTexelWidth, 0.0f);
    //else
    //    vTexOffset = float2(0.0f, g_fTexelHeight);

    //for (int i = -6; i < 7; ++i)
    //{
    //    float2 vTexcoord = In.vTexcoord;
    //    vTexcoord += i * vTexOffset;
		
    //    if (vTexcoord.x > 1.f || vTexcoord.y > 1.f || vTexcoord.x < 0.f || vTexcoord.y < 0.f)
    //        continue;
		
    //    float4 vBrightPixel = g_BloomBlurTarget.Sample(LinearBorderSampler, vTexcoord);
        
    //    if (vBrightPixel.a > 0.03f)
    //        vFinalPixel += vBrightPixel * g_fBlurWeight[6 + i] * g_fBlurIntensity;
		
    //    vFinalPixel.a += vBrightPixel.a * g_fBlurWeight[6 + i] * g_fBlurIntensity;
    //}

    //clip(vFinalPixel.a - 0.01f);
	
    //vFinalPixel.rgb /= fTotal;

    return vFinalPixel;
}

technique11 DefaultTechnique
{
	pass Bright // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BRIGHT();
	}

    pass DownSample_Bright // 1
    {
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DOWNSAMPLE_BRIGHT();
    }

    pass DownSample // 2
    {
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DOWNSAMPLE();
    }

    pass UpSample // 2
    {
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_UPSAMPLE();
    }

	pass BloomBlurH // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOMBLUR(true);
	}

	pass BloomBlurV // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOMBLUR(false);
    }
}




