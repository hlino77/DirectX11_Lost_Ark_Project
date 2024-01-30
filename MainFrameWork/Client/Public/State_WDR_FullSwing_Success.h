#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;

class CState_WDR_FullSwing_Success final : public CState_Skill
{
	enum EFFECTSTATE { TRAIL1, TRAIL2, TRAILEND1, EFFECTEND, STATEEND };

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
	void	Effect_TrailStart2();
	void	Effect_TrailEnd1();
	void	Effect_TrailEnd2();
	void	Effect_End();
	void	Update_Effect();
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_FullSwing_Success&, _float)> m_TickFunc;

	vector<CEffect*> m_Trail1;
	vector<CEffect*> m_Trail2;
	vector<CEffect*> m_Effects;

private:
	//Animation
	_int m_iFullSwing_Success = 0;

	EFFECTSTATE m_eEffectState;
public:
	static CState_WDR_FullSwing_Success* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END
