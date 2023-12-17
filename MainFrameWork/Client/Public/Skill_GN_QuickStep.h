#pragma once
#include "Client_Defines.h"
#include "Player_Skill.h"

BEGIN(Client)

class CSkill_GN_QuickStep final : public CPlayer_Skill
{
private:
	CSkill_GN_QuickStep(CGameObject* pOwner);
	virtual ~CSkill_GN_QuickStep() = default;

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
	_uint					m_iQuickStep_1;
	_uint					m_iQuickStep_2;

	_bool					m_bCombo = false;
	_bool					m_bCombo2 = false;

public:
	static CSkill_GN_QuickStep* Create(CGameObject* pOwner);
	virtual void Free();
};

END


