#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WR_Guillotine_Loop final : public CState_Skill
{
public:
	CState_WR_Guillotine_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_Guillotine_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Init_Camera();
	void	Update_Camera(_uint iAnimFrame, _float fTimeDelta);

	void	Reset_Camera();

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;

	std::function<void(CState_WR_Guillotine_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iGuillotine_Loop = 0;

	//
	_bool m_bEffectStart = false;
	_bool m_bCameraStart[2] = { false, false };

	//Camera
	Vec3 m_vCameraTargetPos;

public:
	static CState_WR_Guillotine_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END