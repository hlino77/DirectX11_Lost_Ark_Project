#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_PerfectSwing_Fail final : public CState_Skill
{
public:
	CState_WDR_PerfectSwing_Fail(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PerfectSwing_Fail() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Effect_Trail();
	void	Update_Effect();
	void	Effect_End();
	void	Reset_Camera();

	void	Effect_Shot();
private:
	void	Init_Camera();
	void	Update_Camera(_float fTimeDelta);
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_PerfectSwing_Fail&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPerfectSwing_Fail = 0;

	_bool m_bEffectEnd = false;
	_bool m_bEffectTrail = false;

	vector<CEffect*> m_Trails;

	Vec3 m_vCameraTargetPos;


	// Sound
	_bool   m_EffectSound = false;
	_bool   m_PlayerSound = false;
	_float  m_EffectSoundAccTime = 0.f;
	_float	m_PlayerSoundAccTime = 0.f;

public:
	static CState_WDR_PerfectSwing_Fail* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END
