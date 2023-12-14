#include "Client_Shader_Light.hlsl"

Texture2D	g_MaskTexture;
Texture2D	g_MaskTexture2;
Texture2D	g_DisolveTexture;

matrix		g_ShadowViewMatrix;

float4		g_vColor;
float4		g_vBlurColor;

float		g_fUVX;
float		g_fUVY;
float		g_fAlphaWeight;
float		g_fDiscardAlpha;

sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
	AddressU = Wrap;
	AddressV = Wrap;
};


sampler MirrorSampler = sampler_state {

	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
	AddressU = mirror;
	AddressV = mirror;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vNormal = mul(float4(In.vNormal, 0.f), WorldMatrix);
    //Out.vWorldPos = mul(float4(In.vPosition, 1.f), WorldMatrix);
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix)).xyz;
    Out.vProjPos = Out.vPosition;
	
	return Out;
}

VS_OUT VS_MODELEFFECT_UV(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	In.vTexUV.x += g_fUVX;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
    float4		vSpecular : SV_TARGET3;
};

struct PS_OUT_SHADOWDEPTH
{
	float4		vShadowDepth : SV_TARGET0;
};

struct PS_OUT_SPAWNER
{
	float4		vDiffuse : SV_TARGET0;
	float4		vBlur : SV_TARGET1;
};



struct PS_OUT_MODELEFFECT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vBlur : SV_TARGET1;
};


struct PS_OUT_METEORBOTTOM
{
	float4		vDiffuse : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = (vector)1.f;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.01f >= Out.vDiffuse.a)
		discard;

	//// -1 ~ 1
	////  0 ~ 1
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f) ;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
	Out.vSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return Out;	
}

PS_OUT PS_TANGENT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = (vector)1.f;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.2f >= Out.vDiffuse.a)
		discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

	//// -1 ~ 1
	////  0 ~ 1
    Out.vNormal = In.vNormal;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
    Out.vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

VS_OUT VS_SHADOW(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;
	
	float4 vPosition = float4(In.vPosition, 1.f);
	vPosition = mul(vPosition, WorldMatrix);
	vPosition = mul(vPosition, g_ShadowViewMatrix);
	vPosition = mul(vPosition, ProjMatrix);

	Out.vPosition = vPosition;
	Out.vProjPos = Out.vPosition;

	return Out;
}

PS_OUT_SHADOWDEPTH PS_SHADOW(PS_IN In)
{
	PS_OUT_SHADOWDEPTH		Out = (PS_OUT_SHADOWDEPTH)0;

	float fDepth = In.vProjPos.z / In.vProjPos.w;

	Out.vShadowDepth = vector(fDepth, 0.0f, 0.0f, 1.0f);
	//Out.vShadowDepth = vector(1.0f, 0.0f, 0.0f, 1.0f);

	return Out;
}

PS_OUT_SPAWNER PS_SPAWNER(PS_IN In)
{
	PS_OUT_SPAWNER		Out = (PS_OUT_SPAWNER)0;

	Out.vDiffuse = g_vColor;
	Out.vDiffuse.a *= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).x;

	if (Out.vDiffuse.a <= 0.01f)
		discard;

	Out.vBlur = g_vBlurColor;
	Out.vBlur.a = g_vColor.a;

	return Out;
}

PS_OUT_MODELEFFECT PS_MODELEFFECT(PS_IN In)
{
	PS_OUT_MODELEFFECT		Out = (PS_OUT_MODELEFFECT)0;

	Out.vDiffuse = g_vColor;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	vMask *= g_fAlphaWeight;

	Out.vDiffuse.a *= vMask.x;

	if (Out.vDiffuse.a <= g_fDiscardAlpha)
		discard;

	Out.vBlur = g_vBlurColor * Out.vDiffuse.a;


	return Out;
}


PS_OUT_MODELEFFECT PS_MODELEFFECTDISCARD(PS_IN In)
{
	PS_OUT_MODELEFFECT		Out = (PS_OUT_MODELEFFECT)0;

	Out.vDiffuse = g_vColor;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	vMask *= g_fAlphaWeight;

	Out.vDiffuse.a *= vMask.x;

	if (Out.vDiffuse.a <= 0.00f)
		discard;

	Out.vBlur = g_vBlurColor * Out.vDiffuse.a;
	
	return Out;
}

PS_OUT_METEORBOTTOM PS_MODELEFFECT_NONEBLUR(PS_IN In)
{
	PS_OUT_METEORBOTTOM		Out = (PS_OUT_METEORBOTTOM)0;

	Out.vDiffuse = g_vColor;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a *= vMask.x;

	if (Out.vDiffuse.a < 0.9f)
		discard;


	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass ShadowDepthPass // 1
	{
		SetRasterizerState(RS_ShadowDepth);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW();
	}
	
	pass SpawnerPass // 2
	{
		SetRasterizerState(RS_ShadowDepth);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SPAWNER();
	}
	
	pass ModelEffectPass // 3
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MODELEFFECT();
	}

	pass ModelMeteorSmokePass // 4
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MODELEFFECT_UV();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MODELEFFECT_NONEBLUR();
	}

	pass ModelEffectDiscardPass // 5
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MODELEFFECTDISCARD();
	}
	
	pass TangentPass // 6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TANGENT();
	}
}