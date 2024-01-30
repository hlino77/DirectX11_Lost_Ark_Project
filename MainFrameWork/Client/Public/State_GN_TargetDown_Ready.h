#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_GN_TargetDown_Ready final : public CState_Skill
{
public:
	CState_GN_TargetDown_Ready(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_TargetDown_Ready() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Effect_Start();
	void	Effect_ZoomOut();
	void	Effect_End();
	void	Reset_Camera();
private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_TargetDown_Ready&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iTargetDown_Loop = 0;

	_uint m_iShotCount = { 0 };

	_float m_fStartDelay = 0.0f;

	_bool m_bZoomOut = false;
public:
	static CState_GN_TargetDown_Ready* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END