#include "Client_Shader_Light.hlsl"
#include "Client_Shader_InOut.hlsl"
bool    g_bDissolve = false;

// For Grass
float3      g_vWind = { 0.f, 0.f, 0.f };
float       g_fPower = 0.f;
float       g_fTime = 2.f;
float       g_fmaxHeight = 2.f;



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

VS_OUT VS_GRASS(STATIC_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    matrix matWV = mul(WorldMatrix, ViewProj);
    matrix matWVP = mul(matWV, ProjMatrix);

    float heightFactor = saturate(In.vPosition.y / g_fmaxHeight);
    float windEffect = sin(g_fTime + In.vPosition.x * g_vWind.x + In.vPosition.y * g_vWind.y + In.vPosition.z * g_vWind.z) * g_fPower * heightFactor;

    float3 modifiedPosition = In.vPosition + windEffect * g_vWind;

    Out.vPosition = mul(float4(modifiedPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), WorldMatrix));
    Out.vNormalV = normalize(mul(Out.vNormal, ViewMatrix).xyz);
    Out.vTexUV = In.vTexUV;
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


    if (g_bDissolve == true)
        ComputeDissolveColor(Out.vDiffuse, In.vTexUV);
    
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
    
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
    
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
        //if (1.f == SpecMaskEmisExtr.y)
        //{
        //    float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
        //    Out.vProperties.r = vSpecular.r * vMRMask.r * (1.f - vSpecular.a);
        //    Out.vProperties.g = vSpecular.r * vMRMask.g * vSpecular.a;
        //}
        //else
        //{
        Out.vProperties.r = clamp(0.0f, 1.0f, 1.f - pow(1.f - vSpecular.b, 1.3f)); // Metalic
        Out.vProperties.g = pow(vSpecular.g, 1.5f); // Roughness
        //}
        
        //Out.vProperties.r = smoothstep(0.0f, 0.95f, 1.f - pow(1.f - vSpecular.b, 2.f)); // Metalic
        //Out.vProperties.g = pow(vSpecular.g, 2.f); // Roughness
        
    }
    else
    {
        Out.vProperties.r = 0.04f;
        Out.vProperties.g = 0.5f;
    }
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
        Out.vEmissive *= g_vBloomColor;
    }
    
    Out.vProperties.z = MT_STATIC;
    Out.vProperties.w = g_fRimLight;

    return Out;
}

float4 PS_ALPHA(VS_OUT In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);

    float4 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
    float4 vLook = In.vProjPos - float4(CameraPosition(), 1.f);

    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);
    


    vColor = (g_vLightDiffuse * vColor) + fSpecular;
    
    return vColor;
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
	
    Out.vProperties.z = MT_STATIC;
    
    return Out;
}

PS_OUT_PHONG PS_CHANGECOLOR(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
    if (0.2f >= Out.vDiffuse.a)
        discard;

    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexUV);
	
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, In.vProjPos.z / In.vProjPos.w);
    Out.vNormalV = vector(In.vNormalV, In.vProjPos.w / 1200.0f);
    
    if (1.f == SpecMaskEmisExtr.x)
    {
        float4 vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);

        Out.vProperties.r = clamp(0.0f, 1.0f, 1.f - pow(1.f - vSpecular.b, 1.3f)); // Metalic
        Out.vProperties.g = pow(vSpecular.g, 1.5f); // Roughness
        
        //Out.vProperties.r = smoothstep(0.0f, 0.95f, 1.f - pow(1.f - vSpecular.b, 2.f)); // Metalic
        //Out.vProperties.g = pow(vSpecular.g, 2.f); // Roughness
    }
    else
    {
        Out.vProperties.r = 0.04f;
        Out.vProperties.g = 0.5f;
    }
    
    if (1.f == SpecMaskEmisExtr.z)
    {
        Out.vEmissive = g_EmissiveTexture.Sample(LinearSampler, In.vTexUV);
        Out.vEmissive *= g_vBloomColor;
    }
    
    float4 vMRMask = g_MRMaskTexture.Sample(LinearSampler, In.vTexUV);
    float4 vGiven;
    
    float maxColor = max(max(vMRMask.r, vMRMask.g), vMRMask.b);
    float maxR = g_vColor_R.r + g_vColor_R.g + g_vColor_R.b;
    float maxG = g_vColor_G.r + g_vColor_G.g + g_vColor_G.b;
    float maxB = g_vColor_B.r + g_vColor_B.g + g_vColor_B.b;
    float fColorFactor;
    
    if (0 != maxColor)
    {
        if (0 != vMRMask.r && vMRMask.r == vMRMask.g && vMRMask.g == vMRMask.b)
        {
            if (maxR > maxG)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else if (maxR > maxB)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else if (maxG > maxB)
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
            else
            {
                vGiven = g_vColor_B;
                fColorFactor = vMRMask.b;
            }
        }
        else if (0 != vMRMask.r && vMRMask.r == vMRMask.g)
        {
            if (maxR > maxG)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
        }
        else if (0 != vMRMask.r && vMRMask.r == vMRMask.b)
        {
            if (maxR > maxB)
            {
                vGiven = g_vColor_R;
                fColorFactor = vMRMask.r;
            }
            else
            {
                vGiven = g_vColor_B;
                fColorFactor = vMRMask.b;
            }
        }
        else if (vMRMask.r == maxColor)
        {
            vGiven = g_vColor_R;
            fColorFactor = vMRMask.r;
        }
        else if (0 != vMRMask.g && vMRMask.g == vMRMask.b)
        {
            if (maxG > maxB)
            {
                vGiven = g_vColor_G;
                fColorFactor = vMRMask.g;
            }
            else
            {
                vGiven = g_vColor_B;
                fColorFactor = vMRMask.b;
            }
        }
        else if (vMRMask.g == maxColor)
        {
            vGiven = g_vColor_G;
            fColorFactor = vMRMask.g;
        }
        else if (vMRMask.b == maxColor)
        {
            vGiven = g_vColor_B;
            fColorFactor = vMRMask.b;
        }
        
        if (0 != vGiven.a)
        {
            Out.vDiffuse.rgb *= (vGiven.rgb + (1 - fColorFactor)) * vGiven.a;
            Out.vDiffuse.a = 1.f;
        }
    }
	
    Out.vProperties.z = MT_DYNAMIC;
    
    return Out;
}

PS_OUT_PHONG PS_NARUTO(VS_OUT In)
{
    PS_OUT_PHONG Out = (PS_OUT_PHONG) 0;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
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

    Out.vTexUV = In.vTexUV;

    return Out;
}

float4 PS_SHADOW(VS_OUT_SHADOW In) : SV_TARGET0
{
    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

   /* if (vColor.a == 0.0f)
        discard;*/

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
        SetRasterizerState(RS_Effect);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SHADOW();
    }

    pass Alpha // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ALPHA();
    }

    pass Diffuse // 4 юс╫ц
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_NARUTO();
    }

    pass ChangeColor // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CHANGECOLOR();
    }

    pass Grass  // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_GRASS();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PBR();
    }

}