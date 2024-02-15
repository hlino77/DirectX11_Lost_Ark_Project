#pragma once
#include "Function_Npc.h"

BEGIN(Client)

class CUI_NPC_ChaosDungeon_NewWnd;

class CGuide_Valtan_Npc final : public CFunction_Npc
{
protected:
	CGuide_Valtan_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuide_Valtan_Npc(const CGuide_Valtan_Npc& rhs);
	virtual ~CGuide_Valtan_Npc() = default;

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
	HRESULT					Ready_ChaosEntranceUI();

private:
	void					Activate_GuideUI();
	void					Send_UI_State();

private:
	class CUI_NPC_ValtanEntrance_Wnd* m_pEntranceUI = { nullptr };
	class CPlayer* m_pPlayer = { nullptr };

public:
	static CGuide_Valtan_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

