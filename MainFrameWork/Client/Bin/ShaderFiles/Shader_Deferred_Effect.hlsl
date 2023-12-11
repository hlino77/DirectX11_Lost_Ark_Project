
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;


texture2D		g_DiffuseTexture;
texture2D		g_ShadeTexture;
texture2D		g_BlurTexture;

texture2D		g_Texture;

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


struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};



PS_OUT PS_MAIN_EFFECT_DEFERRED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexcoord);

	Out.vColor = vShade;

	return Out;
}



PS_OUT PS_MAIN_EFFECT_SHADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

	//if (vDiffuse.a > 0.0f)
	//{
	//	Out.vColor = vDiffuse;
	//	return Out;
	//}
	//
	//vector		vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);

	Out.vColor = vDiffuse;

	return Out;
}



PS_OUT PS_MAIN_EFFECT_BLUR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);

	//if (vDiffuse.a > 0.0f)
	//{
	//	Out.vColor = vDiffuse;
	//	return Out;
	//}
	//
	//vector		vBlur = g_BlurTexture.Sample(LinearSampler, In.vTexcoord);

	//vDiffuse.a *= vDiffuse.a;

	Out.vColor = vDiffuse;

	return Out;
}





PS_OUT PS_MAIN_BLURX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	float4 vFinalPixel = float4(0.0f, 0.0f, 0.0f, 0.0f);

	int iHalfKernel = g_KernelSize / 2;

	for (int i = -iHalfKernel; i <= iHalfKernel; ++i) 
	{
		if (vFinalPixel.a < 1.0f)
		{
			float2 vTexcoord = In.vTexcoord;
			vTexcoord.x += i * g_PixelSize.x;

			vector vBloomPixel = g_BlurTexture.Sample(DefaultSampler, vTexcoord);

			if (vBloomPixel.a > 0.0f)
			{
				float fWeight = g_CenterWeight - (abs(i) * g_WeightAtt);
				vFinalPixel += vBloomPixel * fWeight;
			}
		}
	}

	if (vFinalPixel.a == 0.0f)
		discard;

	Out.vColor = vFinalPixel;

	//Out.vColor = g_BlurTexture.Sample(DefaultSampler, In.vTexcoord);

	return Out;
}


PS_OUT PS_MAIN_BLURY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	float4 vFinalPixel = float4(0.0f, 0.0f, 0.0f, 0.0f);

	int iHalfKernel = g_KernelSize / 2;

	for (int i = -iHalfKernel; i <= iHalfKernel; ++i)
	{
		if (vFinalPixel.a < 1.0f)
		{
			float2 vTexcoord = In.vTexcoord;
			vTexcoord.y += i * g_PixelSize.y;

			vector vBloomPixel = g_BlurTexture.Sample(DefaultSampler, vTexcoord);

			if (vBloomPixel.a > 0.0f)
			{
				float fWeight = g_CenterWeight - (abs(i) * g_WeightAtt);
				vFinalPixel += vBloomPixel * fWeight;
			}
		}
	}

	if (vFinalPixel.a == 0.0f)
		discard;


	Out.vColor = vFinalPixel;

	return Out;
}




technique11 DefaultTechnique
{
	pass Deferred_Blend
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT_DEFERRED();
	}

	pass EffectShade
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT_SHADE();
	}


	pass BlurX
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLURX();
	}

	pass BlurY
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLURY();
	}


	pass EffectShadeBlur
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlendTest, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT_BLUR();
	}
}




