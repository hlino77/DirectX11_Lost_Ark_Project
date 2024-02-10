#include "stdafx.h"
#include "Skill_Crystal.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include <ColliderOBB.h>
#include "Skill.h"


CSkill_Crystal::CSkill_Crystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CSkill(pDevice, pContext)
{
}

CSkill_Crystal::CSkill_Crystal(const CSkill& rhs)
	: CSkill(rhs)
{
}

HRESULT CSkill_Crystal::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Crystal::Initialize(void* pArg)
{
	m_iMaxHp = 1;
	m_iHp = m_iMaxHp;
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_szModelName = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_pSkillOwner = Desc->pOwner;
	m_iCurrLevel = m_pSkillOwner->Get_CurrLevel();
	if (FAILED(Ready_Components()))
		return E_FAIL;
	CNavigationMgr::GetInstance()->Find_FirstCell(m_iCurrLevel, this);
	m_fCellHeight = CNavigationMgr::GetInstance()->Get_Height_OnCell(m_iCurrLevel, this);
	m_fMoveSpeed = 1.5f;
	m_IsSetuponCell = false;
	m_bExplosion = false;
	m_fExplosionDelay = 1.5f;
	m_bRender = false;
	return S_OK;
}

void CSkill_Crystal::Tick(_float fTimeDelta)
{
	if (m_IsSetuponCell)
	{
		CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
	}
	else
	{
		m_pTransformCom->Go_Up(fTimeDelta);
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y >= m_fCellHeight-1.f)
			m_bRender = true;
		if (m_pTransformCom->Get_State(CTransform::STATE_POSITION).y >= m_fCellHeight)
			m_IsSetuponCell = true;
	}
	if (m_bExplosion)
	{
		m_fExplosionDelay -= fTimeDelta;
		if (m_fExplosionDelay <= 0.f)
		{
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->SetActive(false);
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->SetActive(false);
			m_bRender = false;
			if (m_strSoundTag.empty() == false && !m_bSoundOn)
			{
				CGameInstance::GetInstance()->PlaySoundFile(m_strSoundTag, CHANNEL_EFFECT);
				m_bSoundOn = true;
			}
		}
		if (m_fExplosionDelay < -0.2f)
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
		if (m_fExplosionDelay < -0.5f&& !m_bDead)
			Set_Die();
	}
}

void CSkill_Crystal::LateTick(_float fTimeDelta)
{
	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}
	Set_Colliders(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;
	if (m_fExplosionDelay > 0.f)
	{
		if (m_bRender)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
			m_pRendererCom->Add_DebugObject(this);
		}
	}
	m_pRendererCom->Add_DebugObject(this);
}

HRESULT CSkill_Crystal::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom)))
		return E_FAIL;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	Color vValtanBloom = Color::Lerp(Color(1.f, 0.0f, 0.0f, 1.f), Color(0.4f, 1.6f, 1.3f, 1.f), m_fExplosionDelay);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vValtanBloom, sizeof(Color))))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Crystal::Set_SlowMotion(_bool bSlow)
{
}

void CSkill_Crystal::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS)
		{
			if (dynamic_cast<CSkill*>(pOther->Get_Owner())->Is_Destructive())
			{
				m_iHp -= 1;

				if (m_iHp < 1)
				{
					Set_RimLight(0.05f);
					m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->SetActive(false);
					m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->SetActive(false);
					m_bExplosion = true;

					m_fExplosionDelay = 1.f;
				}
			}
		}
	}
}

void CSkill_Crystal::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Crystal::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Crystal::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	WRITE_LOCK
	m_iHp -= iDamage;
	Set_RimLight(0.05f);
	
	if (m_iHp < 1)
	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->SetActive(false);
		m_bExplosion = true;
		if(iGroggy == 1)
			m_fExplosionDelay = 0.f;
		else
			m_fExplosionDelay = 1.f;
	}
	
}
void CSkill_Crystal::Set_Explosion(_bool bExplosion)
{
	if (!m_bExplosion && m_iHp >= 1)
	{
		m_bExplosion = bExplosion;
		Set_RimLight(0.05f);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->SetActive(false);
		m_bExplosion = true;
	}
}

void CSkill_Crystal::Explosion()
{
}

HRESULT CSkill_Crystal::Ready_Coliders()
{
	return S_OK;
}

HRESULT CSkill_Crystal::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 20.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	///* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iCurrLevel, TEXT("Prototype_Component_Model_Skill_Crystal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	{

		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderBody"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->Set_Radius(0.6f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->Set_Offset(Vec3(0.0f, 0.5f, 0.0f));
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SAFEZONE;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSafeZone"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = false;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
				COBBCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderSafeZoneChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SAFEZONE, pCollider);
		}
	}

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->Set_Radius(10.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->Set_Offset(Vec3(0.0f, 0.5f, -10.30f));

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SAFEZONE]->Get_Child());
	pChildCollider->Set_Scale(Vec3(0.6f, 0.5f, 10.f));
	pChildCollider->Set_Offset(Vec3(0.0f, 0.5f, -10.30f));
	pChildCollider->SetActive(false);


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(4.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.5f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}

	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.65f;
	vScale.y = 0.65f;
	vScale.z = 0.65f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;

}


CSkill_Crystal* CSkill_Crystal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill_Crystal* pInstance = new CSkill_Crystal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Crystal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Crystal::Clone(void* pArg)
{
	CSkill_Crystal* pInstance = new CSkill_Crystal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CSkill_Crystal");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CSkill_Crystal::Free()
{
	__super::Free();
	for (auto& Collider : m_Coliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider.second);
}

