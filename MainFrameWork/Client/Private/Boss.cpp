#include "stdafx.h"
#include "GameInstance.h"
#include "Boss.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Pool.h"
#include "Zombie_BT_Attack2.h"
#include "Common_BT_Attack1.h"
#include "Common_BT_Chase.h"
#include "Common_BT_Damage1.h"
#include "Common_BT_Damage2.h"
#include "Common_BT_Dead.h"
#include "Common_BT_Idle.h"
#include "Common_BT_BattleIdle.h"
#include "Common_BT_Move.h"
#include "Common_BT_Spawn.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"
#include "Projectile.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CBoss::CBoss(const CBoss& rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss::Initialize_Prototype()
{
	m_strObjectTag = L"Boss";
	m_iObjType = OBJ_TYPE::BOSS;
    return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);

	CNavigationMgr::GetInstance()->Find_FirstCell(this);


	m_tCullingSphere.Radius = 2.0f;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);


    return S_OK;
}

void CBoss::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBoss::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBoss::Render()
{
	return 	__super::Render();
}

void CBoss::Set_SlowMotion(_bool bSlow)
{
	if (bSlow)
	{
		if (m_iSlowMotionCount <= 0)
		{
			m_fAttackMoveSpeed = 0.1f;
			m_fAnimationSpeed = 0.01f;
			m_pRigidBody->Set_Active(false);
		}

		m_iSlowMotionCount++;
	}
	else
	{
		--m_iSlowMotionCount;

		if (m_iSlowMotionCount <= 0)
		{
			m_fAttackMoveSpeed = 8.0f;
			m_fAnimationSpeed = 1.0f;
			m_pRigidBody->Set_Active(true);
			m_iSlowMotionCount = 0;
		}
	}
}

HRESULT CBoss::Render_ShadowDepth()
{
	return __super::Render_ShadowDepth();
}


void CBoss::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			_int iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			Vec3 vPos = {};
			if (static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().bUseProjPos)
			{
				vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			else
			{
				vPos = static_cast<CProjectile*>(pOther->Get_Owner())->Get_AttackOwner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			_float fForce = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion;
			 iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			int iCritical = rand() % 10;

			iDamage = (CGameInstance::GetInstance()->Random_Int(iDamage - 50, iDamage + 50) + 1) * 26789;
			_bool IsCritical = false;
			if (iCritical < 3)
			{
				IsCritical = true;
				iDamage *= 2;
			}
			Send_Collision(iDamage, vPos, (STATUSEFFECT)static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStatusEffect, fForce, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration);
			Show_Damage(iDamage, IsCritical);
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{

		}
	}
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			_int iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			Vec3 vPos = {};
			if (static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().bUseProjPos)
			{
				vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			else
			{
				vPos = static_cast<CProjectile*>(pOther->Get_Owner())->Get_AttackOwner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			_float fForce = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion;


			int iCritical = rand() % 10;

			iDamage = (CGameInstance::GetInstance()->Random_Int(iDamage - 50, iDamage + 50) + 1) * 26789;
			_bool IsCritical = false;
			if (iCritical < 3)
			{
				IsCritical = true;
				iDamage *= 2;
			}
			Send_Collision(iDamage, vPos, (STATUSEFFECT)static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStatusEffect, fForce, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration);
			Show_Damage(iDamage, IsCritical);
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{

		}
	}
	else if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
		}
	}
}

void CBoss::Follow_ServerPos(_float fDistance, _float fLerpSpeed)
{
	Vec3 vCurrPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	Matrix matTargetWorld = m_matTargetWorld;
	Vec3 vServerPos(matTargetWorld.m[3]);

	Vec3 vDistance = vServerPos - vCurrPos;
	if (vDistance.Length() > fDistance)
	{
		vCurrPos = Vec3::Lerp(vCurrPos, vServerPos, fLerpSpeed);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCurrPos);
	}
}

void CBoss::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}

void CBoss::Add_Colider(_int iColIndex , CSphereCollider* pCollider)
{
	m_Coliders.emplace(iColIndex, pCollider);
}


void CBoss::Set_Die()
{
	for (auto& Collider : m_Coliders)
		Collider.second->SetActive(false);

	m_bDie = true;
}

HRESULT CBoss::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_State */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_SphereColider"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS, pCollider);
	}

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)
			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, pCollider);
	}



	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

HRESULT CBoss::Ready_HP_UI(_uint iTextureIndex)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);


	return S_OK;
}

void CBoss::CullingObject()
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
		
}

HRESULT CBoss::Ready_BehaviourTree()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"), TEXT("Com_Behavior"), (CComponent**)&m_pBehaviorTree)))
		return E_FAIL;

	return S_OK;
}

void CBoss::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame)
{
	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame);
}

CGameObject* CBoss::Clone(void* pArg)
{
	CBoss* pInstance = new CBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
