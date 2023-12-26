#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_WR_WildStomp final : public CPlayer_Skill
{
private:
	CSkill_WR_WildStomp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CPlayer_Slayer* pPlayer);
	virtual ~CSkill_WR_WildStomp() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual HRESULT			Ready_Components();

public:
	virtual HRESULT			Increase_Player_Stat() { return S_OK; }
	virtual HRESULT			Decrease_Player_Stat() { return S_OK; }
	virtual HRESULT			Restore_Player_Stat() { return S_OK; }

public:
	virtual void			Check_ColliderState();

private:
	class CPlayer_Slayer* m_pPlayer = { nullptr };

public:
	static CSkill_WR_WildStomp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END


