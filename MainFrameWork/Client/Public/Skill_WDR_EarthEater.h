#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_WDR_EarthEater final : public CPlayer_Skill
{
private:
	CSkill_WDR_EarthEater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CPlayer_Destroyer* pPlayer);
	virtual ~CSkill_WDR_EarthEater() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual HRESULT			Ready_Components();

public:
	virtual void			Check_ColliderState();

private:
	class CPlayer_Destroyer* m_pPlayer = { nullptr };

public:
	static CSkill_WDR_EarthEater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END



