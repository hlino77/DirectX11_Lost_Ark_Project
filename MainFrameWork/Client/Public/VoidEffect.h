#pragma once

#include "Client_Defines.h"
#include "BindShaderDesc.h"
#include "EffectTool.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Point;

END

BEGIN(Client)

class CEffectTool;
class CVoidEffect : public CGameObject
{
	using Super = CGameObject;
public:
	struct tagVoidEffectDesc
	{
		wstring protoModel				= TEXT("");
		wstring protoDiffuseTexture		= TEXT("");
		wstring protoNoiseTexture		= TEXT("");
		wstring protoMaskTexture		= TEXT("");
		wstring protoEmissiveTexture	= TEXT("");
		wstring protoDissolveTexture	= TEXT("");
	};
private:
	CVoidEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVoidEffect(const CVoidEffect& rhs);
	virtual ~CVoidEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(tagVoidEffectDesc* pDesc);
	virtual HRESULT Ready_Components() override { return E_FAIL; };

public:
	Matrix	m_matPivot = Matrix::Identity;

	Vec3	m_vPosition_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vPosition_End = Vec3(0.f, 0.f, 0.f);
	_bool	m_bPosition_Lerp = false;

	Vec3	m_vRotation_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vRotation_End = Vec3(0.f, 0.f, 0.f);
	_bool	m_bRotation_Lerp = false;

	Vec3	m_vScaling_Start = Vec3(1.f, 1.f, 1.f);
	Vec3	m_vScaling_End = Vec3(1.f, 1.f, 1.f);
	_bool	m_bScaling_Lerp = false;

	Vec3	m_vVelocity_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vVelocity_End = Vec3(0.f, 0.f, 0.f);
	_bool	m_bVelocity_Lerp = false;

	Color	m_vColor_Start = Color(0.f, 0.f, 0.f, 0.f);
	Color	m_vColor_End = Color(0.f, 0.f, 0.f, 0.f);
	_bool	m_bColor_Lerp = false;

	_float	m_fLifeTime = FLT_MAX;
	_float	m_fTimeAcc = 0.f;
	_float	m_fLifeTimeRatio = 0.f;

	Vec2	m_vUV_Speed = Vec2(0.f, 0.f);

	struct tagEffectVariables
	{
		Vec2	vUV_Offset = Vec2(0.f, 0.f);
		Vec2	vUV_Direction = Vec2(1.f, 1.f);
		Vec2	vUV_TileCount = Vec2(1.f, 1.f);
		Vec2	vUV_TileIndex = Vec2(0.f, 0.f);
		Color	vColor_Offset = Vec4(0.f, 0.f, 0.f, 0.f);
	} m_Variables;

	struct tagIntensity
	{
		_float	fBloom = 0.f;
		_float	fRadial = 0.f;
		Vec2	vPadding;
	} m_Intensity;

	//_bool	m_bBlendNonBlend = 0.f;

private:
	CVIBuffer_Point* m_pBuffer		= nullptr;
	CTexture*	m_pDiffuseTexture	= nullptr;
	CTexture*	m_pNoiseTexture		= nullptr;
	CTexture*	m_pMaskTexture		= nullptr;
	CTexture*	m_pEmissiveTexture	= nullptr;
	CTexture*	m_pDissolveTexture	= nullptr;

	EffectMaterialFlag m_tNoisMaskEmisDslv;

public:
	static CVoidEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END