

sampler LinearSampler = sampler_state{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};



RasterizerState RS_Skybox
{
	FillMode = solid;

	/* �ո��� �ø��ϰڴ�. == �ĸ��� �����ְڴ�. */
	CullMode = Front;

	/* �ո��� �ð�������� ���ڴ�. */
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

BlendState BS_AlphaBlendDest
{
	BlendEnable[0] = true;

	SrcBlend = One;
	DestBlend = Dest_Alpha;
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
	BlendOpAlpha = Add;
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