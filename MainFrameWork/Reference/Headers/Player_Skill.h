#pragma once
#include "Base.h"
#include "Player_Controller.h"

BEGIN(Engine)

class CGameObject;
class CPlayer_Controller;
class CModel;
class CCollider;

class ENGINE_DLL CPlayer_Skill : public CBase
{
public:
	enum class SKILL_ATTACKTYPE { NORMAL, NEUTRAL, DESTROY, COUNTER, _END };
	enum class SKILL_CTRLTYPE { NORMAL, COMBO, HOLD, TARGET, _END };

public:
	typedef struct tagPlayerSkillDesc
	{
		_bool			 IsSuperArmor = false;

		SKILL_ATTACKTYPE eAttackType = { SKILL_ATTACKTYPE::_END };
		SKILL_CTRLTYPE   eCtrlType = { SKILL_CTRLTYPE::_END };

		_float			 fSkillDamage = { 0.f };
		_float			 fSkillCoolTime = { 0.f };

		wstring			 strSkill_StartName;
		vector<class CState*> State_Skills;

	}PLAYERSKILL_DESC;
	
protected:
	CPlayer_Skill(CGameObject* pOwner);
	virtual ~CPlayer_Skill() = default;

public:
	virtual HRESULT			Initialize(void* pArg);

public:
	virtual HRESULT			Increase_Player_Stat() { return S_OK; }
	virtual HRESULT			Decrease_Player_Stat() { return S_OK; }
	virtual HRESULT			Restore_Player_Stat()  { return S_OK; }


public:
	virtual const _bool&	Is_SuperArmor() { return m_IsSuperArmor; }
	virtual const _bool&	Is_SkillSuccess() { return m_IsSkillSuccess; }
public:
	virtual const wstring&   Get_Skill_StartName()	{ return m_strSkill_StartName; }
	virtual SKILL_ATTACKTYPE Get_Skill_AttackType() { return m_eAttackType; }
	virtual SKILL_CTRLTYPE   Get_Skill_CtrlType()	{ return m_eCtrlType; }
	virtual KEY				 Get_BindKey()			{ return m_eBindKey; }
	virtual const _float&	 Get_Skill_CoolTime()	{ return m_fSkillCoolTime; }
	
public:
	virtual void			Set_SkillSuccess(_bool IsSuccess) { m_IsSkillSuccess = IsSuccess; }
	virtual void			Set_BindKey(CPlayer_Controller::SKILL_KEY eKey);
	virtual void			Set_Skill_CoolTime(_float fSkillCoolTime) { m_fSkillCoolTime = fSkillCoolTime; }

protected:
	CGameObject*				  m_pOwner = { nullptr };
	wstring						  m_strSkill_StartName;

	SKILL_ATTACKTYPE		      m_eAttackType = { SKILL_ATTACKTYPE::_END };
	SKILL_CTRLTYPE				  m_eCtrlType = { SKILL_CTRLTYPE::_END };
	KEY							  m_eBindKey = { KEY::TYPE_END };
	CPlayer_Controller::SKILL_KEY m_eBindCtrlKey = { CPlayer_Controller::SKILL_KEY::_END };

	_float						  m_fSkillDamage = { 0.f };
	_float						  m_fSkillCoolTime = { 0.f };

	_bool						  m_IsSkillSuccess = { false };
	_bool						  m_IsSuperArmor = { false };

	vector<class CState*>		  m_State_Skills;

public:
	static CPlayer_Skill* Create(CGameObject* pOwner, void* pArg);
	virtual void Free();
};

END


