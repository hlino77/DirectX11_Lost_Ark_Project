#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_PBR.hlsl"
#include "Client_Shader_Light.hlsl"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;
matrix			g_LightViewMatrix, g_LightProjMatrix;
matrix			g_StaticLightViewMatrix;

vector			g_vLightDir;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D		g_PriorityTarget;
Texture2D		g_NormalTarget;
Texture2D		g_DiffuseTarget;
Texture2D		g_ShadeTarget;
Texture2D		g_SpecularTarget;
Texture2D		g_PropertiesTarget;
Texture2D		g_EmissiveTarget;
Texture2D		g_BloomTarget;
Texture2D		g_NormalDepthTarget;
Texture2D		g_SSAOBlurTarget;
Texture2D		g_BlurTarget;
Texture2D		g_ShadowDepthTarget;
Texture2D		g_StaticShadowDepthTarget;

bool			g_bSSAO = true;

float4			g_vCamPosition;

Texture2D		g_Texture;

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

VS_OUT_TARGET VS_MAIN(TARGET_IN In)
{
    VS_OUT_TARGET Out = (VS_OUT_TARGET) 0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

float4 PS_MAIN_DEBUG(VS_OUT_TARGET In) : SV_TARGET
{
    return g_Texture.Sample(LinearSampler, In.vTexcoord);
}

struct PS_OUT_LIGHT
{
	float4	vShade : SV_TARGET0;	
	float4	vSpecular : SV_TARGET1;	
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
			float pcfDepth = g_ShadowDepthTarget.Sample(ShadowSampler, projCoords.xy + float2(x, y) * texelSize).x;
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
			float pcfDepth = g_StaticShadowDepthTarget.Sample(ShadowSampler, projCoords.xy + float2(x, y) * texelSize).x;
			shadow += currentDepth > pcfDepth ? 0.5f : 1.0f;
		}
	}
	shadow /= 9.0f;
	return shadow;
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(VS_OUT_TARGET In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector	vNormalDesc = g_NormalTarget.Sample(PointSampler, In.vTexcoord);
	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
	vector vNormalDepth = g_NormalDepthTarget.Sample(PointSampler, In.vTexcoord);
	float fDot = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal));

	Out.vShade = (g_vLightDiffuse * fDot) + (g_vLightAmbient * g_vMtrlAmbient);
	
	// 여기부터 Specular
    vector vReflect = reflect(normalize(g_vLightDir), vNormal);
	
	vector vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */			//vShadowDepth = input.lightViewPosition.w / 1200.f
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;	//In.vTexcoord.x = vWorldPos.x / 2.f + 0.5f; // vWorldPos.x = vPosition.x / vPosition.w
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	//vWorldPos.z = vDepth.x;
    vWorldPos.z = vNormalDesc.w;
	vWorldPos.w = 1.f;

    float fViewZ = vNormalDepth.w * 1200.f;
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    float4 vLook = vWorldPos - g_vCamPosition;
	
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 20.f);
	//

	return Out;
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONALSHADOW(VS_OUT_TARGET In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTarget.Sample(PointSampler, In.vTexcoord);

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vNormalDepth = g_NormalDepthTarget.Sample(PointSampler, In.vTexcoord);

	float fDot = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal));

	float fNormalOffset = g_fBias;

	float fBias = max((fNormalOffset * 5.0f) * (1.0f - (fDot * -1.0f)), fNormalOffset);

	//float fBias = g_fBias;

	Out.vShade = (g_vLightDiffuse * fDot) + (g_vLightAmbient * g_vMtrlAmbient);

	//DynamicShadow

	vector vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */        //vShadowDepth = input.lightViewPosition.w / 2000.f
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f; // In.vTexcoord.x = vWorldPos.x / 2.f + 0.5f; // vWorldPos.x = vPosition.x / vPosition.w
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	//vWorldPos.z = vDepth.x;
    vWorldPos.z = vNormalDesc.w;
	vWorldPos.w = 1.f;

	//float fViewZ = vDepth.y * 1200.f;
    float fViewZ = vNormalDepth.w * 1200.f;
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos.w = 1.0f;

	//그림자

	float fResultShadow = 1.0f;

	vector vStaticPosition = mul(vWorldPos, g_StaticLightViewMatrix);
	vStaticPosition = mul(vStaticPosition, g_LightProjMatrix);

	float fStaticShadow = PCF_StaticShadowCalculation(vStaticPosition, fBias);

	if (fStaticShadow > 0.51f)
	{
		vector vDynamicPosition = mul(vWorldPos, g_LightViewMatrix);
		vDynamicPosition = mul(vDynamicPosition, g_LightProjMatrix);

		float fShadow = PCF_ShadowCalculation(vDynamicPosition, fBias);

		fResultShadow = min(fStaticShadow, fShadow);
	}
	else
		fResultShadow = fStaticShadow;

	//
	//vector vDynamicPosition = mul(vWorldPos, g_LightViewMatrix);
	//vDynamicPosition = mul(vDynamicPosition, g_LightProjMatrix);

	//float fShadow = PCF_ShadowCalculation(vDynamicPosition, fBias);

	//fResultShadow = fShadow;
	//

	Out.vShade *= saturate(float4(fResultShadow, fResultShadow, fResultShadow, 1.f));

	return Out;
}

