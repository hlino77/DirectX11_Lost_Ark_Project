#pragma once
#include "Boss_Server.h"

BEGIN(Server)

class CBoss_Valtan_Server :
    public CBoss_Server
{
	CBoss_Valtan_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Valtan_Server(const CBoss_Valtan_Server& rhs);
	virtual ~CBoss_Valtan_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	virtual void	Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)override;



public:

	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();
public:
	_bool						Is_GroggyLock() { return m_IsGroggyLock; }
	void						Set_GroggyLock(_bool IsGroggyLock) { m_IsGroggyLock = IsGroggyLock; }

	_uint						Get_HitCount() { return m_iHitCount; }
	void						Set_HitCount(_uint iHitCount) { m_iHitCount = iHitCount; }

	_uint						Get_GroggyCount() { return m_iGroggyCount; }
	void						Set_GroggyCount(_uint iGroggyCount) { m_iGroggyCount = iGroggyCount; }

private:
	virtual HRESULT		Ready_Components();
	virtual	HRESULT		Ready_BehaviourTree();

private:
	_bool	m_IsGroggyLock = false;
	_uint	m_iHitCount = 0;
	_uint	m_iGroggyCount = 0;

public:
	static CBoss_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END