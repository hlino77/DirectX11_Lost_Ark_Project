#pragma once

#include "Client_Defines.h"
#include "BindShaderDesc.h"
#include "EffectTool.h"

BEGIN(Engine)

class CTexture;
class CShader;
class CVIBuffer;

END

BEGIN(Client)

class CEffectTool;
class CVoidEffect : public CGameObject
{
	using Super = CGameObject;
public:
	struct tagVoidEffectDesc
	{
		_int	iEffectType				= -1;
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
	virtual HRESULT Render_Particle();

public:
	void	Reset();

private:
	HRESULT Ready_Components(tagVoidEffectDesc* pDesc);
	virtual HRESULT Ready_Components() override { return E_FAIL; };

public:
	_int	m_iEffectType = -1;

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
	_float	m_fRemainTime = 0.0f;
	_float	m_fWaitingTime = 0.0f;


	Vec2	m_vUV_Start = Vec2(0.f, 0.f);
	Vec2	m_vUV_Speed = Vec2(0.f, 0.f);

	_bool	m_IsSequence = false;
	_bool	m_IsLoop = false;
	_float	m_fSequenceTerm = 0.05f;

	_float	m_fDissolveStart = 1.f;

	Vec3	m_vOriginEmitDir = Vec3(0.0f, 0.0f, 0.0f);
	_float	m_fOriginEmitTerm = 0.005f;

	struct tagFX_Variables
	{
		Vec2	vUV_Offset = Vec2(0.f, 0.f);
		//Vec2	vUV_Direction = Vec2(1.f, 1.f);
		_int	iUV_Wave = 0;
		_float	fUV_WaveSpeed = 1.f;
		Vec2	vUV_TileCount = Vec2(1.f, 1.f);
		Vec2	vUV_TileIndex = Vec2(0.f, 0.f);
		Color	vColor_Offset = Vec4(0.f, 0.f, 0.f, 0.f);
		Color	vColor_Clip = Vec4(0.f, 0.f, 0.f, 0.f);
		Color	vColor_Mul = Vec4(1.f, 1.f, 1.f, 1.f);
	} m_Variables;

	struct tagFX_Intensity
	{
		_float	fBloom = 0.f;
		_float	fRadial = 0.f;
		_float	fDissolveAmount = 0.f;
		_float	padding;
	} m_Intensity;
	
	struct tagFX_Billboard
	{
		_int	iBillboard = true;
		Vec3	padding;
	} m_Billboard;

	struct tagFX_Particle
	{
		_float	fGameTime = 0.f;
		Vec3	vEmitPosition = Vec3(0.f, 0.f, 0.f);
		Vec3	vEmitDirection = Vec3(0.f, 0.f, 0.f);
		_float	fTimeStep = 0.f;

		Vec3	vRandomMul = Vec3(1.f, 1.f, 1.f);
		_float	fSpreadSpeed = 1.f;
		_float	fEmitTerm = 0.005f;
		_float	fParticleLifeTime = 1.f;
		_float	fSequenceTerm = 0.01f;
		_uint	iIsLoop = 0;
	} m_Particle;

	string	m_strPassName = "OneBlend";

	tagVoidEffectDesc m_tVoidEffectDesc;

	_bool	m_bParentPivot = false;

	void SetEffectTool(CEffectTool* pEffectTool) { m_pEffectTool = pEffectTool; }

private:
	CEffectTool* m_pEffectTool = nullptr;
	Matrix	m_matPivot = Matrix::Identity;
	Matrix	m_matOffset = Matrix::Identity;

	_float	m_fTimeAcc = 0.f;
	_float	m_fWaitingAcc = 0.0f;
	_float	m_fLifeTimeRatio = 0.f;

	_float	m_fSequenceTimer = 0.0f;

private:
	CVIBuffer*	m_pBuffer		= nullptr;
	CTexture*	m_pDiffuseTexture	= nullptr;
	CTexture*	m_pNoiseTexture		= nullptr;
	CTexture*	m_pMaskTexture		= nullptr;
	CTexture*	m_pEmissiveTexture	= nullptr;
	CTexture*	m_pDissolveTexture	= nullptr;

	EffectMaterialFlag m_tNoisMaskEmisDslv;

	ID3D11ShaderResourceView* m_pRandomTextureSRV = nullptr;

private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	BoundingOrientedBox	m_tBoundingBox;

public:
	static CVoidEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END