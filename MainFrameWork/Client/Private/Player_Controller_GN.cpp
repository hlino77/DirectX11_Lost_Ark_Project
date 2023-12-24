#include "stdafx.h"
#include "..\Public\Player_Controller_GN.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"

CPlayer_Controller_GN::CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CPlayer_Controller_GN::CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CPlayer_Controller_GN::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	PROJECTILE_DESC Proj_Desc;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;

	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.7f);
	Proj_Desc.vChildScale = Vec3(0.35f, 0.6f, 1.5f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.7f);
	m_HandAttackDesc = Proj_Desc;

	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.7f);
	Proj_Desc.vChildScale = Vec3(0.6f, 0.6f, 1.0f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.2f);
	m_ShotAttackDesc = Proj_Desc;

	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.4f);
	Proj_Desc.vChildScale = Vec3(0.3f, 0.6f, 2.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 2.4f);
	m_LongAttackDesc = Proj_Desc;

	return S_OK;
}

void CPlayer_Controller_GN::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPlayer_Controller_GN::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CPlayer_Controller_GN::DebugRender()
{
}

_bool CPlayer_Controller_GN::Is_Idle()
{
	if (false == __super::Is_Idle())
		return false;

	if (0 != Is_GN_Identity())
		return false;

	return true;
}

_uint CPlayer_Controller_GN::Is_GN_Identity()
{
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

void CPlayer_Controller_GN::Skill(GN_IDENTITY eIndex, SKILL_KEY eKey)
{
	__super::Skill(eKey);

	m_fGN_CoolTime[eIndex][eKey] = m_pSkills[eKey]->Get_Skill_CoolTime();
}

HRESULT CPlayer_Controller_GN::Bind_HandSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey)
		return S_OK;

	m_pHandSkills[eKey] = pSkill;
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Bind_ShotSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey || SKILL_KEY::A == eKey || SKILL_KEY::S == eKey || SKILL_KEY::D == eKey || SKILL_KEY::F == eKey)
		return S_OK;

	m_pShotSkills[eKey] = pSkill;
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Bind_LongSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey || SKILL_KEY::Q == eKey || SKILL_KEY::W == eKey || SKILL_KEY::E == eKey || SKILL_KEY::R == eKey)
		return S_OK;

	m_pLongSkills[eKey] = pSkill;
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

CPlayer_Skill* CPlayer_Controller_GN::Get_PlayerSkill_Weapon(GN_IDENTITY eStance, SKILL_KEY eKey)
{
	switch (eStance)
	{
	case GN_IDENTITY::HAND:
		if(nullptr != m_pHandSkills[eKey])
			return m_pHandSkills[eKey];
		break;

	case GN_IDENTITY::SHOT:
		if (nullptr != m_pShotSkills[eKey])
			return m_pShotSkills[eKey];
		break;

	case GN_IDENTITY::LONG:
		if (nullptr != m_pLongSkills[eKey])
			return m_pLongSkills[eKey];
		break;
	}
	return nullptr;
}

void CPlayer_Controller_GN::Input(const _float& fTimeDelta)
{

}

void CPlayer_Controller_GN::Attack()
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();
	pAttack->InitProjectile(&m_AttackDesc);
}

void CPlayer_Controller_GN::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
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

void CPlayer_Controller_GN::Hit(CGameObject* pHitObject)
{
	if (HIT_TYPE::TYPE_END == m_eHitType || nullptr == pHitObject)
		return;
}

void CPlayer_Controller_GN::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);

	for (size_t i = 0; i < GN_IDENTITY::_END; i++)
	{
		for (size_t j = 0; j < SKILL_KEY::_END; j++)
		{
			if (-1.f == m_fGN_CoolTime[i][j]) continue;

			m_fGN_CoolDownAcc[i][j] += fTimeDelta;

			if (m_fGN_CoolTime[i][j] <= m_fGN_CoolDownAcc[i][j])
			{
				m_fGN_CoolDownAcc[i][j] = 0.f;
				m_fGN_CoolTime[i][j] = -1.f;
			}
		}
	}
}

void CPlayer_Controller_GN::GN_Identity(GN_IDENTITY eIndex)
{
	m_ePreIdentity = m_eIdentity;
	m_eIdentity = eIndex;
	Change_Skill_Iden(eIndex);
	Change_Iden_Collider(eIndex);
}

void CPlayer_Controller_GN::Change_Iden_Collider(GN_IDENTITY eIndex)
{
	switch (eIndex)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_AttackDesc = m_HandAttackDesc;
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_AttackDesc = m_ShotAttackDesc;
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_AttackDesc = m_LongAttackDesc;
		break;
	}
}

void CPlayer_Controller_GN::Change_Skill_Iden(GN_IDENTITY eIndex)
{
	if (GN_IDENTITY::HAND == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pHandSkills[i];
		
			m_fCoolDownAcc[i] = m_fGN_CoolDownAcc[GN_IDENTITY::HAND][i];
			m_fCoolTime[i] = m_fGN_CoolTime[GN_IDENTITY::HAND][i];
		}
	}
	else if (GN_IDENTITY::LONG == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pLongSkills[i];

			m_fCoolDownAcc[i] = m_fGN_CoolDownAcc[GN_IDENTITY::LONG][i];
			m_fCoolTime[i] = m_fGN_CoolTime[GN_IDENTITY::LONG][i];
		}
	}
	else if (GN_IDENTITY::SHOT == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pShotSkills[i];

			m_fCoolDownAcc[i] = m_fGN_CoolDownAcc[GN_IDENTITY::SHOT][i];
			m_fCoolTime[i] = m_fGN_CoolTime[GN_IDENTITY::SHOT][i];
		}
	}
}

CPlayer_Controller_GN* CPlayer_Controller_GN::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPlayer_Controller_GN::Clone(CGameObject* pGameObject, void* pArg)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Controller_GN::Free()
{
	__super::Free();
}
