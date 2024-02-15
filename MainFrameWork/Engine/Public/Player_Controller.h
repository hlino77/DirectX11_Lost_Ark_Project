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

	enum class STATUSEFFECT { COUNTER, GROGGY, EARTHQUAKE, BUG, FEAR, SHOCK, STUN, SILENCE, _END };
	enum class BUFFEFFECT { HALFDAMAGE, MANAREFILL, STIIFIMMUNE, _END };

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

	virtual void	Set_Control_Active(_bool bActive) { m_bCtrlActive = bActive; }
	virtual void	Set_Key_Active(_bool bActive) { m_bKeyActive = bActive; }
	virtual void	Set_Mouse_Active(_bool bActive) { m_bMouseActive = bActive; }

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
	virtual _bool		Is_EstherSkill() PURE;

	virtual void		Get_MoveMessage(Vec3 vPos);  
	virtual void		Get_MoveSpeedMessage(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	virtual void		Get_DirMessage(Vec3 vPos); 
	virtual void		Get_StopMessage()	{ m_vNextMove = Vec3(); m_bStop = true;}
	virtual void		Get_LerpLookMessage(Vec3 vAt, _float fSpeed = 20.f); 
	virtual void		Get_LerpDirLookMessage(Vec3 vAt, _float fSpeed = 20.f);
	virtual void		Get_LookMessage(Vec3 vAt);
	virtual void		Get_AttackMessage(Vec3 vPos = Vec3()) { Attack(vPos); }
	virtual void		Get_SkillMessage(SKILL_KEY eKey) { Skill(eKey); }
	virtual void		Get_SkillAttackMessage(SKILL_KEY eKey, Vec3 vPos = Vec3()) { SkillAttack(eKey, vPos); }
	virtual void		Get_SkillStartMessage(_uint iBindKey);
	virtual void		Get_SkillEndMessage() { m_eSelectedSkill = SKILL_KEY::_END; }
	virtual void		Get_SkillChangeStatMessage(SKILL_KEY eKey) { ChangeStat(eKey); }
	
	virtual void		Get_DashMessage(Vec3 vPos);
	virtual void		Get_DashEndMessage(_float fCoolTime);
	virtual void		Get_HitMessage(_uint iDamage, _float fForce, Vec3 vPos = Vec3());
	virtual void		Get_HitEndMessage() { m_eHitType = HIT_TYPE::TYPE_END; m_IsHitState = false;}

	virtual void		Get_MoveToNpcMessage() { m_fMoveLength = 1.f; }
	virtual void		Get_MoveToCellMessage() { m_fMoveLength = 0.06f; }

	virtual void		Get_RootMessage();
	virtual void		Get_RootZeroMessage();

	virtual void		Get_GrabMessage(CGameObject* pGrabber);
	virtual void		Get_GrabEndMessage();

	virtual void		Get_StatusEffectMessage(_uint iStatus, _float fDurtaion);

	virtual void		Get_DeadMessage() { m_IsDead = true; }
	virtual void		Get_DeadEndMessage() { m_IsDead = false; }

	virtual void		Get_CheckLengthMessage(_float fCheckLength, CGameObject* pOther);

	virtual void		Get_EstherGageAddMessage(_uint iGage);
	virtual void		Get_EshterGageUseMessage() { m_iCurEstherGage = 0; }
	virtual void		Get_EstherMessage(_uint iIndex) { EstherSkill(iIndex); }
	virtual _uint		Get_MaxEstherGauge() { return	m_iMaxEstherGage; }
	virtual _uint		Get_CurrEstherGauge() { return	m_iCurEstherGage; }

	virtual void		Get_BuffMessage(_uint iBuffStatus, _float fAmount, _float fDurtaion);

public:
	_bool				Is_Stop() { return m_bMoveStop; }
	_bool				Is_HitState() { return m_IsHitState; }
	_bool				Is_Dead() { return m_IsDead; }

	/* 스킬 함수 */
public:
	HRESULT					Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT					Set_SkillSuccess(SKILL_KEY eKey, _bool IsSuccess);

public:
	_bool					Is_SkillSuccess(SKILL_KEY eKey);

	SKILL_KEY				Get_Selected_Skill() { return m_eSelectedSkill; }
	class CPlayer_Skill*	Get_PlayerSkill(SKILL_KEY eKey) { 
		if (nullptr != m_pSkills[eKey]) return m_pSkills[eKey];
		else return nullptr;
	}
	const wstring&		    Get_SkillStartName(SKILL_KEY eKey);

	_float					Get_Skill_CoolDown(SKILL_KEY eKey) {
		if ((SKILL_KEY::SPACE != eKey) && (nullptr == m_pSkills[eKey])) return 0.f;
		else return m_fCoolDownAcc[eKey];
	}
	_float					Get_Skill_CoolTime(SKILL_KEY eKey) {
		if ((SKILL_KEY::SPACE != eKey) && (nullptr == m_pSkills[eKey])) return -1.f;
		else return m_fCoolTime[eKey];
	}
	_float					Get_IdentitySkill_CoolDown() {
		return m_fCoolDownAcc[SKILL_KEY::Z];
	}

	HIT_TYPE				Get_HitType() { return m_eHitType; }
	_float					Get_Forced() { return m_fForced; }
	_uint					Get_Damaged() { return m_iDamaged; }
	int64					Get_CalculateDamaged() { return m_iCalculateDamaged; }

	CGameObject*			Get_Grabber() { return m_pGrabber; }
	_bool					Is_GrabState() { return m_IsGrabState; }

	_bool					Get_StatusEffect_State(_uint iStatusEffect) { return m_bStatusEffect[iStatusEffect]; }

	class CPlayer_Skill*	Find_Skill(wstring strSkillName) { return m_Skills.find(strSkillName)->second; }
	const void				Set_SkilltoCtrl(wstring strSkillName, class CPlayer_Skill* pSkill) {  m_Skills.emplace(strSkillName, pSkill); }

	_uint					Get_EstherType() { return m_iEstherType; }
	void					Set_Esther(CGameObject* pEsther) { m_vecEsther.push_back(pEsther); }



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

	/* 상태이상 */
	virtual void			StatusEffect_Duration(const _float& fTimeDelta);
	virtual void			Bug();
	virtual void			Fear();
	virtual void			EarthQuake();
	virtual void			Shock();
	virtual void			Stun();
	virtual void			Silence();

	/* 버프 */
	virtual void			BuffEffect_Duration(const _float& fTimeDelta);
	virtual void			HalfDamage(_float fAmount);
	virtual void			ManaRefill(_float fAmount);
	virtual void			StiffImmune(_float fAmount);

	/* 에스더 */
	virtual void			EstherSkill(_uint iIndex) {};
	virtual void			Esther_Refill(_float fTimeDelta) {};

	/* 마나 */
	virtual void			Refill_Mana(_float fTimeDelta);

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameObject*			m_pOwner = nullptr;
	CTransform*				m_pOwnerTransform = nullptr;
	CRigidBody*				m_pOwnerRigidBody = nullptr;

	_bool					m_bCtrlActive = { true };
	_bool					m_bKeyActive = { true };
	_bool					m_bMouseActive = { true };
	_bool					m_bSkillKeyActive = { true };

	/* 상태이상 변수 */
	_bool					m_bStatusEffect[(_uint)STATUSEFFECT::_END];
	_float					m_fStatusDuration[(_uint)STATUSEFFECT::_END];

	/* 버프 변수 */
	_bool					m_bBuffEffect[(_uint)BUFFEFFECT::_END];
	_float					m_fBuffDuration[(_uint)BUFFEFFECT::_END];
	_float					m_fBuffAmount[(_uint)BUFFEFFECT::_END];


	/* 플레이어 Tick 움직임 */
	_bool					m_bStop = { false };

	/* 플레이어 움직임 변수*/
	Vec3					m_vPrePos;
	Vec3					m_vNextMove;
	_bool					m_bMoveStop = { false };
	_bool					m_IsDir = { false };
	_float					m_fLerpLook_Speed = { 20.f };
	_float					m_fMoveSpeed = { 3.f };
	_float					m_fDefaultMoveSpeed = { 3.f };
	_float					m_fMoveLength = { 0.01f };

	/* 플레이어 히트 변수*/
	HIT_TYPE				m_eHitType = { HIT_TYPE::TYPE_END };
	_uint					m_iDamaged = { 0 };
	int64					m_iCalculateDamaged = { 0 };
	_float					m_fForced = { 0.f };
	Vec3					m_vHitColiPos;
	_bool					m_IsHitState = { false };

	_bool					m_IsDead = { false };

	/* 플레이어 그랩 변수 */
	CGameObject*			m_pGrabber = { nullptr };
	_bool					m_IsGrabState = { false };

	/* 스킬 */
	unordered_map<wstring, class CPlayer_Skill*> m_Skills;

	class CPlayer_Skill*	m_pSkills[SKILL_KEY::_END] = { nullptr };
	SKILL_KEY				m_eSelectedSkill = { SKILL_KEY::_END };

	PROJECTILE_DESC			m_AttackDesc;

	/* 에스더 스킬 */
	_uint					m_iEstherType;

	_float					m_fEstherAcc = { 0.0f };
	_float					m_fEstherFillTime = { 15.f };
	_uint					m_iEstherFill = { 5 };

	_uint					m_iMaxEstherGage = { 100 };
	_uint					m_iCurEstherGage = { 95 };
	vector<CGameObject*>	m_vecEsther;

	/* 쿨 타임 */
	_float					m_fCoolDownAcc[SKILL_KEY::_END];
	_float					m_fCoolTime[SKILL_KEY::_END];

	_float					m_fChangeStatAcc[SKILL_KEY::_END];
	_float					m_fChangeStatTime[SKILL_KEY::_END];

	_float					m_fRefillManaAcc = 0.0f;

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