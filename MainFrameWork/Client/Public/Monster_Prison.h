#pragma once
#include "Monster.h"
#include "Client_Defines.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Client)
class CMonster_Prison :
	public CMonster
{

private:
	CMonster_Prison(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Prison(const CMonster& rhs);
	virtual ~CMonster_Prison() = default;

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
	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)override;
private:
	virtual HRESULT		Ready_Components();

	virtual HRESULT Ready_BehaviourTree() override;

public:
	static CMonster_Prison* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

