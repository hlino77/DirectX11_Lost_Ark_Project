#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"

float4	g_vHairColor_1;
float4	g_vHairColor_2;
bool    g_bDissolve = false;
matrix  g_BoneMatrices[800];

matrix      g_CascadeProj;

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

VS_OUT_SHADOW VS_CASCADE_SHADOW(SKELETAL_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW)0;

    matrix matWVP;

    matWVP = mul(WorldMatrix, g_CascadeProj);

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

    return Out;
}

VS_OUT_OUTLINE VS_MAIN_TOON(SKELETAL_IN In)
{
    VS_OUT_OUTLINE Out = (VS_OUT_OUTLINE) 0;

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
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);
    
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix));
    Out.vWorldPos = mul(vPosition, WorldMatrix);
	
    return Out;
}

VS_OUT_OUTLINE VS_MAIN_OUTLINE(SKELETAL_IN In)
{
    VS_OUT_OUTLINE Out = (VS_OUT_OUTLINE) 0;

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

    
    vector vPosition = mul(vector(In.vPosition.xyz + In.vNormal.xyz * g_fOutlineThickness, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

    vPosition = CreateOutline(vPosition, 0.001f);
    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, WorldMatrix));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);
    
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix));
    Out.vWorldPos = mul(vPosition, WorldMatrix);
	
    return Out;
}

PS_OUT_PBR PS_PBR(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR) 0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
	if (0.2f >= Out.vDiffuse.a)
		discard;
    
    if(g_bDissolve == true)
    {
        if (false == (ComputeDissolveColor(Out.vDiffuse, In.vTexUV)))
            discard;
        
        if (-1 == Out.vDiffuse.a)
        {
            Out.vDiffuse = float4(Out.vDiffuse.rgb, 1);
            Out.vEmissive = float4(Out.vDiffuse.rgb, 1);
        }
    }

    ComputeNormalMapping
        (In.vNormal, In.vTangent, In.vTexUV);
    //float4 vNormalV = float4(In.vNormalV, 0.f);
    //ComputeNormalMapping(vNormalV, In.vTangent, In.vTexUV);
    
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    //Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1200.0f, 0.0f, 0.0f);
    // wä�ο� ���� depth�� xä���� �̻�
    // Depth���� �� ������ ���, ���̴� w�� �̻�
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    //Out.vNormalV = vector(In.vNormalV * 0.5f + 0.5f, In.vProjPos.w / 1200.0f);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
    
    Out.vProperties.z = MT_DYNAMIC;
    
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));
        
        return Out;
    } // PBR ������� return
	
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        //if (1.f == SpecMaskEmisExtr.y)
        //{
        //    float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
        //    //Out.vProperties.x = smoothstep(0.0f, 1.f, vSpecular.r * vMRMask.r * (1.f - vSpecular.a));
        //    //Out.vProperties.y = vSpecular.r * vMRMask.g * vSpecular.a;
            
        //    //Out.vProperties.x = smoothstep(0.0f, 1.f, vSpecular.b * vMRMask.r * (1.f - vSpecular.a));
        //    //Out.vProperties.y = vSpecular.g * vMRMask.g * vSpecular.a;
            
        //    Out.vProperties.r = smoothstep(0.0f, 1.f, vSpecular.b); // Metalic
        //    Out.vProperties.g = vSpecular.g; // Roughness
        //}
        //else
        //{
        Out.vProperties.r = smoothstep(0.0f, 1.f, vSpecular.b); // Metalic
        Out.vProperties.g = vSpecular.g; // Roughness
        //}
        
        //Out.vProperties.r = 1.f - pow(1.f - vSpecular.b, 1.7f); // Metalic
        //Out.vProperties.g = pow(vSpecular.g, 1.7f); // Roughness
    }
    else
    {
        Out.vProperties.r = 0.04f;
        Out.vProperties.g = 0.5f;
    }
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        if (false == any(Out.vEmissive))
        {
            Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
            Out.vEmissive *= g_vBloomColor;
        }
    }

    Out.vProperties.w = g_fRimLight;

    return Out;
}

