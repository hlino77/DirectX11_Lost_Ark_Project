#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "StateMachine.h"
#include <atomic>

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
class CUI_Boss_Hp;

class CBoss : public CMonster
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		_uint	iLevel;
		Vec3	vPos;
	}MODELDESC;



protected:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;
	virtual HRESULT Render_ShadowDepth();

	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)override;

public:
	CShader*				Get_ShaderCom() { return m_pShaderCom; }



	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }


	void					Set_AttackMoveSpeed(_float fSpeed) { m_fAttackMoveSpeed = fSpeed; }
	_float					Get_AttackMoveSpeed() { return m_fAttackMoveSpeed; }


	_bool					Is_Control() { return m_bControl; }
public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);


	virtual	void OnCollisionEnter(const _uint iColLayer, CCollider* pOther);

	void					Follow_ServerPos(_float fDistance, _float fLerpSpeed);


	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void Add_Colider(_int iColIndex, CSphereCollider* pCollider);

	virtual void Set_EffectPos();

	_bool						Is_CounterSkill() { return m_IsCounterSkill; }
	void						Set_CounterSkill(_bool IsCounterSkill) { m_IsCounterSkill = IsCounterSkill; }

	virtual void			Set_Die();

	Vec3						Get_SpawnPosition() { return m_vSpawnPosition; }
	void						Set_SpawnPosition(Vec3 vPosition) { m_vSpawnPosition = vPosition; }
	void						Move_to_SpawnPosition();

	_int						Get_Armor() { return m_iArmor; }
	void						Set_Armor(_int iArmor) { m_iArmor = iArmor; }

	_uint						Get_Phase() { return m_iPhase; }
	void						Set_Phase(_uint iPhase) { m_iPhase = iPhase; }


	_int						Get_GroggyGauge() { return m_iGroggyGauge; }
	void						Set_GroggyGauge(_int iArmorDurability) { m_iGroggyGauge = iArmorDurability; }

	_float					Get_Force() { return m_fForce; }
	void					Set_Force(_float fForce) { m_fForce = fForce; }

	_uint					Get_BaseAtk() { return m_iBaseAtk; }
	void					Set_BaseAtk(_uint iBaseAtk) { m_iBaseAtk = iBaseAtk; }

	_float					Get_BaseForce() { return m_fBaseForce; }
	void					Set_BaseForce(_float fBaseForce) { m_fBaseForce = fBaseForce; }

	_bool						Is_GroggyLock() { return m_IsGroggyLock; }
	void						Set_GroggyLock(_bool IsGroggyLock) { m_IsGroggyLock = IsGroggyLock; }

	_uint						Get_HitCount() { return m_iHitCount; }
	void						Set_HitCount(_uint iHitCount) { m_iHitCount = iHitCount; }

	_uint						Get_GroggyCount() { return m_iGroggyCount; }
	void						Set_GroggyCount(_uint iGroggyCount) { m_iGroggyCount = iGroggyCount; }

	_int						Get_MaxGroggyCount() { return m_iMaxGroggyCount; }
	void						Set_MaxGroggyCount(_int iMaxGroggyCount) { m_iMaxGroggyCount = iMaxGroggyCount; }

	_uint						Get_MaxGroggyGauge() { return m_iMaxGroggyGauge; }
	_int						Get_CurrGroggyGauge() { return m_iGroggyGauge; }

protected:
	virtual HRESULT Ready_Components();
	HRESULT Ready_HP_UI(_uint iTextureIndex);


protected:
	void					CullingObject();
	virtual	HRESULT			Ready_Coliders() { return S_OK; };
	virtual HRESULT			Ready_BehaviourTree();

protected:
	_uint m_iBaseAtk = 0;
	_float m_fBaseForce = 0;
	//HPUI
	virtual HRESULT	Ready_HpUI();

protected:
	_bool m_IsCounterSkill = false;
	Vec3  m_vSpawnPosition = {};
	_int m_iArmor = 0;
	_uint m_iPhase = 1;
	_int m_iGroggyGauge = 0;
	_uint m_iMaxGroggyGauge = 0;

	_float m_fForce = 0.f;;
	_bool	m_IsGroggyLock = false;
	_uint	m_iHitCount = 0;
	_uint	m_iGroggyCount = 0;
	_uint	m_iMaxGroggyCount = 0;
	//HPUI
	CUI_Boss_Hp*	m_pHpUI = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

