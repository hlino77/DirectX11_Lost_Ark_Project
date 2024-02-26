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

	void BroadCast_Ghost(Vec3 vPosition, Vec3 vLook);


public:

	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();
	_int						Get_GroggyObsrob() { return m_iGroggyObsrob; }
	void						Set_GroggyObsrob(_int iGroggyCount) { m_iGroggyObsrob = iGroggyCount; }
	_bool						Is_Rush() { return m_IsRush; }
	void						Set_Rush(_bool IsRush) { m_IsRush = IsRush; }
	_bool						Is_BugSmash() { return m_IsBugSmash; }
	void						Set_BugSmash(_bool IsBugSmash) { m_IsBugSmash = IsBugSmash; }

	_int						Get_Shield() { return m_iShield; }
	void						Set_Shield(_int iShield) { m_iShield = iShield; }

private:
	virtual HRESULT		Ready_Components();
	virtual	HRESULT		Ready_BehaviourTree();
	virtual void Find_NearTarget(_float fTimeDelta);

private:
	_int		m_iGroggyObsrob = 0;
	_bool		m_IsRush = {false};
	_bool		m_bTest = false;
	_int		m_iShield = 0;
	_bool		m_IsBugSmash = { false };
public:
	static CBoss_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END