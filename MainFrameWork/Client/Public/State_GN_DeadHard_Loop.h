#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_GN_DeadHard_Loop final : public CState_Skill
{
public:
	enum DIR { FRONT, BACK, LEFT, RIGHT, _END};

public:
	CState_GN_DeadHard_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_DeadHard_Loop() = default;

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

	std::function<void(CState_GN_DeadHard_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iDeadHard = 0;
	_int m_iDeadHard_Loop = 0;
	_int m_iDeadHard_F = 0;
	_int m_iDeadHard_B = 0;
	_int m_iDeadHard_L = 0;
	_int m_iDeadHard_R = 0;

public:
	static CState_GN_DeadHard_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END