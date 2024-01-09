#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;


class CState_GN_PerfectShot_Loop final : public CState_Skill
{
public:
	CState_GN_PerfectShot_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_PerfectShot_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);
private:
	void	Effect_Glow(_bool bOnOff);
	void	Update_Effect(_float fTimeDelta);

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_PerfectShot_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPerfectShot_Loop = 0;

	_bool m_bEffect = false;
	CEffect* m_pEffectGlow = nullptr;
	CEffect* m_pEffectLazer = nullptr;

	vector<CEffect*> m_ChargeEffects;

	_float m_fEffectChargeAcc = 0.0f;
	_float m_fEffectChargeTime = 0.0f;

	_float m_fEffectCharge2Acc = 0.0f;
	_float m_fEffectCharge2Time = 0.0f;

public:
	static CState_GN_PerfectShot_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END