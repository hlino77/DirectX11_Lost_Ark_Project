#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"
#include "Client_Shader_Effect.hlsl"

VS_OUT_FXTEX VS_MAIN_FXTEX( /* 정점 */FXTEX_IN In)
{
    VS_OUT_FXTEX Out = (VS_OUT_FXTEX) 0;

    float4 vRight = WorldMatrix._11_12_13_14;
    float4 vUp = WorldMatrix._21_22_23_24;

    Out.vPosition = mul(float4(In.vPosition, 1.f), WorldMatrix);
	Out.vPSize = float2(In.vPSize.x * length(vRight), In.vPSize.y * length(vUp));

	return Out;	
}

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

/* 여러개의 정점을 생성한다. */
[maxvertexcount(20)]
void GS_MAIN_FXTEX(point VS_OUT_FXTEX In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

    float3 vLook = float3(0.f, 0.f, 0.f);
    float3  vUp, vRight;

    if (bBillboard)
    {
        vLook = CameraPosition() - In[0].vPosition.xyz;
        vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook)) * In[0].vPSize.x * 0.5f;
        vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;
    }
    else
    {
        float3 vCamLook = CameraPosition() - In[0].vPosition.xyz;
        vUp = WorldMatrix._21_22_23 * In[0].vPSize.y * 0.5f;
        vRight = normalize(cross(vUp, vCamLook)) * In[0].vPSize.x * 0.5f;

       /* vLook = WorldMatrix._31_32_33;
        vUp = WorldMatrix._21_22_23;
        vRight = normalize(cross(vUp, vLook)) * In[0].vPSize.x * 0.5f;
        vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;*/
    }

	matrix		matVP;

	matVP = ViewProj;

	Out[0].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexcoord = float2(0.0f, 0.f);

	Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexcoord = float2(1.0f, 0.f);

	Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexcoord = float2(1.0f, 1.0f);

	Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexcoord = float2(0.0f, 1.0f);
	
	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}

/* w나누기 연산. 진정한 투영변환. */
/* 뷰포트스페이스(윈도우좌표)로 위치를 변환한다. */
/* 래스터라이즈 : 정점에 둘러쌓인 픽셀의 정보를 생성한다. */
/* 픽셀정보는 정점정보에 기반한다. */

/* 전달받은 픽셀의 정보를 이용하여 픽셀의 최종적인 색을 결정하자. */
PS_OUT_EFFECT PS_MAIN_FXTEX(GS_OUT In, uniform bool bOneBlend)
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
    float4 vColor = CalculateEffectColor(vNewUV, In.vTexcoord, fDistortion);
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
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTEX();
        GeometryShader = compile gs_5_0 GS_MAIN_FXTEX();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTEX(true);
        ComputeShader = NULL;
    }

    pass AlphaBlend
    {
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_FXTEX();
        GeometryShader = compile gs_5_0 GS_MAIN_FXTEX();
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTEX(false);
        ComputeShader = NULL;
    }
}