float4 PS_ALPHA(VS_OUT In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    if (g_bDissolve == true)
        ComputeDissolveColor(vColor, In.vTexUV);
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float4 vLook = In.vProjPos - float4(CameraPosition(), 1.f);

    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

    vColor = (g_vLightDiffuse * vColor) + fSpecular;
    
    return vColor;
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
    Out.vProperties.z = MT_DYNAMIC;
	
    if (0 != g_vHairColor_1.a || 0 != g_vHairColor_2.a)
    {
        float4 haircolor_1 = g_vHairColor_1 * Out.vDiffuse.g;
        float4 haircolor_2 = g_vHairColor_2 * (1.f - Out.vDiffuse.g);
		
        float4 vBlendedHairColor = haircolor_1 + haircolor_2;
        Out.vDiffuse = saturate(float4(vBlendedHairColor.rgb, 1.f));

        return Out;
    } // PBR ������� return
    
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
    
    Out.vProperties.z = MT_DYNAMIC;
    
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);

        //Out.vProperties.r = clamp(0.0f, 1.0f, 1.f - pow(1.f - vSpecular.b, 2.f)); // Metalic
        //Out.vProperties.g = pow(vSpecular.g, 1.8f); // Roughness
        
        Out.vProperties.r = smoothstep(0.0f, 1.f, vSpecular.b); // Metalic
        Out.vProperties.g = vSpecular.g; // Roughness
    }
    else
    {
        Out.vProperties.r = 0.04f;
        Out.vProperties.g = 0.5f;
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
        
        return Out;
    } // PBR ������� return
        
    float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
    float4 vGiven;
    
    float maxColor = max(max(vMRMask.r, vMRMask.g), vMRMask.b);
    float maxR = g_vColor_R.r + g_vColor_R.g + g_vColor_R.b;
    float maxG = g_vColor_G.r + g_vColor_G.g + g_vColor_G.b;
    float maxB = g_vColor_B.r + g_vColor_B.g + g_vColor_B.b;
    float fColorFactor;
    
    /* �ִ� ������ ������ ���� ó�� */
    if (0 != maxColor)
    {
        if (maxColor == vMRMask.r)
        {
            if (vMRMask.r == vMRMask.g && vMRMask.g == vMRMask.b) // RGB ���� ��� ���� ��
            {
                if (maxR >= maxG && maxR >= maxB)
                {
                    vGiven = g_vColor_R;
                    fColorFactor = vMRMask.r;
                }
                else if (maxG >= maxB)
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
            else if (vMRMask.r == vMRMask.g) // R�� G�� ���� ��
            {
                if (maxR >= maxG)
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
            else if (vMRMask.r == vMRMask.b) // R�� B�� ���� ��
            {
                if (maxR >= maxB)
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
            else // R�� �ִ밪�� ��
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
        }
        else if (maxColor == vMRMask.g)
        {
            if (vMRMask.g == vMRMask.b) // G�� B�� ���� ��
            {
                if (maxG >= maxB)
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
            else // G�� �ִ밪�� ��
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
        }
        else // B�� �ִ밪�� ��
        {
            vGiven = g_vColor_B;
            fColorFactor = vMRMask.b;
        }

        /* Alpha ���� 0�� �ƴ� ��� ���� ���� */
        if (0 != vGiven.a)
        {
            Out.vDiffuse.rgb *= (vGiven.rgb + (1 - fColorFactor)) * vGiven.a;
            Out.vDiffuse.a = 1.f;
        }
    }
    
    return Out;
}

float4 PS_SHADOW(VS_OUT_SHADOW In) : SV_TARGET0
{
    return float4(In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f, 0.0f);
}

float4 PS_CASCADE_SHADOW(VS_OUT_SHADOW In) : SV_TARGET0
{
    return float4(In.vPosition.z, 0.0f, 0.0f, 0.0f);
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

float4 PS_ALPHABLEND(VS_OUT_OUTLINE In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    
    if (g_bDissolve == true)
    {
        if (false == (ComputeDissolveColor(vColor, In.vTexUV)))
            discard;
    }
    
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        float3 vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
        if (true == any(vEmissive))
        {
            vColor.rgb = vEmissive * g_vBloomColor.rgb;
        }
    }
    
    if (g_fRimLight != 0.0f)
    {
        
        float3 N = In.vNormal.xyz * 2.f - 1.f;
        float3 V = normalize(CameraPosition() - In.vWorldPos.xyz);
        
        float3 vRimLightColor = float3(1.0f, 1.0f, 0.8f);
        if (abs(g_fRimLight - 0.9f) < 0.03f)
        {
            vRimLightColor = float3(0.f, 0.2f, 0.45f);
        }
        if (abs(g_fRimLight - 0.95f) < 0.03f)
        {
            vRimLightColor = float3(0.f, 0.45f, 0.3f);
        }
        if (abs(g_fRimLight - 0.8f) < 0.03f)
        {
            vRimLightColor = float3(0.18, 0.522, 0.514);
        }
        if (abs(g_fRimLight - 0.7f) < 0.03f)
        {
            vRimLightColor = float3(0.4f, 1.f, 0.8f);
        }
        if (abs(g_fRimLight - 0.6f) < 0.03f)
        {
            vRimLightColor = float3(0.f, 0.616f, 0.639f);
        }

        ComputeRimLight(vRimLightColor, N, -V);
        vColor.rgb += vRimLightColor;
    }
    
    return vColor;
}

float4 PS_TOON(VS_OUT_OUTLINE In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    
    if (g_bDissolve == true)
    {
        if (false == (ComputeDissolveColor(vColor, In.vTexUV)))
            discard;
    }
    
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

    float4 vDir = float4(CameraPosition(), 1.f) - In.vWorldPos;
    
    float fDot = dot(normalize(vDir), In.vNormal);
    fDot = pow(fDot, 2.f);
    
    if (0.1f >= fDot)
    {
        vColor = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    return vColor;
}

float4 PS_OUTLINE(VS_OUT_OUTLINE In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vColor = g_vOutlineColor;
    
    if (g_bDissolve == true)
    {
        if (false == (ComputeDissolveColor(vColor, In.vTexUV)))
            discard;
        
    }
    
    return vColor;
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

    pass Alphablend // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_OUTLINE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ALPHABLEND();
    }

    pass Toon // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TOON();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TOON();
    }

    pass Outline // 5
    {
        SetRasterizerState(RS_Outline);
        SetDepthStencilState(DSS_Default, 1);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_OUTLINE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_OUTLINE();
    }

    pass Inline // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 1);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR();
    }

    pass CascadeShadowPass // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_CASCADE_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CASCADE_SHADOW();
    }
}