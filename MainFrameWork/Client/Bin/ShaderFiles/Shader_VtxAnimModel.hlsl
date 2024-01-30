#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"

float4	g_vHairColor_1;
float4	g_vHairColor_2;
bool    g_bDissolve = false;
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
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix));

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
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);
    
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix));
	
	return Out;
}

VS_OUT_SHADOW VS_SHADOW(SKELETAL_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

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
    Out.vProjPos = Out.vPosition;
	
    return Out;
}

PS_OUT_PBR PS_PBR(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR) 0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
	if (0.2f >= Out.vDiffuse.a)
		discard;
    if(g_bDissolve == true)
        ComputeDissolveColor(Out.vDiffuse, In.vTexUV);
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
    //float4 vNormalV = float4(In.vNormalV, 0.f);
    //ComputeNormalMapping(vNormalV, In.vTangent, In.vTexUV);
    
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    //Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
    // w채널에 기존 depth의 x채널을 이사
    // Depth에는 뷰 공간의 노멀, 깊이는 w로 이사
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    //Out.vNormalV = vector(In.vNormalV * 0.5f + 0.5f, In.vProjPos.w / 1200.0f);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
	
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
		
        Out.vProperties.z = MT_DYNAMIC;
        
        return Out;
    } // PBR 적용없이 return
	
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        if (1.f == SpecMaskEmisExtr.y)
        {
            float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
            Out.vProperties.x = vSpecular.r * vMRMask.r * (1.f - vSpecular.a);
            Out.vProperties.y = vSpecular.r * vMRMask.g * vSpecular.a;
        }
        else
        {
            Out.vProperties.x = 1.2f * vSpecular.r;
            Out.vProperties.y = pow(vSpecular.b, 3.f);
        }
    }
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
        Out.vEmissive *= g_vBloomColor;
    }
    
    Out.vProperties.z = MT_DYNAMIC;
    Out.vProperties.w = g_fRimLight;

    return Out;
}

PS_OUT_PHONG PS_PHONG(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV * 0.5f + 0.5f, In.vProjPos.w / 1200.0f);
	
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
		
        Out.vProperties.z = MT_DYNAMIC;
        
        return Out;
    } // PBR 적용없이 return
	
    Out.vProperties.z = MT_DYNAMIC;
    
    return Out;
}

PS_OUT_PHONG PS_CHANGECOLOR(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
    
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);

        Out.vProperties.r = 1.1f * vSpecular.r * Out.vDiffuse.r;
        Out.vProperties.g = 1.f - vSpecular.r;
    }
    else
    {
        Out.vProperties.r = EPSILON;
        Out.vProperties.g = 1.f;
    }
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
        Out.vEmissive *= g_vBloomColor;
    }
    
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
		     
        Out.vProperties.z = MT_DYNAMIC;
        
        return Out;
    } // PBR 적용없이 return
    
    
    float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
    float4 vGiven;
    
    float maxColor = max(max(vMRMask.r, vMRMask.g), vMRMask.b);
    float maxR = g_vColor_R.r + g_vColor_R.g + g_vColor_R.b;
    float maxG = g_vColor_G.r + g_vColor_G.g + g_vColor_G.b;
    float maxB = g_vColor_B.r + g_vColor_B.g + g_vColor_B.b;
    float fColorFactor;
    
    if (0 != maxColor)
    {
        if (0 != vMRMask.r && vMRMask.r == vMRMask.g && vMRMask.g == vMRMask.b)
        {
            if (maxR > maxG)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else if (maxR > maxB)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else if (maxG > maxB)
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
            else
            {
                vGiven = g_vColor_B;
                fColorFactor = vMRMask.b;
            }
        }
        else if (0 != vMRMask.r && vMRMask.r == vMRMask.g)
        {
            if (maxR > maxG)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
        }
        else if (0 != vMRMask.r && vMRMask.r == vMRMask.b)
        {
            if (maxR > maxB)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else
            {
                vGiven = g_vColor_B;
                fColorFactor = vMRMask.b;
            }
        }
        else if (vMRMask.r == maxColor)
        {
            vGiven = g_vColor_R;
            fColorFactor = vMRMask.r;
        }
        else if (0 != vMRMask.g && vMRMask.g == vMRMask.b)
        {
            if (maxG > maxB)
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
            else
            {
                vGiven = g_vColor_B;
                fColorFactor = vMRMask.b;
            }
        }
        else if (vMRMask.g == maxColor)
        {
            vGiven = g_vColor_G;
            fColorFactor = vMRMask.g;
        }
        else if (vMRMask.b == maxColor)
        {
            vGiven = g_vColor_B;
            fColorFactor = vMRMask.b;
        }
        
        if (0 != vGiven.a)
        {
            Out.vDiffuse.rgb *= (vGiven.rgb + (1 - fColorFactor)) * vGiven.a;
            Out.vDiffuse.a = 1.f;
        }
    }
    
    Out.vProperties.z = MT_DYNAMIC;
    
    return Out;
}

float4 PS_SHADOW(VS_OUT_SHADOW In) : SV_TARGET0
{
    return float4(In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f, 0.0f);
}

PS_OUT_PHONG PS_DIFFUSE(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;
    
    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV * 0.5f + 0.5f, In.vProjPos.w / 1200.0f);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    Out.vProperties.z = MT_DYNAMIC;
    
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

	pass Phong // 1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PHONG();
	}

	pass ShadowPass // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_SHADOW();
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SHADOW();
    }

    pass Diffuse // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DIFFUSE();
    }

    pass ChangeColor // 4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CHANGECOLOR();
    }
}