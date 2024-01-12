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

PS_OUT_EFFECT PS_MAIN_FXMESH(VS_OUT In, uniform bool bOneBlend)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    float2 vNewUV = float2(0.f, 0.f);
    
    if (!bUV_Wave)
        vNewUV = (In.vTexUV + vUV_TileIndex) / vUV_TileCount + vUV_Offset;
    else
        vNewUV = ((((In.vTexUV + vUV_TileIndex) / vUV_TileCount - 0.5f) * 2.f * (1.f + vUV_Offset)) * 0.5f + 0.5f) * fUV_WaveSpeed;
    
    float4 vColor = CalculateEffectColor(vNewUV);
    
    if (bOneBlend)
        Out.vOneBlend = vColor;
    else
        Out.vAlphaBlend = vColor;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, vNewUV) * fIntensity_Bloom;
    }

    return Out;
}

technique11 DefaultTechnique
{
    pass OneBlend // 0
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(true);
    }

    pass AlphaBlend // 0
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXMESH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXMESH(false);
    }
}