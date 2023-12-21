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
	virtual HRESULT			Ready_Components();

public:
	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

public:
	virtual HRESULT			Increase_Player_Stat() { return S_OK; }
	virtual HRESULT			Decrease_Player_Stat() { return S_OK; }
	virtual HRESULT			Restore_Player_Stat() { return S_OK; }

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



