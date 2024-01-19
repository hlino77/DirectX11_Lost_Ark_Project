#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"

VS_OUT VS_MAIN(STATIC_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), WorldMatrix));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);

    Out.vTexUV = In.vTexUV;
    //Out.vWorldPos = mul(float4(In.vPosition, 1.f), WorldMatrix);
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix)).xyz;
    Out.vProjPos = Out.vPosition;
	
	return Out;
}

PS_OUT_PBR PS_PBR(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR) 0;

    Out.vDiffuse = (vector) 1.f;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
    
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
    
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        if (1.f == SpecMaskEmisExtr.y)
        {
            float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
            Out.vProperties.r = vSpecular.r * vMRMask.r * (1.f - vSpecular.a);
            Out.vProperties.g = vSpecular.r * vMRMask.g * vSpecular.a;
        }
        else
        {
            Out.vProperties.r = 1.1f * vSpecular.r * Out.vDiffuse.r;
            Out.vProperties.g = 1.f - vSpecular.r;
        }
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
    
    Out.vProperties.z = MT_STATIC;
    Out.vProperties.w = g_fRimLight;

    return Out;
}


PS_OUT_PBR PS_PBR_EFFECT(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR)0;

    Out.vDiffuse = (vector) 1.f;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);

    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        if (1.f == SpecMaskEmisExtr.y)
        {
            float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
            Out.vProperties.r = vSpecular.r * vMRMask.r * (1.f - vSpecular.a);
            Out.vProperties.g = vSpecular.r * vMRMask.g * vSpecular.a;
        }
        else
        {
            Out.vProperties.r = 1.1f * vSpecular.r * Out.vDiffuse.r;
            Out.vProperties.g = 1.f - vSpecular.r;
        }
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

    Out.vProperties.z = MT_STATIC;
    Out.vProperties.w = g_fRimLight;

    return Out;
}




PS_OUT_PBR PS_PBR_DISSOLVE(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR)0;

    if (g_fDissolveAmount > 0.0001f)
    {
        float fDissolve = g_DissolveTexture.Sample(LinearSampler, In.vTexUV).x;

        if (fDissolve <= g_fDissolveAmount)
            discard;
    }

    Out.vDiffuse = (vector) 1.f;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);

    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        if (1.f == SpecMaskEmisExtr.y)
        {
            float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
            Out.vProperties.r = vSpecular.r * vMRMask.r * (1.f - vSpecular.a);
            Out.vProperties.g = vSpecular.r * vMRMask.g * vSpecular.a;
        }
        else
        {
            Out.vProperties.r = 1.1f * vSpecular.r * Out.vDiffuse.r;
            Out.vProperties.g = 1.f - vSpecular.r;
        }
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

    Out.vProperties.z = MT_STATIC;
    Out.vProperties.w = g_fRimLight;

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
        PixelShader = compile ps_5_0 PS_PBR_EFFECT();
    }

    pass PBR_DISSOLVE // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR_DISSOLVE();
    }

}