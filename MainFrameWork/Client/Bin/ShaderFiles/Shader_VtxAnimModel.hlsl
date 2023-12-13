#include "Client_Shader_Defines.hlsl"

float		g_BlendRatio;
float4		g_vBlurColor;
float		g_fAlpha;

matrix		g_BoneMatrices[800];

Texture2D	g_DiffuseTexture;
Texture2D	g_NormalTexture;

sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float3		vTangent : TANGENT;
	float2		vTexUV : TEXCOORD0;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal :	BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;

	matWVP = mul(WorldMatrix, ViewProj);

	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	float4x4	vMatX = g_BoneMatrices[In.vBlendIndex.x];
	float4x4	vMatY = g_BoneMatrices[In.vBlendIndex.y];
	float4x4	vMatZ = g_BoneMatrices[In.vBlendIndex.z];
	float4x4	vMatW = g_BoneMatrices[In.vBlendIndex.w];

	float4x4	BoneMatrix = vMatX * In.vBlendWeight.x +
		vMatY * In.vBlendWeight.y +
		vMatZ * In.vBlendWeight.z +
		vMatW * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT VS_TANGENT(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;

	matWVP = mul(WorldMatrix, ViewProj);

	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);


	float4x4	vMatX = g_BoneMatrices[In.vBlendIndex.x];
	float4x4	vMatY = g_BoneMatrices[In.vBlendIndex.y];
	float4x4	vMatZ = g_BoneMatrices[In.vBlendIndex.z];
	float4x4	vMatW = g_BoneMatrices[In.vBlendIndex.w];


	float4x4	BoneMatrix = vMatX * In.vBlendWeight.x +
		vMatY * In.vBlendWeight.y +
		vMatZ * In.vBlendWeight.z +
		vMatW * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix));
	Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent));

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal :	BINORMAL;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
	float4		vModelNormal : SV_TARGET3;
};


struct PS_OUT_NONEOUTLINE
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};


struct PS_OUT_EFFECT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vBlur : SV_TARGET1;
};

struct PS_OUT_SHADOW
{
	float4		vDepth : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vModelNormal = Out.vNormal;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);

	if (0.0f == Out.vDiffuse.a)
		discard;

	return Out;	
}

PS_OUT_NONEOUTLINE PS_NONEOUTLINE(PS_IN In)
{
	PS_OUT_NONEOUTLINE		Out = (PS_OUT_NONEOUTLINE)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);

	if (0.0f == Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT_SHADOW PS_SHADOWDEPTH(PS_IN In)
{
	PS_OUT_SHADOW		Out = (PS_OUT_SHADOW)0;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f, 0.0f);
	//Out.vDepth = vector(1.0f, 0.0f, 0.0f, 1.0f);
	return Out;
}

PS_OUT_EFFECT PS_EFFECT(PS_IN In)
{
	PS_OUT_EFFECT		Out = (PS_OUT_EFFECT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vDiffuse.a *= g_fAlpha;
	Out.vBlur = g_vBlurColor * Out.vDiffuse.a;

	if (0.0f == Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT PS_TANGENT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (0.0f == Out.vDiffuse.a)
		discard;

	vector	vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	float3	vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3		WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vModelNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);

	

	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass EffectPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EFFECT();
	}


	pass ModelNormalTexPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_TANGENT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TANGENT();
	}


	pass ShadowPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOWDEPTH();
	}


	pass NoneOutLinePass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NONEOUTLINE();
	}

}