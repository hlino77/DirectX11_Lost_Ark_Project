#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_InOut.hlsl"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

cbuffer PerFrame
{
    float4x4 g_ViewToTexSpace; // Proj*Texture
    float4 g_OffsetVectors[14];
    float4 g_FrustumCorners[4];

	// Coordinates given in view space.
    float g_OcclusionRadius = 0.5f;
    float g_OcclusionFadeStart = 0.2f;
    float g_OcclusionFadeEnd = 2.0f;
    float g_SurfaceEpsilon = 0.05f;
};

Texture2D		g_NormalDepthTarget;

Texture2D		g_Texture;
Texture2D		g_RandomVectorTexture;

SamplerState samNormalDepth
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;

	// Set a very far depth value if sampling outside of the NormalDepth map
	// so we do not get false occlusions.
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f);
};

sampler RandomVecSampler = sampler_state
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct VS_IN_SSAO
{
	float3		vPosition : POSITION;
    float3		vToFarPlaneIndex : NORMAL;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT_SSAO
{	
	float4		vPosition : SV_POSITION;
    float3		vToFarPlane : TEXCOORD0;
	float2		vTexcoord : TEXCOORD1;
};

VS_OUT_SSAO VS_MAIN_SSAO( /* 정점 */VS_IN_SSAO In)
{
    VS_OUT_SSAO Out = (VS_OUT_SSAO) 0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	// We store the index to the frustum corner in the normal x-coord slot.
    Out.vToFarPlane = g_FrustumCorners[In.vToFarPlaneIndex.x].xyz;
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

// Determines how much the sample point q occludes the point p as a function
// of distZ.
float OcclusionFunction(float fDistZ)
{
	//
	// If depth(q) is "behind" depth(p), then q cannot occlude p.  Moreover, if 
	// depth(q) and depth(p) are sufficiently close, then we also assume q cannot
	// occlude p because q needs to be in front of p by Epsilon to occlude p.
	//
	// We use the following function to determine the occlusion.  
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//
	
    float fOcclusion = 0.0f;
    if (fDistZ > g_SurfaceEpsilon)
    {
        float fFadeLength = g_OcclusionFadeEnd - g_OcclusionFadeStart;
		
		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
        fOcclusion = saturate((g_OcclusionFadeEnd - fDistZ) / fFadeLength);
    }
	
    return fOcclusion;
}

float4 PS_SSAO(VS_OUT_SSAO In) : SV_Target
{
	// p -- the point we are computing the ambient occlusion for.
	// n -- normal vector at p.
	// q -- a random offset from p.
	// r -- a potential occluder that might occlude p.

	// Get viewspace normal and z-coord of this pixel.  The tex-coords for
	// the fullscreen quad we drew are already in uv-space.
    float4 vViewNormalDepth = g_NormalDepthTarget.SampleLevel(samNormalDepth, In.vTexcoord, 0.0f);
	
    float3 vViewNormal = vViewNormalDepth.xyz;
    float fViewZ = vViewNormalDepth.w * g_fFar;
	
	//
	// Reconstruct full view space position (x,y,z).
	// Find t such that p = t*pin.ToFarPlane.
	// p.z = t*pin.ToFarPlane.z
	// t = p.z / pin.ToFarPlane.z
	//
	
    float3 P = (fViewZ / In.vToFarPlane.z) * In.vToFarPlane;
	
	// Extract random vector and map from [0,1] --> [-1, +1].
    float3 vRandVec = 2.0f * g_RandomVectorTexture.SampleLevel(RandomVecSampler, 4.0f * In.vTexcoord, 0.0f).rgb - 1.0f;
	
    float fOcclusionSum = 0.0f;
	
	[unroll]
    for (int i = 0; i < 14; ++i)
    {
		// Are offset vectors are fixed and uniformly distributed (so that our offset vectors
		// do not clump in the same direction).  If we reflect them about a random vector
		// then we get a random uniform distribution of offset vectors.
        float3 vOffset = reflect(g_OffsetVectors[i].xyz, vRandVec);
        //float3 offset = reflect(gOffsetVectors[i].xyz, n);

        float fFlip = sign(dot(vOffset, vViewNormal.xyz));
		
		// Sample a point near p within the occlusion radius.
        //float3 q = p + gOcclusionRadius * offset;
        float3 Q = P + fFlip * g_OcclusionRadius * vOffset; // q = 차폐 검사할 벡터

		// Project q and generate projective tex-coords.
        float4 vProjQ = mul(float4(Q, 1.0f), g_ViewToTexSpace);
        vProjQ /= vProjQ.w;

		// Find the nearest depth value along the ray from the eye to q (this is not
		// the depth of q, as q is just an arbitrary point near p and might
		// occupy empty space).  To find the nearest depth we look it up in the depthmap.
		
        float RZ = g_NormalDepthTarget.SampleLevel(samNormalDepth, vProjQ.xy, 0.0f).w;
        RZ *= g_fFar;
		
		// Reconstruct full view space position r = (rx,ry,rz).  We know r
		// lies on the ray of q, so there exists a t such that r = t*q.
		// r.z = t*q.z ==> t = r.z / q.z

        float3 R = (RZ / Q.z) * Q;
		
		//
		// Test whether r occludes p.
		//   * The product dot(n, normalize(r - p)) measures how much in front
		//     of the plane(p,n) the occluder point r is.  The more in front it is, the
		//     more occlusion weight we give it.  This also prevents self shadowing where 
		//     a point r on an angled plane (p,n) could give a false occlusion since they
		//     have different depth values with respect to the eye.
		//   * The weight of the occlusion is scaled based on how far the occluder is from
		//     the point we are computing the occlusion of.  If the occluder r is far away
		//     from p, then it does not occlude it.
		// 
		
        float fDistZ = P.z - R.z;
        float dP = max(dot(vViewNormal, normalize(R - P)), 0.0f);
        
        float fOcclusion = dP * OcclusionFunction(fDistZ);
        fOcclusionSum += fOcclusion;
    }
	
    fOcclusionSum /= 14.0f;
	
    float fAccess = 1.0f - fOcclusionSum;

	// Sharpen the contrast of the SSAO map to make the SSAO affect more dramatic.

    return saturate(pow(fAccess, 4.0f));
}

VS_OUT_TARGET VS_MAIN_SSAO_BLUR( /* 정점 */TARGET_IN In)
{
    VS_OUT_TARGET Out = (VS_OUT_TARGET) 0;
	
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

cbuffer cbPerFrame
{
    float gTexelWidth = 1.f / 1600.f;
    float gTexelHeight = 1.f / 900.f;
};

cbuffer cbSettings
{
    float gWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };
};

cbuffer cbFixed
{
    static const int gBlurRadius = 5;
};

SamplerState samInputImage
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

Texture2D g_SSAOBlurTarget;

float4 PS_SSAO_BLUR(VS_OUT_TARGET In, uniform bool bHorizontal) : SV_Target
{
    float2 texOffset;
    if (bHorizontal)
        texOffset = float2(gTexelWidth, 0.0f);
    else
        texOffset = float2(0.0f, gTexelHeight);

	// The center value always contributes to the sum.
    float4 color = gWeights[5] * g_SSAOBlurTarget.SampleLevel(samInputImage, In.vTexcoord, 0.0);
    float totalWeight = gWeights[5];
	 
    float4 centerNormalDepth = g_NormalDepthTarget.SampleLevel(samNormalDepth, In.vTexcoord, 0.0f);

    for (float i = -gBlurRadius; i <= gBlurRadius; ++i)
    {
		// We already added in the center weight.
        if (i == 0)
            continue;

        float2 tex = In.vTexcoord + i * texOffset;

        float4 neighborNormalDepth = g_NormalDepthTarget.SampleLevel(samNormalDepth, tex, 0.0f);

		//
		// If the center value and neighbor values differ too much (either in 
		// normal or depth), then we assume we are sampling across a discontinuity.
		// We discard such samples from the blur.
		//
	
        if (dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
		    abs(neighborNormalDepth.a - centerNormalDepth.a) <= 0.2f)
        {
            float weight = gWeights[i + gBlurRadius];

			// Add neighbor pixel to blur.
            color += weight * g_SSAOBlurTarget.SampleLevel(samInputImage, tex, 0.0);
            totalWeight += weight;
        }
    }

	// Compensate for discarded samples by making total weights sum to 1.
    return color / totalWeight;
}

technique11 DefaultTechnique
{
    pass SSAO // 0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN_SSAO();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO();
    }

	pass SSAO_Blur_H // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN_SSAO_BLUR();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO_BLUR(true);
    }

    pass SSAO_Blur_V // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN_SSAO_BLUR();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO_BLUR(false);
    }
}




