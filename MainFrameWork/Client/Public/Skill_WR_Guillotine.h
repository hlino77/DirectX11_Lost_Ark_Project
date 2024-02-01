#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_WR_Guillotine final : public CPlayer_Skill
{
private:
	CSkill_WR_Guillotine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CPlayer_Slayer* pPlayer);
	virtual ~CSkill_WR_Guillotine() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Ready_Components();

public:
	virtual void			Check_ColliderState();

private:
	class CPlayer_Slayer* m_pPlayer = { nullptr };

public:
	static CSkill_WR_Guillotine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END



