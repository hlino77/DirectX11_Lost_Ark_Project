#include "Player_Controller.h"
#include "GameInstance.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Player_Skill.h"

CPlayer_Controller::CPlayer_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CPlayer_Controller::CPlayer_Controller(const CPlayer_Controller& rhs)
	: CComponent(rhs)
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		m_fCoolDownAcc[i] = rhs.m_fCoolDownAcc[i];
		m_fCoolTime[i] = rhs.m_fCoolTime[i];
		m_fChangeStatAcc[i] = rhs.m_fChangeStatAcc[i];
		m_fChangeStatTime[i] = rhs.m_fChangeStatTime[i];
	}
}

HRESULT CPlayer_Controller::Initialize_Prototype()
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		m_fCoolDownAcc[i] = 0.f;
		m_fCoolTime[i] = -1.f;

		m_fChangeStatAcc[i] = 0.f;
		m_fChangeStatTime[i] = -1.f;
	}

	return S_OK;
}

HRESULT CPlayer_Controller::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		CONTROLL_DESC* pCtrlDesc = (CONTROLL_DESC*)pArg;
		m_pOwner = pCtrlDesc->pOwner;

		m_pOwnerTransform = pCtrlDesc->pOwnerTransform;

		m_pOwnerRigidBody = pCtrlDesc->pOwnerRigidBody;
	}

	return S_OK;
}

void CPlayer_Controller::Tick(_float fTimeDelta)
{
	if (false == m_bStop) 
		Move(fTimeDelta);
	if (true == m_bStop) 
		Look_Lerp(fTimeDelta);

	/* CoolTime */
	Skill_CoolTime(fTimeDelta);
	Skill_ChangeStat_CoolTime(fTimeDelta);

	/* Skill Collider */
	Skill_Check_Collider();
}

void CPlayer_Controller::LateTick(_float fTimeDelta)
{
	m_vPrePos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
}

void CPlayer_Controller::DebugRender()
{
}

_bool CPlayer_Controller::Is_Idle()
{
	if (false == Is_Run() && false == Is_Skill() && false == Is_Attack() && false == Is_Dash())
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Tap(KEY eKey)
{
	if (false == m_bKeyActive)
		return false;

	return KEY_TAP(eKey);
}

_bool CPlayer_Controller::Is_Hold(KEY eKey)
{
	if (false == m_bKeyActive)
		return false;

	return KEY_HOLD(eKey);
}

_bool CPlayer_Controller::Is_HoldorTap(KEY eKey)
{
	if (false == m_bKeyActive)
		return false;

	return KEY_HOLD(eKey) || KEY_TAP(eKey);
}

_bool CPlayer_Controller::Is_Away(KEY eKey)
{
	if (false == m_bKeyActive)
		return false;

	return KEY_AWAY(eKey) || KEY_NONE(eKey);
}

_bool CPlayer_Controller::Is_Run()
{
	if (false == m_bKeyActive)
		return false;

	if (KEY_HOLD(KEY::RBTN) || KEY_TAP(KEY::RBTN))
	{
		return true;
	}
		
	return false;
}

_bool CPlayer_Controller::Is_Skill()
{
	if (false == m_bKeyActive)
		return false;

	if (-1.f == m_fCoolTime[SKILL_KEY::Q] && nullptr != m_pSkills[SKILL_KEY::Q] && (KEY_HOLD(KEY::Q) || KEY_TAP(KEY::Q)))
	{
		m_eSelectedSkill = SKILL_KEY::Q;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::W] && nullptr != m_pSkills[SKILL_KEY::W] && (KEY_HOLD(KEY::W) || KEY_TAP(KEY::W)))
	{
		m_eSelectedSkill = SKILL_KEY::W;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::E] && nullptr != m_pSkills[SKILL_KEY::E] && (KEY_HOLD(KEY::E) || KEY_TAP(KEY::E)))
	{
		m_eSelectedSkill = SKILL_KEY::E;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::R] && nullptr != m_pSkills[SKILL_KEY::R] && (KEY_HOLD(KEY::R) || KEY_TAP(KEY::R)))
	{
		m_eSelectedSkill = SKILL_KEY::R;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::A] && nullptr != m_pSkills[SKILL_KEY::A] && (KEY_HOLD(KEY::A) || KEY_TAP(KEY::A)))
	{
		m_eSelectedSkill = SKILL_KEY::A;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::S] && nullptr != m_pSkills[SKILL_KEY::S] && (KEY_HOLD(KEY::S) || KEY_TAP(KEY::S)))
	{
		m_eSelectedSkill = SKILL_KEY::S;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::D] && nullptr != m_pSkills[SKILL_KEY::D] && (KEY_HOLD(KEY::D) || KEY_TAP(KEY::D)))
	{
		m_eSelectedSkill = SKILL_KEY::D;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::F] && nullptr != m_pSkills[SKILL_KEY::F] && (KEY_HOLD(KEY::F) || KEY_TAP(KEY::F)))
	{
		m_eSelectedSkill = SKILL_KEY::F;
		return true;
	}

	m_eSelectedSkill = SKILL_KEY::_END;
	return false;
}

