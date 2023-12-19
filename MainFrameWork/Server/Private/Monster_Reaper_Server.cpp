#include "stdafx.h"
#include "Monster_Reaper_Server.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Skill_Server.h"
#include "Common_BT_Attack1_Server.h"
#include "Plant_BT_Attack_Root_Server.h"
#include "Plant_BT_Attack_Shake_Server.h"
#include "Common_BT_Chase_Server.h"
#include "Common_BT_DamageLeft_Server.h"
#include "Common_BT_DamageRight_Server.h"
#include "Common_BT_Dead_Server.h"
#include "Common_BT_Idle_Server.h"
#include "Common_BT_BattleIdle_Server.h"
#include "Common_BT_Move_Server.h"
#include "Common_BT_Spawn_Server.h"
#include "Common_BT_IF_Dead_Server.h"
#include "Common_BT_IF_Hit_Server.h"
#include "Common_BT_IF_Hit_Left_Server.h"
#include "Common_BT_IF_Near_Server.h"
#include "Common_BT_IF_Spawn_Server.h"
#include "Common_BT_WHILE_Within_Range_Server.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"
#include "Common_BT_IF_Attacked.h"
#include "Common_BT_IF_Far_Server.h"
#include "Reaper_BT_Attack3_Server.h"
#include "Reaper_BT_Attack2_Server.h"
#include "Reaper_BT_Attack1_Server.h"

CMonster_Reaper_Server::CMonster_Reaper_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster_Server(pDevice, pContext)
{
}

CMonster_Reaper_Server::CMonster_Reaper_Server(const CMonster_Server& rhs)
	: CMonster_Server(rhs)
{
}

HRESULT CMonster_Reaper_Server::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CMonster_Reaper_Server::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_szModelName = L"Reaper";
	m_strObjectTag = L"Monster_Reaper";
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);

	m_vecAttackRanges.push_back(1.f);
	m_vecAttackRanges.push_back(2.f);
	m_fAttackRange = m_vecAttackRanges[0];
	m_fNoticeRange = 20.f;
	m_pRigidBody->SetMass(2.0f);

	return S_OK;
}

void CMonster_Reaper_Server::Tick(_float fTimeDelta)
{
	CNavigationMgr::GetInstance()->SetUp_OnCell(this);
	m_fScanCoolDown += fTimeDelta;
	m_pBehaviorTree->Tick(fTimeDelta);
	if (m_fScanCoolDown > 0.5f)
	{
		m_fScanCoolDown = 0.f;
		Find_NearTarget();
	}
	m_pRigidBody->Tick(fTimeDelta);
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	m_PlayAnimation.get();
	if (Get_Target_Distance() > 0.5f)
		Set_to_RootPosition(fTimeDelta);

}

void CMonster_Reaper_Server::LateTick(_float fTimeDelta)
{


	{
		READ_LOCK
			for (auto& CollisionStay : m_CollisionList)
				OnCollisionStay(CollisionStay.iColLayer, CollisionStay.pCollider);
	}

	Set_Colliders(fTimeDelta);

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Reaper_Server::Render()
{

	return S_OK;
}

void CMonster_Reaper_Server::Set_SlowMotion(_bool bSlow)
{
}

void CMonster_Reaper_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Reaper_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Reaper_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

HRESULT CMonster_Reaper_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	///* For.Com_State */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Monster_Reaper";
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;




	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;

}

