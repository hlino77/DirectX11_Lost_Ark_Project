#include "stdafx.h"
#include "GameInstance.h"
#include "Monster_Zombie.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Pool.h"
#include "Zombie_BT_Attack1.h"
#include "Zombie_BT_Attack2.h"
#include "Zombie_BT_Chase.h"
#include "Zombie_BT_DamageLeft.h"
#include "Zombie_BT_DamageRight.h"
#include "Zombie_BT_Dead.h"
#include "Zombie_BT_Idle.h"
#include "Zombie_BT_BattleIdle.h"
#include "Zombie_BT_IF_Dead.h"
#include "Zombie_BT_IF_Hit.h"
#include "Zombie_BT_IF_Hit_Left.h"
#include "Zombie_BT_IF_Near.h"
#include "Zombie_BT_IF_Spawn.h"
#include "Zombie_BT_Move.h"
#include "Zombie_BT_Spawn.h"
#include "Zombie_BT_WHILE_Within_Range.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"

CMonster_Zombie::CMonster_Zombie(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CMonster_Zombie::CMonster_Zombie(const CMonster_Zombie& rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Zombie::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster_Zombie::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_iLayer = Desc->iLayer;
	

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;
	

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);

	m_pRigidBody->SetMass(2.0f);
	m_iHp = 10;




    return S_OK;
}

void CMonster_Zombie::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::A)|| KEY_TAP(KEY::S)|| KEY_TAP(KEY::K))
		m_IsHit = true;
	if (KEY_TAP(KEY::A))
		m_IsLeft = false;
	if (KEY_TAP(KEY::S))
		m_IsLeft = true;
	if (KEY_TAP(KEY::K))
		m_iHp = 0;
	if (KEY_TAP(KEY::R))
	{
		m_fAnimationSpeed = 1.f;
		m_bDie = false;
		m_iHp = 10;
	}
	m_fScanCoolDown += fTimeDelta;
	if (m_fScanCoolDown > 0.5f)
	{
		m_fScanCoolDown = 0.f;
		Find_NearTarget();
	}
	CNavigationMgr::GetInstance()->SetUp_OnCell(this);
	if (!m_bDie)
		m_pBehaviorTree->Tick(fTimeDelta);
	m_pRigidBody->Tick(fTimeDelta);
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos.y = 0.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

}

void CMonster_Zombie::LateTick(_float fTimeDelta)
{
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);

	if (nullptr == m_pRendererCom)
		return;

	CullingObject();
}

HRESULT CMonster_Zombie::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	m_PlayAnimation.get();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
				return S_OK;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
				return S_OK;
		}
	}

	Safe_Release(pGameInstance);



    return S_OK;
}

HRESULT CMonster_Zombie::Render_ShadowDepth()
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;

	Matrix matLightVeiw = pGameInstance->Get_DirectionLightMatrix();

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &matLightVeiw)))
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

	Safe_Release(pGameInstance);


	return S_OK;
}

void CMonster_Zombie::Set_SlowMotion(_bool bSlow)
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



HRESULT CMonster_Zombie::Ready_Components()
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
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), TEXT("Prototype_Component_Model_Monster_Zombie"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_SphereColider"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY, pCollider);
	}

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)
			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK, pCollider);
	}



	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

HRESULT CMonster_Zombie::Ready_BehaviourTree()
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
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pDead = CZombie_BT_Dead::Create(&ActionDesc);

	CBT_Decorator::DECORATOR_DESC DecoratorDesc = {};
	DecoratorDesc.pBehaviorTree = m_pBehaviorTree;
	DecoratorDesc.pGameObject = this;
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfDead = CZombie_BT_IF_Dead::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfDead->AddChild(pDead)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("dmg_idle_2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pDamageLeft = CZombie_BT_DamageLeft::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("dmg_idle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pDamageRight = CZombie_BT_DamageRight::Create(&ActionDesc);

	CBT_Decorator* pIfHitLeft = CZombie_BT_IF_Hit_Left::Create(&DecoratorDesc);//왼쪽을 맞았는가
	if (FAILED(pIfHitLeft->AddChild(pDamageLeft)))
		return E_FAIL;

	CBT_Composite::COMPOSITE_DESC CompositeDesc = {};
	CompositeDesc.pGameObject = this;
	CompositeDesc.pBehaviorTree = m_pBehaviorTree;
	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Hit = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Hit->AddChild(pIfDead))) return E_FAIL;
	//if (FAILED(pSelector_Hit->AddChild(pIfMaz))) return E_FAIL; 상태이상 보류중
	if (FAILED(pSelector_Hit->AddChild(pIfHitLeft))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pDamageRight))) return E_FAIL;

	CBT_Decorator* pIfHit = CZombie_BT_IF_Hit::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit))) 
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("respawn_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pSpawn = CZombie_BT_Spawn::Create(&ActionDesc);

	CBT_Decorator* pIfSpawn = CZombie_BT_IF_Spawn::Create(&DecoratorDesc);//스폰 직후인가?
	if (FAILED(pIfSpawn->AddChild(pSpawn))) 
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_1_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pAttack1 = CZombie_BT_Attack1::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pBattleIdle = CZombie_BT_BattleIdle::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_2_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pAttack2 = CZombie_BT_Attack2::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceNear = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSequenceNear->AddChild(pAttack1))) 
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pBattleIdle)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack2)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pBattleIdle)))
		return E_FAIL;

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Near = CZombie_BT_IF_Near::Create(&DecoratorDesc);//플레이어와 가까운가?
	if (FAILED(pIf_Near->AddChild(pSequenceNear))) return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("run_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pChase = CZombie_BT_Chase::Create(&ActionDesc);


	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Within_Range = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Within_Range->AddChild(pIf_Near))) 
		return E_FAIL;
	if (FAILED(pSelector_Within_Range->AddChild(pChase)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("idle_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pIdle_0 = CZombie_BT_Idle::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("idle_normal_1_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pIdle_1 = CZombie_BT_Idle::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("walk_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pMove = CZombie_BT_Move::Create(&ActionDesc);



	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceIdle = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSequenceIdle->AddChild(pIdle_0))) 
		return E_FAIL;
	if (FAILED(pSequenceIdle->AddChild(pMove)))
		return E_FAIL;
	if (FAILED(pSequenceIdle->AddChild(pIdle_1))) 
		return E_FAIL;
	if (FAILED(pSequenceIdle->AddChild(pMove))) 
		return E_FAIL;

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::WHILE;
	CBT_Decorator* pWhile_Within_Range = CZombie_BT_WHILE_Within_Range::Create(&DecoratorDesc);//플레이어와 가까운가?
	if (FAILED(pWhile_Within_Range->AddChild(pSelector_Within_Range))) 
		return E_FAIL;

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pRoot = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pRoot->AddChild(pIfHit))) 
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIfSpawn))) 
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pWhile_Within_Range))) 
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pSequenceIdle))) 
		return E_FAIL;

	m_pBehaviorTree->SetRoot(pRoot);

	return S_OK;
}


CMonster_Zombie* CMonster_Zombie::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Zombie* pInstance = new CMonster_Zombie(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Gunslinger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Zombie::Clone(void* pArg)
{
	CMonster_Zombie* pInstance = new CMonster_Zombie(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster_Zombie");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Zombie::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
