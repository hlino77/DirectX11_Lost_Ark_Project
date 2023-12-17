#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_GN_Skill_W final : public CState
{
public:
	CState_GN_Skill_W(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_Skill_W() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_Skill_W&, _float)> m_TickFunc;

private:
	CPlayer_Controller::SKILL_KEY	m_iSkillKey = { CPlayer_Controller::SKILL_KEY::W };

public:
	static CState_GN_Skill_W* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END