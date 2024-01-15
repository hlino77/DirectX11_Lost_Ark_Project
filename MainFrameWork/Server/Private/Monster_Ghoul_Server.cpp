#include "stdafx.h"
#include "Monster_Ghoul_Server.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Skill_Server.h"
#include "Common_BT_Attack1_Server.h"
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
#include <Common_BT_IF_Attacked_Server.h>
#include <Ghoul_BT_Attack_1_Server.h>
#include <Ghoul_BT_Attack_2_Server.h>
#include <Ghoul_BT_Attack_3_Server.h>
#include <Common_BT_IF_Twist_Server.h>
#include "Common_BT_IF_Downed_Server.h"
#include "Common_BT_Stand_Server.h"
#include <Common_BT_Twist_Server.h>
#include <Common_BT_IF_Bound_Server.h>
#include <Common_BT_Bound_Server.h>
#include <Common_BT_IF_FirstHit_Server.h>
#include <Common_BT_Bug_Server.h>
#include <Common_BT_IF_Bug_Server.h>
#include <Common_BT_IF_Maz_Server.h>
#include <Common_BT_IF_Stun_Server.h>
#include <Common_BT_Stun_Server.h>
#include <Common_BT_IF_Shock_Server.h>
#include <Common_BT_Shock_Server.h>
#include <Common_BT_IF_Fear_Server.h>
#include <Common_BT_Fear_Server.h>
#include <Common_BT_IF_Earthquake_Server.h>
#include <Common_BT_Earthquake_Server.h>
#include <Common_BT_BoundLand_Server.h>
#include <Common_BT_IF_BoundLand_Server.h>
#include <Common_BT_TwistLand_Server.h>
#include <Common_BT_IF_TwistLand_Server.h>


CMonster_Ghoul_Server::CMonster_Ghoul_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster_Server(pDevice, pContext)
{
}

CMonster_Ghoul_Server::CMonster_Ghoul_Server(const CMonster_Server& rhs)
	: CMonster_Server(rhs)
{
}

HRESULT CMonster_Ghoul_Server::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CMonster_Ghoul_Server::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_vecAttackRanges.push_back(1.f);
	m_vecAttackRanges.push_back(4.5f);
	m_fAttackRange = m_vecAttackRanges[0];
	m_fNoticeRange = 20.f;
	m_pRigidBody->SetMass(2.0f);
	m_fRootTargetDistance = 0.9f;
	m_iMaxHp = 40000000;
	m_iHp = m_iMaxHp;

	return S_OK;
}

void CMonster_Ghoul_Server::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
}

void CMonster_Ghoul_Server::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CMonster_Ghoul_Server::Render()
{

	return S_OK;
}

void CMonster_Ghoul_Server::Set_SlowMotion(_bool bSlow)
{
}

void CMonster_Ghoul_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Ghoul_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Ghoul_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

HRESULT CMonster_Ghoul_Server::Ready_Components()
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
	wstring strComName = L"Prototype_Component_Model_Monster_Ghoul";
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

