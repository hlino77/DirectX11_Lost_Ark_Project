#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;

class CState_SP_Onestroke final : public CState_Skill
{
public:
	CState_SP_Onestroke(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual ~CState_SP_Onestroke() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);
private:
	void	Effect_Shot();
	void	Effect_Brush();
	void	Update_Brush();
	void	Update_StartParticle();
	void	End_StartParticle();

	void	Init_Camera();
	void	Update_Camera(_uint iAnimFrame, _float fTimeDelta);

	void	Reset_Camera();
private:
	class CPlayer_Doaga* m_pPlayer = nullptr;

	std::function<void(CState_SP_Onestroke&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iOnestroke = 0;

	_bool m_bBrush = false;
	_bool m_bEffect = false;
	_bool m_bBrushUpdate = true;

	vector<CEffect*> m_Effects;
	vector<CEffect*> m_StartParticle;

	//Camera
	Vec3 m_vCameraTargetPos;
	Vec3 m_vRadialPos;


	// Sound
	_bool m_EffectSound = false;
	_bool m_PlayerSound = false;

	_float m_EffectSoundAccTime = 0.f;


public:
	static CState_SP_Onestroke* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual void Free() override;
};

END