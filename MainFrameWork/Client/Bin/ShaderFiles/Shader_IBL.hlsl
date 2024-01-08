#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_InOut.hlsl"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

VS_OUT_TARGET VS_MAIN(TARGET_IN In)
{
    VS_OUT_TARGET Out = (VS_OUT_TARGET) 0;
	
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

    Out.vTexcoord = In.vTexcoord;

    return Out;
}

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 localPos : TEXCOORD1;
};

float4 PS_MAIN(VS_OUT_TARGET In) : SV_TARGET
{
    float2 uv = SampleSphericalMap(normalize(In.vPosition.xyz));
    //float3 colour = shaderTexture.Sample(textureSampler, uv).rgb;
		
    //return float4(colour, 1.0);
    return 1.f;
}

technique11 DefaultTechnique
{
    pass RectToCubemap // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    //pass IntegrateBRDF // 1
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_None, 0);
    //    SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    GeometryShader = NULL;
    //    HullShader = NULL;
    //    DomainShader = NULL;
    //    PixelShader = compile ps_5_0 PS_SSAO_BLUR(true);
    //}

    //pass Irradiance // 2
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_None, 0);
    //    SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    GeometryShader = NULL;
    //    HullShader = NULL;
    //    DomainShader = NULL;
    //    PixelShader = compile ps_5_0 PS_SSAO_BLUR(false);
    //}

    //pass PreFilter // 2
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_None, 0);
    //    SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    GeometryShader = NULL;
    //    HullShader = NULL;
    //    DomainShader = NULL;
    //    PixelShader = compile ps_5_0 PS_SSAO_BLUR(false);
    //}
}




