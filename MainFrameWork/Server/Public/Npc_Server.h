#pragma once
#include "Server_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Server)

class CNpc_Server abstract : public CGameObject
{
protected:
	CNpc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc_Server(const CNpc_Server& rhs);
	virtual ~CNpc_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Debug();


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

public:
	virtual HRESULT	Actice_Npc_Function(int32 iLevel, int32 iPlayerID) PURE;

protected:
	virtual HRESULT		Ready_Components();
	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();

protected:

public:
	virtual void Free();
};

END