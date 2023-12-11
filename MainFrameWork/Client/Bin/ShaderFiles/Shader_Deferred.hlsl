
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;
matrix			g_LightViewMatrix, g_LightProjMatrix;
matrix			g_StaticLightViewMatrix;


vector			g_vLightDir;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;
vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);

texture2D		g_NormalTexture;
texture2D		g_DiffuseTexture;
texture2D		g_ShadeTexture;
texture2D		g_DepthTexture;
texture2D		g_ModelNormalTexture;
texture2D		g_BlurTexture;
texture2D		g_ShadowDepthTexture;
texture2D		g_StaticShadowDepthTexture;

float3			g_CamPosition;


texture2D		g_Texture;


float2  g_PixelSize;
int     g_KernelSize; //커널은 이미지 처리에서 필터 또는 윈도우라고도 불리는 작은 행렬 또는 마스크
float   g_CenterWeight;
float	g_WeightAtt;

float	g_fBias;



float	g_fShadowSizeRatio;
float	g_fStaticShadowSizeRatio;
float2	g_vWinSize;


sampler DefaultSampler = sampler_state {

	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
};


sampler ShadowSampler = sampler_state {
	Filter = MIN_MAG_MIP_POINT;
};



struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};



VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};


struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};


PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4	vShade : SV_TARGET0;	
};


//PCF
float PCF_ShadowCalculation(float4 fragPosLightSpace, float fBias)
{
	// perform perspective divide
	float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// Transform to [0,1] range
	projCoords.x = projCoords.x * 0.5f + 0.5f;
	projCoords.y = projCoords.y * -0.5f + 0.5f;

	
	float2 vTexCoords = saturate(projCoords.xy);

	if (vTexCoords.x != projCoords.x || vTexCoords.y != projCoords.y)
		return 1.0f;

	float currentDepth = projCoords.z;
	if (currentDepth > 1.0)
		return 1.0;


	//fragPosLightSpace.w -= fBias;

	currentDepth -= fBias;

	// PCF
	float shadow = 0.0;
	float2 texelSize = float2(1.f / g_vWinSize.x, 1.f / g_vWinSize.y);
	texelSize /= g_fShadowSizeRatio;

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = g_ShadowDepthTexture.Sample(ShadowSampler, projCoords.xy + float2(x, y) * texelSize).x;
			shadow += currentDepth > pcfDepth ? 0.5f : 1.0f;
		}
	}
	shadow /= 9.0f;
	return shadow;
}


float PCF_StaticShadowCalculation(float4 fragPosLightSpace, float fBias)
{
	// perform perspective divide
	float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// Transform to [0,1] range
	projCoords.x = projCoords.x * 0.5f + 0.5f;
	projCoords.y = projCoords.y * -0.5f + 0.5f;


	float2 vTexCoords = saturate(projCoords.xy);

	if (vTexCoords.x != projCoords.x || vTexCoords.y != projCoords.y)
		return 1.0f;

	float currentDepth = projCoords.z;
	if (currentDepth > 1.0)
		return 1.0;


	//fragPosLightSpace.w -= fBias;

	currentDepth -= fBias;

	// PCF
	float shadow = 0.0;
	float2 texelSize = float2(1.f / g_vWinSize.x, 1.f / g_vWinSize.y);
	texelSize /= g_fStaticShadowSizeRatio;

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = g_StaticShadowDepthTexture.Sample(ShadowSampler, projCoords.xy + float2(x, y) * texelSize).x;
			shadow += currentDepth > pcfDepth ? 0.5f : 1.0f;
		}
	}
	shadow /= 9.0f;
	return shadow;
}


PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	//vector		vModelNormalDesc = g_ModelNormalTexture.Sample(PointSampler, In.vTexcoord);
	/*if (vModelNormalDesc.a != 1.0f)
	{

	}*/

	vector		vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector vDepth = g_DepthTexture.Sample(PointSampler, In.vTexcoord);

	float fDot = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal));

	Out.vShade = (g_vLightDiffuse * fDot) + (g_vLightAmbient * g_vMtrlAmbient);



	vector vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */        //vShadowDepth = input.lightViewPosition.w / 2000.f
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f; // In.vTexcoord.x = vWorldPos.x / 2.f + 0.5f; // vWorldPos.x = vPosition.x / vPosition.w
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepth.x;
	vWorldPos.w = 1.f;

	float fViewZ = vDepth.y * 1200.f;
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos.w = 1.0f;


	vector		vModelNormalDesc = g_ModelNormalTexture.Sample(PointSampler, In.vTexcoord);
	float4		vResultNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (vModelNormalDesc.a != 1.0f)
	{
		float3 vCamPosition = g_CamPosition;
		float3 vPixelPos = vWorldPos.xyz;
		float3 vDir = vCamPosition - vPixelPos;
		float3 vModelNormal = vModelNormalDesc.xyz * 2.f - 1.f;

		float fNormalDot = dot(normalize(vDir), vModelNormal);

		if (fNormalDot < 0.2f)
			vResultNormal.xyz = pow(fNormalDot / 0.2f, 2.0f);
		else
			Out.vShade = saturate(Out.vShade * 1.5f);
	}


	Out.vShade *= vResultNormal;

	return Out;
}




