#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_SP_DimensionalShift_Start final : public CState_Skill
{
public:
	CState_SP_DimensionalShift_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual ~CState_SP_DimensionalShift_Start() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Trail();
	void	TrailEnd();
private:
	class CPlayer_Doaga* m_pPlayer = nullptr;

	std::function<void(CState_SP_DimensionalShift_Start&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iDimensionalShift_Start = 0;

	Vec3 m_vColliPos;

	_bool m_bTrail = false;


	// Sound
	_bool m_EffectSound = false;
	_bool m_PlayerSound = false;

public:
	static CState_SP_DimensionalShift_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual void Free() override;
};

END