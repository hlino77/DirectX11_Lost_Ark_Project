
#include "Client_Shader_Defines.hpp"


matrix		g_ViewMatrix, g_ProjMatrix;
texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
float3		g_vCamPosition;
float4		g_vEmissive;

sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
};

struct VS_IN
{
	float4		vTranslation : TEXCOORD0;
	float4		vDirection : TEXCOORD1;
	float4		vScale : TEXCOORD2;
	float4		vColor : TEXCOORD3;
	float4		vBlur : TEXCOORD4;
};


struct VS_OUT
{
	float3		vTranslation : TEXCOORD0;
	float3		vDirection : TEXCOORD1;
	float2		vScale : TEXCOORD2;
	float4		vColor : TEXCOORD3;
	float4		vBlur : TEXCOORD4;
};



struct GS_IN
{
	float3		vTranslation : TEXCOORD0;
	float3		vDirection : TEXCOORD1;
	float2		vScale : TEXCOORD2;
	float4		vColor : TEXCOORD3;
	float4		vBlur : TEXCOORD4;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vBlur : TEXCOORD2;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vTranslation = In.vTranslation.xyz;
	Out.vDirection = In.vDirection.xyz;
	Out.vScale = In.vScale.xy;
	Out.vColor = In.vColor;
	Out.vBlur = In.vBlur;

	return Out;
}




[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];


	/* 받아온 정점을 기준으로하여 사각형을 구성하기위한 정점 여섯개를 만들거야. */
	float3		vLook = g_vCamPosition - In[0].vTranslation;

	float3		vRight = normalize(cross(In[0].vDirection, vLook)) * In[0].vScale.x * 0.5f;
	float3		vUp = normalize(cross(vRight, vLook));

	vUp = vUp * dot(vUp, In[0].vDirection);

	matrix		matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);


	Out[0].vPosition = vector(In[0].vTranslation + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexcoord = float2(0.0f, 0.f);
	Out[0].vColor = In[0].vColor;
	Out[0].vBlur = In[0].vBlur;


	Out[1].vPosition = vector(In[0].vTranslation - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexcoord = float2(1.0f, 0.f);
	Out[1].vColor = In[0].vColor;
	Out[1].vBlur = In[0].vBlur;

	Out[2].vPosition = vector(In[0].vTranslation - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexcoord = float2(1.0f, 1.0f);
	Out[2].vColor = In[0].vColor;
	Out[2].vBlur = In[0].vBlur;

	Out[3].vPosition = vector(In[0].vTranslation + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexcoord = float2(0.0f, 1.0f);
	Out[3].vColor = In[0].vColor;
	Out[3].vBlur = In[0].vBlur;

	
	
	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vBlur : TEXCOORD2;
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

	Out.vDiffuse.a *= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).x;

	if (0.0f == Out.vDiffuse.a)
		discard;

	Out.vBlur = In.vBlur;
	Out.vBlur *= Out.vDiffuse.a;

	//saturate(Out.vDiffuse + g_vEmissive);

	//float4 vMask =;
	

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
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}