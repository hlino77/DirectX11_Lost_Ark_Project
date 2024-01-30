#pragma once
#include "Function_Npc.h"

BEGIN(Client)

class CUI_NPC_ChaosDungeon_NewWnd;

class CGuide_Chaos_Npc final : public CFunction_Npc
{
protected:
	CGuide_Chaos_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuide_Chaos_Npc(const CGuide_Chaos_Npc& rhs);
	virtual ~CGuide_Chaos_Npc() = default;

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
	class CUI_NPC_ChaosDungeon_NewWnd* m_pChaosUI = { nullptr };


public:
	static CGuide_Chaos_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

