#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"

Texture2D	g_MaskTexture;
Texture2D	g_MaskTexture2;
Texture2D	g_DisolveTexture;

matrix		g_ShadowViewMatrix;

float4		g_vColor;
float4		g_vBlurColor;

float		g_fUVX;
float		g_fUVY;
float		g_fAlphaWeight;
float		g_fDiscardAlpha;

VS_OUT VS_MAIN(STATIC_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), WorldMatrix));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);

    Out.vTexUV = In.vTexUV;
    //Out.vWorldPos = mul(float4(In.vPosition, 1.f), WorldMatrix);
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), WorldMatrix)).xyz;
    Out.vProjPos = Out.vPosition;
	
	return Out;
}

PS_OUT_PBR PS_PBR(VS_OUT In)
{
    PS_OUT_PBR Out = (PS_OUT_PBR) 0;

    Out.vDiffuse = (vector) 1.f;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
    
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
    
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        if (1.f == SpecMaskEmisExtr.y)
        {
            float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
            Out.vMetallic = vSpecular * vMRMask.r * (1.f - vSpecular.a);
            Out.vRoughness = vSpecular * vMRMask.g * vSpecular.a;
        }
        else
        {
            Out.vMetallic = Out.vDiffuse * vSpecular.r * (1.f - vSpecular.a);
            Out.vRoughness = Out.vDiffuse * vSpecular.g * vSpecular.a;
        }
    }
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
    }
    
    return Out;
}

PS_OUT_PHONG PS_PHONG(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
	
    return Out;
}

PS_OUT_PHONG PS_NARUTO(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV) * 0.3f;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);

    if (0.2f >= Out.vDiffuse.a)
        discard;

    return Out;
}

VS_OUT_SHADOW VS_SHADOW(STATIC_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;
	
	float4 vPosition = float4(In.vPosition, 1.f);
	vPosition = mul(vPosition, WorldMatrix);
    vPosition = mul(vPosition, ViewProj);

    Out.vPosition = vPosition;
    Out.vProjPos = Out.vPosition;

    return Out;
}

float4 PS_SHADOW(VS_OUT_SHADOW In) : SV_TARGET0
{
    return float4(In.vProjPos.z / In.vProjPos.w, 0.0f, 0.0f, 0.0f);
}

technique11 DefaultTechnique
{
    pass PBR // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR();
    }

    pass Phong // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PHONG();
    }

    pass ShadowPass // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SHADOW();
    }

    pass Diffuse // 3 юс╫ц
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_NARUTO();
    }
}