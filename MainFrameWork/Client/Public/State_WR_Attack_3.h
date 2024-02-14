#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_WR_Attack_3 final : public CState
{
public:
	CState_WR_Attack_3(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_Attack_3() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Attack_3();

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;

	std::function<void(CState_WR_Attack_3&, _float)> m_TickFunc;

private:
	//Animation
	_bool m_bEffect = false;

	_int m_Attack_3 = 0;
	_int m_IdentityAttack_3 = 0;

	_uint m_iAttackCnt = 0;
	vector<_int> m_AttackFrames;

	_bool m_IsAttackContinue = false;

public:
	static CState_WR_Attack_3* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END