#include "stdafx.h"
#include "..\Public\Controller_WDR.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Destroyer.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"
#include "Esther.h"

CController_WDR::CController_WDR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CController_WDR::CController_WDR(const CController_WDR& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CController_WDR::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CController_WDR::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;

	/* 기본공격 */
	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[0] = Proj_Desc;

	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 10.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[1] = Proj_Desc;

	/* 아덴공격 */
	Proj_Desc.fRadius = 1.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 300;
	Proj_Desc.fRepulsion = 5.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[2] = Proj_Desc;

	Proj_Desc.fRadius = 1.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 300;
	Proj_Desc.fRepulsion = 0.f;
	m_Attack_Desces[3] = Proj_Desc;

	/* 아덴스킬 */
	Proj_Desc.fAttackTime = 0.2f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 800;
	Proj_Desc.fRepulsion = -6.f;
	Proj_Desc.bUseProjPos = true;
	m_Attack_Desces[4] = Proj_Desc;

	return S_OK;
}

void CController_WDR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->Tick(fTimeDelta);
	}

	Check_Iden_State(fTimeDelta);
}

void CController_WDR::LateTick(_float fTimeDelta)
{
	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->LateTick(fTimeDelta);
	}

	__super::LateTick(fTimeDelta);
}

void CController_WDR::DebugRender()
{
}

_bool CController_WDR::Is_EstherSkill()
{
	if (false == static_cast<CPlayer*>(m_pOwner)->Is_PartyLeader())
		return false;

	if (m_iCurEstherGage < m_iMaxEstherGage)
		return false;

	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::Z))
	{
		m_iCurEstherGage = 0;
		m_iEstherType = 0;
		return true;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::X))
	{
		m_iCurEstherGage = 0;
		m_iEstherType = 1;
		return true;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::C))
	{
		m_iCurEstherGage = 0;
		m_iEstherType = 2;
		return true;
	}

	return false;
}

void CController_WDR::Check_Iden_State(_float fTimeDelta)
{
	if (true == m_IsIdentity)
	{
		m_fTimdeAcc += fTimeDelta;
		if (1.f <= m_fTimdeAcc)
		{
			m_fTimdeAcc = 0.f;
			m_fIdentityGage--;
		}

		if (0 >= m_fIdentityGage)
		{
			m_fTimdeAcc = 0.f;
			m_IsIdentity = false;
			m_fIdentityGage = 30.f;
			static_cast<CPlayer_Destroyer*>(m_pOwner)->Set_SuperArmorState(false);
		}
	}
}

_bool CController_WDR::Is_Identity()
{
	if (false == m_bKeyActive || false == m_bCtrlActive)
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

HRESULT CController_WDR::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	__super::Bind_Skill(eKey, pSkill);
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

void CController_WDR::Input(const _float& fTimeDelta)
{

}

void CController_WDR::Attack(Vec3 vPos)
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

void CController_WDR::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
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

void CController_WDR::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

void CController_WDR::Esther_Refill(_float fTimeDelta)
{
	if (false == static_cast<CPlayer*>(m_pOwner)->Is_PartyLeader() ||
		m_pOwner->Get_CurrLevel() != LEVELID::LEVEL_VALTANMAIN)
		return;

	m_fEstherAcc += fTimeDelta;
	if (m_fEstherAcc >= m_fEstherFillTime)
	{
		m_fEstherAcc = 0.0f;

		m_iCurEstherGage += m_iEstherFill;
		if (m_iCurEstherGage >= m_iMaxEstherGage)
		{
			m_iCurEstherGage = m_iMaxEstherGage;
		}
	}
}

void CController_WDR::EstherSkill(_uint iIndex)
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

void CController_WDR::Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos)
{
	if (true == m_IsDead)
		return;

	__super::Get_HitMessage(iDamge, fForce, vPos);

	m_iCalculateDamaged = (CGameInstance::GetInstance()->Random_Int(_int((_float)m_iDamaged * 0.8f), _int((_float)m_iDamaged * 1.f))) * 100;
	// 데미지하락 및 밉라이트?
	CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
	tPcStat.iCurHp -= m_iCalculateDamaged;
	if (m_pOwner->Is_Control())
		static_cast<CPlayer*>(m_pOwner)->Show_Damage(m_iCalculateDamaged, false);
	if (0 >= tPcStat.iCurHp)
	{
		tPcStat.iCurHp = 0;
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Dead_Start"));
		static_cast<CPlayer*>(m_pOwner)->Send_Hp();
		return;
	}
	else
	{
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		static_cast<CPlayer*>(m_pOwner)->Send_Hp();
	}
	

	if (true == m_bBuffEffect[(_uint)BUFFEFFECT::STIIFIMMUNE])
	{
		return;
	}

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

void CController_WDR::Get_GrabMessage(CGameObject* pGrabber)
{
	__super::Get_GrabMessage(pGrabber);

	static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Grabbed"));

	static_cast<CPlayer*>(m_pOwner)->Set_SuperArmorState(false);
	static_cast<CPlayer*>(m_pOwner)->Set_Invincible(false);
	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(false);

	m_bBuffEffect[(_uint)BUFFEFFECT::STIIFIMMUNE] = false;
	m_fBuffDuration[(_uint)BUFFEFFECT::STIIFIMMUNE] = -1;

	m_eHitType = HIT_TYPE::TYPE_END;

}

void CController_WDR::Get_GrabEndMessage()
{
	__super::Get_GrabEndMessage();
}

void CController_WDR::Get_DeadMessage()
{
	__super::Get_DeadMessage();

	m_fIdentityGage = 0.0f;
	m_iMarbleCnt = 0;
	m_IsIdentity = false;
}

void CController_WDR::Get_WDR_IdentityMessage()
{
	if (m_fMaxGage <= m_fIdentityGage)
	{
		m_IsIdentity = true;
		m_fIdentityGage = m_fMaxGage;
		static_cast<CPlayer_Destroyer*>(m_pOwner)->Set_SuperArmorState(true);
	}
}

void CController_WDR::Get_UseMarbleMessage()
{
	if (0 == m_iMarbleCnt)
		return;

	if (1 == m_iMarbleCnt)
	{
		m_fIdentityGage += 2.f;
		m_iMarbleCnt = 0;
	}
	else if (2 == m_iMarbleCnt)
	{
		m_fIdentityGage += 4.f;
		m_iMarbleCnt = 0;
	}
	else if (3 == m_iMarbleCnt)
	{
		m_fIdentityGage += 8.f;
		m_iMarbleCnt = 0;
	}
}

void CController_WDR::Get_HitMarbleMessage()
{
	if (false == m_IsAddMarble)
		return;

	m_iMarbleCnt += m_iReserveMarbleCnt;
	if (3 <= m_iMarbleCnt)
		m_iMarbleCnt = 3;

	m_IsAddMarble = false;
	m_iReserveMarbleCnt = 0;
}

CController_WDR* CController_WDR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController_WDR* pInstance = new CController_WDR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CController_WDR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController_WDR::Clone(CGameObject* pGameObject, void* pArg)
{
	CController_WDR* pInstance = new CController_WDR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CController_WDR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController_WDR::Free()
{
	__super::Free();
}
