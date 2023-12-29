#pragma once
#include "GameObject.h"
#include "BindShaderDesc.h"
#include <random>

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;

END

BEGIN(Client)

class CEffect : public CGameObject
{
	using Super = CGameObject;
public:
	typedef struct tagEffectDesc
	{
		wstring protoModel = TEXT("");
		wstring protoDiffuseTexture = TEXT("");
		wstring protoNoiseTexture = TEXT("");
		wstring protoMaskTexture = TEXT("");
		wstring protoEmissiveTexture = TEXT("");
		wstring protoDissolveTexture = TEXT("");

		Vec3	vPosition_Start = Vec3(0.f, 0.f, 0.f);
		Vec3	vPosition_End = Vec3(0.f, 0.f, 0.f);

		Vec3	vRotation_Start = Vec3(0.f, 0.f, 0.f);
		Vec3	vRotation_End = Vec3(0.f, 0.f, 0.f);

		Vec3	vScaling_Start = Vec3(1.f, 1.f, 1.f);
		Vec3	vScaling_End = Vec3(1.f, 1.f, 1.f);

		Vec3	vVelocity_Start = Vec3(0.f, 0.f, 0.f);
		Vec3	vVelocity_End = Vec3(0.f, 0.f, 0.f);

		Color	vColor_Start = Color(0.f, 0.f, 0.f, 0.f);
		Color	vColor_End = Color(0.f, 0.f, 0.f, 0.f);

		_float	fLifeTime = FLT_MAX;

		Vec2	vUV_Start = Vec2(0.f, 0.f);
		Vec2	vUV_Speed = Vec2(0.f, 0.f);

		_bool	IsSequence = false;
		_float	fSequenceTerm = 0.05f;

		struct tagFX_Variables
		{
			Vec2	vUV_Offset = Vec2(0.f, 0.f);
			Vec2	vUV_Direction = Vec2(1.f, 1.f);
			Vec2	vUV_TileCount = Vec2(1.f, 1.f);
			Vec2	vUV_TileIndex = Vec2(0.f, 0.f);
			Color	vColor_Offset = Vec4(0.f, 0.f, 0.f, 0.f);
		} tVariables;

		struct tagFX_Intensity
		{
			_float	fBloom = 0.f;
			_float	fRadial = 0.f;
			Vec2	vPadding;
		} tIntensity;
	}EFFECTDESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc);
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void Tick(_float fTimeDelta)		override;
	virtual void LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;
	
protected:
	Vec3	m_vPosition_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vPosition_End = Vec3(0.f, 0.f, 0.f);

	Vec3	m_vRotation_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vRotation_End = Vec3(0.f, 0.f, 0.f);

	Vec3	m_vScaling_Start = Vec3(1.f, 1.f, 1.f);
	Vec3	m_vScaling_End = Vec3(1.f, 1.f, 1.f);

	Vec3	m_vVelocity_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vVelocity_End = Vec3(0.f, 0.f, 0.f);

	Color	m_vColor_Start = Color(0.f, 0.f, 0.f, 0.f);
	Color	m_vColor_End = Color(0.f, 0.f, 0.f, 0.f);

	_float	m_fLifeTime = FLT_MAX;

	Vec2	m_vUV_Start = Vec2(0.f, 0.f);
	Vec2	m_vUV_Speed = Vec2(0.f, 0.f);

	_bool	m_IsSequence = false;
	_float	m_fSequenceTerm = 0.05f;

	struct tagFX_Variables
	{
		Vec2	vUV_Offset = Vec2(0.f, 0.f);
		Vec2	vUV_Direction = Vec2(1.f, 1.f);
		Vec2	vUV_TileCount = Vec2(1.f, 1.f);
		Vec2	vUV_TileIndex = Vec2(0.f, 0.f);
		Color	vColor_Offset = Vec4(0.f, 0.f, 0.f, 0.f);
	} m_Variables;

	struct tagFX_Intensity
	{
		_float	fBloom = 0.f;
		_float	fRadial = 0.f;
		Vec2	vPadding;
	} m_Intensity;

	tagEffectDesc m_tVoidEffectDesc;

protected:
	Matrix	m_matPivot = Matrix::Identity;
	Matrix	m_matOffset = Matrix::Identity;

	_float	m_fTimeAcc = 0.f;
	_float	m_fLifeTimeRatio = 0.f;

	_float	m_fSequenceTimer = 0.0f;
	//random_device*		m_RandomDevice = nullptr;

	CTexture* m_pDiffuseTexture		= nullptr;
	CTexture* m_pNoiseTexture		= nullptr;
	CTexture* m_pMaskTexture		= nullptr;
	CTexture* m_pEmissiveTexture	= nullptr;
	CTexture* m_pDissolveTexture	= nullptr;

	EffectMaterialFlag m_tNoisMaskEmisDslv;

protected:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Free();
};

END