_bool CPlayer_Controller::Is_Interect()
{
	if (false == m_bKeyActive)
		return false;

	if (KEY_TAP(KEY::G))
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Dash()
{
	if (false == m_bKeyActive)
		return false;

	if ( KEY_HOLD(KEY::SPACE) ||KEY_TAP(KEY::SPACE))
	{
		if (-1.f == m_fCoolTime[SKILL_KEY::SPACE])
			return true;
		else
			return false;
	}

	return false;
}

_bool CPlayer_Controller::Is_Attack()
{
	if (false == m_bKeyActive)
		return false;

	if (KEY_HOLD(KEY::LBTN) || KEY_TAP(KEY::LBTN))
		return true;

	return false;
}

void CPlayer_Controller::Get_RootMessage()
{
	Vec3 vTargetPos = m_pOwner->Get_TargetPos();
	Vec3 vPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = vTargetPos - vPos;
	_float fDist = vDir.Length();
	if (0.05f >= fDist)
	{
		m_pOwner->Get_ModelCom()->Set_RootDist(0.f);
	}
}

void CPlayer_Controller::Get_RootZeroMessage()
{
	m_pOwner->Get_ModelCom()->Set_RootDist(0.f);
	m_pOwnerTransform->Set_State(CTransform::STATE_POSITION, m_vPrePos);
}

void CPlayer_Controller::Get_GrabMessage(CGameObject* pGrab)
{
	m_pGrabber = pGrab;
}

void CPlayer_Controller::Get_GrabEndMessage()
{
	m_pGrabber = nullptr;
}

void CPlayer_Controller::Get_HitMessage(_uint iDamage, _float fForce, Vec3 vPos)
{
	m_iDamaged = iDamage;

	m_vHitColiPos = vPos;

	if (10.f <= fForce && 20.f > fForce)
	{
		m_eHitType = HIT_TYPE::DMG;
		m_fForced = fForce - 10.f;

		m_IsHitState = true;
	}
	else if (20.f <= fForce && 30.f > fForce)
	{
		m_eHitType = HIT_TYPE::DOWN;
		m_fForced = fForce - 20.f;

		m_IsHitState = true;
	}
	else if (30.f <= fForce && 40.f > fForce)
	{
		m_eHitType = HIT_TYPE::KNOCKDOWN;
		m_fForced = fForce - 30.f;

		m_IsHitState = true;
	}
	else if (40.f <= fForce && 50.f > fForce)
	{
		m_eHitType = HIT_TYPE::BOUND;
		m_fForced = fForce - 40.f;

		m_IsHitState = true;
	}
	else if (50.f <= fForce)
	{
		m_eHitType = HIT_TYPE::TWIST;
		m_fForced = fForce - 50.f;

		m_IsHitState = true;
	}
	else
	{
		m_eHitType = HIT_TYPE::WEAK;
	}
}

void CPlayer_Controller::Get_MoveMessage(Vec3 vPos, _float fMoveSpeed)
{
	if (Vec3() == vPos)
	{
		Get_StopMessage();
		return;
	}

	m_vNextMove = vPos; 
	m_bStop = false; 
	m_IsDir = false; 
	m_fMoveSpeed = fMoveSpeed;
}

void CPlayer_Controller::Get_DirMessage(Vec3 vPos, _float fMoveSpeed)
{
	if (Vec3() == vPos)
	{
		Get_StopMessage();
		return;
	}

	m_vNextMove = vPos; 
	m_bStop = false; 
	m_IsDir = true; 
	m_fMoveSpeed = fMoveSpeed;
}

void CPlayer_Controller::Get_LerpLookMessage(Vec3 vAt, _float fSpeed)
{
	if (Vec3() == vAt)
	{
		Get_StopMessage();
		return;
	}

	vAt.y = 0.f;

	m_vNextMove = vAt; 
	m_fLerpLook_Speed = fSpeed;
	m_bStop = true; 
	m_IsDir = false; 
}

void CPlayer_Controller::Get_LerpDirLookMessage(Vec3 vAt, _float fSpeed)
{
	if (Vec3() == vAt)
	{
		Get_StopMessage();
		return;
	}

	vAt.y = 0.f;

	Vec3 vPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = vAt - vPos;
	vDir.Normalize();

	m_vNextMove = vDir;
	m_fLerpLook_Speed = fSpeed;
	m_bStop = true; 
	m_IsDir = true;
}

void CPlayer_Controller::Get_LookMessage(Vec3 vAt)
{
	if (Vec3() == vAt)
	{
		Get_StopMessage();
		return;
	}

	Look(vAt);
}

void CPlayer_Controller::Get_DashMessage(Vec3 vPos)
{
	if (Vec3() == vPos)
	{
		Get_StopMessage();
		return;
	}

	Look(vPos);
}

HRESULT CPlayer_Controller::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	m_pSkills[eKey] = pSkill;

	return S_OK;
}

