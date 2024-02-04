#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_GN_Attack_Hand1 final : public CState
{
	typedef struct EffectFrameDesc
	{
		EffectFrameDesc() {};
		EffectFrameDesc(_uint iFrameIndex, _uint iWeaponIndex) : iFrame(iFrameIndex), iWeapon(iWeaponIndex) {}

		_int iFrame = -1;
		_int iWeapon = -1;
	}EFFECTFRAMEDESC;

public:
	CState_GN_Attack_Hand1(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_Attack_Hand1() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Attack_Hand(_float fAttackTime);

private:
	void	Effect_Shot();

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_Attack_Hand1&, _float)> m_TickFunc;

private:
	//Animation
	_int m_Attack_Hand1 = 0;

	_uint m_iAttackCnt = 0;
	_uint m_iEffectCnt = 0;

	_bool m_IsAttackContinue = false;

	vector<_uint> m_AttackFrames;
	vector<EffectFrameDesc> m_EffectFrames;

public:
	static CState_GN_Attack_Hand1* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END