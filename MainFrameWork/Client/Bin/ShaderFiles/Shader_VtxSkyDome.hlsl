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

float4 PS_MAIN(VS_OUT_SKY In) : SV_TARGET0
{
    return g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
}

technique11 DefaultTechnique
{
	pass SkyDome
	{
        SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}