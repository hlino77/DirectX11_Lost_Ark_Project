#include "stdafx.h"
#include "Boss_King_Server.h"
#include "BehaviorTree.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "Common_BT_Chase_Server.h"
#include "Common_BT_DamageLeft_Server.h"
#include "Common_BT_DamageRight_Server.h"
#include "Common_BT_Dead_Server.h"
#include "Common_BT_Idle_Server.h"
#include "Common_BT_BattleIdle_Server.h"
#include "Common_BT_Move_Server.h"
#include "Common_BT_Spawn_Server.h"
#include "Common_BT_WHILE_Within_Range_Server.h"
#include "Common_BT_IF_Dead_Server.h"
#include "Common_BT_IF_Hit_Server.h"
#include "Common_BT_IF_Hit_Left_Server.h"
#include "Common_BT_IF_Near_Server.h"
#include "Common_BT_IF_Far_Server.h"
#include "Common_BT_IF_Spawn_Server.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"
#include "Common_BT_IF_Attacked.h"
#include "Common_BT_Attack1_Server.h"
#include "Common_BT_Spawn_Server.h"


CBoss_King_Server::CBoss_King_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss_Server(pDevice, pContext)
{
}

CBoss_King_Server::CBoss_King_Server(const CBoss_King_Server& rhs)
	: CBoss_Server(rhs)
{
}

HRESULT CBoss_King_Server::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_King_Server::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBoss_King_Server::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBoss_King_Server::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	cout << CGameInstance::GetInstance()->wstring_to_string(m_strAction) << endl;
}

HRESULT CBoss_King_Server::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}


void CBoss_King_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_King_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_King_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_King_Server::Set_Colliders(_float fTimeDelta)
{
	__super::Set_Colliders(fTimeDelta);
}

HRESULT CBoss_King_Server::Ready_Coliders()
{
	if (FAILED(__super::Ready_Coliders()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoss_King_Server::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_King_Server::Ready_BehaviourTree()
{
	if (FAILED(__super::Ready_BehaviourTree()))
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
	AnimationDesc.strAnimName = TEXT("dead_1_loop");
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
	AnimationDesc.strAnimName = TEXT("dmg_critical_start_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("dmg_critical_loop_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("dmg_critical_end_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Left";
	CBT_Action* pDamage = CCommon_BT_DamageLeft_Server::Create(&ActionDesc);

	CBT_Composite::COMPOSITE_DESC CompositeDesc = {};
	CompositeDesc.pGameObject = this;
	CompositeDesc.pBehaviorTree = m_pBehaviorTree;
	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Hit = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Hit->AddChild(pIfDead))) return E_FAIL;
	//if (FAILED(pSelector_Hit->AddChild(pIfMaz))) return E_FAIL; 상태이상 보류중
	if (FAILED(pSelector_Hit->AddChild(pDamage))) return E_FAIL;
	
	CBT_Decorator* pIfHit = CCommon_BT_IF_Hit_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("sk_arcanacard_fx");
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
	ActionDesc.strActionName = L"Action_Attack2";
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pAttack2 = CCommon_BT_Attack1_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_3_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.strActionName = L"Action_Attack2";
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("att_battle_3_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("att_battle_3_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.strActionName = L"Action_Attack3";
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	CBT_Action* pAttack3 = CCommon_BT_Attack1_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceNear = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSequenceNear->AddChild(pAttack1)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack2)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack3)))
		return E_FAIL;
	CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
	if (FAILED(pIfAttacked->AddChild(pSequenceNear)))
		return E_FAIL;



	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CCommon_BT_BattleIdle_Server::Create(&ActionDesc);

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

	m_pBehaviorTree->SetRoot(pRoot);

	return S_OK;
}

CBoss_Server* CBoss_King_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_King_Server* pInstance = new CBoss_King_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_King_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_King_Server::Clone(void* pArg)
{
	CBoss_King_Server* pInstance = new CBoss_King_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_King_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_King_Server::Free()
{
	__super::Free();
}
