#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"

// For Grass
float3      g_vWind = { 0.f, 0.f, 0.f };
float       g_fPower = 0.f;
float       g_fTime = 2.f;
float       g_fmaxHeight = 2.f;



VS_OUT VS_MAIN(STATIC_INSTANCE_IN In)
{
    VS_OUT		Out = (VS_OUT)0;

    matrix		matWVP;


    Out.fRimLight = In.matWorld._14;
    In.matWorld._14 = 0.0f;

    matWVP = mul(In.matWorld, ViewProj);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), In.matWorld));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);

    Out.vTexUV = In.vTexUV;
    //Out.vWorldPos = mul(float4(In.vPosition, 1.f), WorldMatrix);
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), In.matWorld)).xyz;
    Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT VS_GRASS_INSTANCE(STATIC_INSTANCE_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    matrix matWV = mul(In.matWorld, ViewMatrix);
    matrix matWVP = mul(matWV, ProjMatrix);

    float heightFactor = saturate(In.vPosition.y / g_fmaxHeight);
    float windEffect = sin(g_fTime + In.vPosition.x * g_vWind.x + In.vPosition.y * g_vWind.y + In.vPosition.z * g_vWind.z) * g_fPower * heightFactor;

    float3 modifiedPosition = In.vPosition + windEffect * g_vWind;

    Out.vPosition = mul(float4(modifiedPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), In.matWorld));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);
    Out.vTexUV = In.vTexUV;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), In.matWorld)).xyz;
    Out.vProjPos = Out.vPosition;

    return Out;
}




PS_OUT_PBR PS_PBR(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR)0;

    Out.vDiffuse = (vector) 1.f;

    Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);

    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(PointSampler, In.vTexUV);
        //if (1.f == SpecMaskEmisExtr.y)
        //{
        //    //float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
        //    //Out.vProperties.r = smoothstep(0.0f, 1.0f, vSpecular.r * vMRMask.r * (1.f - vSpecular.a));
        //    //Out.vProperties.g = vSpecular.r * vMRMask.g * vSpecular.a;
            
        //    Out.vProperties.r = smoothstep(0.0f, 1.0f, vSpecular.b); // Metalic
        //    Out.vProperties.g = vSpecular.g; // Roughness
        //}
        //else
        //{
        Out.vProperties.r = clamp(0.0f, 1.0f, 1.f - pow(1.f - vSpecular.b, 1.3f)); // Metalic
        Out.vProperties.g = pow(vSpecular.g, 1.5f); // Roughness
        //}
        
        //Out.vProperties.r = smoothstep(0.0f, 0.95f, 1.f - pow(1.f - vSpecular.b, 2.f)); // Metalic
        //Out.vProperties.g = pow(vSpecular.g, 2.f); // Roughness
    }
    else
    {
        Out.vProperties.r = 0.04f;
        Out.vProperties.g = 0.5f;
    }

    if (1.f == SpecMaskEmisExtr.z)
    {
        Out.vEmissive = g_EmissiveTexture.Sample(PointSampler, In.vTexUV);
        Out.vEmissive *= g_vBloomColor;
    }


    Out.vProperties.z = MT_STATIC;
    Out.vProperties.w = In.fRimLight;
   
    return Out;
}

PS_OUT_PHONG PS_PHONG(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG)0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    Out.vProperties.z = MT_STATIC;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass PBRInstance
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR();
    }

    //pass PBRInstance_Alpha
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_Default, 0);
    //    SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    GeometryShader = NULL;
    //    PixelShader = compile ps_5_0 PS_PBR();
    //}

    pass Diffuse // �ӽ�
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PHONG();
    }


    pass GrassInstance
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_GRASS_INSTANCE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR();
    }

}