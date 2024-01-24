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
class CMonster_Crystal :
	public CMonster
{

private:
	CMonster_Crystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Crystal(const CMonster& rhs);
	virtual ~CMonster_Crystal() = default;

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
public:
	void					Set_Explosion(_bool bExplosion) { m_bExplosion = bExplosion; }
	_bool					Get_Explosion() { return m_bExplosion; }

private:
	virtual HRESULT		Ready_Components();

	virtual HRESULT Ready_BehaviourTree() override;
private:
	_float m_fCellHeight = 0.f;
	_bool  m_bExplosion = false;
	_float  m_fExplosionDelay = 0.f;

public:
	static CMonster_Crystal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

