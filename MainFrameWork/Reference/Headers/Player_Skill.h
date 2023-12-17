#pragma once
#include "Base.h"
#include "Player_Controller.h"

BEGIN(Engine)

class CGameObject;
class CPlayer_Controller;
class CModel;
class CCollider;

class ENGINE_DLL CPlayer_Skill abstract : public CBase
{
public:
	enum class SKILL_ATTACKTYPE	  { NORMAL, NEUTRAL, DESTROY, _END};
	enum class SKILL_CTRLTYPE	  { NORMAL, COMBO, HOLD, TARGET, _END };

protected:
	CPlayer_Skill(CGameObject* pOwner);
	virtual ~CPlayer_Skill() = default;

public:
	virtual HRESULT			Initialize();
	virtual void			Tick(_float fTimeDelta) PURE;
	virtual void			UnCtrl_Tick(_float fTimeDelta) PURE;
	virtual HRESULT			Render_Debug()  PURE;

	virtual void			Enter() PURE;
	virtual void			Exit() PURE;

public:
	virtual HRESULT			Increase_Player_Stat() { return S_OK; }
	virtual HRESULT			Decrease_Player_Stat() { return S_OK; }
	virtual HRESULT			Restore_Player_Stat()  { return S_OK; }

public:
	virtual void			SkillEnd_Term() PURE;
	virtual void			SkillCancel_Term() PURE;
	virtual const _bool&	Is_SkillEnd() { return m_bIsSkillEnd; }
	virtual const _bool&	Is_SkillCancel() { return m_bIsSkillCancel; }

public:
	virtual KEY				Get_BindKey() { return m_eBindKey; }
	virtual _float			Get_Skill_CoolTime() { return m_fSkillCoolTime; }
public:
	virtual void			Set_BindKey(CPlayer_Controller::SKILL_KEY eKey);
	virtual void			Set_Skill_CoolTime(_float fSkillCoolTime) { m_fSkillCoolTime = fSkillCoolTime; }

protected:
	CGameObject*			m_pOwner = { nullptr };

	SKILL_ATTACKTYPE		m_eAttackType = { SKILL_ATTACKTYPE::_END };
	SKILL_CTRLTYPE			m_eCtrlType = { SKILL_CTRLTYPE::_END };
	KEY						m_eBindKey = { KEY::TYPE_END };

	_bool					m_bIsSkillEnd = { false };
	_bool					m_bIsSkillCancel = { false };

	_float					m_fSkillDamage = { 0.f };
	_float					m_fSkillCoolTime = { 0.f };

	/* 나중에 콜라이더를 들도록 만들어보자 */

public:
	virtual void Free();
};

END


