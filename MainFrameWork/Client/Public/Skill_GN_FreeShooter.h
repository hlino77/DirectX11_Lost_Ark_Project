#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_GN_FreeShooter final : public CPlayer_Skill
{
private:
	CSkill_GN_FreeShooter(CGameObject* pOwner);
	virtual ~CSkill_GN_FreeShooter() = default;

public:
	virtual HRESULT			Initialize();
	virtual void			Tick(_float fTimeDelta);
	virtual void			UnCtrl_Tick(_float fTimeDelta);
	virtual HRESULT			Render_Debug();

	virtual void			Enter();
	virtual void			Exit();

public:
	virtual HRESULT			Increase_Player_Stat() { return S_OK; }
	virtual HRESULT			Decrease_Player_Stat() { return S_OK; }
	virtual HRESULT			Restore_Player_Stat() { return S_OK; }

public:
	virtual _bool			SkillCancel_Term();
	virtual _bool			SkillEnd_Term();

private:
	_uint					m_iFreeShoter;

public:
	static CSkill_GN_FreeShooter* Create(CGameObject* pOwner);
	virtual void Free();
};

END


