#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"
#include "Client_Shader_Effect.hlsl"

VS_OUT_FXTRAIL VS_MAIN_FXTRAIL( /* Á¤Á¡ */FXTRAIL_IN In)
{
    VS_OUT_FXTRAIL Out = (VS_OUT_FXTRAIL) 0;
    
    //matrix matWVP = mul(WorldMatrix, ViewProj);
    matrix matWVP = ViewProj;

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = float2(In.vTexcoord.x, In.vTexcoord.y);
    Out.fAlpha = In.fAlpha;

	return Out;
}

PS_OUT_EFFECT PS_MAIN_FXTRAIL(VS_OUT_FXTRAIL In, uniform bool bOneBlend, uniform int iSamplerState)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    float2 vNewUV = float2(0.f, 0.f);
    
    if (!bUV_Wave)
        vNewUV = (In.vTexcoord + vUV_TileIndex) / vUV_TileCount + vUV_Offset;
    else
    {
        vNewUV = (In.vTexcoord - 0.5f) * 2.f;
        float2 vDir = normalize(vNewUV);
        vNewUV += vDir * vUV_Offset.x;
        vNewUV.x -= int(vNewUV.x);
        vNewUV.y -= int(vNewUV.y);
        vNewUV = vNewUV * 0.5f + 0.5f;
    }

    float fDistortion = 0.f;

    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vColor = CalculateEffectColor(vNewUV, In.vTexcoord, fDistortion, iSamplerState);
    
    vColor.a *= In.fAlpha;
    Out.vDistortion = fDistortion;
    
    if (bOneBlend)
        Out.vOneBlend = vColor;
    else
        Out.vAlphaBlend = vColor;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        Out.vEmissive = float4(vColor.rgb * vColor.a * fIntensity_Bloom, 1.f);
    }
    
    return Out;
}

technique11 DefaultTechnique
{
    pass OneBlend
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(true, 0);
        ComputeShader = NULL;
    }

    pass AlphaBlend
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(false, 0);
        ComputeShader = NULL;
    }

    pass OneBlendClamp
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(true, 1);
        ComputeShader = NULL;
    }

    pass AlphaBlendClamp
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(false, 1);
        ComputeShader = NULL;
    }

    pass OneBlendBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(true, 2);
        ComputeShader = NULL;
    }

    pass AlphaBlendBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(false, 2);
        ComputeShader = NULL;
    }

    pass OneBlendXWrapYBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(true, 3);
        ComputeShader = NULL;
    }

    pass AlphaBlendXWrapYBorder
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(false, 3);
        ComputeShader = NULL;
    }

    pass OneBlendXBorderYWrap
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(true, 4);
        ComputeShader = NULL;
    }

    pass AlphaBlendXBorderYWrap
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTRAIL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTRAIL(false, 4);
        ComputeShader = NULL;
    }
}