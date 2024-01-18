#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_PowerShoulder_Loop final : public CState_Skill
{
public:
	CState_WDR_PowerShoulder_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PowerShoulder_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Update_Effect();
	void	Effect_End();
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_PowerShoulder_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int	m_iPowerShoulder_Loop = 0;

	_bool	m_bComboContinue = false;

	_bool	m_bEffectEnd = false;
public:
	static CState_WDR_PowerShoulder_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END