PS_OUT_LIGHT PS_MAIN_DIRECTIONALSHADOW(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector vDepth = g_DepthTexture.Sample(PointSampler, In.vTexcoord);


	float fDot = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal));

	float fNormalOffset = g_fBias;


	float fBias = max((fNormalOffset * 5.0f) * (1.0f - (fDot * -1.0f)), fNormalOffset);

	//float fBias = g_fBias;


	//float fBias = g_fBias;

	

	Out.vShade = (g_vLightDiffuse * fDot) + (g_vLightAmbient * g_vMtrlAmbient);

	//DynamicShadow

	vector vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */        //vShadowDepth = input.lightViewPosition.w / 2000.f
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f; // In.vTexcoord.x = vWorldPos.x / 2.f + 0.5f; // vWorldPos.x = vPosition.x / vPosition.w
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepth.x;
	vWorldPos.w = 1.f;

	float fViewZ = vDepth.y * 1200.f;
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos.w = 1.0f;

	



	//외각선
	vector		vModelNormalDesc = g_ModelNormalTexture.Sample(PointSampler, In.vTexcoord);
	float4		vResultNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if (vModelNormalDesc.a != 1.0f)
	{
		float3 vCamPosition = g_CamPosition;
		float3 vPixelPos = vWorldPos.xyz;
		float3 vDir = vCamPosition - vPixelPos;
		float3 vModelNormal = vModelNormalDesc.xyz * 2.f - 1.f;

		float fNormalDot = dot(normalize(vDir), vModelNormal);

		if (fNormalDot < 0.2f)
			vResultNormal.xyz = pow(fNormalDot / 0.2f, 2.0f);
		else
			Out.vShade = saturate(Out.vShade * 1.5f);
	}

	//그림자

	float fResultShadow = 1.0f;

	vector vStaticPosition = mul(vWorldPos, g_StaticLightViewMatrix);
	vStaticPosition = mul(vStaticPosition, g_LightProjMatrix);

	//float fStaticShadow = PCF_StaticShadowCalculation(vStaticPosition, fBias);


	//if (fStaticShadow > 0.51f)
	//{
	//	vector vDynamicPosition = mul(vWorldPos, g_LightViewMatrix);
	//	vDynamicPosition = mul(vDynamicPosition, g_LightProjMatrix);

	//	float fShadow = PCF_ShadowCalculation(vDynamicPosition, fBias);

	//	fResultShadow = min(fStaticShadow, fShadow);
	//}
	//else
	//	fResultShadow = fStaticShadow;



	//
	vector vDynamicPosition = mul(vWorldPos, g_LightViewMatrix);
	vDynamicPosition = mul(vDynamicPosition, g_LightProjMatrix);

	float fShadow = PCF_ShadowCalculation(vDynamicPosition, fBias);

	fResultShadow = fShadow;
	//

	Out.vShade *= saturate(float4(fResultShadow, fResultShadow, fResultShadow, 1.f) * vResultNormal);

	return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	if (vDiffuse.a == 0.f)
		discard;
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);

	Out.vColor = vDiffuse * vShade;

	return Out;
}


PS_OUT PS_MAIN_EFFECT_DEFERRED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexcoord);


	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);

	Out.vColor = vShade;

	return Out;
}





PS_OUT PS_MAIN_BLUR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexcoord);


	float4 vFinalPixel = float4(0.0, 0.0, 0.0, 0.0);

	int iHalfKernel = g_KernelSize / 2;

	for (int i = -iHalfKernel; i <= iHalfKernel; ++i) {
		for (int j = -iHalfKernel; j <= iHalfKernel; ++j) {

			float2 vTexcoord = In.vTexcoord + float2(i * g_PixelSize.x, j * g_PixelSize.y);

			vector vBloomPixel = g_BlurTexture.Sample(DefaultSampler, vTexcoord);

			if (vBloomPixel.a > 0.0f)
			{
				float fWeightX = g_CenterWeight - (abs(i) * g_WeightAtt);
				float fWeightY = g_CenterWeight - (abs(j) * g_WeightAtt);

				vFinalPixel += vBloomPixel * fWeightX * fWeightY;
			}	
		}
	}

	/*if (vFinalPixel.a == 0.0f)
		discard;*/

	//if (vDiffuse.a != 0.0f)
	//	vFinalPixel += vDiffuse;

	Out.vColor = vFinalPixel;

	return Out;
}


//
//float g_sigma = 10.f;
//
//PS_OUT PS_MAIN_BLUR(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//
//	vector		vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexcoord);
//
//	float4 vFinalPixel = float4(0.0, 0.0, 0.0, 0.0);
//	float  kernelSum = 0.0;
//
//	for (int i = -g_KernelSize / 2; i <= g_KernelSize / 2; ++i) {
//		for (int j = -g_KernelSize / 2; j <= g_KernelSize / 2; ++j) {
//
//			// 가우시안 함수를 이용한 가중치 적용
//			float weight = exp(-(i * i + j * j) / (2.0 * g_sigma * g_sigma));
//
//			float2 offset = float2(i * g_PixelSize.x, j * g_PixelSize.y);
//			vector vBloomPixel = g_BlurTexture.Sample(PointSampler, In.vTexcoord + offset);
//
//			vFinalPixel += vBloomPixel * weight;
//			kernelSum += weight;
//		}
//	}
//
//	//가중치를 정규화하여 밝기 유지
//	if(vFinalPixel.a > 0.0f)
//		vFinalPixel /= kernelSum;
//
//	if (vDiffuse.a != 0.0f)
//		vFinalPixel += vDiffuse;
//
//	Out.vColor = vFinalPixel;
//
//	return Out;
//}




technique11 DefaultTechnique
{
	pass Target_Debug
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	pass Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
	}


	pass Blur
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLUR();
	}


	pass Deferred_Blend
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_BlendDefault, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT_DEFERRED();
	}
	

	pass Light_DirectionalShadow
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONALSHADOW();
	}


}




