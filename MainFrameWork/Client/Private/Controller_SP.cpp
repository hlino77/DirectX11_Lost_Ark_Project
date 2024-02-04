#include "stdafx.h"
#include "..\Public\Controller_SP.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Doaga.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"
#include "Esther.h"

CController_SP::CController_SP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CController_SP::CController_SP(const CController_SP& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CController_SP::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CController_SP::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;

	/* 기본공격 */
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.2f);
	Proj_Desc.vChildScale = Vec3(1.f, 0.6f, 1.0f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.2f);
	Proj_Desc.iDamage = 80;
	Proj_Desc.fRepulsion = 5.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[0] = Proj_Desc;

	/* 차지공격 */
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.6f);
	Proj_Desc.vChildScale = Vec3(1.f, 0.6f, 1.4f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.6f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.fRepulsion = 21.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[1] = Proj_Desc;


	///* 아덴스킬 */
	PROJECTILE_DESC Proj_Iden_Desc;
	Proj_Iden_Desc.pAttackOwner = m_pOwner;
	Proj_Iden_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Iden_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Iden_Desc.fAttackTime = 0.05f;
	Proj_Iden_Desc.fRadius = 2.2f;
	Proj_Iden_Desc.vOffset = Vec3(0.0f, 0.2f, 0.1f);
	Proj_Iden_Desc.iDamage = 2;
	Proj_Iden_Desc.iStatusEffect = (_uint)STATUSEFFECT::EARTHQUAKE;
	Proj_Iden_Desc.fStatusDuration = 3.f;
	Proj_Iden_Desc.bUseProjPos = true;
	m_Attack_Desces[2] = Proj_Iden_Desc;
	
	

	return S_OK;
}

void CController_SP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->Tick(fTimeDelta);
	}
}

void CController_SP::LateTick(_float fTimeDelta)
{
	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->LateTick(fTimeDelta);
	}

	__super::LateTick(fTimeDelta);
}

void CController_SP::DebugRender()
{
}

void CController_SP::Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos)
{
	if (true == m_IsDead)
		return;

	__super::Get_HitMessage(iDamge, fForce, vPos);

	// 데미지하락 및 밉라이트?

	if (HIT_TYPE::WEAK != m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperiorArmor())
	{
		if (HIT_TYPE::DMG == m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperArmor())
		{
			m_IsHitState = true;

			static_cast<CPlayer*>(m_pOwner)->Set_TargetPos(Vec3(m_vHitColiPos.x, m_fForced, m_vHitColiPos.z));
			static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Hit_Common"));
		}
		else if (HIT_TYPE::DMG != m_eHitType)
		{
			m_IsHitState = true;

			_float fCheckHit = 0.0f;
			switch (m_eHitType)
			{
			case Engine::CPlayer_Controller::DOWN:
				fCheckHit = m_fForced;
				break;
			case Engine::CPlayer_Controller::KNOCKDOWN:
				fCheckHit = 10.f + m_fForced;
				break;
			case Engine::CPlayer_Controller::BOUND:
				fCheckHit = 20.f + m_fForced;
				break;
			case Engine::CPlayer_Controller::TWIST:
				fCheckHit = 30.f + m_fForced;
				break;
			}
			static_cast<CPlayer*>(m_pOwner)->Set_TargetPos(Vec3(m_vHitColiPos.x, fCheckHit, m_vHitColiPos.z));
			static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Hit"));
		}
	}
}

void CController_SP::Get_GrabMessage(CGameObject* pGrabber)
{
	__super::Get_GrabMessage(pGrabber);

	static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Grabbed"));

	static_cast<CPlayer*>(m_pOwner)->Set_SuperArmorState(false);
	static_cast<CPlayer*>(m_pOwner)->Set_Invincible(false);
	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(false);
}

void CController_SP::Get_GrabEndMessage()
{
	__super::Get_GrabEndMessage();
}

void CController_SP::Get_DeadMessage()
{
	__super::Get_DeadMessage();

	m_fIdentityGage = 0;
	m_iMarbleCnt = 0;
}

_uint CController_SP::Is_SP_Identity()
{
	if (false == m_bKeyActive || false == m_bCtrlActive)
		return 0;

	if (KEY_TAP(KEY::Z))
	{
		if (2 > m_iMarbleCnt)
			return 0;

		m_iIdenSkill = MOON;

		return 1;
	}
	else if (KEY_TAP(KEY::X))
	{
		if (1 > m_iMarbleCnt)
			return 0;

		m_iIdenSkill = SUN;

		return 2;
	}

	return 0;
}

HRESULT CController_SP::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	__super::Bind_Skill(eKey, pSkill);
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

CPlayer_Skill* CController_SP::Get_PlayerSkill_MG(SKILL_KEY eKey)
{
	return nullptr;
}

void CController_SP::Input(const _float& fTimeDelta)
{

}

void CController_SP::Attack(Vec3 vPos)
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();

	if (Vec3() != vPos)
	{
		m_AttackDesc.vAttackPos = vPos;
	}
	else
	{
		m_AttackDesc.AttackMatrix = m_pOwner->Get_TransformCom()->Get_WorldMatrix();
	}

	pAttack->InitProjectile(&m_AttackDesc);
}

void CController_SP::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
{
	if (nullptr == m_pSkills[eKey])
		return;

	CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
	if (Vec3() != vPos)
	{
		m_pSkills[eKey]->Set_SkillProjPos(vPos);
	}
	else
	{
		m_pSkills[eKey]->Set_SkillProjMat(m_pOwner->Get_TransformCom()->Get_WorldMatrix());
	}

	pSkill->InitProjectile(&m_pSkills[eKey]->Get_Skill_Proj_Desc());
}

void CController_SP::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

void CController_SP::EstherSkill(_uint iIndex)
{
	for (auto& pEsther : m_vecEsther)
	{
		if (static_cast<CEsther*>(pEsther)->Get_EstherType() == iIndex)
		{
			if (false == pEsther->Is_Active())
				static_cast<CEsther*>(pEsther)->Leader_Active_Esther();
		}
	}
}

void CController_SP::Get_SP_IdentityMessage()
{
	if (MOON == m_iIdenSkill)
	{
		m_iMarbleCnt -= 2;
		if (0 >= m_iMarbleCnt)
			m_iMarbleCnt = 0;
	}
	else if(SUN == m_iIdenSkill)
	{
		m_iMarbleCnt -= 1;
		if (0 >= m_iMarbleCnt)
			m_iMarbleCnt = 0;
	}
}

void CController_SP::Increase_IdenGage(_float iGage)
{
	if (m_iMarbleCnt == m_iMaxMarbleCnt)
		return;

	m_fIdentityGage += iGage;

	if (m_fMaxGage <= m_fIdentityGage)
	{
		m_fIdentityGage = 0;

		m_iMarbleCnt++;

		if (m_iMaxMarbleCnt <= m_iMarbleCnt)
			m_iMarbleCnt = m_iMaxMarbleCnt;
	}
}

CController_SP* CController_SP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController_SP* pInstance = new CController_SP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CController_SP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController_SP::Clone(CGameObject* pGameObject, void* pArg)
{
	CController_SP* pInstance = new CController_SP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CController_SP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController_SP::Free()
{
	__super::Free();
}
