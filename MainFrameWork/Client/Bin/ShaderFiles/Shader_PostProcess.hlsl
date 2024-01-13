#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Light.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D	g_PrePostProcessTarget;
Texture2D	g_EffectOneBlendTarget;
Texture2D	g_EffectAlphaBlendTarget;
Texture2D	g_ShadeTarget;
Texture2D	g_BloomTarget;
Texture2D	g_BlurTarget;
Texture2D	g_BlendedTarget;

Texture2D g_Texture;

float2  g_PixelSize;
int     g_KernelSize; //커널은 이미지 처리에서 필터 또는 윈도우라고도 불리는 작은 행렬 또는 마스크
float   g_CenterWeight;
float	g_WeightAtt;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

float4 PS_MAIN_POSTPROCESS(PS_IN In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);

    return vColor;
}

cbuffer ScreenTone
{
    float g_fGrayScale = 1.f;
    float g_fContrastValue = 1.f;
    float g_fSaturation = 1.f;
}

float4 PS_MAIN_SCREENTONE(PS_IN In) : SV_TARGET0
{
    vector vColor = g_BlendedTarget.Sample(LinearSampler, In.vTexcoord);
	
    vColor *= g_fSaturation;
	
    float avg = (vColor.r + vColor.g + vColor.b) / 3.0;
    vector vNewColor;
    vNewColor.a = 1.0f;
    vNewColor.rgb = avg * (1.0 - g_fGrayScale) + vColor.rgb * g_fGrayScale;

    return 0.5f + g_fContrastValue * (vNewColor - 0.5f);
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

float4 PS_MAIN_BLENDEFFECT(PS_IN In) : SV_TARGET0
{
    float4 vColor = g_PrePostProcessTarget.Sample(LinearSampler, In.vTexcoord);
    float4 vBloom = g_BloomTarget.Sample(LinearSampler, In.vTexcoord);
	
    float fBrightness = dot(vBloom.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    vBloom *= Tonemap_ACES(fBrightness);
	
    vColor += vBloom;
    
	float4 vEffectOneBlend = g_EffectOneBlendTarget.Sample(LinearSampler, In.vTexcoord);
	float4 vEffectAlphaBlend = g_EffectAlphaBlendTarget.Sample(LinearSampler, In.vTexcoord);
	
	if (EPSILON < vEffectOneBlend.a)
	{
        vColor = float4(vEffectOneBlend.rgb * 1.f + vColor.rgb * 1.f, 1.f);
    }
	
    if (EPSILON < vEffectAlphaBlend.a)
    {
        vColor = float4(vEffectAlphaBlend.rgb * vEffectAlphaBlend.a + vColor.rgb * (1.f - vEffectAlphaBlend.a), 1.f);
    }
	
    return vColor;
}

technique11 DefaultTechnique
{
	pass PostProcess // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POSTPROCESS();
	}

	pass ScreenTone // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SCREENTONE();
    }

    pass BlendEffect // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLENDEFFECT();
    }
}




