#pragma once
#include "Function_Npc.h"

BEGIN(Client)

class CUI_NPC_ChaosDungeon_NewWnd;

class CUpgrade_Npc final : public CFunction_Npc
{
protected:
	CUpgrade_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUpgrade_Npc(const CUpgrade_Npc& rhs);
	virtual ~CUpgrade_Npc() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			LateTick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

private:
	virtual HRESULT			Ready_Components() override;
	HRESULT					Ready_UpgradeUI();

private:

private:
	class CUI_NPC_ItemUpgrade* m_pUpgradeUI = { nullptr };
	class CPlayer* m_pPlayer = { nullptr };

public:
	static CUpgrade_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

