#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect_Custom_EarthEaterParticle;
class CEffect_Custom_EarthEaterSmallParticle;

class CState_WDR_EarthEater final : public CState_Skill
{
public:
	CState_WDR_EarthEater(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_EarthEater() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Start();
	void	Effect_Swing();
	void	Effect_End();

	void	Effect_Exit();
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_EarthEater&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iEarthEater = 0;
	_bool	m_bEffectOn[2];

	vector<CEffect_Custom_EarthEaterParticle*> m_Particles;
	vector<CEffect_Custom_EarthEaterSmallParticle*> m_SmallParticles;

	_bool m_bSwing = false;


	// Sound
	_bool   m_EffectSound = false;
	_bool   m_LateEffectSound = false;
	_bool   m_PlayerSound = false;
	_float  m_EffectSoundAccTime = 0.f;
	_float	m_PlayerSoundAccTime = 0.f;


public:
	static CState_WDR_EarthEater* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END