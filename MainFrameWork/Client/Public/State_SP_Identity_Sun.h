#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;
class CState_SP_Identity_Sun final : public CState
{
public:
	CState_SP_Identity_Sun(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual ~CState_SP_Identity_Sun() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Effect_Trail();
	void	TrailEnd();
private:
	class CPlayer_Doaga* m_pPlayer = nullptr;

	std::function<void(CState_SP_Identity_Sun&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iIdentity_Sun = 0;
	Vec3 m_vColliPos;

	_uint m_iAttackCnt = 0;
	vector<_int> m_AttackFrames;

	_bool m_bTrail = false;

	vector<CEffect*> m_Trails;
public:
	static CState_SP_Identity_Sun* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual void Free() override;
};

END