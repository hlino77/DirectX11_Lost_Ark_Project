#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_GN_Gunkata final : public CPlayer_Skill
{
private:
	CSkill_GN_Gunkata(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CPlayer_Gunslinger* pPlayer);
	virtual ~CSkill_GN_Gunkata() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Ready_Components();

public:
	virtual void			Check_ColliderState();

private:
	class CPlayer_Gunslinger* m_pPlayer = { nullptr };

public:
	static CSkill_GN_Gunkata* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END



