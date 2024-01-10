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
		_bool   bUseFactor = { true };
		_bool	bUseProjPos = { false };
		_uint	iDamage = { 0 };
		_uint	iStatusEffect = { (_uint)STATUSEFFECT::EFFECTEND };
		_float	fStatusDuration = { 0.f };
		_float	fRepulsion = { false };
		_bool	IsCounter = { false };

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

public:
	CGameObject*			Get_AttackOwner() { return m_pAttackOwner; }

private:
	void	Shoot(_float fActiveTime);
	void	AttackEnd();

	void	Spawn_Projectile(const _float& fTimeDelta);
	void	Reset_Projectile();

protected:
	virtual HRESULT			Ready_Components();

	void	InitAsSpawner(PROJECTILE_DESC* pDesc);
	void	InitAsAttack(PROJECTILE_DESC* pDesc);

protected:
	CRenderer*						m_pRendererCom = nullptr;

	CGameObject*					m_pAttackOwner = nullptr;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	PROJINFO_DESC			m_ProjInfoDesc;
	PROJINFO_DESC			m_SpawnProjDesc;

	_float m_fActiveTime = 0.0f;
	_float m_fCurrTime = 0.0f;

	_float m_fMoveSpeed = 1.f;
	_bool  m_IsMove = { false };

	/* 스폰 변수 */
	_bool  m_bReserveColli = { false };
	_bool  m_IsSpawner = { false };
	_bool  m_IsRandomSpawn = { false };

	_uint  m_iSpawnColLayer = { 0 };
	_uint  m_iSpawnUseCol = { 0 };
	_uint  m_iSpawnCnt = { 0 };
	_int   m_iSpawnMaxCnt = { 0 };
	_float m_fSpawnAttackTime = { 0.f };
	_float m_fSpawnTime = { 0.f };
	_float m_fSpawnAcc = { 0.f };
	_float m_fSpawnRadius = { 0.f };
	Vec3   m_vSpawnOffset;
	Vec3   m_vSpawnChildScale;
	Vec3   m_vSpawnChildOffset;
	Vec4   m_vRandSpawnFigure;
	Matrix m_matSpawn;

	_bool m_bEnd = false;

	_bool m_bColliderOut[COLEND];
	CSphereCollider* m_AttackCollider[COLEND];

public:
	virtual void Free();
	static CProjectile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);


};

END

