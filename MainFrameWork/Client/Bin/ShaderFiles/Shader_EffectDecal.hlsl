#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"
#include "Client_Shader_Effect.hlsl"

matrix g_ViewMatrixInv;
matrix g_ProjMatrixInv;

VS_OUT_FXDECAL VS_MAIN_FXDECAL( /* 정점 */FXDECAL_IN In)
{
    VS_OUT_FXDECAL Out = (VS_OUT_FXDECAL) 0;

    matrix matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

	return Out;	
}

cbuffer TransformInverse
{
    matrix WorldInv;
};

Texture2D g_NormalDepthTarget;
Texture2D g_NormalTarget;
Texture2D g_PropertiesTarget;

PS_OUT_EFFECT PS_MAIN_FXDECAL(VS_OUT_FXDECAL In, uniform bool bOneBlend, uniform int iSamplerState)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    float2 vTexUV;
    vTexUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    // Decal Box의 x,y 위치를 UV좌표로 변환.
	
    if (0.f != g_PropertiesTarget.Sample(PointSampler, vTexUV).z)
        discard;
    
    float4 vNormalDepth = g_NormalDepthTarget.Sample(PointSampler, vTexUV);
    float4 vNormal = g_NormalTarget.Sample(PointSampler, vTexUV);
    
    //vNormal.xyz = vNormal.xyz * 2.f - 1.f;
    //if (dot(vNormal.xyz, float3(0.f, 1.f, 0.f)) < 0.86f)
    //    discard;
    
    float fViewZ = vNormalDepth.w * 1200.f;
	// Decal Box x,y 위치 기준으로 깊이 값을 가져온다.
    
    // 캐릭터 픽셀은 생략
    //if (vNormalDepth.z == 1.f)
    //    discard;    

    float4 vProjPos = 0.f;
    vProjPos.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
    vProjPos.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
    vProjPos.z = (vNormal.w) * fViewZ;
    vProjPos.w = fViewZ;

    float4 vViewPos = mul(vProjPos, g_ProjMatrixInv);
    float4 vWorldPos = mul(vViewPos, g_ViewMatrixInv);
    float4 vLocalPos = mul(vWorldPos, WorldInv);
    
    float3 vObjectAbsPos = abs(vLocalPos.xyz);
    vObjectAbsPos.y = 0.f;
    
    clip(0.5f - vObjectAbsPos);
    // 중점을 기준으로 0.5 이기때문에, 0.5를 뺀 음수값은 상자 밖이므로 자름.

	// 데칼박스 버퍼가 -0.5 ~0.5 사이므로, 0.5를 더해줘서 UV좌표로 만들어줌.
    float2 vDecalUV = vLocalPos.xz + 0.5f;
    
    float fDistortion = 0.f;
    float4 vColor = CalculateEffectColor(vDecalUV, In.vTexcoord, fDistortion, iSamplerState);
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

technique11 DefaultTechnique
{
    pass OneBlend
    {
        SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL(true, 0);
        ComputeShader = NULL;
    }

    pass AlphaBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL(false, 0);
        ComputeShader = NULL;
    }

    pass OneBlendClamp
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL(true, 1);
        ComputeShader = NULL;
    }

    pass AlphaBlendClamp
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL(false, 1);
        ComputeShader = NULL;
    }

    pass OneBlendBorder
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL(true, 2);
        ComputeShader = NULL;
    }

    pass AlphaBlendBorder
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlendEffect, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL(false, 2);
        ComputeShader = NULL;
    }
}
