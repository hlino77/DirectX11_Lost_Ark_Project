#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_PBR.hlsl"
#include "Client_Shader_Light.hlsl"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;
matrix			g_LightViewMatrix, g_LightProjMatrix;
matrix			g_StaticLightViewMatrix;

Texture2D		g_PriorityTarget;
Texture2D		g_NormalTarget;
Texture2D		g_NormalDepthTarget;
Texture2D		g_DiffuseTarget;
Texture2D		g_PropertiesTarget;
Texture2D		g_EmissiveTarget;
Texture2D		g_BloomTarget;
Texture2D		g_SSAOBlurTarget;
Texture2D		g_BlurTarget;
Texture2D		g_ShadowDepthTarget;
Texture2D		g_StaticShadowDepthTarget;
texture2D		g_OutlineTarget;

bool			g_bSSAO = true;
bool			g_bShadow = false;

float4			g_vCamPosition;

Texture2D		g_Texture;

float2  g_PixelSize;
int     g_KernelSize; //커널은 이미지 처리에서 필터 또는 윈도우라고도 불리는 작은 행렬 또는 마스크
float   g_CenterWeight;
float	g_WeightAtt;

float	g_fBias;

float	g_fShadowSizeRatio;
float	g_fStaticShadowSizeRatio;
float2	g_vWinSize = float2(1600.f, 900.f);	


Texture2D		g_CascadeTarget1;
Texture2D		g_CascadeTarget2;
Texture2D		g_CascadeTarget3;

matrix			g_CascadeProj[3];


// For Fog
float  g_fFogStartHeight =  0.f;
float  g_fFogEndHeight   =  0.f;
float  g_fFogDensity	 =  0.f; // Fog Power 
float3 g_vFogColor       =  float3(0.f, 0.f, 0.f); // FogColor (R, G, B) Same == Gray
float  g_fFogTime        =  0.f;
float  g_fFogChangeSpeed =  0.f;
float  g_fFogMinValue    =  0.f;


sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
};

sampler ShadowSampler = sampler_state {
	Filter = MIN_MAG_MIP_POINT;
};

sampler ShadowClampSampler = sampler_state {
	Filter = MIN_MAG_MIP_POINT;
	AddressU = clamp;
	AddressV = clamp;
	AddressW = clamp;
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



float PCF_StaticShadowCalculation_Cascade(float4 vWorldPos, float fBias, float fViewZ)
{
	float4 vProjCoords;

	float fShadow = 1.0f;

	fBias = 0.001f;

	if (fViewZ <= 15.0f)
	{
		vProjCoords = mul(vWorldPos, g_CascadeProj[0]);

		vProjCoords.x = vProjCoords.x * 0.5f + 0.5f;
		vProjCoords.y = vProjCoords.y * -0.5f + 0.5f;

		//float2 vTexCoords = saturate(vProjCoords.xy);

		/*if (vTexCoords.x != projCoords.x || vTexCoords.y != projCoords.y)
			return 1.0f;*/

		float currentDepth = vProjCoords.z;
		if (currentDepth > 1.0)
			return 1.0;

		currentDepth -= fBias;

		float2 texelSize = float2(1.f / 1600.0f, 1.f / 1600.0f);

		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				float pcfDepth = g_CascadeTarget1.Sample(ShadowClampSampler, vProjCoords.xy + float2(x, y) * texelSize).x;
				fShadow += currentDepth > pcfDepth ? 0.5f : 1.0f;
			}
		}
		fShadow /= 9.0f;

	}
	else if (fViewZ > 15.0f && fViewZ <= 30.0f)
	{
		vProjCoords = mul(vWorldPos, g_CascadeProj[1]);

		vProjCoords.x = vProjCoords.x * 0.5f + 0.5f;
		vProjCoords.y = vProjCoords.y * -0.5f + 0.5f;

		//float2 vTexCoords = saturate(vProjCoords.xy);

	/*	if (vTexCoords.x != projCoords.x || vTexCoords.y != projCoords.y)
			return 1.0f;*/

		float currentDepth = vProjCoords.z;
		if (currentDepth > 1.0)
			return 1.0;

		currentDepth -= fBias;

		float2 texelSize = float2(1.f / 1600.0f, 1.f / 1600.0f);

		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				float pcfDepth = g_CascadeTarget2.Sample(ShadowClampSampler, vProjCoords.xy + float2(x, y) * texelSize).x;
				fShadow += currentDepth > pcfDepth ? 0.5f : 1.0f;
			}
		}
		fShadow /= 9.0f;
	}
	else if (fViewZ > 30.0f)
	{
		vProjCoords = mul(vWorldPos, g_CascadeProj[2]);

		vProjCoords.x = vProjCoords.x * 0.5f + 0.5f;
		vProjCoords.y = vProjCoords.y * -0.5f + 0.5f;

		float2 vTexCoords = saturate(vProjCoords.xy);

		if (vTexCoords.x != vProjCoords.x || vTexCoords.y != vProjCoords.y)
			return 1.0f;

		float currentDepth = vProjCoords.z;
		if (currentDepth > 1.0)
			return 1.0;

		currentDepth -= fBias;

		float2 texelSize = float2(1.f / 1600.0f, 1.f / 1600.0f);

		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				float pcfDepth = g_CascadeTarget3.Sample(ShadowClampSampler, vProjCoords.xy + float2(x, y) * texelSize).x;
				fShadow += currentDepth > pcfDepth ? 0.5f : 1.0f;
			}
		}
		fShadow /= 9.0f;
	}
	else
	{


		return 1.0f;
	}

	return fShadow;
}

