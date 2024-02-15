#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"

struct VS_OUT_SKY
{
	float4	vPosition : SV_POSITION;
    float2	vTexcoord : TEXCOORD0;
};

VS_OUT_SKY VS_MAIN(STATIC_IN In)
{
    VS_OUT_SKY Out = (VS_OUT_SKY) 0;

	matrix matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
	
	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexUV;
	
	return Out;
}

cbuffer cbSkyFloorDesc
{
    float2 vTileCount = float2(1.f, 1.f);
    float2 vFloorUVoffset = float2(0.f, 0.f);
    float fAlpha = 1.f;
    float3 vPadding;
};

float4 PS_MAIN(VS_OUT_SKY In) : SV_TARGET0
{
    return pow(g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord), 2.2f);
}

float3 g_vColorMul = float3(1.f, 1.f, 1.f);

float4 PS_MAIN_FLOOR(VS_OUT_SKY In) : SV_TARGET0
{
    float2 vTexcoord = In.vTexcoord * vTileCount + vFloorUVoffset;
	
    vector vNoise = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord + 4.f * vFloorUVoffset);
    float2 noise = pow(vNoise.r, 3.f);
    vTexcoord += noise;
	
    float4 vSample = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    //float3 vColor = pow(vSample.rgb, 2.2f);
    float3 vColor = vSample.rgb;
	
	vColor *= g_vColorMul;
    
	float alpha = clamp(fAlpha * (vSample.a + EPSILON), 0.f, 1.f);

	clip(alpha - 0.05f);

    return float4(vColor, alpha);
}

technique11 DefaultTechnique
{
	pass SkyDome
	{
        SetRasterizerState(RS_Effect);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass SkyFloor
	{
        SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FLOOR();
	}

	//pass SkyFloor
	//{
 //       SetRasterizerState(RS_Default);
	//	SetDepthStencilState(DSS_None, 0);
	//	SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	GeometryShader = NULL;
	//	PixelShader = compile ps_5_0 PS_MAIN();
	//}
}