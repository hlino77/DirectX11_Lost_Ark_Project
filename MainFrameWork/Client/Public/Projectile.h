#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Client)
class CProjectile : public CGameObject
{
public:
	enum ATTACKCOLLIDER { SPHERE, OBB, COLEND };

public:
	typedef struct tagProjectileDesc
	{
		_bool   bUseFactor;
		_bool	bUseProjPos;
		_uint	iDamage;
		_uint	iStatusEffect;
		_float	fStatusDuration;
		_float	fRepulsion;
	}PROJINFO_DESC;

protected:
	CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectile(const CProjectile& rhs);
	virtual ~CProjectile() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render() { return S_OK; }
	virtual HRESULT	Render_Debug();
	virtual HRESULT InitProjectile(void* pArg);


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;


	virtual HRESULT	Ready_Coliders() { return S_OK; }
	virtual PROJINFO_DESC Get_ProjInfo() { return m_ProjInfoDesc; }

	CSphereCollider* Get_Collider(ATTACKCOLLIDER eCollider) { return m_AttackCollider[eCollider]; }

	void	Shoot(_float fActiveTime);
	void	AttackEnd();

	void	Reset_Projectile();

public:
	CGameObject*			Get_AttackOwner() { return m_pAttackOwner; }

protected:
	virtual HRESULT			Ready_Components();

protected:
	CRenderer*						m_pRendererCom = nullptr;

	CGameObject*					m_pAttackOwner = nullptr;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	PROJINFO_DESC m_ProjInfoDesc;

	_float m_fActiveTime = 0.0f;
	_float m_fCurrTime = 0.0f;

	_float m_fMoveSpeed = 1.f;
	_bool  m_IsMove = { false };

	_bool m_bEnd = false;

	_bool m_bColliderOut[COLEND];
	CSphereCollider* m_AttackCollider[COLEND];

public:
	virtual void Free();
	static CProjectile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);


};

END