float4 PS_MAIN_DEFERRED(VS_OUT_TARGET In) : SV_TARGET
{
    vector vDiffuse = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
    {
        float4 vPriority = g_PriorityTarget.Sample(LinearSampler, In.vTexcoord);
        return vPriority;
    }
    float4	vEmissive =	g_BloomTarget.Sample(LinearSampler, In.vTexcoord);
	
    float fAO = 1.f;
	
    if (true == g_bSSAO)
        fAO = g_SSAOBlurTarget.Sample(LinearSampler, In.vTexcoord).r;
	
    return fAO * (vDiffuse) + vEmissive;
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

	float fDot = saturate(dot(normalize(g_vLightDir.xyz) * -1.f, vNormal.xyz));

	float fNormalOffset = g_fBias;

	float fBias = max((fNormalOffset * 5.0f) * (1.0f - (fDot * -1.0f)), fNormalOffset);

	float fResultShadow = 1.0f;

	if (g_bShadow > EPSILON)
	{
		vector vStaticPosition = mul(vWorldPos, g_StaticLightViewMatrix);
		vStaticPosition = mul(vStaticPosition, g_LightProjMatrix);

		//float fStaticShadow = PCF_StaticShadowCalculation(vStaticPosition, fBias);
		float fStaticShadow = PCF_StaticShadowCalculation_Cascade(vWorldPos, fBias, fViewZ);
		if (fStaticShadow > 0.51f)
		{
			vector vDynamicPosition = mul(vWorldPos, g_LightViewMatrix);
			vDynamicPosition = mul(vDynamicPosition, g_LightProjMatrix);

			float fShadow = PCF_ShadowCalculation(vDynamicPosition, fBias);

			fResultShadow = min(fStaticShadow, fShadow);
		}
		else
			fResultShadow = fStaticShadow;
	}

	float3 V = normalize(g_vCamPosition.xyz - vWorldPos.xyz);

	float3 F0 = float3(0.04f, 0.04f, 0.04f);
	F0 = lerp(F0, vAlbedo.xyz, fMetallic); // 반사율 F0

	// calculate per-light radiance
	float3 L = normalize(-g_vLightDir);
	float3 H = normalize(V + L);

	float3 vPBR_Color = BRDF(fRoughness, fMetallic, vAlbedo.xyz, F0, N, V, L, H, fAO);

	float3 vColor = float3(0.f, 0.f, 0.f);

	float3 vEmissive = g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord).rgb;
	vEmissive = pow(vEmissive, 2.2f);

	vColor = vPBR_Color * fResultShadow + vEmissive;

	//vColor = pow(vColor, float3(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f));

	if (fRimLight != 0.0f)
	{
		float3 vRimLightColor = float3(1.0f, 1.0f, 0.8f);
		if (abs(fRimLight - 0.9f) < 0.03f)
		{
			vRimLightColor = float3(0.f, 0.2f, 0.45f);
		}
		if (abs(fRimLight - 0.95f) < 0.03f)
		{
			vRimLightColor = float3(0.f, 0.45f, 0.3f);
		}
		if (abs(fRimLight - 0.8f) < 0.03f)
		{
			vRimLightColor = float3(0.18, 0.522, 0.514);
		}
		if (abs(fRimLight - 0.7f) < 0.03f)
		{
			vRimLightColor = float3(0.4f, 1.f, 0.8f);
		}
		if (abs(fRimLight - 0.6f) < 0.03f)
		{
			vRimLightColor = float3(0.f, 0.616f, 0.639f);
		}

		ComputeRimLight(vRimLightColor, N, -V);
		vColor += vRimLightColor;
	}


#pragma region Fog

	// Not Setting FogValue
	if (g_fFogStartHeight == 0.f)
	{
		return float4(vColor, 1.f); // Basic Rendering (Not Use Fog)
	}

	float height = vWorldPos.y; 
	float fogFactor = 0.0f;

	if (height < g_fFogStartHeight) 
	{
		float heightDifference = g_fFogStartHeight - height;
		
		float densityModifier = sin(g_fFogTime * g_fFogChangeSpeed) * 0.5f + 0.5f;

		densityModifier = (densityModifier * (1 - g_fFogMinValue)) + g_fFogMinValue;

		fogFactor = 1.0f - exp( -heightDifference * g_fFogDensity * densityModifier);
	}

	float3 finalColorWithFog = lerp(vColor.rgb, g_vFogColor, fogFactor);
	return float4(finalColorWithFog, 1.f);

#pragma endregion

}
	

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

	pass Deferred // 1
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

    pass PBR_Deferred // 2
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




