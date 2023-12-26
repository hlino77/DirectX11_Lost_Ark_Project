#pragma once
#include "State.h"
#include "Player_Controller.h"

BEGIN(Engine)

class ENGINE_DLL CState_Skill abstract : public CState
{
public:
	CState_Skill(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController);
	virtual ~CState_Skill() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() {};
	virtual void	Tick_State(_float fTimeDelta) {};
	virtual void	Exit_State() {};

public:
	virtual void Bind_SkillKey(CPlayer_Controller::SKILL_KEY eKey);

	virtual const _float& Get_SkillEndTime() { return m_fSkillEndTime; }
	virtual const _float& Get_SkillTimeAcc() { return m_fSkillTimeAcc; }
	virtual const _float& Get_SkillSuccessTime_Min() { return m_fSkillSuccessTime_Min; }
	virtual const _float& Get_SkillSuccessTime_Max() { return m_fSkillSuccessTime_Max; }

	virtual const _bool& Is_SkillAttack() { return m_IsAttack; }

protected:
	//SkillKey
	CPlayer_Controller::SKILL_KEY m_eSkillSelectKey = { CPlayer_Controller::SKILL_KEY::_END };
	KEY							  m_eSkillBindKey = { KEY::TYPE_END };

	_bool	m_bComboContinue = { false };

	_float	m_fSkillTimeAcc = { 0.f };
	_float	m_fSkillEndTime = { 0.f };

	_float	m_fSkillSuccessTime_Min = { 0.f };
	_float	m_fSkillSuccessTime_Max = { 0.f };

	_bool	m_IsAttack = { false };

	_uint m_iSkillCnt = 0;
	vector<_int> m_SkillFrames;

public:
	virtual void Free() override;
};

END