
#include "Client_Shader_Defines.hlsl"
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_NormalTexture;
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
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	
	return Out;
}




VS_OUT VS_MODELEFFECT_UV(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	In.vTexUV.x += g_fUVX;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

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
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
	float4		vModelNormal : SV_TARGET3;
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

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	//float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	//float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	//
	//vNormal = normalize(mul(vNormal, WorldMatrix));

	if (0.01f >= Out.vDiffuse.a)
		discard;

	//// -1 ~ 1
	////  0 ~ 1
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f) ;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
	Out.vModelNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return Out;	
}



PS_OUT PS_TANGENT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = (vector)1.f;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);



	if (0.01f >= Out.vDiffuse.a)
		discard;

	vector	vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	float3	vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3		WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
	vNormal = normalize(mul(vNormal, WorldMatrix));

	//// -1 ~ 1
	////  0 ~ 1
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
	Out.vModelNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return Out;
}



VS_OUT VS_SHADOW(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	float4 vPosition = float4(In.vPosition, 1.f);
	vPosition = mul(vPosition, g_WorldMatrix);
	vPosition = mul(vPosition, g_ShadowViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);


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



PS_OUT_METEORBOTTOM PS_METEORBOTTOM(PS_IN In)
{
	PS_OUT_METEORBOTTOM		Out = (PS_OUT_METEORBOTTOM)0;

	Out.vDiffuse = g_vColor;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);


	Out.vDiffuse.a *= vMask.x * g_fAlphaWeight;

	if (Out.vDiffuse.a <= 0.00f)
		discard;

	Out.vDiffuse.x += vMask.x * Out.vDiffuse.a * 0.3f;
	Out.vDiffuse.y += vMask.x * Out.vDiffuse.a * 0.2f;
	//Out.vDiffuse.z += vMask.x * 1.0f;
	
	float4 vEmissive = float4(1.0f, 1.0f, 1.0f, 0.0f);

	Out.vDiffuse += vEmissive * vMask.x * Out.vDiffuse.a * 0.4f;

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






PS_OUT_METEORBOTTOM PS_FIREBALL(PS_IN In)
{
	PS_OUT_METEORBOTTOM		Out = (PS_OUT_METEORBOTTOM)0;

	Out.vDiffuse = g_vColor;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);


	In.vTexUV.x += g_fUVX;
	In.vTexUV.y += g_fUVX;
	float4 vMask2 = g_MaskTexture2.Sample(DefaultSampler, In.vTexUV);


	Out.vDiffuse += float4(1.0f, 1.0f, 0.0f, 0.0f) * vMask2.x;


	return Out;
}




PS_OUT_MODELEFFECT PS_FIREBALLPLANE(PS_IN In)
{
	PS_OUT_MODELEFFECT		Out = (PS_OUT_MODELEFFECT)0;

	float2 vUV = In.vTexUV;
	vUV.x += g_fUVY * 0.1f;
	vUV.x *= 2.0f;
	vUV.y += g_fUVY;

	float4 vMask2 = g_MaskTexture2.Sample(DefaultSampler, vUV);


	if (vMask2.x <= g_fAlphaWeight)
		discard;


	Out.vDiffuse = g_vColor;


	float2 vUVX = In.vTexUV;
	vUVX.x += g_fUVX;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, vUVX);

	Out.vDiffuse.a *= vMask.x;

	if (Out.vDiffuse.a <= 0.0f)
		discard;


	Out.vBlur = Out.vDiffuse;


	return Out;
}




PS_OUT_MODELEFFECT PS_SYURIKEN(PS_IN In)
{
	PS_OUT_MODELEFFECT		Out = (PS_OUT_MODELEFFECT)0;


	float2 vDisolveUV = In.vTexUV;
	vDisolveUV.x *= 10.0f;

	float fAlpha = g_DisolveTexture.Sample(DefaultSampler, vDisolveUV).x;

	if (fAlpha <= g_fAlphaWeight)
		discard;

	Out.vDiffuse = g_vColor;

	In.vTexUV.x *= 2.0f;
	In.vTexUV.y += g_fUVX;

	float4 vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse += float4(0.0001f, 0.5f, 0.0f, 0.0f) * vMask.x;

	Out.vBlur = g_vBlurColor;

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


	pass ShadowDepthPass
	{
		SetRasterizerState(RS_ShadowDepth);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_SHADOW();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW();
	}
	

	pass SpawnerPass
	{
		SetRasterizerState(RS_ShadowDepth);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SPAWNER();
	}
	

	pass ModelEffectPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MODELEFFECT();
	}

	pass ModelMeteorBottomPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_METEORBOTTOM();
	}


	pass ModelMeteorSmokePass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MODELEFFECT_UV();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MODELEFFECT_NONEBLUR();
	}


	pass ModelFireBallPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MODELEFFECT_UV();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FIREBALL();
	}


	pass ModelFirePlanePass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FIREBALLPLANE();
	}
	

	pass ModelEffectDiscardPass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MODELEFFECTDISCARD();
	}
	
	pass ModelSyurikenSpherePass
	{
		SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_BlendDestAlpha, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SYURIKEN();
	}


	pass TangentPass
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TANGENT();
	}

}