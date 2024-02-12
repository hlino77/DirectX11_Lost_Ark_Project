#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;

class CState_SP_Flyheaven final : public CState_Skill
{
public:
	CState_SP_Flyheaven(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual ~CState_SP_Flyheaven() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Shot();
	void	Effect_Trail();
	void	TrailEnd();
private:
	class CPlayer_Doaga* m_pPlayer = nullptr;

	std::function<void(CState_SP_Flyheaven&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iFlyheaven = 0;

	vector<CEffect*> m_Trails;

	_bool m_bTrail = false;

	// Sound
	_bool m_EffectSound = false;
	_bool m_PlayerSound = false;

public:
	static CState_SP_Flyheaven* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual void Free() override;
};

END