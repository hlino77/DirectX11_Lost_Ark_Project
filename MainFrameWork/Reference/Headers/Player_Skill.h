#pragma once
#include "Base.h"
#include "Player_Controller.h"

BEGIN(Engine)

class CGameObject;
class CPlayer_Controller;
class CModel;
class CCollider;

class ENGINE_DLL CPlayer_Skill abstract : public CGameObject
{
public:
	enum class SKILL_ATTACKTYPE { NORMAL, NEUTRAL, DESTROY, COUNTER, _END };
	enum class SKILL_CTRLTYPE { NORMAL, COMBO, HOLD, TARGET, _END };

public:
	typedef struct tagPlayerSkillDesc
	{
		CGameObject*	 pOwner = { nullptr };
		wstring			 strSkill_StartName;
		vector<class CState*> State_Skills;

	}PLAYERSKILL_DESC;
	
protected:
	CPlayer_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjectTag, _int iObjType);
	virtual ~CPlayer_Skill() = default;

public:
	virtual HRESULT			Initialize(void* pArg);
	virtual HRESULT			Ready_Component() { return S_OK; };
	virtual HRESULT			Ready_Collider() { return S_OK; };
	
public:
	virtual HRESULT			Increase_Player_Stat() { return S_OK; }
	virtual HRESULT			Decrease_Player_Stat() { return S_OK; }
	virtual HRESULT			Restore_Player_Stat()  { return S_OK; }


public:
	virtual const _bool&	 Is_Attack()			{ return m_IsAttack; }
	virtual const _bool&	 Is_SuperArmor()		{ return m_IsSuperArmor; }
	virtual const _bool&	 Is_SkillSuccess()		{ return m_IsSkillSuccess; }
public:
	virtual CGameObject*			Get_SkillOwner()	    { return m_pOwner; }
	virtual const wstring&			Get_Skill_Name()		{ return m_strSkill_Name; }
	virtual const wstring&			Get_Skill_StartName()	{ return m_strSkill_StartName; }
	virtual SKILL_ATTACKTYPE		Get_Skill_AttackType()  { return m_eAttackType; }
	virtual SKILL_CTRLTYPE			Get_Skill_CtrlType()	{ return m_eCtrlType; }
	virtual KEY						Get_BindKey()			{ return m_eBindKey; }
	virtual const _float&			Get_Skill_CoolTime()	{ return m_fSkillCoolTime; }
	virtual const class CTexture*	Get_Skill_Texture()		{ return m_pSkillTextureCom; }
	virtual PROJECTILE_DESC			Get_Skill_Proj_Desc()	{ return m_SkillProjDesc; }


public:
	virtual void			Set_SkillProjPos(Vec3 vPos) { m_SkillProjDesc.vAttackPos = vPos; }
	virtual void			Set_AttackState(_bool bAttack) { m_IsAttack = bAttack; }
	virtual void			Set_SkillSuccess(_bool IsSuccess) { m_IsSkillSuccess = IsSuccess; }
	virtual void			Set_BindKey(CPlayer_Controller::SKILL_KEY eKey);
	virtual void			Set_Skill_CoolTime(_float fSkillCoolTime) { m_fSkillCoolTime = fSkillCoolTime; }

public:
	virtual void			Check_ColliderState();

protected:
	class CGameObject*			  m_pOwner = { nullptr };
	class CTexture*				  m_pSkillTextureCom = { nullptr };


	wstring						  m_strSkill_Name;
	wstring						  m_strSkill_StartName;

	PROJECTILE_DESC				  m_SkillProjDesc;
	vector<PROJECTILE_DESC>		  m_vecSkillProjDesces;

	SKILL_ATTACKTYPE		      m_eAttackType = { SKILL_ATTACKTYPE::_END };
	SKILL_CTRLTYPE				  m_eCtrlType = { SKILL_CTRLTYPE::_END };
	KEY							  m_eBindKey = { KEY::TYPE_END };
	CPlayer_Controller::SKILL_KEY m_eBindCtrlKey = { CPlayer_Controller::SKILL_KEY::_END };

	_float						  m_fSkillDamage = { 0.f };
	_float						  m_fSkillCoolTime = { 0.f };

	_bool						  m_IsSkillSuccess = { false };
	_bool						  m_IsSuperArmor = { false };
	_bool						  m_IsAttack = { false };

	vector<class CState*>		  m_State_Skills;

public:
	virtual void Free();
};

END


