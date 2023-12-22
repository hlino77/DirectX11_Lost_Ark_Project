#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"

VS_OUT_FXTEX VS_MAIN_FXTEX( /* 정점 */FXTEX_IN In)
{
    VS_OUT_FXTEX Out = (VS_OUT_FXTEX) 0;

    vector vRight = WorldMatrix._11_12_13_14;
    vector vUp = WorldMatrix._21_22_23_24;

    Out.vPosition = mul(float4(In.vPosition, 1.f), WorldMatrix);
	Out.vPSize = float2(In.vPSize.x * length(vRight), In.vPSize.y * length(vUp));

	return Out;	
}

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
    //float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
    //float4		vProjPos : TEXCOORD1;
};

/* 여러개의 정점을 생성한다. */
[maxvertexcount(20)]
void GS_MAIN_FXTEX(point VS_OUT_FXTEX In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

	/* 받아온 정점을 기준으로하여 사각형을 구성하기위한 정점 여섯개를 만들거야. */
    float3  vLook = CameraPosition() - In[0].vPosition.xyz;

	float3	vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
	float3	vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

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
float4 PS_MAIN_FXTEX(GS_OUT In) : SV_TARGET0
{
    float2 vNewUV = In.vTexcoord;
    float  fMask = 0.f;
    float3 vEmissive = float3(0.f, 0.f, 0.f);
     
    if (EPSILON < NoisMaskEmisDslv.x)   // Noise
    {
        vNewUV = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).rg;
        vNewUV += (vNewUV - 0.5f) * 2.f;
    }   
    if (EPSILON < NoisMaskEmisDslv.y)   // Mask
    {
        fMask = g_MaskTexture.Sample(LinearSampler, vNewUV).r;
        clip(fMask);
    }
    
    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vColor.a = fMask;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        float3 vEmissive = g_EmissiveTexture.Sample(LinearSampler, vNewUV).rgb;
        vColor.rgb += vEmissive;
    }
	if (EPSILON < NoisMaskEmisDslv.w)	// Dissolve
    {
        float fDissolve = g_DissolveTexture.Sample(LinearSampler, vNewUV).x;
        
	    //Discard the pixel if the value is below zero
        clip(fDissolve - g_fDissolveAmount);
	    //Make the pixel emissive if the value is below ~f
        //if (fDissolve - g_fDissolveAmount < 0.25f)/*0.08f*/
        //{
        //    vEmissive = float3(0.3f, 0.3f, 0.3f);
        //}
    }
    
    return vColor;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* 여러 셰이더에 대해서 각각 어떤 버젼으로 빌드하고 어떤 함수를 호출하여 해당 셰이더가 구동되는지를 설정한다. */
        VertexShader = compile vs_5_0 VS_MAIN_FXTEX();
        GeometryShader = compile gs_5_0 GS_MAIN_FXTEX();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTEX();
        ComputeShader = NULL;
    }
}




