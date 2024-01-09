#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_GN_TerminatingShot_Start final : public CState_Skill
{
public:
	CState_GN_TerminatingShot_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_TerminatingShot_Start() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Shot();

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_TerminatingShot_Start&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iTerminatingShot1 = 0;

	vector<wstring> m_ParticleName;
public:
	static CState_GN_TerminatingShot_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END