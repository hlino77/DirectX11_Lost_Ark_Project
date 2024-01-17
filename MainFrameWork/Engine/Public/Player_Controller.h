#pragma once
#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"
#include "State.h"

BEGIN(Engine)

class ENGINE_DLL CPlayer_Controller abstract : public CComponent
{
public:
	enum SKILL_KEY { SPACE, Q, W, E, R, A, S, D, F, Z, _END };
	enum HIT_TYPE { WEAK, DMG, DOWN, KNOCKDOWN, BOUND, TWIST, TYPE_END };

public:
	typedef struct tagControllerDesc
	{
		CGameObject*	pOwner = { nullptr };
		CTransform*		pOwnerTransform = { nullptr };
		CRigidBody*		pOwnerRigidBody = { nullptr };
	}CONTROLL_DESC;

protected:
	CPlayer_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Controller(const CPlayer_Controller& rhs);
	virtual ~CPlayer_Controller() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual void	DebugRender();

	virtual void	Set_Key_Active(_bool bActive) { m_bKeyActive = bActive; }

public:
	_bool				Is_Tap(KEY eKey);
	_bool				Is_Hold(KEY eKey);
	_bool				Is_HoldorTap(KEY eKey);
	_bool				Is_Away(KEY eKey);

public:
	_bool				Is_Idle();
	_bool				Is_Run();
	_bool				Is_Skill();
	_bool				Is_Interect();
	_bool				Is_Dash();
	_bool				Is_Attack();

	virtual void		Get_MoveMessage(Vec3 vPos, _float fMoveSpeed = 3.f);  
	virtual void		Get_DirMessage(Vec3 vPos, _float fMoveSpeed = 3.f); 
	virtual void		Get_StopMessage()	{ m_vNextMove = Vec3(); m_bStop = true;}
	virtual void		Get_LerpLookMessage(Vec3 vAt, _float fSpeed = 20.f); 
	virtual void		Get_LerpDirLookMessage(Vec3 vAt, _float fSpeed = 20.f);
	virtual void		Get_LookMessage(Vec3 vAt);
	virtual void		Get_AttackMessage(Vec3 vPos = Vec3()) { Attack(vPos); }
	virtual void		Get_SkillMessage(SKILL_KEY eKey) { Skill(eKey); }
	virtual void		Get_SkillAttackMessage(SKILL_KEY eKey, Vec3 vPos = Vec3()) { SkillAttack(eKey, vPos); }
	virtual void		Get_SkillEndMessage() { m_eSelectedSkill = SKILL_KEY::_END; }
	virtual void		Get_SkillChangeStatMessage(SKILL_KEY eKey) { ChangeStat(eKey); }
	
	virtual void		Get_DashMessage(Vec3 vPos);
	virtual void		Get_DashEndMessage(_float fCoolTime) { m_fCoolTime[SKILL_KEY::SPACE] = fCoolTime; }
	virtual void		Get_HitMessage(_uint iDamage, _float fForce, Vec3 vPos = Vec3());
	virtual void		Get_HitEndMessage() { m_eHitType = HIT_TYPE::TYPE_END; m_IsHitState = false;}

	virtual void		Get_MoveToNpcMessage() { m_fMoveLength = 1.f; }
	virtual void		Get_MoveToCellMessage() { m_fMoveLength = 0.06f; }

	virtual void		Get_RootMessage();
	virtual void		Get_RootZeroMessage();

public:
	_bool				Is_Stop() { return m_bMoveStop; }
	_bool				Is_HitState() { return m_IsHitState; }

	/* 스킬 함수 */
public:
	HRESULT					Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT					Set_SkillSuccess(SKILL_KEY eKey, _bool IsSuccess);

public:
	_bool					Is_SkillSuccess(SKILL_KEY eKey);

	SKILL_KEY				Get_Selected_Skill() { return m_eSelectedSkill; }
	class CPlayer_Skill*	Get_PlayerSkill(SKILL_KEY eKey) { if (nullptr != m_pSkills[eKey]) return m_pSkills[eKey]; }
	const wstring&		    Get_SkillStartName(SKILL_KEY eKey);

