#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_FullSwing_Success final : public CState_Skill
{
public:
	CState_WDR_FullSwing_Success(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_FullSwing_Success() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_TrailStart();
	void	Effect_TrailEnd();
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_FullSwing_Success&, _float)> m_TickFunc;
	vector<class CEffect*> m_Trails;
private:
	//Animation
	_int m_iFullSwing_Success = 0;


	_bool m_bTrailStart = false;
	_bool m_bTrailEnd = false;
public:
	static CState_WDR_FullSwing_Success* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END
