
#include "Client_Shader_Defines.hpp"


matrix		g_ViewMatrix, g_ProjMatrix;
texture2D	g_DiffuseTexture;
texture2D	g_MaskTexture;
texture2D	g_MaskTexture2;
texture2D	g_DisolveTexture;

float3		g_vCamPosition;
float4		g_vEmissive;
float		g_fDiscardAlpha;
float		g_fAlphaWeight;


sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
};

struct VS_IN
{
	float4		vTranslation : TEXCOORD0;
	float4		vUV : TEXCOORD1;
	float4		vScale : TEXCOORD2;
	float4		vColor : TEXCOORD3;
	float4		vBlur : TEXCOORD4;
};


struct VS_OUT
{
	float3		vTranslation : TEXCOORD0;
	float4		vUV : TEXCOORD1;
	float2		vScale : TEXCOORD2;
	float4		vColor : TEXCOORD3;
	float4		vBlur : TEXCOORD4;
};



struct GS_IN
{
	float3		vTranslation : TEXCOORD0;
	float4		vUV : TEXCOORD1;
	float2		vScale : TEXCOORD2;
	float4		vColor : TEXCOORD3;
	float4		vBlur : TEXCOORD4;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vColor : TEXCOORD2;
	float4		vBlur : TEXCOORD3;
};


struct GS_OUT_FIRE
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float2		vOriginUV : TEXCOORD1;
	float4		vColor : TEXCOORD2;
	float4		vBlur : TEXCOORD3;
};




VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Out.vTranslation = In.vTranslation.xyz;
	Out.vUV = In.vUV;
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
	float3		vUp = float3(0.0f, 1.0f, 0.0f);

	float3		vRight = normalize(cross(vUp, vLook)) * In[0].vScale.x;
	vUp = normalize(cross(vLook, vRight)) * In[0].vScale.y;


	matrix		matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	//Out.vTexUV = float2((g_vUVIndex.x + In.vTexUV.x) / g_vMaxCount.x, (g_vUVIndex.y + In.vTexUV.y) / g_vMaxCount.y);

	Out[0].vPosition = vector(In[0].vTranslation + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexcoord = float2((In[0].vUV.x + 0.0f) / In[0].vUV.z, (In[0].vUV.y + 0.0f) / In[0].vUV.a);
	Out[0].vColor = In[0].vColor;
	Out[0].vBlur = In[0].vBlur;


	Out[1].vPosition = vector(In[0].vTranslation - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexcoord = float2((In[0].vUV.x + 1.0f) / In[0].vUV.z, (In[0].vUV.y + 0.0f) / In[0].vUV.a);
	Out[1].vColor = In[0].vColor;
	Out[1].vBlur = In[0].vBlur;

	Out[2].vPosition = vector(In[0].vTranslation - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexcoord = float2((In[0].vUV.x + 1.0f) / In[0].vUV.z, (In[0].vUV.y + 1.0f) / In[0].vUV.a);
	Out[2].vColor = In[0].vColor;
	Out[2].vBlur = In[0].vBlur;

	Out[3].vPosition = vector(In[0].vTranslation + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexcoord = float2((In[0].vUV.x + 0.0f) / In[0].vUV.z, (In[0].vUV.y + 1.0f) / In[0].vUV.a);
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



[maxvertexcount(20)]
void GS_FIRE(point GS_IN In[1], inout TriangleStream<GS_OUT_FIRE> OutStream)
{
	GS_OUT_FIRE		Out[4];


	/* 받아온 정점을 기준으로하여 사각형을 구성하기위한 정점 여섯개를 만들거야. */
	float3		vLook = g_vCamPosition - In[0].vTranslation;
	float3		vUp = float3(0.0f, 1.0f, 0.0f);

	float3		vRight = normalize(cross(vUp, vLook)) * In[0].vScale.x;
	vUp = normalize(cross(vLook, vRight)) * In[0].vScale.y;


	matrix		matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	//Out.vTexUV = float2((g_vUVIndex.x + In.vTexUV.x) / g_vMaxCount.x, (g_vUVIndex.y + In.vTexUV.y) / g_vMaxCount.y);

	Out[0].vPosition = vector(In[0].vTranslation + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexcoord = float2((In[0].vUV.x + 0.0f) / In[0].vUV.z, (In[0].vUV.y + 0.0f) / In[0].vUV.a);
	Out[0].vOriginUV = float2(0.0f, 0.0f);
	Out[0].vColor = In[0].vColor;
	Out[0].vBlur = In[0].vBlur;


	Out[1].vPosition = vector(In[0].vTranslation - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexcoord = float2((In[0].vUV.x + 1.0f) / In[0].vUV.z, (In[0].vUV.y + 0.0f) / In[0].vUV.a);
	Out[1].vOriginUV = float2(1.0f, 0.0f);
	Out[1].vColor = In[0].vColor;
	Out[1].vBlur = In[0].vBlur;

	Out[2].vPosition = vector(In[0].vTranslation - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexcoord = float2((In[0].vUV.x + 1.0f) / In[0].vUV.z, (In[0].vUV.y + 1.0f) / In[0].vUV.a);
	Out[2].vOriginUV = float2(1.0f, 1.0f);
	Out[2].vColor = In[0].vColor;
	Out[2].vBlur = In[0].vBlur;

	Out[3].vPosition = vector(In[0].vTranslation + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexcoord = float2((In[0].vUV.x + 0.0f) / In[0].vUV.z, (In[0].vUV.y + 1.0f) / In[0].vUV.a);
	Out[3].vOriginUV = float2(0.0f, 1.0f);
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
	float4		vColor : TEXCOORD2;
	float4		vBlur : TEXCOORD3;
};


struct PS_IN_FIRE
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		vOriginUV : TEXCOORD1;
	float4		vColor : TEXCOORD2;
	float4		vBlur : TEXCOORD3;
};



struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = In.vColor;

	Out.vDiffuse.a *= g_MaskTexture.Sample(DefaultSampler, In.vTexUV).x;

	if (g_fDiscardAlpha >= Out.vDiffuse.a)
		discard;

	//saturate(Out.vDiffuse + g_vEmissive);

	//float4 vMask =;
	

	return Out;	
}



PS_OUT PS_FIRE(PS_IN_FIRE In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = In.vColor;

	Out.vDiffuse.a *= g_MaskTexture2.Sample(DefaultSampler, In.vTexUV).x;

	if (g_fDiscardAlpha >= Out.vDiffuse.a)
		discard;

	float fDisolve = g_DisolveTexture.Sample(DefaultSampler, In.vOriginUV).x;

	if (fDisolve <= In.vBlur.x)
		discard;


	float4 vDiffuse = float4(1.0f, 1.0f, 0.0f, 1.0f) * g_MaskTexture.Sample(DefaultSampler, In.vTexUV).x;
		
	Out.vDiffuse += vDiffuse;


	//saturate(Out.vDiffuse + g_vEmissive);

	//float4 vMask =;


	return Out;
}



PS_OUT PS_DISOLVE(PS_IN_FIRE In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = In.vColor;

	Out.vDiffuse.a *= g_MaskTexture.Sample(DefaultSampler, In.vTexUV).x;

	if (g_fDiscardAlpha >= Out.vDiffuse.a)
		discard;

	float fDisolve = g_DisolveTexture.Sample(DefaultSampler, In.vOriginUV).x;

	if (fDisolve <= In.vBlur.x)
		discard;

	//saturate(Out.vDiffuse + g_vEmissive);

	//float4 vMask =;


	return Out;
}


PS_OUT PS_EXPLOSION(PS_IN_FIRE In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = 1.0f;

	Out.vDiffuse.a *= g_MaskTexture2.Sample(DefaultSampler, In.vTexUV).x;

	Out.vDiffuse.a *= 5.0f;

	if (g_fDiscardAlpha >= Out.vDiffuse.a)
		discard;

	float fDisolve = g_DisolveTexture.Sample(DefaultSampler, In.vOriginUV).x;

	if (fDisolve <= In.vBlur.x)
		discard;


	return Out;
}





technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass FirePass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_FIRE();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_FIRE();
	}


	pass DisolvePass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_FIRE();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DISOLVE();
	}


	pass ExplosionPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_FIRE();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_EXPLOSION();
	}
}