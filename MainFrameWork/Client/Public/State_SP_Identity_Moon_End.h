#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_SP_Identity_Moon_End final : public CState
{
public:
	CState_SP_Identity_Moon_End(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual ~CState_SP_Identity_Moon_End() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Doaga* m_pPlayer = nullptr;

	std::function<void(CState_SP_Identity_Moon_End&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iIdentity_Moon_End = 0;

	_uint m_iAttackCnt = 0;
	vector<_int> m_AttackFrames;

public:
	static CState_SP_Identity_Moon_End* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual void Free() override;
};

END