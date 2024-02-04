#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"
#include "Projectile.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CPlayer;
class CEsther;

class CEsther_Bahuntur_Skill_Floor final : public CGameObject
{
private:
	CEsther_Bahuntur_Skill_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Bahuntur_Skill_Floor(const CEsther_Bahuntur_Skill_Floor& rhs);
	virtual ~CEsther_Bahuntur_Skill_Floor() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();

public:

	CShader* Get_ShaderCom() { return m_pShaderCom; }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

public:
	void					Set_DebugRender_State(_bool bRender) { m_bDebugRender = bRender; }

	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false, _bool bReverse = false, _bool bUseY = false);

	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

	void					Call_Finish();
	void					Call_Act1(_float fTimeDelta) { Act1(fTimeDelta); }
	void					Call_Act2(_float fTimeDelta) { Act2(fTimeDelta); }

public:
	void					Ready();

public:
	virtual HRESULT			Ready_Coliders() { return S_OK; }

private:
	virtual HRESULT			Ready_Components();

	void					CullingObject();

	
	void					Act1(_float fTimeDelta);
	void					Act2(_float fTimeDelta);

private:
	_bool							m_bDebugRender = { false };

	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

private:
	_bool							m_IsFinish = false;

	/* 컬링 절두체 */
	BoundingSphere	m_tCullingSphere;


	_bool			m_IsDissolve = { false };
	_bool			m_IsReverseDissolve = { false };
	_float			m_fDissolveAcc = { 0.0f };
	_float			m_fMaxDissolve = { 1.f };

	class CTexture* m_pDissolveTexture = { nullptr };

public:
	static CEsther_Bahuntur_Skill_Floor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

