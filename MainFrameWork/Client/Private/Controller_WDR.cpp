#include "stdafx.h"
#include "..\Public\Controller_WDR.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Destroyer.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"

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
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[0] = Proj_Desc;

	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 10.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[1] = Proj_Desc;

	/* 아덴공격 */
	Proj_Desc.fRadius = 1.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 5.f;
	Proj_Desc.bUseFactor = false;
	m_Attack_Desces[2] = Proj_Desc;

	Proj_Desc.fRadius = 1.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 0.f;
	m_Attack_Desces[3] = Proj_Desc;

	/* 아덴스킬 */
	Proj_Desc.fAttackTime = 0.2f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 250.f;
	Proj_Desc.fRepulsion = -6.f;
	Proj_Desc.bUseProjPos = true;
	m_Attack_Desces[4] = Proj_Desc;

	return S_OK;
}

void CController_WDR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Check_Iden_State(fTimeDelta);
}

void CController_WDR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CController_WDR::DebugRender()
{
}

void CController_WDR::Check_Iden_State(_float fTimeDelta)
{
	if (true == m_IsIdentity)
	{
		m_fTimdeAcc += fTimeDelta;
		if (1.f <= m_fTimdeAcc)
		{
			m_fTimdeAcc = 0.f;
			m_iIdentityGage--;
		}

		if (0 >= m_iIdentityGage)
		{
			m_fTimdeAcc = 0.f;
			m_IsIdentity = false;
			m_iIdentityGage = 0;
			static_cast<CPlayer_Destroyer*>(m_pOwner)->Set_SuperiorArmorState(false);
		}
	}
}

_bool CController_WDR::Is_Identity()
{
	if (KEY_HOLD(KEY::Z) || KEY_TAP(KEY::Z))
	{
		if (true == m_IsIdentity)
		{
			if (-1 == m_fCoolTime[SKILL_KEY::Z])
				return true;
			else
				return false;
		}

		if (m_iMaxGage <= m_iIdentityGage)
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

void CController_WDR::Attack()
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();
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
	pSkill->InitProjectile(&m_pSkills[eKey]->Get_Skill_Proj_Desc());
}

void CController_WDR::Hit(CGameObject* pHitObject)
{
	if (HIT_TYPE::TYPE_END == m_eHitType || nullptr == pHitObject)
		return;
}

void CController_WDR::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

void CController_WDR::Get_WDR_IdentityMessage()
{
	if (m_iMaxGage <= m_iIdentityGage)
	{
		m_IsIdentity = true;
		m_iIdentityGage = m_iMaxGage;
		static_cast<CPlayer_Destroyer*>(m_pOwner)->Set_SuperiorArmorState(true);
	}
}

void CController_WDR::Get_UseMarbleMessage()
{
	if (0 == m_iMarbleCnt)
		return;

	if (1 == m_iMarbleCnt)
	{
		m_iIdentityGage += 5;
		m_iMarbleCnt = 0;
	}
	else if (2 == m_iMarbleCnt)
	{
		m_iIdentityGage += 10;
		m_iMarbleCnt = 0;
	}
	else if (3 == m_iMarbleCnt)
	{
		m_iIdentityGage += 15;
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
