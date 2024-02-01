#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_MG_Sonatine final : public CPlayer_Skill
{
private:
	CSkill_MG_Sonatine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CPlayer_Bard* pPlayer);
	virtual ~CSkill_MG_Sonatine() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Ready_Components();

public:
	virtual void			Check_ColliderState() override;

private:
	class CPlayer_Bard* m_pPlayer = { nullptr };

public:
	static CSkill_MG_Sonatine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END



