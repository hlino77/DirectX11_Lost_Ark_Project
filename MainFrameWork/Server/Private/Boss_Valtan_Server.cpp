#include "stdafx.h"
#include "Boss_Valtan_Server.h"
#include "BehaviorTree.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "Common_BT_Chase_Server.h"
#include "Common_BT_Damage1_Server.h"
#include "Common_BT_Damage2_Server.h"
#include "Common_BT_Dead_Server.h"
#include "Common_BT_Idle_Server.h"
#include "Common_BT_BattleIdle_Server.h"
#include "Common_BT_Move_Server.h"
#include "Common_BT_Spawn_Server.h"
#include "Common_BT_WHILE_Within_Range_Server.h"
#include "Common_BT_IF_Dead_Server.h"
#include "Common_BT_IF_Hit_Server.h"
#include "Common_BT_IF_SecondHit_Server.h"
#include "Common_BT_IF_Near_Server.h"
#include "Common_BT_IF_Far_Server.h"
#include "Common_BT_IF_Spawn_Server.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"
#include "Common_BT_IF_Attacked_Server.h"
#include "Common_BT_Attack1_Server.h"
#include "Common_BT_Spawn_Server.h"
#include <Common_BT_IF_Skill_Server.h>
#include "King_BT_Attack_Attack3_Server.h"
#include "King_BT_Attack_Attack4_Server.h"
#include "King_BT_Attack_Charge_Swing_Server.h"
#include "King_BT_Attack_Erruption_Server.h"
#include "Boss_BT_Counter_Server.h"
#include "Boss_BT_IF_Countered_Server.h"
#include "Boss_BT_Groggy_Server.h"
#include "Boss_BT_IF_Groggy_Server.h"
#include "Valtan_BT_Attack_Attack0_Server.h"
#include "Valtan_BT_Attack_Attack1_Server.h"
#include "Valtan_BT_Attack_Attack2_Server.h"
#include "Valtan_BT_Attack_Attack3_Server.h"
#include "Valtan_BT_Attack_Attack4_Server.h"
#include "Valtan_BT_Attack_Attack5_Server.h"
#include "Valtan_BT_Attack_Attack6_Server.h"
#include "Valtan_BT_Attack_Attack7_Server.h"
#include "Valtan_BT_Attack_Attack8_Server.h"
#include "Valtan_BT_Attack_Attack9_Server.h"
#include "Valtan_BT_Attack_Attack10_Server.h"
#include "Valtan_BT_Attack_Attack11_Server.h"
#include "Valtan_BT_Attack_Attack12_Server.h"
#include "Valtan_BT_Attack_Attack13_Server.h"
#include "Valtan_BT_Attack_Attack14_Server.h"
#include "Valtan_BT_Attack_Attack15_Server.h"
#include "Valtan_BT_IF_Phase1_Server.h"
#include "Valtan_BT_IF_NoArmor_Server.h"
#include "Valtan_BT_Attack_Attack16_Server.h"
#include "Valtan_BT_Attack_Attack18_Server.h"
#include "Valtan_BT_Attack_Attack17_Server.h"
#include "Valtan_BT_Attack_Attack19_Server.h"
#include "Valtan_BT_Attack_Attack20_Server.h"
#include "Valtan_BT_Attack_Attack21_Server.h"
#include "Valtan_BT_Attack_Attack22_Server.h"
#include "Valtan_BT_Attack_Attack23_Server.h"
#include "Valtan_BT_Attack_Attack24_Server.h"
#include "Valtan_BT_IF_Phase2_Server.h"
#include "Valtan_BT_IF_Phase3_Server.h"
#include <Valtan_BT_IF_Hp_UnderSpecialSkillRatio.h>
#include <Valtan_BT_IF_Armor_Server.h>
#include <Valtan_BT_Repeat_Server.h>



CBoss_Valtan_Server::CBoss_Valtan_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss_Server(pDevice, pContext)
{
}

