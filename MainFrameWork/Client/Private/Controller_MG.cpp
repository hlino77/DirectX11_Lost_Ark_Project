#include "stdafx.h"
#include "..\Public\Controller_MG.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"

CController_MG::CController_MG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CController_MG::CController_MG(const CController_MG& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CController_MG::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CController_MG::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Desc.fAttackTime = 0.5f;

	/* 기본공격 */
	Proj_Desc.fRadius = 0.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.2f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 0.f;
	Proj_Desc.fMoveSpeed = 10.f;
	Proj_Desc.IsMove = true;
	m_Attack_Desces[0] = Proj_Desc;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Desc.fAttackTime = 0.1;

	/* 아덴스킬 */
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.6f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.4f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.6f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = true;
	m_Attack_Desces[4] = Proj_Desc;

	return S_OK;
}

void CController_MG::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
}

void CController_MG::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CController_MG::DebugRender()
{
}

_uint CController_MG::Is_MG_Identity()
{
	if (100 > m_iIdentityGage)
		return 0;

	if (KEY_TAP(KEY::Z))
	{
		return 2;
	}
	else if (KEY_TAP(KEY::X))
	{
		return 1;
	}

	return 0;
}

HRESULT CController_MG::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	__super::Bind_Skill(eKey, pSkill);
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

CPlayer_Skill* CController_MG::Get_PlayerSkill_MG(SKILL_KEY eKey)
{
	return nullptr;
}

void CController_MG::Input(const _float& fTimeDelta)
{

}

void CController_MG::Attack()
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();
	pAttack->InitProjectile(&m_AttackDesc);
}

void CController_MG::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
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

void CController_MG::Hit(CGameObject* pHitObject)
{
	if (HIT_TYPE::TYPE_END == m_eHitType || nullptr == pHitObject)
		return;
}

void CController_MG::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

void CController_MG::Get_MG_IdentityMessage()
{
	if (100 <= m_iIdentityGage)
		m_iBubble = 1;
	if (200 <= m_iIdentityGage)
		m_iBubble = 2;
	if (300 <= m_iIdentityGage)
		m_iBubble = 3;

	m_iIdentityGage = 0;
}

void CController_MG::Increase_IdenGage(_uint iGage)
{
	m_iIdentityGage += iGage;

	if (m_iMaxGage <= m_iIdentityGage)
	{
		m_iIdentityGage = m_iMaxGage;
	}
}

CController_MG* CController_MG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController_MG* pInstance = new CController_MG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CController_MG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController_MG::Clone(CGameObject* pGameObject, void* pArg)
{
	CController_MG* pInstance = new CController_MG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CController_MG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController_MG::Free()
{
	__super::Free();
}
