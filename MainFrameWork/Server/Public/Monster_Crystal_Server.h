#pragma once
#include "Monster_Server.h"
#include "Server_Defines.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Server)
class CMonster_Crystal_Server :
	public CMonster_Server
{

protected:
	CMonster_Crystal_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Crystal_Server(const CMonster_Server& rhs);
	virtual ~CMonster_Crystal_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;

	HRESULT Send_Dead();

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;
	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)override;
protected:
	virtual HRESULT		Ready_Components();

	virtual HRESULT Ready_BehaviourTree() override;
public:
	static CMonster_Crystal_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