_bool CPlayer_Controller::Pick(_uint screenX, _uint screenY, Vec3& pickPos, _float& distance)
{
	return true;
}

void CPlayer_Controller::Input(const _float& fTimeDelta)
{
}



void CPlayer_Controller::Move(const _float& fTimeDelta)
{
	Vec3 vNext = m_vNextMove;
	Vec3 vCur = m_vPrePos;
	vNext.y = 0.0f; vCur.y = 0.0f;

	if (Vec3(vNext - vCur).Length() <= 0.05f)
	{
		m_bMoveStop = true;
		return;
	}
	else
	{
		m_bMoveStop = false;
	}

	if (false == m_IsDir)
	{
		Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
		Vec3 vNextPos = m_vNextMove;

		vPos.y = 0.f, vNextPos.y = 0.f;
		Vec3 vDir = vNextPos - vPos;
		m_pOwnerTransform->Move_ToPos(vDir, 12.f, m_fMoveSpeed, fTimeDelta);
	}
	else if (true == m_IsDir)
	{
		Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
		Vec3 vNextPos = m_vNextMove;

		vPos.y = 0.f, vNextPos.y = 0.f;
		Vec3 vDir = vNextPos - vPos;
		m_pOwnerTransform->Move_Dir(vDir, fTimeDelta, m_fMoveSpeed);
	}
}

void CPlayer_Controller::Look_Lerp(const _float& fTimeDelta)
{
	if (Vec3() == m_vNextMove)
		return;

	if (false == m_IsDir)
	{
		Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
		Vec3 vDir = m_vNextMove - vPos;
		m_pOwnerTransform->LookAt_Lerp_ForLand(vDir, m_fLerpLook_Speed, fTimeDelta);
	}
	else if (true == m_IsDir)
	{
		m_pOwnerTransform->LookAt_Lerp_ForLand(m_vNextMove, m_fLerpLook_Speed, fTimeDelta);
	}
}

void CPlayer_Controller::Look(Vec3 vAt)
{
	m_pOwnerTransform->LookAt_ForLandObject(vAt);
}

void CPlayer_Controller::Attack(Vec3 vPos)
{
}

HRESULT CPlayer_Controller::Set_SkillSuccess(SKILL_KEY eKey, _bool IsSuccess)
{
	if (nullptr == m_pSkills[eKey])
		return E_FAIL;

	m_pSkills[eKey]->Set_SkillSuccess(IsSuccess);
	return S_OK;
}

_bool CPlayer_Controller::Is_SkillSuccess(SKILL_KEY eKey)
{
	if (nullptr == m_pSkills[eKey])
		return false;

	return m_pSkills[eKey]->Is_SkillSuccess();
}

const wstring& CPlayer_Controller::Get_SkillStartName(SKILL_KEY eKey)
{
	return m_pSkills[eKey]->Get_Skill_StartName();
}

void CPlayer_Controller::Skill(SKILL_KEY eKey)
{
	m_fCoolTime[eKey] = m_pSkills[eKey]->Get_Skill_CoolTime();
}

void CPlayer_Controller::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
{
}

void CPlayer_Controller::ChangeStat(SKILL_KEY eKey)
{
	if (nullptr == m_pSkills[eKey] || eKey == SKILL_KEY::_END)
		return;

	m_fChangeStatTime[eKey] = m_pSkills[eKey]->Change_Player_Status();
	m_fChangeStatAcc[eKey] = 0.f;
}



void CPlayer_Controller::Skill_CoolTime(const _float& fTimeDelta)
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		if (-1.f == m_fCoolTime[i]) continue;

		m_fCoolDownAcc[i] += fTimeDelta;

		if (m_fCoolTime[i] <= m_fCoolDownAcc[i])
		{
			m_fCoolDownAcc[i] = 0.f;
			m_fCoolTime[i] = -1.f;
		}
	}
}

void CPlayer_Controller::Skill_ChangeStat_CoolTime(const _float& fTimeDelta)
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		if (-1.f == m_fChangeStatTime[i]) continue;

		m_fChangeStatAcc[i] += fTimeDelta;

		if (m_fChangeStatTime[i] <= m_fChangeStatAcc[i])
		{
			m_fChangeStatTime[i] = m_pSkills[i]->Restore_Player_Status();
			m_fChangeStatAcc[i] = 0.f;
		}
	}
}

void CPlayer_Controller::Skill_Check_Collider()
{
	if (SKILL_KEY::_END == m_eSelectedSkill)
		return;

	m_pSkills[m_eSelectedSkill]->Check_ColliderState();
}

void CPlayer_Controller::Free()
{
	__super::Free();

	for (auto& iter : m_Skills)
	{
		Safe_Release(iter.second);
	}
	m_Skills.clear();
}
