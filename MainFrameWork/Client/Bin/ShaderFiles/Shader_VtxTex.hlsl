#include "Client_Shader_Defines.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_DepthTexture;
Texture2D	g_TextBoxTexture;

float		g_Alpha = 1.0f;
float4		g_Color;
Texture2D	g_MaskTexture;
Texture2D	g_MaskTexture2;
float2		g_vMaskUV;
float2		g_vScaleUV;



sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	
	return Out;
}


VS_OUT VS_TEXTBOX(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV * g_vScaleUV;

	return Out;
}

struct VS_OUT_SOFTEFFECT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};


VS_OUT_SOFTEFFECT VS_MAIN_SOFTEFFECT(VS_IN In)
{
	VS_OUT_SOFTEFFECT		Out = (VS_OUT_SOFTEFFECT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;	
}


PS_OUT PS_TEXTBOX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_TextBoxTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}


struct PS_IN_SOFTEFFECT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFTEFFECT(PS_IN_SOFTEFFECT In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float2		vProjPos = In.vProjPos.xy / In.vProjPos.w;	

	float2		vTexUV;

	vTexUV.x = vProjPos.x * 0.5f + 0.5f;
	vTexUV.y = vProjPos.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);	
	float		fOldZ = vDepthDesc.y * 300.f;

	Out.vColor.a = Out.vColor.a * saturate(fOldZ - In.vProjPos.w);

	return Out;
}



PS_OUT PS_MAIN_COLORTEX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);

	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}


PS_OUT PS_MAIN_MASKED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);


	In.vTexUV += g_vMaskUV;

	float4 vMaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_Alpha;

	Out.vColor.a *= vMaskColor.a;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}


PS_OUT PS_SKILL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float4 vMaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	if (vMaskColor.x != 1.0f)
		discard;

	In.vTexUV += g_vMaskUV;

	float4 vShadowMask = g_MaskTexture2.Sample(DefaultSampler, In.vTexUV);
	
	if (vShadowMask.x == 1.0f)
		Out.vColor.xyz *= 0.5f;
		

	return Out;
}

PS_OUT PS_MAIN_PIX_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Color;

	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}



technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass SoftEffect
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SOFTEFFECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFTEFFECT();
	}

	pass ColorTex
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COLORTEX();
	}
	
	pass Masked
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MASKED();
	}


	pass SkillPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SKILL();
	}


	pass TextBoxPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_TEXTBOX();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TEXTBOX();
	}


	pass PixColorPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_PIX_COLOR();
	}
}