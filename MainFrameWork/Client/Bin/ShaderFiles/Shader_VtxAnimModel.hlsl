#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"

float	g_BlendRatio;
float4	g_vBlurColor;
float	g_fAlpha;

float4	g_vHairColor_1;
float4	g_vHairColor_2;

matrix	g_BoneMatrices[800];

VS_OUT VS_MAIN_NO_NORMAL(SKELETAL_IN In)
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

VS_OUT VS_MAIN(SKELETAL_IN In)
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
	
	return Out;
}

VS_OUT VS_SHADOW(SKELETAL_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWVP;

    matWVP = mul(WorldMatrix, ViewProj);

    float fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
	
    float4x4 vMatX = g_BoneMatrices[In.vBlendIndex.x];
    float4x4 vMatY = g_BoneMatrices[In.vBlendIndex.y];
    float4x4 vMatZ = g_BoneMatrices[In.vBlendIndex.z];
    float4x4 vMatW = g_BoneMatrices[In.vBlendIndex.w];

    float4x4 BoneMatrix = vMatX * In.vBlendWeight.x +
		vMatY * In.vBlendWeight.y +
		vMatZ * In.vBlendWeight.z +
		vMatW * fWeightW;

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, WorldMatrix));

    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix));
	
    return Out;
}

PS_OUT_PBR PS_PBR(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR) 0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
	if (0.2f >= Out.vDiffuse.a)
		discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = In.vNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
	
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
		
        return Out;
    } // PBR 적용없이 return
	
    float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
    float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);

    if (vMRMask.a * vMRMask.b == 0.f)
    {
        Out.vMetallic = vSpecular * vMRMask.r;
        Out.vRoughness = vSpecular * vMRMask.g;
    }

    return Out;
}

PS_OUT_PBR PS_PBR_NOMASK(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR) 0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
	if (0.2f >= Out.vDiffuse.a)
		discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = In.vNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
	
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
		
        return Out;
    } // PBR 적용없이 return
	
    float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);

    Out.vMetallic = Out.vDiffuse * vSpecular.r;
    Out.vRoughness = Out.vDiffuse * vSpecular.g;

    return Out;
}

PS_OUT_PHONG PS_PHONG(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = In.vNormal;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
	
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
		
        return Out;
    } // PBR 적용없이 return
	
    return Out;
}

PS_OUT_SHADOW PS_SHADOWDEPTH(VS_OUT In)
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f, 0.0f);

    return Out;
}

PS_OUT_PHONG PS_DIFFUSE(VS_OUT In) : SV_TARGET0
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;
    
    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    return Out;
}

technique11 DefaultTechnique
{
    pass PBR // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR();
    }

	pass PBR_NoMask // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR_NOMASK();
    }

	pass Phong // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PHONG();
	}

	pass ShadowPass // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOWDEPTH();
	}

    pass DiffusePass // 4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DIFFUSE();
    }
}