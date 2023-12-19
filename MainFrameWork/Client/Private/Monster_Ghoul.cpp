#include "stdafx.h"
#include "GameInstance.h"
#include "Monster_Ghoul.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "CollisionManager.h"
#include "Pool.h"
#include "Zombie_BT_Attack2.h"
#include "Common_BT_Attack1.h"
#include "Common_BT_Chase.h"
#include "Common_BT_DamageLeft.h"
#include "Common_BT_DamageRight.h"
#include "Common_BT_Dead.h"
#include "Common_BT_Idle.h"
#include "Common_BT_BattleIdle.h"
#include "Common_BT_Move.h"
#include "Common_BT_Spawn.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"
#include "BindShaderDesc.h"
#include <Ghoul_BT_Attack_1.h>
#include <Ghoul_BT_Attack_2.h>
#include <Ghoul_BT_Attack_3.h>



CMonster_Ghoul::CMonster_Ghoul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CMonster_Ghoul::CMonster_Ghoul(const CMonster_Ghoul& rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Ghoul::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster_Ghoul::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;

	

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);

	m_pRigidBody->SetMass(2.0f);
	m_iHp = 10;

	m_vecAttackRanges.push_back(1.5f);
	m_vecAttackRanges.push_back(1.5f);

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

    return S_OK;
}

void CMonster_Ghoul::Tick(_float fTimeDelta)
{

	CNavigationMgr::GetInstance()->SetUp_OnCell(this);
	if (!m_bDie)
		m_pBehaviorTree->Tick_Action(m_strAction, fTimeDelta);

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	m_PlayAnimation.get();
	if(Get_Target_Distance()> 0.5f)
		Set_to_RootPosition(fTimeDelta);
}

void CMonster_Ghoul::LateTick(_float fTimeDelta)
{

	if (nullptr == m_pRendererCom)
		return;

	CullingObject();

	Set_Colliders(fTimeDelta);
}

HRESULT CMonster_Ghoul::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

    return S_OK;
}

HRESULT CMonster_Ghoul::Render_ShadowDepth()
{
	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;*/

			/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
				return E_FAIL;*/


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
			return S_OK;
	}

	return S_OK;
}

void CMonster_Ghoul::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if(pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		cout << "���� Body : �÷��̾� Attack -> ENTER" << endl;

	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		cout << "���� Body : �÷��̾� Body -> ENTER" << endl;
}

void CMonster_Ghoul::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Ghoul::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		cout << "���� Body : �÷��̾� Attack ->EXIT" << endl;

	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		cout << "���� Body : �÷��̾� Body ->EXIT" << endl;
}

void CMonster_Ghoul::Set_SlowMotion(_bool bSlow)
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

HRESULT CMonster_Ghoul::Ready_Components()
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
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), TEXT("Prototype_Component_Model_Monster_Ghoul"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_SphereColider"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pCollider);
	}

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)
			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, pCollider);
	}


	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}


	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

HRESULT CMonster_Ghoul::Ready_BehaviourTree()
{

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"), TEXT("Com_Behavior"), (CComponent**)&m_pBehaviorTree)))
		return E_FAIL;

	CBT_Action::ACTION_DESC ActionDesc = {};
	ActionDesc.pBehaviorTree = m_pBehaviorTree;
	ActionDesc.pGameObject = this;
	ActionDesc.vecAnimations.clear();
	CBT_Action::ANIMATION_DESC AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("dead_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 1.5f;
	ActionDesc.strActionName = L"Action_Dead";
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pDead = CCommon_BT_Dead::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("dmg_idle_2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Left";
	CBT_Action* pDamageLeft = CCommon_BT_DamageLeft::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("dmg_idle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Right";
	CBT_Action* pDamageRight = CCommon_BT_DamageRight::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("respawn_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Respawn";
	CBT_Action* pSpawn = CCommon_BT_Spawn::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_1_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack1";
	CBT_Action* pAttack1 = CCommon_BT_Attack1::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CCommon_BT_BattleIdle::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("run_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Chase";
	CBT_Action* pChase = CCommon_BT_Chase::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Idle_0";
	CBT_Action* pIdle_0 = CCommon_BT_Idle::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_normal_1_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Idle_1";
	CBT_Action* pIdle_1 = CCommon_BT_Idle::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("walk_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Move";
	CBT_Action* pMove = CCommon_BT_Move::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_2_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("att_battle_2_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("att_battle_2_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack2";
	ActionDesc.iLoopAnimationIndex = 1;
	ActionDesc.fMaxLoopTime = 1.3f;
	CBT_Action* pAttack2 = CGhoul_BT_Attack_1::Create(&ActionDesc);
	ActionDesc.iLoopAnimationIndex = -1;
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_3_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack3";
	CBT_Action* pAttack3 = CGhoul_BT_Attack_2::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_4");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack4";
	CBT_Action* pAttack4 = CGhoul_BT_Attack_3::Create(&ActionDesc);
	m_pBehaviorTree->Init_PreviousAction(L"Action_Respawn");

	return S_OK;
}



HRESULT CMonster_Ghoul::Ready_Coliders()
{
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->Set_Radius(0.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->Set_Offset(Vec3(0.0f, 0.5f, 0.0f));

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER]->Set_Radius(0.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER]->Set_Offset(Vec3(0.0f, 0.7f, 1.0f));

	return S_OK;
}

void CMonster_Ghoul::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if(Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

CMonster_Ghoul* CMonster_Ghoul::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Ghoul* pInstance = new CMonster_Ghoul(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_Ghoul");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Ghoul::Clone(void* pArg)
{
	CMonster_Ghoul* pInstance = new CMonster_Ghoul(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster_Ghoul");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Ghoul::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBehaviorTree);
}