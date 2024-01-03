#pragma once
#include "Boss.h"

BEGIN(Client)

class CBoss_Valtan :
    public CBoss
{
	CBoss_Valtan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Valtan(const CBoss_Valtan& rhs);
	virtual ~CBoss_Valtan() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;
public:

	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();


protected:
	virtual HRESULT		Ready_Components();
	virtual	HRESULT		Ready_BehaviourTree();
public:
public:
	static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END