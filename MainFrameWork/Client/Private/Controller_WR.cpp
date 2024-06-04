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

#include "Esther.h"

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
	Proj_Desc.iDamage = 900;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = true;
	m_Attack_Desces[4] = Proj_Desc;

	return S_OK;
}

void CController_WR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* Skill Tick */
	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->Tick(fTimeDelta);
	}

	Check_Iden_State(fTimeDelta);
}

void CController_WR::LateTick(_float fTimeDelta)
{
	/* Skill Tick */
	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->LateTick(fTimeDelta);
	}


	__super::LateTick(fTimeDelta);
}

void CController_WR::DebugRender()
{
}

_bool CController_WR::Is_EstherSkill()
{
	if (false == static_cast<CPlayer*>(m_pOwner)->Is_PartyLeader())
		return false;

	if (static_cast<CPlayer*>(m_pOwner)->Get_EstherGage() < static_cast<CPlayer*>(m_pOwner)->Get_EstherMaxGage())
		return false;

	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::Z))
	{
		m_iEstherType = 0;
		return true;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::X))
	{
		m_iEstherType = 1;
		return true;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::C))
	{
		m_iEstherType = 2;
		return true;
	}

	return false;
}

void CController_WR::Check_Iden_State(_float fTimeDelta)
{
	if (-10 == m_pOwner->Get_WeaponIndex())
	{
		m_IsIdentity = false;
		m_fIdentityGage = -1.f;
	}

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
			m_fIdentityGage = -1.f;


			CB_UpdateIdentityAuraPivot.clear();

			static_cast<CPlayer*>(m_pOwner)->Delete_Effect(L"Slayer_Rage_Aura");
		}
		else
		{
			//Matrix matPivot;
			//CB_UpdateIdentityAuraPivot(matPivot);
			CEffect* pAuraEffect = static_cast<CPlayer*>(m_pOwner)->Get_Effect(L"Slayer_Rage_Aura");
			if (pAuraEffect)
				pAuraEffect->Update_Pivot(m_pOwner->Get_TransformCom()->Get_WorldMatrix());
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

void CController_WR::Input(_float fTimeDelta)
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

void CController_WR::Skill_CoolTime(_float fTimeDelta)
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

void CController_WR::Esther_Refill(_float fTimeDelta)
{
	if (false == static_cast<CPlayer*>(m_pOwner)->Is_PartyLeader() ||
		m_pOwner->Get_CurrLevel() != LEVELID::LEVEL_VALTANMAIN)
		return;

	m_fEstherAcc += fTimeDelta;
	if (m_fEstherAcc >= m_fEstherFillTime)
	{
		m_fEstherAcc = 0.0f;

		_uint iCurGage = static_cast<CPlayer*>(m_pOwner)->Get_EstherGage();
		_uint iMaxGage = static_cast<CPlayer*>(m_pOwner)->Get_EstherMaxGage();

		iCurGage += m_iEstherFill;
		if (iCurGage >= iMaxGage)
		{
			iCurGage = iMaxGage;
		}
		static_cast<CPlayer*>(m_pOwner)->Set_EstherGage(iCurGage);
		static_cast<CPlayer*>(m_pOwner)->Send_EstherGauge();
	}
}

void CController_WR::EstherSkill(_uint iIndex)
{
	for (auto& pEsther : m_vecEsther)
	{
		if (static_cast<CEsther*>(pEsther)->Get_EstherType() == iIndex)
		{
			if(false == pEsther->Is_Active())
				static_cast<CEsther*>(pEsther)->Leader_Active_Esther();
		}
	}
}

void CController_WR::Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos)
{
	if (true == m_IsDead)
		return;

	__super::Get_HitMessage(iDamge, fForce, vPos);

	m_iCalculateDamaged = (CGameInstance::GetInstance()->Random_Int(_int((_float)m_iDamaged * 0.9f), _int((_float)m_iDamaged * 1.1f))) * 100;
	// 데미지하락 및 밉라이트?
	CPlayer::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
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
		m_eHitType = HIT_TYPE::TYPE_END;
		return;
	}

	if (HIT_TYPE::WEAK != m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperiorArmor() ||
		HIT_TYPE::WEAK != m_eHitType && true == static_cast<CPlayer*>(m_pOwner)->Is_SafeZonePierce())
	{
		static_cast<CPlayer*>(m_pOwner)->Off_SafeZonePierce();

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
	static_cast<CPlayer*>(m_pOwner)->Set_Invincible(false);
	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(false);

	m_bBuffEffect[(_uint)BUFFEFFECT::STIIFIMMUNE] = false;
	m_fBuffDuration[(_uint)BUFFEFFECT::STIIFIMMUNE] = -1;
}

void CController_WR::Get_GrabEndMessage()
{
	__super::Get_GrabEndMessage();
}

void CController_WR::Get_DeadMessage()
{
	__super::Get_DeadMessage();

	m_fIdentityGage = 0.0f;
}

void CController_WR::Get_EshterGageUseMessage()
{
	static_cast<CPlayer*>(m_pOwner)->Set_EstherGage(0);
	static_cast<CPlayer*>(m_pOwner)->Send_EstherGauge();
}

void CController_WR::Get_WR_IdentityMessage()
{
	m_IsIdentity = true;
	m_fIdentityGage = m_fMaxGage;
	m_pOwner->Set_WeaponIndex(10);
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
