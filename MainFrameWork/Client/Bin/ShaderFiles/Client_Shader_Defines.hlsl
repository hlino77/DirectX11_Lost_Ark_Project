#ifndef _CLIENT_SHADER_DEFINES_HLSL_
#define _CLIENT_SHADER_DEFINES_HLSL_

sampler LinearSampler = sampler_state{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
	AddressW = wrap;
};

sampler LinearClampSampler = sampler_state{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = clamp;
	AddressV = clamp;
	AddressW = clamp;
};

sampler LinearBorderSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
};

sampler LinearXWrapYBorderSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = BORDER;
	AddressW = BORDER;
};

sampler LinearXBorderYWrapSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = wrap;
	AddressW = BORDER;
};

sampler AnisotropicClampSampler = sampler_state
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;
    AddressU = clamp;
    AddressV = clamp;
    AddressW = clamp;
    ComparisonFunc = ALWAYS;
    MaxLOD = 0.f;
    MinLOD = 0.f;
};

sampler PointSampler = sampler_state{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
	AddressW = wrap;
};

sampler MirrorSampler = sampler_state
{
    filter = min_mag_mip_linear;
	/*minfilter = linear;
	magfilter = linear;
	mipfilter = linear;*/
    AddressU = mirror;
    AddressV = mirror;
    AddressW = mirror;
};

RasterizerState RS_Skybox
{
	FillMode = solid;

	/* 앞면을 컬링하겠다. == 후면을 보여주겠다. */
	CullMode = Front;

	/* 앞면을 시계방향으로 쓰겠다. */
	FrontCounterClockwise = false;
};

RasterizerState RS_Outline
{
    FillMode = solid;

	/* 앞면을 컬링하겠다. == 후면을 보여주겠다. */
    CullMode = Front;

	/* 앞면을 시계방향으로 쓰겠다. */
    FrontCounterClockwise = false;
};

RasterizerState RS_Effect
{
	FillMode = Solid;
	CullMode = None;
	FrontCounterClockwise = false;
};

RasterizerState RS_Default
{
	FillMode = solid;
};

RasterizerState RS_Wireframe
{
	FillMode = wireframe;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};
    
DepthStencilState DSS_Skybox
{
	DepthEnable = false;
	DepthWriteMask = zero;		
};

DepthStencilState DSS_None
{
	DepthEnable = false;
	DepthWriteMask = zero;
	DepthFunc = always;
};

DepthStencilState DSS_Effect
{
    DepthEnable = true;
    DepthWriteMask = zero;
};

DepthStencilState DSS_Outline
{
    DepthEnable = true;
    DepthWriteMask = zero;

    StencilEnable = true;

    FrontFaceStencilFunc = not_equal;
    FrontFaceStencilPass = keep;
    FrontFaceStencilFail = keep;

    BackFaceStencilFunc = not_equal;
    BackFaceStencilPass = keep;
    BackFaceStencilFail = keep;
};

DepthStencilState DSS_Inline
{
    DepthEnable = true;
    DepthWriteMask = zero;

    StencilEnable = true;

    FrontFaceStencilFunc = always;
    FrontFaceStencilPass = keep;
    FrontFaceStencilFail = keep;
};

BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

BlendState BS_AlphaBlendEffect
{
	BlendEnable[0] = true;
    BlendEnable[1] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;

	SrcBlendAlpha = Src_Alpha;
	DestBlendAlpha = Dest_Alpha;
	BlendOpAlpha = MAX;
};

BlendState BS_AlphaBlendDest
{
	BlendEnable[0] = true;

	SrcBlend = One;
	DestBlend = Dest_Alpha;
	BlendOp = Add;
};

BlendState BS_OneBlend
{
    BlendEnable[0] = true;
    BlendEnable[1] = true;

    SrcBlend = One;
    DestBlend = One;
    BlendOp = Add;
};

BlendState BS_BlurBlend
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Dest_Alpha;
	BlendOp = Add;
	SrcBlendAlpha = One;
	DestBlendAlpha = Zero;
	BlendOpAlpha = Add;
};

BlendState BS_BlendDestAlpha
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
	SrcBlendAlpha = Src_Alpha;
	DestBlendAlpha = Dest_Alpha;
	BlendOpAlpha = MAX;
};

BlendState BS_BlendDefault
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	BlendOp = Add;
};

RasterizerState RS_ShadowDepth
{
	FillMode = Solid;
	CullMode = None;
};

BlendState BS_AlphaBlendTest
{
	BlendEnable[0] = true;

	SrcBlend = One;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

BlendState BS_ModelEffect
{
	BlendEnable[0] = true;

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
	SrcBlendAlpha = One;
	DestBlendAlpha = Zero;
	BlendOpAlpha = Add;
};

#endif