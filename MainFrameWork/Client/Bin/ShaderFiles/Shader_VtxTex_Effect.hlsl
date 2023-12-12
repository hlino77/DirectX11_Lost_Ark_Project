#include "Client_Shader_Defines.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
float4		g_vColor;
float4		g_vBlurColor;

Texture2D	g_MaskTexture;
Texture2D	g_MaskTexture2;
float2		g_MaskUV;


float2 g_vUVIndex;
float2 g_vMaxCount;


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


VS_OUT VS_UV(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexUV = float2((g_vUVIndex.x + In.vTexUV.x) / g_vMaxCount.x, (g_vUVIndex.y + In.vTexUV.y) / g_vMaxCount.y);


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
	float4		vBlur : SV_TARGET1;
};

struct PS_OUT_MULTIMASK
{
	float4		vColor : SV_TARGET0;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	float fAlpha = vMask.x;

	if (fAlpha == 0.0f)
		discard;

	Out.vColor = g_vColor;
	Out.vColor.a *= fAlpha;
	Out.vBlur = g_vBlurColor;

	
	return Out;
}


PS_OUT_MULTIMASK PS_NONEBLUR(PS_IN In)
{
	PS_OUT_MULTIMASK		Out = (PS_OUT_MULTIMASK)0;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	float fAlpha = vMask.x;

	if (fAlpha == 0.0f)
		discard;

	Out.vColor = g_vColor;
	Out.vColor.a *= fAlpha;

	return Out;
}




PS_OUT_MULTIMASK PS_MULTIMASK(PS_IN In)
{
	PS_OUT_MULTIMASK		Out = (PS_OUT_MULTIMASK)0;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	float4 vMask2 = g_MaskTexture2.Sample(DefaultSampler, In.vTexUV);

	float fAlpha = vMask.x + vMask2.x;

	//saturate(fAlpha);
	if (fAlpha == 0.0f)
		discard;

	Out.vColor = g_vColor;
	Out.vColor.a = fAlpha;

	return Out;
}




PS_OUT_MULTIMASK PS_TELEPORT(PS_IN In)
{
	PS_OUT_MULTIMASK		Out = (PS_OUT_MULTIMASK)0;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	if (vMask.x <= 0.1f)
		discard;

	float4 vMask2 = g_MaskTexture2.Sample(DefaultSampler, In.vTexUV);

	if (vMask2.x <= 0.1f)
		discard;

	Out.vColor = g_vColor;
	Out.vColor.a *= vMask.x;

	return Out;
}




BlendState BS_BaseDest
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Dest_Alpha;
	BlendOp = Add;
	SrcBlendAlpha = Src_Alpha;
	DestBlendAlpha = Dest_Alpha;
	BlendOpAlpha = Add;
};



technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlendDest, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass MultiMaskPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BaseDest, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MULTIMASK();
	}


	pass UVPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BaseDest, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_UV();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}


	pass NONEBLUR
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BaseDest, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NONEBLUR();
	}


	pass TeleportPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BaseDest, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TELEPORT();
	}

}