#pragma once
#include "Boss_Server.h"
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
class CBoss_Golem_Server :
	public CBoss_Server
{

protected:
	CBoss_Golem_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Golem_Server(const CBoss_Golem_Server& rhs);
	virtual ~CBoss_Golem_Server() = default;

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


protected:
	virtual HRESULT		Ready_Components()override;

	virtual HRESULT Ready_BehaviourTree()override;

public:
	static CBoss_Golem_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

