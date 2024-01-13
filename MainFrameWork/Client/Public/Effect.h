#pragma once
#include "GameObject.h"
#include "BindShaderDesc.h"
#include <random>
#include "Effect_Manager.h"

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
		_int	iEffectType = -1;
		wstring EffectTag = TEXT("");
		wstring protoModel = TEXT("");
		wstring protoDiffuseTexture = TEXT("");
		wstring protoNoiseTexture = TEXT("");
		wstring protoMaskTexture = TEXT("");
		wstring protoEmissiveTexture = TEXT("");
		wstring protoDissolveTexture = TEXT("");

		Vec3	vPosition_Start = Vec3(0.f, 0.f, 0.f);
		Vec3	vPosition_End = Vec3(0.f, 0.f, 0.f);
		_bool	bPosition_Lerp = true;

		Vec3	vRotation_Start = Vec3(0.f, 0.f, 0.f);
		Vec3	vRotation_End = Vec3(0.f, 0.f, 0.f);
		_bool	bRotation_Lerp = true;

		Vec3	vScaling_Start = Vec3(1.f, 1.f, 1.f);
		Vec3	vScaling_End = Vec3(1.f, 1.f, 1.f);
		_bool	bScaling_Lerp = true;

		Vec3	vVelocity_Start = Vec3(0.f, 0.f, 0.f);
		Vec3	vVelocity_End = Vec3(0.f, 0.f, 0.f);
		_bool	bVelocity_Lerp = true;

		Color	vColor_Start = Color(0.f, 0.f, 0.f, 0.f);
		Color	vColor_End = Color(0.f, 0.f, 0.f, 0.f);
		_bool	bColor_Lerp = true;

		_float	fLifeTime = FLT_MAX;
		_float	fWaitingTime = 0.0f;
		_float	fRemainTime = 0.0f;
		_bool	bParentPivot = false;

		Vec2	vUV_Start = Vec2(0.f, 0.f);
		Vec2	vUV_Speed = Vec2(0.f, 0.f);

		_bool	IsSequence = false;
		_bool	IsLoop = false;
		_float	fSequenceTerm = 0.05f;
		_float	fDissolveStart = 1.f;
			
		// Variables
		Vec2	vUV_Offset = Vec2(0.f, 0.f);
		_int	iUV_Wave = false;
		_float	fUV_WaveSpeed = 1.f;
		Vec2	vUV_TileCount = Vec2(1.f, 1.f);
		Vec2	vUV_TileIndex = Vec2(0.f, 0.f);
		Color	vColor_Offset = Color(0.f, 0.f, 0.f, 0.f);
		Color	vColor_Clip = Color(0.f, 0.f, 0.f, 0.f);
		Color	vColor_Mul = Color(1.f, 1.f, 1.f, 1.f);

		// Intensity
		_float	fBloom = 0.f;
		_float	fRadial = 0.f;
		_float	fDissolveAmount = 0.f;
		
		// Billboard
		_int	iBillboard = true;
		
		// Particle
		Vec3	vEmitPosition = Vec3(0.f, 0.f, 0.f);
		Vec3	vEmitDirection = Vec3(0.f, 0.f, 0.f);
		Vec3	vRandomMul = Vec3(1.f, 1.f, 1.f);
		_float	fSpreadSpeed = 0.f;
		_float	fEmitTerm = 0.f;
		_float	fParticleLifeTime = 0.f;

		string	strPassName = "OneBlend";

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

	virtual void Reset(CEffect_Manager::EFFECTPIVOTDESC& tEffectDesc);
	virtual void EffectEnd();
	virtual void Update_Pivot(Matrix& matPivot);

protected:
	Vec3	m_vPosition_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vPosition_End = Vec3(0.f, 0.f, 0.f);
	_bool	m_bPosition_Lerp = true;

	Vec3	m_vRotation_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vRotation_End = Vec3(0.f, 0.f, 0.f);
	_bool	m_bRotation_Lerp = true;

	Vec3	m_vScaling_Start = Vec3(1.f, 1.f, 1.f);
	Vec3	m_vScaling_End = Vec3(1.f, 1.f, 1.f);
	_bool	m_bScaling_Lerp = true;

	Vec3	m_vVelocity_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vVelocity_End = Vec3(0.f, 0.f, 0.f);
	_bool	m_bVelocity_Lerp = true;

	Color	m_vColor_Start = Color(0.f, 0.f, 0.f, 0.f);
	Color	m_vColor_End = Color(0.f, 0.f, 0.f, 0.f);
	_bool	m_bColor_Lerp = true;

	_float	m_fLifeTime = FLT_MAX;
	_float	m_fWaitingTime = 0.0f;
	_float	m_fRemainTime = 0.0f;

	Vec2	m_vUV_Start = Vec2(0.f, 0.f);
	Vec2	m_vUV_Speed = Vec2(0.f, 0.f);

	_bool	m_IsSequence = false;
	_bool	m_IsLoop = false;
	_float	m_fSequenceTerm = 0.05f;

	struct tagFX_Variables
	{
		Vec2	vUV_Offset = Vec2(0.f, 0.f);
		_int	iUV_Wave = false;
		_float	fUV_WaveSpeed = 1.f;
		Vec2	vUV_TileCount = Vec2(1.f, 1.f);
		Vec2	vUV_TileIndex = Vec2(0.f, 0.f);
		Color	vColor_Offset = Vec4(0.f, 0.f, 0.f, 0.f);
		Color	vColor_Clip = Vec4(0.01f, 0.01f, 0.01f, 0.01f);
		Color	vColor_Mul = Color(1.f, 1.f, 1.f, 1.f);
	} m_Variables;

	struct tagFX_Intensity
	{
		_float	fBloom = 0.f;
		_float	fRadial = 0.f;
		_float	fDissolveAmount = 0.f;
		_float	padding;
	} m_Intensity;

	tagEffectDesc m_tVoidEffectDesc;

protected:
	Matrix	m_matPivot = Matrix::Identity;
	Matrix	m_matOffset = Matrix::Identity;
	Matrix	m_matCombined = Matrix::Identity;

	_float	m_fTimeAcc = 0.f;
	_float	m_fWaitingAcc = 0.0f;
	_float	m_fLifeTimeRatio = 0.f;

	_float	m_fSequenceTimer = 0.0f;
	_float	m_fDissolveStart = 1.f;
	//random_device*		m_RandomDevice = nullptr;

	CTexture* m_pDiffuseTexture		= nullptr;
	CTexture* m_pNoiseTexture		= nullptr;
	CTexture* m_pMaskTexture		= nullptr;
	CTexture* m_pEmissiveTexture	= nullptr;
	CTexture* m_pDissolveTexture	= nullptr;

	EffectMaterialFlag m_tNoisMaskEmisDslv;
	string	m_strPassName = "OneBlend";

protected:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Free();
};

END