HRESULT CMonster_Reaper_Server::Ready_BehaviourTree()
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
	ActionDesc.strActionName = L"Action_Dead";
	CBT_Action* pDead = CCommon_BT_Dead_Server::Create(&ActionDesc);

	CBT_Decorator::DECORATOR_DESC DecoratorDesc = {};
	DecoratorDesc.pBehaviorTree = m_pBehaviorTree;
	DecoratorDesc.pGameObject = this;
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfDead = CCommon_BT_IF_Dead_Server::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfDead->AddChild(pDead)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("dmg_idle_2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Left";
	CBT_Action* pDamageLeft = CCommon_BT_DamageLeft_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("dmg_idle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Right";
	CBT_Action* pDamageRight = CCommon_BT_DamageRight_Server::Create(&ActionDesc);

	CBT_Decorator* pIfHitLeft = CCommon_BT_IF_Hit_Left_Server::Create(&DecoratorDesc);//왼쪽을 맞았는가
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

	CBT_Decorator* pIfHit = CCommon_BT_IF_Hit_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("evt1_att_battle_1_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Respawn";
	CBT_Action* pSpawn = CCommon_BT_Spawn_Server::Create(&ActionDesc);

	CBT_Decorator* pIfSpawn = CCommon_BT_IF_Spawn_Server::Create(&DecoratorDesc);//스폰 직후인가?
	if (FAILED(pIfSpawn->AddChild(pSpawn)))
		return E_FAIL;



	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("run_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Chase";
	CBT_Action* pChase = CCommon_BT_Chase_Server::Create(&ActionDesc);


	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Far = CCommon_BT_IF_Far_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
	if (FAILED(pIf_Far->AddChild(pChase))) return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_1_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack1";
	CBT_Action* pAttack1 = CCommon_BT_Attack1_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CCommon_BT_BattleIdle_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_2_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack2";
	CBT_Action* pAttack2 = CReaper_BT_Attack1_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_3_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack3";
	CBT_Action* pAttack3 = CReaper_BT_Attack2_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_4_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_4_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_4_03_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack4";
	CBT_Action* pAttack4 = CReaper_BT_Attack3_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_4_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_4_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("att_battle_4_03_2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack5";
	CBT_Action* pAttack5 = CReaper_BT_Attack3_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceNear = CBT_Composite::Create(&CompositeDesc);

	if (FAILED(pSequenceNear->AddChild(pAttack1)))
		return E_FAIL;

	if (FAILED(pSequenceNear->AddChild(pAttack2)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack5)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack3)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack4)))
		return E_FAIL;

	CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
	if (FAILED(pIfAttacked->AddChild(pSequenceNear)))
		return E_FAIL;

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelectorNear = CBT_Composite::Create(&CompositeDesc);

	if (FAILED(pSelectorNear->AddChild(pIfAttacked)))
		return E_FAIL;
	if (FAILED(pSelectorNear->AddChild(pBattleIdle)))
		return E_FAIL;
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Near = CCommon_BT_IF_Near_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
	if (FAILED(pIf_Near->AddChild(pSelectorNear))) return E_FAIL;

	CBT_Composite* pSelector_Within_Range = CBT_Composite::Create(&CompositeDesc);

	if (FAILED(pSelector_Within_Range->AddChild(pIf_Far)))
		return E_FAIL;
	if (FAILED(pSelector_Within_Range->AddChild(pIf_Near)))
		return E_FAIL;





	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Idle_0";
	CBT_Action* pIdle_0 = CCommon_BT_Idle_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_normal_1_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Idle_1";
	CBT_Action* pIdle_1 = CCommon_BT_Idle_Server::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("walk_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Move";
	CBT_Action* pMove = CCommon_BT_Move_Server::Create(&ActionDesc);



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
	if (FAILED(pSequenceIdle->AddChild(pMove)))
		return E_FAIL;

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::WHILE;
	CBT_Decorator* pWhile_Within_Range = CCommon_BT_WHILE_Within_Range_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
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

CMonster_Reaper_Server* CMonster_Reaper_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Reaper_Server* pInstance = new CMonster_Reaper_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_Reaper");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Reaper_Server::Clone(void* pArg)
{
	CMonster_Reaper_Server* pInstance = new CMonster_Reaper_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CMonster_Reaper" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMonster_Reaper_Server::Free()
{
	__super::Free();



	Safe_Release(m_pModelCom);

	Safe_Release(m_pTransformCom);
}
