#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;
class CUI_HoldingFrame;
class CState_WDR_PerfectSwing_Loop final : public CState_Skill
{
	enum class CameraState { CHARGE1, CHARGE2 };
public:
	CState_WDR_PerfectSwing_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PerfectSwing_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Effect_Start();
	void	Update_Effect(_float fTimeDelta);

private:
	void	Update_Camera_Charge(_float fTimeDelta);
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;
	CUI_HoldingFrame* m_pHoldingUI = { nullptr };
	std::function<void(CState_WDR_PerfectSwing_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPerfectSwing_Loop = 0;

	_int m_iPerfectSwing_Loop_1 = 0;
	_int m_iPerfectSwing_Loop_2 = 0;

	_bool m_bEffect = false;

	vector<CEffect*> m_Effects;

	_float m_fEffectAcc = 0.0f;
	_float m_fEffectDelay = 0.0f;

	CameraState m_eCameraState;

	_float m_fCamShakeDelay = 0.0f;
	_float m_fCamShakeAcc = 0.0f;

	_float	m_fUI_AccTime = { 0.0f };

public:
	static CState_WDR_PerfectSwing_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END
