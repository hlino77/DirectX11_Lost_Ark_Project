#include "Client_Shader_Defines.hlsl"

matrix		g_ViewMatrix, g_ProjMatrix;
Texture2D	g_MaskTexture;
float3		g_vCamPosition;
float4		g_vEmissive;
float4		g_vColor;
float4		g_vBlurColor;

sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
};

struct VS_IN
{
	float4		vStartRight : TEXCOORD0;
	float4		vStartLeft : TEXCOORD1;
	float4		vPrevRight : TEXCOORD2;
	float4		vPrevLeft : TEXCOORD3;
};


struct VS_OUT
{
	float3		vStartRight : TEXCOORD0;
	float3		vStartLeft : TEXCOORD1;
	float3		vPrevRight : TEXCOORD2;
	float3		vPrevLeft : TEXCOORD3;
};



struct GS_IN
{
	float3		vStartRight : TEXCOORD0;
	float3		vStartLeft : TEXCOORD1;
	float3		vPrevRight : TEXCOORD2;
	float3		vPrevLeft : TEXCOORD3;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vStartRight = In.vStartRight.xyz;
	Out.vStartLeft = In.vStartLeft.xyz;
	Out.vPrevRight = In.vPrevRight.xyz;
	Out.vPrevLeft = In.vPrevLeft.xyz;

	return Out;
}




[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];


	/* 받아온 정점을 기준으로하여 사각형을 구성하기위한 정점 여섯개를 만들거야. */

	matrix		matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);


	Out[0].vPosition = vector(In[0].vStartRight, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);

	Out[1].vPosition = vector(In[0].vStartLeft, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);

	Out[2].vPosition = vector(In[0].vPrevLeft, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);

	Out[3].vPosition = vector(In[0].vPrevRight, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);



	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[1]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[3]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();
}






struct PS_IN
{
	float4		vPosition : SV_POSITION;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vBlur : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_vColor;

	if (0.0f == Out.vDiffuse.a)
		discard;



	Out.vBlur = g_vBlurColor;
	Out.vBlur *= g_vColor.a;
	
	//saturate(Out.vDiffuse + g_vEmissive);

	//float4 vMask =;
	

	/*if (0.01f >= Out.vDiffuse.a)
		discard;*/


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