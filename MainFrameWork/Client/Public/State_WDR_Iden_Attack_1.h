#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_WDR_Iden_Attack_1 final : public CState
{
public:
	CState_WDR_Iden_Attack_1(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_Iden_Attack_1() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_Iden_Attack_1&, _float)> m_TickFunc;

private:
	//Animation
	_int m_Attack_1 = 0;

	_uint m_iAttackCnt = 0;
	vector<_int> m_AttackFrames;

	_bool m_IsAttackContinue = false;
	_bool m_IsSkillContinue = false;

public:
	static CState_WDR_Iden_Attack_1* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END