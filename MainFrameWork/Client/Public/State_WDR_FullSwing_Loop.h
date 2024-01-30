#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_FullSwing_Loop final : public CState_Skill
{
public:
	CState_WDR_FullSwing_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_FullSwing_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);


	void	Update_Effect(_float fTimeDelta);
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_FullSwing_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iFullSwing_Loop = 0;


	vector<CEffect*> m_Effects;

	_float m_fEffectAcc = 0.0f;
	_float m_fEffectDelay = 0.0f;

	_float m_fCamShakeDelay = 0.0f;
	_float m_fCamShakeAcc = 0.0f;

public:
	static CState_WDR_FullSwing_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END