float4 PS_MAIN_DEFERRED(VS_OUT_TARGET In) : SV_TARGET
{
    vector vDiffuse = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
    {
        float4 vPriority = g_PriorityTarget.Sample(LinearSampler, In.vTexcoord);
        return vPriority;
    }
    float4	vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
    float4	vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
    float4	vEmissive =	g_BloomTarget.Sample(LinearSampler, In.vTexcoord);
	
    float fAO = 1.f;
	
    if (true == g_bSSAO)
        fAO = g_SSAOBlurTarget.Sample(LinearSampler, In.vTexcoord).r;
	
    return fAO * (vDiffuse * vShade + vSpecular) + vEmissive;
}

float4 PS_MAIN_PBR_DEFERRED(VS_OUT_TARGET In) : SV_TARGET
{
    float4 vAlbedo = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);
    if (vAlbedo.a == 0.f)
    {
        float4 vPriority = g_PriorityTarget.Sample(LinearSampler, In.vTexcoord);
        return vPriority;
    }
    vAlbedo = pow(vAlbedo, 2.2f);
	
    float4	vNormal = g_NormalTarget.Sample(LinearSampler, In.vTexcoord);
    float3	N = vNormal.xyz * 2.f - 1.f;
    float4 vProperties = g_PropertiesTarget.Sample(LinearSampler, In.vTexcoord);
    
    float fMetallic = vProperties.x /* 임시 */ /* * 0.8f*/;
    float fRoughness = vProperties.y;
    float fRimLight = vProperties.w;
	
    float4 vNormalDepth = g_NormalDepthTarget.Sample(PointSampler, In.vTexcoord);
	
    float fAO = 1.f;
	
	if (true == g_bSSAO)
        fAO = g_SSAOBlurTarget.Sample(LinearSampler, In.vTexcoord).r;
    
	float4	vWorldPos;
	
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vNormal.w;
    vWorldPos.w = 1.f;
	
    float fViewZ = vNormalDepth.w * 1200.f;
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    float3 V = normalize(g_vCamPosition.xyz - vWorldPos.xyz);
	
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, vAlbedo.xyz, fMetallic); // 반사율 F0

	// calculate per-light radiance
    float3 L = -normalize(g_vLightDir);
    float3 H = normalize(V + L);
	
    float3 vBRDF_factor = BRDF(fRoughness, fMetallic, vAlbedo.xyz, F0, N, V, L, H, fAO);
	
    float3 vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord).rgb;
	
    float3 vColor = float3(0.f, 0.f, 0.f);
    vColor += g_vLightDiffuse.rgb *  /*shadow * */vBRDF_factor * vShade;
    vColor = vColor / (vColor + float3(1.f, 1.f, 1.f));
    vColor = pow(vColor, float3(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f));
	
    float3 vEmissive = g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord).rgb;

    vColor += vEmissive;

	if (fRimLight != 0.0f)
	{
		float3 vRimLightColor = float3(1.0f, 1.0f, 0.8f);
		ComputeRimLight(vRimLightColor, N, -V);
		vColor += vRimLightColor;
	}

    return float4(vColor, 1.f);
}

float4 PS_MAIN_EFFECT_DEFERRED(VS_OUT_TARGET In) : SV_TARGET
{
	vector		vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);

	vector		vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);

    return vShade;
}

float4 PS_MAIN_BLUR(VS_OUT_TARGET In) : SV_TARGET
{
	vector		vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);


	float4 vFinalPixel = float4(0.0, 0.0, 0.0, 0.0);

	int iHalfKernel = g_KernelSize / 2;

	for (int i = -iHalfKernel; i <= iHalfKernel; ++i) {
		for (int j = -iHalfKernel; j <= iHalfKernel; ++j) {

			float2 vTexcoord = In.vTexcoord + float2(i * g_PixelSize.x, j * g_PixelSize.y);

            vector vBloomPixel = g_SSAOBlurTarget.Sample(DefaultSampler, vTexcoord);

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

    return vFinalPixel;
}


//
//float g_sigma = 10.f;
//
//PS_OUT PS_MAIN_BLUR(PS_IN In)
//{
//	PS_OUT		Out = (PS_OUT)0;
//
//	vector		vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);
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
//			vector vBloomPixel = g_BlurTarget.Sample(PointSampler, In.vTexcoord + offset);
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
	pass Target_Debug // 0
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

	pass Light_Directional // 1
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

	pass Light_Point // 2
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

	pass Deferred // 3
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

	pass Blur // 4
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

	pass Deferred_Blend // 5
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
	
	pass Light_DirectionalShadow // 6
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

    pass PBR_Deferred // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PBR_DEFERRED();
    }
}