	_float					Get_Skill_CoolDown(SKILL_KEY eKey) {
		if ((SKILL_KEY::SPACE != eKey) && (nullptr == m_pSkills[eKey])) return 0.f;
		else return m_fCoolDownAcc[eKey];
	}
	_float					Get_Skill_CoolTime(SKILL_KEY eKey) {
		if ((SKILL_KEY::SPACE != eKey) && (nullptr == m_pSkills[eKey])) return -1.f;
		else return m_fCoolTime[eKey];
	}

	HIT_TYPE				Get_HitType() { return m_eHitType; }
	_float					Get_Forced() { return m_fForced; }
	_uint					Get_Damaged() { return m_iDamaged; }

	class CPlayer_Skill*	Find_Skill(wstring strSkillName) { return m_Skills.find(strSkillName)->second; }
	const void				Set_SkilltoCtrl(wstring strSkillName, class CPlayer_Skill* pSkill) {  m_Skills.emplace(strSkillName, pSkill); }

public:
	/* 언젠가는 쓰겠지 */
	_bool					Pick(_uint screenX, _uint screenY, Vec3 & pickPos, _float & distance);

protected:
	virtual void			Move(const _float& fTimeDelta);
	virtual void			Look_Lerp(const _float& fTimeDelta);
	virtual void			Look(Vec3 vAt);
	virtual void			Input(const _float & fTimeDelta);
	virtual void			Attack(Vec3 vPos);
	virtual void			Skill(SKILL_KEY eKey);
	virtual void			ChangeStat(SKILL_KEY eKey);
	virtual void			SkillAttack(SKILL_KEY eKey, Vec3 vPos);

	virtual void			Skill_CoolTime(const _float& fTimeDelta);
	virtual void			Skill_ChangeStat_CoolTime(const _float& fTimeDelta);
	virtual void			Skill_Check_Collider();

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameObject*			m_pOwner = nullptr;
	CTransform*				m_pOwnerTransform = nullptr;
	CRigidBody*				m_pOwnerRigidBody = nullptr;

	_bool					m_bKeyActive = { true };

	/* 플레이어 Tick 움직임 */
	_bool					m_bStop = { false };

	/* 플레이어 움직임 변수*/
	Vec3					m_vPrePos;
	Vec3					m_vNextMove;
	_bool					m_bMoveStop = { false };
	_bool					m_IsDir = { false };
	_float					m_fLerpLook_Speed = { 20.f };
	_float					m_fMoveSpeed = { 3.f };

	_float					m_fMoveLength = { 0.05f };

	/* 플레이어 히트 변수*/
	HIT_TYPE				m_eHitType = { HIT_TYPE::TYPE_END };
	_uint					m_iDamaged = { 0 };
	_float					m_fForced = { 0.f };
	Vec3					m_vHitColiPos;
	_bool					m_IsHitState = { false };

	/* 스킬 */
	unordered_map<wstring, class CPlayer_Skill*> m_Skills;

	class CPlayer_Skill*	m_pSkills[SKILL_KEY::_END] = { nullptr };
	SKILL_KEY				m_eSelectedSkill = { SKILL_KEY::_END };

	PROJECTILE_DESC			m_AttackDesc;

	/* 쿨 타임 */
	_float					m_fCoolDownAcc[SKILL_KEY::_END];
	_float					m_fCoolTime[SKILL_KEY::_END];

	_float					m_fChangeStatAcc[SKILL_KEY::_END];
	_float					m_fChangeStatTime[SKILL_KEY::_END];


	/* SG엔진 변수 */
	Vec3					m_vMaxLinearSpeed;
	Vec3					m_vLinearSpeed;
	Vec3					m_vMaxAngularSpeed;
	Vec3					m_vAngularSpeed;

public:
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	PURE;
	virtual void Free() override;
};

END