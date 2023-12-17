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
}

HRESULT CPlayer_Controller::Initialize_Prototype()
{
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
	if (false == m_bStop) Move(fTimeDelta);
	if (true == m_bStop) Look_Lerp(fTimeDelta);

	/* CoolTime */
	Skill_CoolTime(fTimeDelta);
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

_bool CPlayer_Controller::Is_Run()
{
	if (KEY_HOLD(KEY::RBTN) || KEY_TAP(KEY::RBTN))
	{
		return true;
	}
		
	return false;
}

_bool CPlayer_Controller::Is_Skill()
{
	if (KEY_HOLD(KEY::Q) || KEY_TAP(KEY::Q))
	{
		m_eSelectedSkill = SKILL_KEY::Q;
		return true;
	}
	else if (KEY_HOLD(KEY::W) || KEY_TAP(KEY::W))
	{
		m_eSelectedSkill = SKILL_KEY::W;
		return true;
	}
	else if (KEY_HOLD(KEY::E) || KEY_TAP(KEY::E))
	{
		m_eSelectedSkill = SKILL_KEY::E;
		return true;
	}
	else if (KEY_HOLD(KEY::R) || KEY_TAP(KEY::R))
	{
		m_eSelectedSkill = SKILL_KEY::R;
		return true;
	}
	else if (KEY_HOLD(KEY::A) || KEY_TAP(KEY::A))
	{
		m_eSelectedSkill = SKILL_KEY::A;
		return true;
	}
	else if (KEY_HOLD(KEY::S) || KEY_TAP(KEY::S))
	{
		m_eSelectedSkill = SKILL_KEY::S;
		return true;
	}
	else if (KEY_HOLD(KEY::D) || KEY_TAP(KEY::D))
	{
		m_eSelectedSkill = SKILL_KEY::D;
		return true;
	}
	else if (KEY_HOLD(KEY::F) || KEY_TAP(KEY::F))
	{
		m_eSelectedSkill = SKILL_KEY::F;
		return true;
	}

	return false;
}

_bool CPlayer_Controller::Is_Interect()
{
	if (KEY_TAP(KEY::G))
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Dash()
{
	if (KEY_TAP(KEY::SPACE))
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
	if (KEY_HOLD(KEY::LBTN) || KEY_TAP(KEY::LBTN))
		return true;

	return false;
}

HRESULT CPlayer_Controller::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey)
		return S_OK;

	m_pSkills[eKey] = pSkill;

	return S_OK;
}

_bool CPlayer_Controller::Is_SkillEnd(SKILL_KEY eKey)
{
	return m_pSkills[eKey]->Is_SkillEnd();
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
	if (Vec3(m_vNextMove - m_vPrePos).Length() <= 0.05f)
	{
		m_bMoveStop = true;
		return;
	}
	else
		m_bMoveStop = false;
		

	Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = m_vNextMove - vPos;
	m_pOwnerTransform->Move_ToPos(vDir, 12.f, 3.f, fTimeDelta);
}

void CPlayer_Controller::Look_Lerp(const _float& fTimeDelta)
{
	if (Vec3() == m_vNextMove)
		return;

	Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = m_vNextMove - vPos;
	m_pOwnerTransform->LookAt_Lerp_ForLand(vDir, 20.f, fTimeDelta);
}

void CPlayer_Controller::Look(Vec3 vAt)
{
	m_pOwnerTransform->LookAt_ForLandObject(vAt);
}

void CPlayer_Controller::Attack()
{
}

void CPlayer_Controller::Skill(SKILL_KEY eKey)
{
	m_fCoolTime[eKey] = m_pSkills[eKey]->Get_Skill_CoolTime();
}

void CPlayer_Controller::Hit()
{
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

void CPlayer_Controller::Free()
{
	__super::Free();
}