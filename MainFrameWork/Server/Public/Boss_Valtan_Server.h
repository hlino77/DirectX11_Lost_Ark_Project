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


	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy) override;

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;


public:

	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();


private:
	virtual HRESULT		Ready_Components();
	virtual	HRESULT		Ready_BehaviourTree();
	virtual void Find_NearTarget(_float fTimeDelta);
private:
	vector<_int> m_vecGrabbedPlayerIDs;

public:
	static CBoss_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END