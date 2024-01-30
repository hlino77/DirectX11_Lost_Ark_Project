#pragma once

#include "Server_Defines.h"
#include "Monster_Server.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
class CBehaviorTree;
END


BEGIN(Server)
class CBoss_Server : public CMonster_Server
{



protected:
	CBoss_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Server(const CBoss_Server& rhs);
	virtual ~CBoss_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration,_uint iGroggy);
public:

	_bool						Is_CounterSkill() { return m_IsCounterSkill; }
	void						Set_CounterSkill(_bool IsCounterSkill) { m_IsCounterSkill = IsCounterSkill; }
	
	_bool						Is_Countered() { return m_IsCountered; }
	void						Set_Countered(_bool IsCountered) { m_IsCountered = IsCountered; }

	_bool						Is_Groggy() { return m_IsGroggy; }
	void						Set_Groggy(_bool IsGroggy) { m_IsGroggy = IsGroggy; }

	_bool						Get_SkipAction() { return m_bSkipAction; }
	void						Set_SkipAction(_bool bCounter) { m_bSkipAction = bCounter; }



	
	_int						Get_Armor() { return m_iArmor; }
	void						Set_Armor(_int iArmor) { m_iArmor = iArmor; }

	_int						Get_ArmorDurability() { return m_iArmorDurability; }
	void						Set_ArmorDurability(_int iArmorDurability) { m_iArmorDurability = iArmorDurability; }

	_int						Get_MaxArmorDurability() { return m_iMaxArmorDurability; }

	_int						Get_GroggyGauge() { return m_iGroggyGauge; }
	void						Set_GroggyGauge(_int iArmorDurability) { m_iGroggyGauge = iArmorDurability; }

	_int						Get_MaxGroggyGauge() { return m_iMaxGroggyGauge; }

	_uint						Get_Phase() { return m_iPhase; }
	void						Set_Phase(_uint iPhase) { m_iPhase = iPhase; }

	Vec3						Get_SpawnPosition() { return m_vSpawnPosition; }
	void						Set_SpawnPosition(Vec3 vPosition) { m_vSpawnPosition = vPosition; }
	void						Move_to_SpawnPosition();

	_bool						Is_GroggyLock() { return m_IsGroggyLock; }
	void						Set_GroggyLock(_bool IsGroggyLock) { m_IsGroggyLock = IsGroggyLock; }

	_int						Get_GroggyCount() { return m_iGroggyCount; }
	void						Set_GroggyCount(_int iGroggyCount) { m_iGroggyCount = iGroggyCount; }

	_int						Get_MaxGroggyCount() { return m_iMaxGroggyCount; }
	void						Set_MaxGroggyCount(_int iMaxGroggyCount) { m_iMaxGroggyCount = iMaxGroggyCount; }

	_bool						Is_Dummy() { return m_bDummy; }
	void						Set_Dummy(_bool bDummy) { m_bDummy = bDummy; }

protected:
	virtual HRESULT		Ready_Components();
	virtual	HRESULT		Ready_BehaviourTree();
	void Set_Colliders(_float fTimeDelta);
	HRESULT Ready_Coliders();

protected:
	_bool							m_IsCounterSkill = false;
	_bool							m_IsCountered = false;
	_bool							m_IsGroggy = false;

	_bool							m_bSkipAction = false;

	_int							m_iGroggyGauge = 0;
	_uint							m_iMaxGroggyGauge = 0;
	_int							m_iArmorDurability = 0;
	_uint							m_iMaxArmorDurability = 0;
	_uint							m_iPhase = 0;
	_int							m_iArmor = 0;
	Vec3							m_vSpawnPosition;
	_bool							m_IsGroggyLock = false;
	_bool							m_Is3Counter = false;
	_int							m_iGroggyCount = 0;
	_uint							m_iMaxGroggyCount = 0;
	_float							m_fTimeCount = 0.f;
	_bool							m_bDummy = false;
public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

