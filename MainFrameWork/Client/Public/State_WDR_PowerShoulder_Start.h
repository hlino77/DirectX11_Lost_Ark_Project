#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CEffect;
class CState_WDR_PowerShoulder_Start final : public CState_Skill
{
public:
	CState_WDR_PowerShoulder_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PowerShoulder_Start() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Start();
	void	Effect_End();
	void	Update_Effect();
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_PowerShoulder_Start&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPowerShoulder_Start = 0;

	_bool	m_bComboContinue = false;
	_bool	m_bEffect = false;
	_bool	m_bEffectEnd = false;

	vector<CEffect*> m_Effects;

public:
	static CState_WDR_PowerShoulder_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END