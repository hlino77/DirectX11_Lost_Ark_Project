#pragma once

#include "Server_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
class CBehaviorTree;
END


BEGIN(Server)
class CMonster_Server : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		_uint	iLevel;
	}MODELDESC;

public:


protected:
	CMonster_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Server(const CMonster_Server& rhs);
	virtual ~CMonster_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	void Update_StatusEffect(_float fTimeDelta);

public:


	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }

	void					Set_FollowDistance(_float fDistance) { m_fFollowDistance = fDistance; }
	_float					Get_FollowDistance() { return m_fFollowDistance; }
	
	void					Send_Collision(_uint iDamage, Vec3 vHitPos, STATUSEFFECT eEffect, _float fForce, _float fDuration);

	virtual void				Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration);
	
public:

	void Find_NearTarget(_float fTimeDelta);

	void					Send_NearTarget();

	_float					Get_NearTargetDistance();

	Vec3 Get_Target_Direction();

	Vec3 Get_Target_RandomDirection();

	void Set_RandomPosition();

	void Set_RandomPosition(_float fRange);

	void Move_to_RandomPosition(_float fTimeDelta);

	_float Get_Target_Distance();

	_bool Is_Close_To_RandomPosition();

	_bool Is_Close_To_TargetRandomPosition();
	
	Vec3 Get_RandomPosition() { return m_vRandomPosition; }


	void LookAt_Target_Direction_Lerp(_float fTimeDelta);

	void LookAt_Target_Direction();

	//Send Packet
	void				Send_State(const wstring& szName);
	void				Set_State(const wstring& szName);
	void				Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	void				Send_ColliderState(const _uint& iLayer);
	void				Send_SlowMotion(_bool bSlow);
	void				Send_Hp();
	void				Send_Monster_Action();

	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();


	void				Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void				Body_Collision(CGameObject* pObject);
	void				Hit_Attack(CCollider* pCollider);

	virtual void				Set_Die();
public:
	_bool						Is_Hit() { return m_IsHit; }
	void						Set_Hit(_bool bHit) { m_IsHit = bHit; }

	_bool						Is_Left() { return m_IsLeft; }
	void						Set_Left(_bool IsLeft) { m_IsLeft = IsLeft; }

	_bool						Is_Spawn() { return m_IsSpawn; }
	void						Set_Spawn(_bool IsSpawn) { m_IsSpawn = IsSpawn; }

	_bool						Is_SuperArmor() { return m_IsSuperArmor; }
	void						Set_SuperArmor(_bool IsSuperArmor) { m_IsSuperArmor = IsSuperArmor; }

	_bool						Is_Bound() { return m_IsBound; }
	void						Set_Bound(_bool IsBound) { m_IsBound = IsBound; }

	_bool						Is_Twist() { return m_IsTwist; }
	void						Set_Twist(_bool IsTwist) { m_IsTwist = IsTwist; }

	_bool						Is_Countered() { return m_IsDowned; }
	void						Set_Downed(_bool IsDowned) { m_IsDowned = IsDowned; }

	_bool						Is_TwistLanding() { return m_IsTwistLanding; }
	void						Set_TwistLanding(_bool IsLanding) { m_IsTwistLanding = IsLanding; }

	_bool						Is_SecondHit() { return m_IsSecondHit; }
	void						Set_SecondHit(_bool bHit) { m_IsSecondHit = bHit; }

	_bool						Is_Maz();
	_bool						Was_Maz() { return m_bWasMaz; }
	void						Set_WasMaz(_bool bWasMaz) { m_bWasMaz = bWasMaz; }


	_bool						Is_BoundLanding() { return m_IsBoundLanding; }
	void						Set_BoundLanding(_bool IsLanding) { m_IsBoundLanding = IsLanding; }


	void					Set_AnimationSpeed(_float fAnimationSpeed) { m_fAnimationSpeed = fAnimationSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

	_bool					Is_Attacked() { return m_IsAttacked; }
	void					Set_Attacked(_bool IsAttacked) { m_IsAttacked = IsAttacked; }
	_bool						Is_SetuponCell() { return m_IsSetuponCell; }
	void						Set_SetuponCell(_bool IsSetuponCell) { m_IsSetuponCell = IsSetuponCell; }

	void Set_RootTargetDistance(_float fDistance) { m_fRootTargetDistance = fDistance; }


	_bool					Is_Skill();
	void					Reset_SkillStack(){		m_iSkillStack = 0;			m_fSkillCoolDown = 0.f; }
	void					Add_SkillStack() { m_iSkillStack++; }

	void					Set_AttackRange(_int iRangeIndex);
	_float					Get_AttackRange() { return m_fAttackRange; }

	void					Set_NoticeRange(_float fNoticeRange) { m_fNoticeRange = fNoticeRange; }
	_float					Get_NoticeRange() { return m_fNoticeRange; }

	void					Set_Action(wstring strAction) { m_strAction = strAction; }
	wstring					Get_Action() { return m_strAction; }
	
	_float					Get_StatusEffect(STATUSEFFECT eEffect) { return m_fStatusEffects[(_uint)eEffect]; }

protected:
	virtual HRESULT		Ready_Components();
	void				Set_to_RootPosition(_float fTimeDelta, _float _TargetDistance = 0.f);
	virtual HRESULT		Ready_BehaviourTree() {		return S_OK;	}

protected:
	wstring							m_strAction = L"";
	CRenderer*						m_pRendererCom = nullptr;
	CBehaviorTree*					m_pBehaviorTree = nullptr;
	_float							m_fScanCoolDown = 0.f;
	std::future<HRESULT>			m_PlayAnimation;
	_float							m_fMoveSpeed = 1.5f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;
	vector<_float>					m_vecAttackRanges ;
	_float							m_fAttackRange = 0.f;
	_float							m_fNoticeRange = 0.f;
	unordered_map<wstring, _uint>	m_BoneIndex;
	_float							m_fFollowDistance = 0.0f;
	_float							m_fTimeCount = 0.f;
	atomic<_int>					m_iSlowMotionCount = 0;
	_bool							m_IsAttacked = true;
	_bool							m_IsHit = false;
	_bool							m_IsSecondHit = false;
	_bool							m_IsBound = false;
	_bool							m_IsTwist = false;
	_float							m_fHitTerm = 0.f;
	_bool							m_IsLeft = false;
	_bool							m_IsSuperArmor = false;
	_bool							m_IsSpawn = true;
	_bool							m_IsDowned = false;
	_bool							m_IsTwistLanding = false;
	_bool							m_IsBoundLanding = false;
	_bool							m_bWasMaz = false;
	_uint							m_iSkillStack	=	0;
	_float							m_fSkillCoolDown = 0.f;
	Vec3							m_vRandomPosition = {};
	_float							m_fStatusEffects[(_uint)STATUSEFFECT::EFFECTEND] = {};
	_float							m_fRootTargetDistance = 0.f;
	_bool							m_IsSetuponCell = true;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */


public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

