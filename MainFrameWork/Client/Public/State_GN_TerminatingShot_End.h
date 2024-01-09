#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_GN_TerminatingShot_End final : public CState_Skill
{
public:
	CState_GN_TerminatingShot_End(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_TerminatingShot_End() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void Effect_Shot();

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_TerminatingShot_End&, _float)> m_TickFunc;

	vector<wstring> m_ParticleName;
private:
	//Animation
	_int m_iTerminatingShot2 = 0;

public:
	static CState_GN_TerminatingShot_End* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END