HRESULT CMonster_Ghoul_Server::Ready_BehaviourTree()
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
	AnimationDesc.fAnimSpeed = 1.2f;
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
	AnimationDesc.strAnimName = TEXT("twistknockdown");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Twist";
	CBT_Action* pTwist = CCommon_BT_Twist_Server::Create(&ActionDesc);


	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfTwist = CCommon_BT_IF_Twist_Server::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfTwist->AddChild(pTwist)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("bound");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Bound";
	CBT_Action* pBound = CCommon_BT_Bound_Server::Create(&ActionDesc);


	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfBound = CCommon_BT_IF_Bound_Server::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfBound->AddChild(pBound)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("abn_bug_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Bug";
	CBT_Action* pBug = CCommon_BT_Bug_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfBug = CCommon_BT_IF_Bug_Server::Create(&DecoratorDesc);//넉백인가
	if (FAILED(pIfBug->AddChild(pBug)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("abn_earthquake_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Earthquake";
	CBT_Action* pEarthquake = CCommon_BT_Earthquake_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfEarthquake = CCommon_BT_IF_Earthquake_Server::Create(&DecoratorDesc);//넉백인가
	if (FAILED(pIfEarthquake->AddChild(pEarthquake)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("abn_fear_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Fear";
	CBT_Action* pFear = CCommon_BT_Fear_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfFear = CCommon_BT_IF_Fear_Server::Create(&DecoratorDesc);//넉백인가
	if (FAILED(pIfFear->AddChild(pFear)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("abn_shock_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Shock";
	CBT_Action* pShock = CCommon_BT_Shock_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfShock = CCommon_BT_IF_Shock_Server::Create(&DecoratorDesc);//넉백인가
	if (FAILED(pIfShock->AddChild(pShock)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("abn_stun_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Stun";
	CBT_Action* pStun = CCommon_BT_Stun_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfStun = CCommon_BT_IF_Stun_Server::Create(&DecoratorDesc);//넉백인가
	if (FAILED(pIfStun->AddChild(pStun)))
		return E_FAIL;

	CBT_Composite::COMPOSITE_DESC CompositeDesc = {};
	CompositeDesc.pGameObject = this;
	CompositeDesc.pBehaviorTree = m_pBehaviorTree;
	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Maz = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Maz->AddChild(pIfStun)))
		return E_FAIL;
	if (FAILED(pSelector_Maz->AddChild(pIfShock)))
		return E_FAIL;
	if (FAILED(pSelector_Maz->AddChild(pIfFear)))
		return E_FAIL;
	if (FAILED(pSelector_Maz->AddChild(pIfEarthquake)))
		return E_FAIL;
	if (FAILED(pSelector_Maz->AddChild(pIfBug)))
		return E_FAIL;

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfMaz = CCommon_BT_IF_Maz_Server::Create(&DecoratorDesc);//상태이상인가?
	if (FAILED(pIfMaz->AddChild(pSelector_Maz)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("dmg_idle_2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.1f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Left";
	CBT_Action* pDamageLeft = CCommon_BT_Damage2_Server::Create(&ActionDesc);

	CBT_Decorator* pIfFirstHit = CCommon_BT_IF_FirstHit_Server::Create(&DecoratorDesc);//왼쪽을 맞았는가
	if (FAILED(pIfFirstHit->AddChild(pDamageLeft)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("dmg_idle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.1f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Damage_Right";
	CBT_Action* pDamageRight = CCommon_BT_Damage1_Server::Create(&ActionDesc);

	CBT_Decorator* pIfSecondHit = CCommon_BT_IF_SecondHit_Server::Create(&DecoratorDesc);//왼쪽을 맞았는가
	if (FAILED(pIfSecondHit->AddChild(pDamageRight)))
		return E_FAIL;

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Hit = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Hit->AddChild(pIfDead))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfTwist))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfBound))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfMaz)))return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfFirstHit))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfSecondHit))) return E_FAIL;

	CBT_Decorator* pIfHit = CCommon_BT_IF_Hit_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("bound_land");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BoundLand";
	CBT_Action* pBoundLand = CCommon_BT_BoundLand_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfBoundLand = CCommon_BT_IF_BoundLand_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfBoundLand->AddChild(pBoundLand)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("twistknockdown_land");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_TwistLand";
	CBT_Action* pTwistLand = CCommon_BT_TwistLand_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfTwistLand = CCommon_BT_IF_TwistLand_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfTwistLand->AddChild(pTwistLand)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Stand_Up";
	CBT_Action* pStandUp = Common_BT_Stand_Server::Create(&ActionDesc);


	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfDowned = CCommon_BT_IF_Downed_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfDowned->AddChild(pStandUp)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("respawn_1");
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
	AnimationDesc.strAnimName = TEXT("att_battle_2_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("att_battle_2_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.3f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_2_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack2";

	CBT_Action* pAttack2 = CGhoul_BT_Attack_1_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_3_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	ActionDesc.strActionName = L"Action_Attack3";
	CBT_Action* pAttack3 = CGhoul_BT_Attack_2_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_4");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack4";
	CBT_Action* pAttack4 = CGhoul_BT_Attack_3_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceNear = CBT_Composite::Create(&CompositeDesc);


	if (FAILED(pSequenceNear->AddChild(pAttack2)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack3)))
		return E_FAIL;
	if (FAILED(pSequenceNear->AddChild(pAttack4)))
		return E_FAIL;


	CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
	if (FAILED(pIfAttacked->AddChild(pSequenceNear)))
		return E_FAIL;


	ActionDesc.vecAnimations.clear();
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

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::WHILE;
	CBT_Decorator* pWhile_Within_Range = CCommon_BT_WHILE_Within_Range_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
	if (FAILED(pWhile_Within_Range->AddChild(pSelector_Within_Range)))
		return E_FAIL;

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pRoot = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pRoot->AddChild(pIfHit)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIfTwistLand)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIfBoundLand)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIfDowned)))
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

CMonster_Ghoul_Server* CMonster_Ghoul_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Ghoul_Server* pInstance = new CMonster_Ghoul_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_Ghoul_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Ghoul_Server::Clone(void* pArg)
{
	CMonster_Ghoul_Server* pInstance = new CMonster_Ghoul_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CMonster_Ghoul_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMonster_Ghoul_Server::Free()
{
	__super::Free();



	Safe_Release(m_pModelCom);

	Safe_Release(m_pTransformCom);
}
