#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Effect.hlsl"

VS_OUT VS_MAIN_FXMESH(STATIC_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), WorldMatrix));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);

    Out.vTexUV = In.vTexUV;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix)).xyz;
	
	return Out;
}

PS_OUT_EFFECT PS_MAIN_FXMESH(VS_OUT In, uniform bool bOneBlend, uniform int iSamplerState)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    float2 vNewUV = float2(0.f, 0.f);
    
    if (!bUV_Wave)
        vNewUV = (In.vTexUV + vUV_TileIndex) / vUV_TileCount + vUV_Offset;
    else
        vNewUV = ((((In.vTexUV + vUV_TileIndex) / vUV_TileCount - 0.5f) * 2.f * (1.f + vUV_Offset)) * 0.5f + 0.5f) * fUV_WaveSpeed;
    
    float fDistortion = 0.f;
    float4 vColor = CalculateEffectColor(vNewUV, In.vTexUV, fDistortion, iSamplerState);
    Out.vDistortion = fDistortion;
    
    if (bOneBlend)
        Out.vOneBlend = vColor;
    else
        Out.vAlphaBlend = vColor;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        Out.vEmissive = vColor * fIntensity_Bloom;
    }

    return Out;
}


PS_OUT_EFFECT PS_MAIN_FXMESH_WDIDEN(VS_OUT In, uniform bool bOneBlend, uniform int iSamplerState)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT)0;

    float2 vNewUV = float2(0.f, 0.f);

    vNewUV = (In.vTexUV + vUV_TileIndex) / vUV_TileCount + vUV_Offset;

    float fDistortion = 0.f;
    float4 vColor = CalculateEffectColor(vNewUV, In.vTexUV, fDistortion, iSamplerState);
    Out.vDistortion = fDistortion;

    vColor.a *= 1.0f - pow(In.vTexUV.y, 2.0f);

    if (vColor.a < 0.1f)
        discard;

    Out.vAlphaBlend = vColor;
    //Out.vOneBlend = float4(1.0f, 1.0f, 1.0f, 1.0f);

    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        Out.vEmissive = vColor * fIntensity_Bloom;
    }

    return Out;
}


technique11 DefaultTechnique
{
    pass OneBlend
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(true, 0);
    }

    pass AlphaBlend
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(false, 0);
    }

    pass OneBlendClamp
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(true, 1);
    }

    pass AlphaBlendClamp
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(false, 1);
    }

    pass OneBlendBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(true, 2);
    }

    pass AlphaBlendBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(false, 2);
    }

    pass OneBlendXWrapYBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(true, 3);
    }

    pass AlphaBlendXWrapYBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(false, 3);
    }
    
    pass OneBlendXBorderYWrap
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(true, 4);
    }

    pass AlphaBlendXBorderYWrap
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(false, 4);
    }

    pass WDIden
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH_WDIDEN(false, 0);
    }
}