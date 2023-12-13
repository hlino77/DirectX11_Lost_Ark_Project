#include "Client_Shader_Defines.hlsl"

matrix		g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_NormalTexture;
Texture2D	g_MaskTexture;

sampler LightningSampler = sampler_state {

	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/

	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
	float4		vColor : TEXCOORD5;
	float4		vBlur : TEXCOORD6;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;

	float4		vColor : TEXCOORD2;
	float4		vBlur : TEXCOORD3;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	In.vTexUV.x += In.vRight.a;

	In.vRight.a = 0.0f;

	matrix matWorld = matrix(In.vRight, In.vUp, In.vLook, In.vTranslation);


	matWV = mul(matWorld, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	
	Out.vColor = In.vColor;
	Out.vBlur = In.vBlur;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;

	float4		vColor : TEXCOORD2;
	float4		vBlur : TEXCOORD3;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vBlur : SV_TARGET1;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = In.vColor;

	float4 vMask = g_MaskTexture.Sample(LightningSampler, In.vTexUV);

	Out.vDiffuse *= vMask.x;

	if (0.0f >= Out.vDiffuse.a)
		discard;


	Out.vBlur = In.vBlur * Out.vDiffuse.a;

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

}