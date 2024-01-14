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
float		g_fRatio;
float		g_fRoughness;
float		g_PI = 3.141592f;

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

	if (0.3f >= Out.vColor.a)
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
	Out.vColor = pow(Out.vColor, 1 / 2.2);
	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}


PS_OUT PS_MAIN_MASKED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Color;

	float4 vMaskColor = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a *= g_Alpha;
	Out.vColor.a *= vMaskColor.x;

	if (0.01f >= Out.vColor.a)
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

PS_OUT PS_MAIN_TEXTURE_CUTX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);
	Out.vColor = pow(Out.vColor, 1.f / 2.2f);
	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	if (g_fRatio <= In.vTexUV.x)
		discard;

	return Out;
}

PS_OUT PS_MAIN_TEXTURE_CUTX_NOGAMMA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);
	Out.vColor.a *= g_Alpha;
	if (0.0f >= Out.vColor.a)
		discard;

	if (g_fRatio <= In.vTexUV.x)
		discard;

	return Out;
}
PS_OUT PS_MAIN_TEXTURE_CUTY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);
	Out.vColor = pow(Out.vColor, 1.f / 2.2f);
	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	if ((1.f - g_fRatio) >= In.vTexUV.y)
		discard;

	return Out;
}

PS_OUT PS_MAIN_COOLTIME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);
	Out.vColor.a *= g_Alpha;

	float2 fTempUV = In.vTexUV * 2.0f - 1.0f;

	if (0.0f >= Out.vColor.a)
		discard;

	float fAngle = atan2(-fTempUV.x, fTempUV.y);

	if ((-g_PI < fAngle) && (g_fRatio > fAngle))
		Out.vColor = pow(Out.vColor, 1.f / 2.2f);
	else
		Out.vColor.rgb *= 0.4f;

	return Out;
}

PS_OUT PS_MAIN_COLORFRAME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);
	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	float2	UVOffset = float2(0.05, 0.05);
	float2	UVScale = float2(0.8, 0.8);

	bool isBorder = (In.vTexUV.x < UVOffset.x || In.vTexUV.x > 1.0 - UVOffset.x 
		|| In.vTexUV.y < UVOffset.y || In.vTexUV.y > 1.0 - UVOffset.y);

	Out.vColor = isBorder ? float4(0.8, 0.8, 0.0, 0.6f) : pow(Out.vColor, 1.f / 2.2f);

	return Out;
}

PS_OUT PS_MAIN_WRGAUGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);

	float2 Distance = abs(In.vTexUV - 0.5f);
	float fAlpha = (2.f * g_Alpha) - (saturate(length(Distance)));
	
	Out.vColor.a *= g_Alpha;

	float2 fTempUV = In.vTexUV * 2.0f - 1.0f;

	if (0.0f >= Out.vColor.a)
		discard;

	float fAngle = atan2(-fTempUV.x, fTempUV.y);

	if ((-g_PI < fAngle) && (g_fRatio < fAngle))
		return Out;//Out.vColor = pow(Out.vColor, 1.f / 2.2f);
	else
		Out.vColor.rgb *= 0.f;

	return Out;
}

PS_OUT PS_MAIN_WRSHINE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);

	float2 Distance = abs(In.vTexUV - 0.5f);

	float fAlpha = (2.f * g_Alpha) - (saturate(length(Distance)));
	Out.vColor.a *= fAlpha;

	if (0.0f >= Out.vColor.a)
		discard;

	Out.vColor = pow(Out.vColor, 1.f / 2.2f);

	return Out;
}

PS_OUT PS_MAIN_WREYESHINE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = saturate(g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * g_Color);

	float2 Distance = abs(In.vTexUV - 0.5f);

	float fAlpha = (2.f * g_Alpha) - (saturate(length(Distance)));
	Out.vColor.a *= fAlpha;

	if (0.0f >= Out.vColor.a)
		discard;

	return Out;
}
	
PS_OUT PS_MAIN_SATURATION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;

	if (0.0f >= Out.vColor.a)
		discard;

	Out.vColor.rgb *= g_fRoughness;

	return Out;
}

PS_OUT PS_MAIN_ORANGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;
	float2 distanceFromCenter = abs(In.vTexUV - 0.5);
	float roughness = 0.5;

	float fAlphaTemp = Out.vColor.r;
	//Out.vColor.a = fAlphaTemp;
	//float alpha = saturate(Out.vColor.a - smoothstep(0, roughness, length(distanceFromCenter)));
	Out.vColor.a *= fAlphaTemp;

	if( 0.1f >= Out.vColor.a)
		discard;

	//Out.vColor.rgb = float3(1.f, 1.f, 1.f);

	float3 vOrangeColor = float3(0.83f, 0.55f, 0.26f);
	vOrangeColor *= g_Color;
	Out.vColor.rgb = vOrangeColor;
	//float roughFactor = saturate(1.0 - smoothstep(0, roughness, length(distanceFromCenter)));
	//float3 interpolatedColor = lerp(Out.vColor.rgb, vOrangeColor, roughFactor);
	//Out.vColor.rgb = interpolatedColor;

	return Out;
}

PS_OUT PS_MAIN_DISCARDBLACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	float fAlphaTemp = Out.vColor.r;
	Out.vColor.a *= fAlphaTemp;

	if (0.1f >= Out.vColor.a)
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

	pass PixTexturePassX
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEXTURE_CUTX();
	}

	pass CoolTimePass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COOLTIME();
	}

	pass PS_MAIN_WRGAUGE
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WRGAUGE();
	}

	pass PS_MAIN_WRSHINE//Num.10
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WRSHINE();
	}

	pass PS_MAIN_WREYESHINE
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_WREYESHINE();
	}

	pass PixTexturePassY
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEXTURE_CUTY();
	}

	pass PixTextureSaturation
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SATURATION();
	}

	pass PixTextureRoughOrange//No.14
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ORANGE();
	}

	pass PixTextureDeleteBlack
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISCARDBLACK();
	}

	pass PixTexturePassX_NoGamma
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEXTURE_CUTX_NOGAMMA();
	}
	
}