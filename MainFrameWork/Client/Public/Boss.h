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
class CPartObject;
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

	enum class BOSS_TYPE
	{
		GOLEM,
		KING,
		VAlTAN,
		BOSS_END
	};


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

	void Set_Die(_float fTime=1.f);

	void Set_HpUIRender(_bool bRender);

	virtual void Disable_HpUI();

	void Set_HpUI(_int iHpCount, _int iMaxHp, wstring strBossName);

	_bool						Is_CounterSkill() { return m_IsCounterSkill; }
	void						Set_CounterSkill(_bool IsCounterSkill) { m_IsCounterSkill = IsCounterSkill; }



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

	void				Set_BossType(BOSS_TYPE eBossType) { m_eBossType = eBossType; }
	BOSS_TYPE			Get_BossType() { return m_eBossType; }

	_bool						Is_Dummy() { return m_bDummy; }
	void						Set_Dummy(_bool bDummy) { m_bDummy = bDummy; }
	void					Load_WorldMatrix(Matrix& matWorld);

	CPartObject* Get_Weapon() { return m_pWeapon; }
	virtual	void Reserve_WeaponAnimation(wstring strAnimName, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fAnimspeed);
	virtual	void Set_Weapon_Render(_bool IsRender);
	virtual	void Set_Weapon_RimLight(_float fTime, _float fColor);

	Vec2					Get_DoughnutRadii() { return m_vDoughnutRadii; }


protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_HpUI();


protected:
	void					CullingObject();
	virtual	HRESULT			Ready_Coliders() { return S_OK; };
	virtual HRESULT			Ready_BehaviourTree();

protected:
	CPartObject* m_pWeapon = nullptr;

	_uint m_iBaseAtk = 0;
	_float m_fBaseForce = 0;

protected:
	_bool m_IsCounterSkill = false;
	Vec3  m_vSpawnPosition = {};
	_int m_iArmor = 0;
	_uint m_iPhase = 1;
	_int m_iGroggyGauge = 0;
	_uint m_iMaxGroggyGauge = 0;
	_bool							m_bDummy = false;
	_float m_fForce = 0.f;;
	_bool	m_IsGroggyLock = false;
	_uint	m_iHitCount = 0;
	_uint	m_iGroggyCount = 0;
	_uint	m_iMaxGroggyCount = 0;

	//Test¿ë
	_bool							m_bDbgCout = false;
	_float							m_fTimeCount = 0.f;
	_bool							m_bTest = false;
	Vec2							m_vDoughnutRadii = {5.f,10.f};
	
	//HPUI
	CUI_Boss_Hp*	m_pBossHpUI = { nullptr };
	BOSS_TYPE m_eBossType = { BOSS_TYPE::BOSS_END };
public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