CBoss_Valtan_Server::CBoss_Valtan_Server(const CBoss_Valtan_Server& rhs)
	: CBoss_Server(rhs)
{
}

HRESULT CBoss_Valtan_Server::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan_Server::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_vecAttackRanges.clear();
	m_fMoveSpeed = 4.f;
	m_vecAttackRanges.push_back(2.f);
	m_vecAttackRanges.push_back(3.5f);
	m_IsSuperArmor = true;
	m_iArmor = 2;
	m_iPhase = 1;
	m_fAttackRange = m_vecAttackRanges[0];
	m_fRootTargetDistance = 0.f;
	m_iMaxHp = 1991561183;
	m_iHp = m_iMaxHp;
	m_fNoticeRange = 150.f;
	m_vSpawnPosition = Vec3(117.f, 0.f, 100.f);
	return S_OK;
}

void CBoss_Valtan_Server::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBoss_Valtan_Server::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBoss_Valtan_Server::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}


void CBoss_Valtan_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_Valtan_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_Valtan_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_Valtan_Server::Set_Colliders(_float fTimeDelta)
{
	__super::Set_Colliders(fTimeDelta);
}

HRESULT CBoss_Valtan_Server::Ready_Coliders()
{
	if (FAILED(__super::Ready_Coliders()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoss_Valtan_Server::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan_Server::Ready_BehaviourTree()
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
	AnimationDesc.fRootDist = 1.5f;
	AnimationDesc.fAnimSpeed = 1.1f;
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
	AnimationDesc.fChangeTime = 0.4f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("dmg_critical_end_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Counter";

	CBT_Action* pCounter = CBoss_BT_Counter_Server::Create(&ActionDesc);
	ActionDesc.vecAnimations.clear();

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfCountered = CBoss_BT_IF_Countered_Server::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfCountered->AddChild(pCounter)))
		return E_FAIL;

	AnimationDesc.strAnimName = TEXT("abn_groggy_1_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("abn_groggy_1_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.4f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("abn_groggy_1_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Groggy";
	CBT_Action* pGroggy = CBoss_BT_Groggy_Server::Create(&ActionDesc);
	ActionDesc.vecAnimations.clear();

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfGroggy = CBoss_BT_IF_Groggy_Server::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfGroggy->AddChild(pGroggy)))
		return E_FAIL;

	CBT_Composite::COMPOSITE_DESC CompositeDesc = {};
	CompositeDesc.pGameObject = this;
	CompositeDesc.pBehaviorTree = m_pBehaviorTree;
	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Hit = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Hit->AddChild(pIfDead))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfCountered))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfGroggy))) return E_FAIL;


	CBT_Decorator* pIfHit = CCommon_BT_IF_Hit_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime =0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("turn_l_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	ActionDesc.strActionName = L"Action_Respawn";
	CBT_Action* pSpawn = CCommon_BT_Spawn_Server::Create(&ActionDesc);

	CBT_Decorator* pIfSpawn = CCommon_BT_IF_Spawn_Server::Create(&DecoratorDesc);//스폰 직후인가?
	if (FAILED(pIfSpawn->AddChild(pSpawn)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_12_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);



	//65번 2페 시작 모션
	ActionDesc.strActionName = L"Action_Phase2";

	CBT_Action* pPhase2 = CCommon_BT_Spawn_Server::Create(&ActionDesc);

	CBT_Decorator* pIf_Hp_UnderRatio120 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>( pIf_Hp_UnderRatio120 )->Set_Ratio( 120.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio120->AddChild(pPhase2))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("respawn_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//65번 유령페이즈 시작 모션
	ActionDesc.strActionName = L"Action_Phase3";
	CBT_Action* pPhase3 = CCommon_BT_Spawn_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceLine16 = CBT_Composite::Create(&CompositeDesc);	

	CBT_Decorator* pIf_Hp_UnderRatio16 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>(pIf_Hp_UnderRatio120)->Set_Ratio(16.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio16->AddChild(pSequenceLine16))) return E_FAIL;

	// Ready all AttackActions
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_4_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_6_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 5.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_4_02");
	AnimationDesc.iStartFrame = 30;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 0.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;
	//돌진&1패 이후 카운터 돌진
	ActionDesc.strActionName = L"Action_Attack1";
	CBT_Action* pAttack1 = CValtan_BT_Attack_Attack1_Server::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_9_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_9_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;	
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_9_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//반격
	ActionDesc.strActionName = L"Action_Attack2";
	CBT_Action* pAttack2 = CValtan_BT_Attack_Attack2_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_20_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//휠윈드
	ActionDesc.strActionName = L"Action_Attack3";
	CBT_Action* pAttack3 = CValtan_BT_Attack_Attack3_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_19_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	// 양손 찍고 폭발
	ActionDesc.strActionName = L"Action_Attack4";
	CBT_Action* pAttack4 = CValtan_BT_Attack_Attack4_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_18_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_18_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_18_03-2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	// 어디로든지문 돌격
	ActionDesc.strActionName = L"Action_Attack5";
	CBT_Action* pAttack5 = CValtan_BT_Attack_Attack5_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_19_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//앞뒤앞
	ActionDesc.strActionName = L"Action_Attack7";
	CBT_Action* pAttack7 = CValtan_BT_Attack_Attack7_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_10_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_10_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//도끼찍고 휘두루기
	ActionDesc.strActionName = L"Action_Attack8";
	CBT_Action* pAttack8 = CValtan_BT_Attack_Attack8_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_20_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//점프 찍기
	ActionDesc.strActionName = L"Action_Attack9";
	CBT_Action* pAttack9 = CValtan_BT_Attack_Attack9_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_20_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//점프 십자 찍기
	ActionDesc.strActionName = L"Action_Attack10";
	CBT_Action* pAttack10 = CValtan_BT_Attack_Attack10_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_5_02_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.strAnimName = TEXT("att_battle_5_02_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//십자 찍기
	ActionDesc.strActionName = L"Action_Attack1!";
	CBT_Action* pAttack11 = CValtan_BT_Attack_Attack11_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_2_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_2_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_2_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_2_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//4번 찍기
	ActionDesc.strActionName = L"Action_Attack12";
	CBT_Action* pAttack12 = CValtan_BT_Attack_Attack12_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_14_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//침묵 찍기
	ActionDesc.strActionName = L"Action_Attack13";
	CBT_Action* pAttack13 = CValtan_BT_Attack_Attack13_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_8_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_8_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.bIsLoop = false;
	AnimationDesc.strAnimName = TEXT("att_battle_8_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 후 비
	ActionDesc.strActionName = L"Action_Attack14";
	CBT_Action* pAttack14 = CValtan_BT_Attack_Attack14_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//감금
	ActionDesc.strActionName = L"Action_Attack15";
	CBT_Action* pAttack15 = CValtan_BT_Attack_Attack15_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_14_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_14_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_14_04-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	
	AnimationDesc.strAnimName = TEXT("att_battle_14_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_14_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_04-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_14_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_04-2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//3카운터
	ActionDesc.strActionName = L"Action_Attack17";
	CBT_Action* pAttack17= CValtan_BT_Attack_Attack17_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_11_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_4_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_4_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//원혼구슬
	ActionDesc.strActionName = L"Action_Attack18";
	CBT_Action* pAttack18 = CValtan_BT_Attack_Attack18_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_13_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	
	AnimationDesc.strAnimName = TEXT("att_battle_21_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//잡기
	ActionDesc.strActionName = L"Action_Attack19";
	CBT_Action* pAttack19 = CValtan_BT_Attack_Attack19_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_15_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 0.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_15_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_15_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_15_05");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//130줄 전멸기
	ActionDesc.strActionName = L"Action_Attack20";
	CBT_Action* pAttack20 = CValtan_BT_Attack_Attack20_Server::Create(&ActionDesc);


	CBT_Decorator* pIf_Hp_UnderRatio130 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>(pIf_Hp_UnderRatio120)->Set_Ratio(130.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio130->AddChild(pAttack20))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_12_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_12_05");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_06");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_07");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_08");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_09");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_10");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_11");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//110줄 임포스터
	ActionDesc.strActionName = L"Action_Attack21";
	CBT_Action* pAttack21 = CValtan_BT_Attack_Attack21_Server::Create(&ActionDesc);

	CBT_Decorator* pIf_Hp_UnderRatio110 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>(pIf_Hp_UnderRatio120)->Set_Ratio(110.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio110->AddChild(pAttack21))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_12_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_20_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_5_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	
	AnimationDesc.strAnimName = TEXT("att_battle_12_09");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_10");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_11");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//88번 지형파괴
	ActionDesc.strActionName = L"Action_Attack0";
	CBT_Action* pAttack0 = CValtan_BT_Attack_Attack0_Server::Create(&ActionDesc);


	CBT_Decorator* pIf_Hp_UnderRatio88 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>(pIf_Hp_UnderRatio120)->Set_Ratio(88.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio88->AddChild(pAttack0))) return E_FAIL;


	AnimationDesc.strAnimName = TEXT("att_battle_12_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_20_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_20_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_5_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_09");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_10");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_11");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//30줄 지형파괴
	ActionDesc.strActionName = L"Action_Attack16";
	CBT_Action* pAttack16 = CValtan_BT_Attack_Attack16_Server::Create(&ActionDesc);

	CBT_Decorator* pIf_Hp_UnderRatio30 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>(pIf_Hp_UnderRatio120)->Set_Ratio(30.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio30->AddChild(pAttack16))) return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_12_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	
	AnimationDesc.strAnimName = TEXT("att_battle_13_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_02-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_13_05-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//65번 버러지패턴
	ActionDesc.strActionName = L"Action_Attack22";
	CBT_Action* pAttack22 = CValtan_BT_Attack_Attack22_Server::Create(&ActionDesc);

	CBT_Decorator* pIf_Hp_UnderRatio65 = CValtan_BT_IF_Hp_UnderSpecialSkillRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderSpecialSkillRatio*>(pIf_Hp_UnderRatio120)->Set_Ratio(65.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio65->AddChild(pAttack22))) return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_18_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_18_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_18_03-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	AnimationDesc.strAnimName = TEXT("att_battle_5_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_05");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_19_06");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//16번 연환파신권
	ActionDesc.strActionName = L"Action_Attack23";
	CBT_Action* pAttack23 = CValtan_BT_Attack_Attack23_Server::Create(&ActionDesc);

	if (FAILED(pSequenceLine16->AddChild(pAttack23))) return E_FAIL;
	if (FAILED(pSequenceLine16->AddChild(pPhase3))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_21_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//유령 잡기
	ActionDesc.strActionName = L"Action_Attack24";
	CBT_Action* pAttack24 = CValtan_BT_Attack_Attack24_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CCommon_BT_BattleIdle_Server::Create(&ActionDesc);

	//for Phase1

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Phase1 = CValtan_BT_IF_Phase1_Server::Create(&DecoratorDesc);//플레이어와 가까운가?


	{
		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::REPEAT;
		DecoratorDesc.iRepeatCount = 99;
		CBT_Decorator* pRepeat_99 = CValtan_BT_Repeat_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pRepeat_99->AddChild(pAttack20))) return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack3)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack1)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack10)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack1)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack8)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack14)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack1)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack14)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pRepeat_99)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Armor = CValtan_BT_IF_Armor_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_Armor->AddChild(pSequenceNormalAttack))) return E_FAIL;


		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNoArmor = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceNoArmor->AddChild(pAttack3)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pAttack10)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pAttack8)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pAttack1)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_NoArmor = CValtan_BT_IF_NoArmor_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_NoArmor->AddChild(pSequenceNoArmor))) return E_FAIL;
		
		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorAttack = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSelectorAttack->AddChild(pIf_Armor)))
			return E_FAIL;
		if (FAILED(pSelectorAttack->AddChild(pIf_NoArmor)))
			return E_FAIL;
		CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
		if (FAILED(pIfAttacked->AddChild(pSelectorAttack)))
			return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorNear = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelectorNear->AddChild(pIfAttacked)))
			return E_FAIL;
		if (FAILED(pSelectorNear->AddChild(pBattleIdle)))
			return E_FAIL;
		



		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelector_Within_Range = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelector_Within_Range->AddChild(pSelectorNear)))
			return E_FAIL;

		if (FAILED(pIf_Phase1->AddChild(pSelector_Within_Range))) return E_FAIL;
	}

	//for Phase2

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Phase2 = CValtan_BT_IF_Phase2_Server::Create(&DecoratorDesc);//플레이어와 가까운가?


	{

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceSkill = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSequenceSkill->AddChild(pAttack17)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pAttack18)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pAttack19)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Skill = CCommon_BT_IF_Skill_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_Skill->AddChild(pSequenceSkill))) return E_FAIL;



		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack1)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack2)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack3)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack4)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack5)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack7)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack8)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack9)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack10)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack11)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack12)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack13)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack14)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack15)))
			return E_FAIL;


		CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
		if (FAILED(pIfAttacked->AddChild(pSequenceNormalAttack)))
			return E_FAIL;


		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorNear = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelectorNear->AddChild(pIfAttacked)))
			return E_FAIL;
		if (FAILED(pSelectorNear->AddChild(pBattleIdle)))
			return E_FAIL;


		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelector_Within_Range = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelector_Within_Range->AddChild(pIf_Skill)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pSelectorNear)))
			return E_FAIL;

		if (FAILED(pIf_Phase2->AddChild(pSelector_Within_Range))) return E_FAIL;
	}

	//for Phase 3

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Phase3 = CValtan_BT_IF_Phase3_Server::Create(&DecoratorDesc);//플레이어와 가까운가?


	{

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceSkill = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceSkill->AddChild(pAttack17)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pAttack18)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pAttack19)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Skill = CCommon_BT_IF_Skill_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_Skill->AddChild(pSequenceSkill))) return E_FAIL;



		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack1)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack2)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack3)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack4)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack5)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack7)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack8)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack9)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack10)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack11)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack12)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack13)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack14)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack15)))
			return E_FAIL;


		CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
		if (FAILED(pIfAttacked->AddChild(pSequenceNormalAttack)))
			return E_FAIL;




		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorNear = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelectorNear->AddChild(pIfAttacked)))
			return E_FAIL;
		if (FAILED(pSelectorNear->AddChild(pBattleIdle)))
			return E_FAIL;


		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelector_Within_Range = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelector_Within_Range->AddChild(pIf_Skill)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pSelectorNear)))
			return E_FAIL;
		if (FAILED(pIf_Phase3->AddChild(pSelector_Within_Range)))
			return E_FAIL;
	}





	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Phase = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Phase->AddChild(pIf_Phase1)))
		return E_FAIL;
	if (FAILED(pSelector_Phase->AddChild(pIf_Phase2)))
		return E_FAIL;
	if (FAILED(pSelector_Phase->AddChild(pIf_Phase3)))
		return E_FAIL;

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pRoot = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pRoot->AddChild(pIfHit)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIfSpawn)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio130)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio120)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio110)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio88)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio65)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio30)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Hp_UnderRatio16)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pSelector_Phase)))
		return E_FAIL;

	m_pBehaviorTree->SetRoot(pRoot);

	return S_OK;
}

CBoss_Server* CBoss_Valtan_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Valtan_Server* pInstance = new CBoss_Valtan_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Valtan_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Valtan_Server::Clone(void* pArg)
{
	CBoss_Valtan_Server* pInstance = new CBoss_Valtan_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_Valtan_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_Valtan_Server::Free()
{
	__super::Free();
}
