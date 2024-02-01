#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_SP_Inkshot final : public CPlayer_Skill
{
private:
	CSkill_SP_Inkshot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CPlayer_Doaga* pPlayer);
	virtual ~CSkill_SP_Inkshot() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Ready_Components();

public:
	virtual void			Check_ColliderState() override;

private:
	class CPlayer_Doaga* m_pPlayer = { nullptr };

public:
	static CSkill_SP_Inkshot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END



