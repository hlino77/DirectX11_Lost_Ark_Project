#include "stdafx.h"
#include "..\Public\Controller_WR.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"
#include "Player_Slayer.h"
#include "Effect.h"

CController_WR::CController_WR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CController_WR::CController_WR(const CController_WR& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CController_WR::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CController_WR::Initialize(void* pArg)
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
	Proj_Desc.vChildScale = Vec3(0.8f, 0.6f, 1.f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.2f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 5.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[0] = Proj_Desc;

	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.2f);
	Proj_Desc.vChildScale = Vec3(1.2f, 0.6f, 1.f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.2f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 5.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[1] = Proj_Desc;

	/* 아덴공격 */
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 6.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[2] = Proj_Desc;

	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[3] = Proj_Desc;

	/* 아덴스킬 */
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.6f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.4f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.6f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = true;
	m_Attack_Desces[4] = Proj_Desc;

	return S_OK;
}

void CController_WR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Check_Iden_State(fTimeDelta);
}

void CController_WR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CController_WR::DebugRender()
{
}

void CController_WR::Check_Iden_State(_float fTimeDelta)
{
	if (true == m_IsIdentity)
	{
		m_fTimeAcc += fTimeDelta;
		if (1.f <= m_fTimeAcc)
		{
			m_fTimeAcc = 0.f;
			m_fIdentityGage--;
		}

		if (0.f >= m_fIdentityGage)
		{
			m_fTimeAcc = 0.f;
			m_IsIdentity = false;
			m_fIdentityGage = 30.f;

			static_cast<CPlayer_Slayer*>(m_pOwner)->Get_Effect(L"Slayer_Rage_Aura")->EffectEnd();
		}
		else
		{
			Matrix matPivot;
			CB_UpdateIdentityAuraPivot(matPivot);
			CEffect* pAuraEffect = static_cast<CPlayer_Slayer*>(m_pOwner)->Get_Effect(L"Slayer_Rage_Aura");
			if (pAuraEffect)
				pAuraEffect->Update_Pivot(matPivot);
		}
	}
	else if (false == m_IsIdentity && -1.f == m_fIdentityGage)
	{
		m_fIdenTimeAcc += fTimeDelta;
		if (m_fIdenCoolTime <= m_fIdenTimeAcc)
		{
			m_fIdenTimeAcc = 0.f;
			m_fIdentityGage = 0.f;
		}
	}
}

_bool CController_WR::Is_Identity()
{
	if (false == m_bKeyActive)
		return false;

	if (KEY_HOLD(KEY::Z) || KEY_TAP(KEY::Z))
	{
		if (true == m_IsIdentity)
		{
			if (-1 == m_fCoolTime[SKILL_KEY::Z])
				return true;
			else
				return false;
		}

		if (m_fMaxGage <= m_fIdentityGage)
			return true;
		else
			return false;
	}
	
	return false;
}

HRESULT CController_WR::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	__super::Bind_Skill(eKey, pSkill);
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

CPlayer_Skill* CController_WR::Get_PlayerSkill_WR(SKILL_KEY eKey)
{
	return nullptr;
}

void CController_WR::Input(const _float& fTimeDelta)
{

}

void CController_WR::Attack(Vec3 vPos)
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

void CController_WR::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
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

void CController_WR::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

void CController_WR::UpdateIdentityAuraPivot()
{
	if (0.f >= m_fIdentityGage)
	{
		m_fTimeAcc = 0.f;
		m_IsIdentity = false;
		m_fIdentityGage = 30.f;

		static_cast<CPlayer_Slayer*>(m_pOwner)->Get_Effect(L"Slayer_Rage_Aura")->EffectEnd();
		// 여기서 End_Effect();
	}
	else
	{
		Matrix matPivot;
		CB_UpdateIdentityAuraPivot(matPivot);
		static_cast<CPlayer_Slayer*>(m_pOwner)->Get_Effect(L"Slayer_Rage_Aura")->Update_Pivot(matPivot);
	}
}

void CController_WR::Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos)
{
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

void CController_WR::Get_GrabMessage(CGameObject* pGrabber)
{
	__super::Get_GrabMessage(pGrabber);

	static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Grabbed"));

	static_cast<CPlayer*>(m_pOwner)->Set_SuperArmorState(false);
	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(false);
}

void CController_WR::Get_GrabEndMessage()
{
	__super::Get_GrabEndMessage();
}

void CController_WR::Get_DeadMessage()
{
	__super::Get_DeadMessage();

	m_fIdentityGage = -1.0f;
}

void CController_WR::Get_WR_IdentityMessage()
{
	if (m_fMaxGage <= m_fIdentityGage)
	{
		m_IsIdentity = true;
		m_fIdentityGage = m_fMaxGage;
	}
}

void CController_WR::Increase_IdenGage(_float iGage)
{
	if (true == m_IsIdentity)
		return;

	m_fIdentityGage += iGage;

	if (m_fMaxGage <= m_fIdentityGage)
	{
		m_fIdentityGage = m_fMaxGage;
	}
}

CController_WR* CController_WR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController_WR* pInstance = new CController_WR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CController_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController_WR::Clone(CGameObject* pGameObject, void* pArg)
{
	CController_WR* pInstance = new CController_WR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CController_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController_WR::Free()
{
	__super::Free();
}
