#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;

class CState_GN_FocusShot_Start final : public CState_Skill
{
public:
	CState_GN_FocusShot_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_FocusShot_Start() = default;

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

	std::function<void(CState_GN_FocusShot_Start&, _float)> m_TickFunc;

	_bool m_bEffect = false;
	CEffect* m_pEffectGlow = nullptr;
	CEffect* m_pEffectLazer = nullptr;

private:
	//Animation
	_int m_iFocusShot_Start = 0;

public:
	static CState_GN_FocusShot_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END