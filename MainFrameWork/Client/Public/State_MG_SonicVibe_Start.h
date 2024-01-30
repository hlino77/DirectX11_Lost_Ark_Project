#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_MG_SonicVibe_Start final : public CState_Skill
{
public:
	CState_MG_SonicVibe_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual ~CState_MG_SonicVibe_Start() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Bard* m_pPlayer = nullptr;

	std::function<void(CState_MG_SonicVibe_Start&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iSonicVibe = 0;

	Vec3 m_vColliPos;

	_float m_fTimeAcc = 0.0f;
	_float m_fReleaseTime = 1.f;
	_float m_fEndTime = 3.f;


public:
	static CState_MG_SonicVibe_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual void Free() override;
};

END