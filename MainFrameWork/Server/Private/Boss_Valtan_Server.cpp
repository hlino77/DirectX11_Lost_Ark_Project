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
#include "Valtan_BT_Dead_Server.h"
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
#include "Valtan_BT_Spawn_Server.h"
#include <Common_BT_IF_Skill_Server.h>
#include "Boss_BT_Counter_Server.h"
#include "Boss_BT_IF_Countered_Server.h"
#include "Boss_BT_Groggy_Server.h"
#include "Boss_BT_IF_Groggy_Server.h"
#include "Valtan_BT_Attack_FirstTerrainDestruction_Server.h"
#include "Valtan_BT_Attack_Rush_Server.h"
#include "Valtan_BT_Attack_CounterAttack_Server.h"
#include "Valtan_BT_Attack_WhirlWind_Server.h"
#include "Valtan_BT_Attack_FistSmashExplosion_Server.h"
#include "Valtan_BT_Attack_TeleportRush_Server.h"
#include "Valtan_BT_Attack_TrunningPizza_Server.h"
#include "Valtan_BT_Attack_FrontBackWave_Server.h"
#include "Valtan_BT_Attack_DoubleSwingChopSwing_Server.h"
#include "Valtan_BT_Attack_DoubleJumpWave_Server.h"
#include "Valtan_BT_Attack_JumpSeismic_Server.h"
#include "Valtan_BT_Attack_SwingSeismic_Server.h"
#include "Valtan_BT_Attack_MultipleChop_Server.h"
#include "Valtan_BT_Attack_SilenceChop_Server.h"
#include "Valtan_BT_Attack_RainingAxe_Server.h"
#include "Valtan_BT_Attack_Imprisonment_Server.h"
#include "Valtan_BT_IF_Phase1_Server.h"
#include "Valtan_BT_IF_NoArmor_Server.h"
#include "Valtan_BT_Attack_SecondTerrainDestruction_Server.h"
#include "Valtan_BT_Attack_TrippleCounterChop_1_Server.h"
#include "Valtan_BT_Attack_TrippleCounterChop_2_Server.h"
#include "Valtan_BT_Attack_TrippleCounterChop_3_Server.h"
#include "Valtan_BT_Attack_RushGrab_Server.h"
#include "Valtan_BT_Attack_WipeAssult_Server.h"
#include "Valtan_BT_Attack_Imposter_Server.h"
#include "Valtan_BT_Attack_BugSmash_Server.h"
#include "Valtan_BT_Attack_ChainDestructionFist_Server.h"
#include "Valtan_BT_Attack_GhostGrab_Server.h"
#include "Valtan_BT_IF_Phase2_Server.h"
#include "Valtan_BT_IF_Phase3_Server.h"
#include <Valtan_BT_IF_Hp_UnderRatio.h>
#include <Valtan_BT_IF_Armor_Server.h>
#include <Valtan_BT_Repeat_Server.h>
#include <Boss_BT_ArmorBreak_Server.h>
#include <Boss_BT_IF_ArmorBreak_Server.h>
#include <Valtan_BT_Phase3_Server.h>
#include <Valtan_BT_Phase2_Server.h>
#include <Valtan_BT_Attack_CounterAttack_1_Server.h>
#include <Player_Server.h>
#include "NavigationMgr.h"


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
	m_bTest = true;

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
	m_iMaxHp = 8214139495;
	m_iHp = m_iMaxHp;
	m_iMaxArmorDurability = (_uint)((_float)m_iMaxHp / 160.f * 3.f);
	m_iArmorDurability = m_iMaxArmorDurability;
	m_fNoticeRange = 150.f;
	m_pTransformCom->LookAt_Dir(Vec3(0.f, 0.f, 1.f));
	m_pTransformCom->Turn_Axis(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-1.f));
	m_iMaxGroggyGauge = 2000;
	m_iGroggyGauge = m_iMaxGroggyGauge;
	return S_OK;
}

void CBoss_Valtan_Server::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fSkillCoolDown = 0.f;
}

void CBoss_Valtan_Server::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_iPhase == 3)
		Move_to_SpawnPosition();
}

HRESULT CBoss_Valtan_Server::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}


void CBoss_Valtan_Server::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	WRITE_LOCK
		 if (!m_IsInvincible)
		{
			_uint iDamage_Result = _uint((_float)iDamage * ((10.f - (_float)m_iArmor) / 10.f));
			_uint iGroggy_Result = iGroggy;
			_bool	bGroggyObsorb = false;
			STATUSEFFECT eEfect = STATUSEFFECT::EFFECTEND;
			m_iHp -= iDamage_Result;
			if (m_IsGroggyLock)
				iGroggy_Result = 0;
			if (m_iGroggyObsrob > 0)
			{
				m_iGroggyObsrob -= iGroggy_Result;
				iGroggy_Result = 0;
			}
			else if (m_iGroggyCount > 0 && m_iMaxGroggyCount > 0)
			{
				m_iGroggyCount -= iGroggy_Result;
				bGroggyObsorb = true;
				if (m_iGroggyCount < 1)
				{
					m_IsHit = true;
					m_bSkipAction = true;
					m_IsGroggy = true;
					m_iGroggyCount = 0;
					m_iMaxGroggyCount = 0;
				}
			}
			else if (!m_IsGroggy && m_iGroggyGauge > 0 && !m_IsGroggyLock)
				m_iGroggyGauge -= iGroggy_Result;


			if (m_IsGroggy && m_iGroggyGauge > 0 && m_iArmor > 0)
				m_iArmorDurability -= iDamage;
			if ((_uint)STATUSEFFECT::COUNTER == iStatusEffect && m_IsCounterSkill)
			{
				eEfect = STATUSEFFECT::COUNTER;
				m_bSkipAction = true;
				m_IsHit = true;
				m_IsCounterSkill = false;
				m_IsCountered = true;
			}
			if ( m_iGroggyGauge < 1|| (_uint)STATUSEFFECT::GROGGY == iStatusEffect &&m_IsRush)
			{
				eEfect = STATUSEFFECT::GROGGY;
				m_IsHit = true;
				m_bSkipAction = true;
				m_IsGroggy = true;
				m_IsRush = false;
			}


			m_fStatusEffects[iStatusEffect] += fDuration;
			if (m_iHp < 1.f && m_iPhase == 2)
				m_iHp = 1;

			if (m_iHp < 1.f)
				m_IsHit = true;
			Send_Collision(iDamage_Result, Vec3(_float(eEfect), _float(eEfect), _float(eEfect)) , m_iGroggyGauge, (_float)m_iGroggyCount, (_float)bGroggyObsorb, iGroggy_Result);
		}
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

void CBoss_Valtan_Server::BroadCast_Ghost(Vec3 vPosition, Vec3 vLook)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if(m_iPhase==3&& !m_bDummy)
	{

		wstring szComponentName = L"Boss_Valtan_CounterGhost";
		CBoss_Server::MODELDESC Desc;
		Desc.strFileName = L"Valtan_CounterGhost";
		Desc.iObjectID = g_iObjectID++;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
		Desc.iLevel = m_iCurrLevel;
		Desc.vPosition = vPosition;
		wstring szMonsterName = L"Prototype_GameObject_Boss_Valtan_CounterGhost";
		CBoss_Server* pBoss = dynamic_cast<CBoss_Server*>(pGameInstance->Add_GameObject(m_iCurrLevel, Desc.iLayer, szMonsterName, &Desc));
		if (pBoss == nullptr)
			return;
		pBoss->Get_TransformCom()->LookAt_Dir(vLook);
		CNavigationMgr::GetInstance()->Find_FirstCell(pBoss->Get_CurrLevel(), pBoss);

		Protocol::S_CREATE_OBJCECT tMonsterPkt;

		tMonsterPkt.set_iobjectid(pBoss->Get_ObjectID());
		tMonsterPkt.set_iobjecttype(pBoss->Get_ObjectType());
		tMonsterPkt.set_strname(CAsUtils::ToString(pBoss->Get_ModelName()));
		tMonsterPkt.set_ilayer(pBoss->Get_ObjectLayer());
		tMonsterPkt.set_ilevel(pBoss->Get_CurrLevel());

		tMonsterPkt.set_bcontroll(true);

		auto vPos = tMonsterPkt.mutable_vpos();
		vPos->Resize(3, 0.0f);
		Vec3 vPosition = pBoss->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
		CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
	}


	Safe_Release(pGameInstance);
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
	Vec3 vScale;
	vScale.x = 0.012f;
	vScale.y = 0.012f;
	vScale.z = 0.012f;

	m_pTransformCom->Set_Scale(vScale);
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
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 0.f;
	AnimationDesc.fAnimSpeed = 1.15f;
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//1
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//2
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_start");
	AnimationDesc.iStartFrame = 7;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 0.3f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//4
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//5
	AnimationDesc.strAnimName = TEXT("evt1_att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//6
	AnimationDesc.strAnimName = TEXT("att_battle_19_05");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	ActionDesc.strActionName = L"Action_Dead";
	CBT_Action* pDead = CValtan_BT_Dead_Server::Create(&ActionDesc);
	AnimationDesc.fRootDist = 1.5f;

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
	AnimationDesc.fChangeTime = 0.2f;
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
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("dmg_parts_start_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("dmg_parts_loop_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("dmg_parts_end_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_ArmorBreak";
	CBT_Action* pArmorBreak = CBoss_BT_ArmorBreak_Server::Create(&ActionDesc);
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfArmorBreak = CBoss_BT_IF_ArmorBreak_Server::Create(&DecoratorDesc);//죽었는가
	if (FAILED(pIfArmorBreak->AddChild(pArmorBreak)))
		return E_FAIL;


	ActionDesc.vecAnimations.clear();

	CBT_Composite::COMPOSITE_DESC CompositeDesc = {};
	CompositeDesc.pGameObject = this;
	CompositeDesc.pBehaviorTree = m_pBehaviorTree;
	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Hit = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Hit->AddChild(pIfDead))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfCountered))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfGroggy))) return E_FAIL;
	if (FAILED(pSelector_Hit->AddChild(pIfArmorBreak))) return E_FAIL;


	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfHit = CCommon_BT_IF_Hit_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.fRootDist = 0.f;
	AnimationDesc.strAnimName = TEXT("abn_groggy_1_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("abn_groggy_1_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fAnimSpeed = 0.3f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fAnimSpeed = 1.15f;

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	
	AnimationDesc.strAnimName = TEXT("walk_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.IsEndInstant = true;
	AnimationDesc.fMaxLoopTime = 0.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 3.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.fRootDist = 1.5f;

	ActionDesc.strActionName = L"Action_Respawn";
	CBT_Action* pSpawn = CValtan_BT_Spawn_Server::Create(&ActionDesc);
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
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


	AnimationDesc.strAnimName = TEXT("att_battle_12_05");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.IsEndInstant = true;
	AnimationDesc.fMaxLoopTime = 0.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	//65번 2페 시작 모션
	ActionDesc.strActionName = L"Action_Phase2";

	CBT_Action* pPhase2 = CValtan_BT_Phase2_Server::Create(&ActionDesc);
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio120 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>( pIf_Hp_UnderRatio120 )->Set_Ratio( 120.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio120->AddChild(pPhase2))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("respawn_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//65번 유령페이즈 시작 모션
	ActionDesc.strActionName = L"Action_Phase3";
	CBT_Action* pPhase3 = CValtan_BT_Phase3_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceLine16 = CBT_Composite::Create(&CompositeDesc);	
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio16 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio16)->Set_Ratio(16.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio16->AddChild(pSequenceLine16))) return E_FAIL;

	// Ready all AttackActions
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_4_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 0.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;

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
	ActionDesc.strActionName = L"Action_Rush";
	CBT_Action* pRush = CValtan_BT_Attack_Rush_Server::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_9_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_9_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;	
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_9_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 0.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//반격
	ActionDesc.strActionName = L"Action_CounterAttack";
	CBT_Action* pCounterAttack = CValtan_BT_Attack_CounterAttack_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_9_01_end-2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.1f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;
	ActionDesc.strActionName = L"Action_CounterAttack_1";
	CBT_Action* pCounterAttack_1 = CValtan_BT_Attack_CounterAttack_1_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelectorCounterAttack = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelectorCounterAttack->AddChild(pCounterAttack))) return E_FAIL;
	if (FAILED(pSelectorCounterAttack->AddChild(pCounterAttack_1))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.3f;
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
	ActionDesc.strActionName = L"Action_WhirlWind";
	CBT_Action* pWhirlWind = CValtan_BT_Attack_WhirlWind_Server::Create(&ActionDesc);

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
	ActionDesc.strActionName = L"Action_FistSmashExplosion";
	CBT_Action* pFistSmashExplosion_Server = CValtan_BT_Attack_FistSmashExplosion_Server::Create(&ActionDesc);

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
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_18_03-2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	// 어디로든지문 돌격
	ActionDesc.strActionName = L"Action_TeleportRush";
	CBT_Action* pTeleportRush = CValtan_BT_Attack_TeleportRush_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_15_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_15_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_15_03");
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_15_04");
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_15_05");
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	// 소용돌이 후 장판
	ActionDesc.strActionName = L"Action_TrunningPizza";
	CBT_Action* pTrunningPizza = CValtan_BT_Attack_TrunningPizza_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_19_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.IsEndInstant = true;
	AnimationDesc.fMaxLoopTime = 0.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	AnimationDesc.strAnimName = TEXT("att_battle_2_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//앞뒤앞
	ActionDesc.strActionName = L"Action_FrontBackWave";
	CBT_Action* pFrontBackWave = CValtan_BT_Attack_FrontBackWave_Server::Create(&ActionDesc);

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
	ActionDesc.strActionName = L"Action_DoubleSwingChopSwing";
	CBT_Action* pDoubleSwingChopSwing = CValtan_BT_Attack_DoubleSwingChopSwing_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_7_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_7_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 찍기
	ActionDesc.strActionName = L"Action_DoubleJumpWave";
	CBT_Action* pDoubleJumpWave = CValtan_BT_Attack_DoubleJumpWave_Server::Create(&ActionDesc);

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

	//점프 십자 찍기
	ActionDesc.strActionName = L"Action_JumpSeismic";
	CBT_Action* pJumpSeismic = CValtan_BT_Attack_JumpSeismic_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_1_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_1_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	//십자 찍기
	ActionDesc.strActionName = L"Action_SwingSeismic";
	CBT_Action* pSwingSeismic = CValtan_BT_Attack_SwingSeismic_Server::Create(&ActionDesc);

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
	ActionDesc.strActionName = L"Action_MultipleChop";
	CBT_Action* pMultipleChop = CValtan_BT_Attack_MultipleChop_Server::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("att_battle_2_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3번 찍기 페이즈3
	ActionDesc.strActionName = L"Action_MultipleChop_1";
	CBT_Action* pMultipleChop_1 = CValtan_BT_Attack_MultipleChop_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("evt1_att_battle_5_01_end");
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
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_02_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//침묵 찍기
	ActionDesc.strActionName = L"Action_SilenceChop";
	CBT_Action* pSilenceChop = CValtan_BT_Attack_SilenceChop_Server::Create(&ActionDesc);

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
	AnimationDesc.fMaxLoopTime = 10.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.bIsLoop = false;
	AnimationDesc.strAnimName = TEXT("att_battle_8_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 후 비
	ActionDesc.strActionName = L"Action_RainingAxe";
	CBT_Action* pRainingAxe = CValtan_BT_Attack_RainingAxe_Server::Create(&ActionDesc);

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
	AnimationDesc.fMaxLoopTime = 4.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//감금
	ActionDesc.strActionName = L"Action_Imprisonment";
	CBT_Action* pImprisonment = CValtan_BT_Attack_Imprisonment_Server::Create(&ActionDesc);

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
	AnimationDesc.fMaxLoopTime = 2.5f;
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
	//3카운터-1
	ActionDesc.strActionName = L"Action_TrippleCounterChop_1";
	CBT_Action* pTrippleCounterChop_1 = CValtan_BT_Attack_TrippleCounterChop_1_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_14_01");
	AnimationDesc.iStartFrame = 30;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
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
	//3카운터-2
	ActionDesc.strActionName = L"Action_TrippleCounterChop_2";
	CBT_Action* pTrippleCounterChop_2 = CValtan_BT_Attack_TrippleCounterChop_2_Server::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_14_01");
	AnimationDesc.iStartFrame = 30;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_14_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
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
	//3카운터-3
	ActionDesc.strActionName = L"Action_TrippleCounterChop_3";
	CBT_Action* pTrippleCounterChop_3 = CValtan_BT_Attack_TrippleCounterChop_3_Server::Create(&ActionDesc);

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceTrippleCounterChop = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSequenceTrippleCounterChop->AddChild(pTrippleCounterChop_1))) return E_FAIL;
	if (FAILED(pSequenceTrippleCounterChop->AddChild(pTrippleCounterChop_2))) return E_FAIL;
	if (FAILED(pSequenceTrippleCounterChop->AddChild(pTrippleCounterChop_3))) return E_FAIL;

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio47 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio47)->Set_Ratio(47.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio47->AddChild(pSequenceTrippleCounterChop))) return E_FAIL;


	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.IsEndInstant = true;
	AnimationDesc.fMaxLoopTime = 0.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	AnimationDesc.strAnimName = TEXT("att_battle_13_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_13_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.5f;
	AnimationDesc.IsEndInstant = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	AnimationDesc.strAnimName = TEXT("att_battle_13_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	

	AnimationDesc.strAnimName = TEXT("att_battle_21_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_21_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//잡기
	ActionDesc.strActionName = L"Action_RushGrab";
	CBT_Action* pRushGrab = CValtan_BT_Attack_RushGrab_Server::Create(&ActionDesc);

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
	ActionDesc.strActionName = L"Action_WipeAssult";
	CBT_Action* pWipeAssult = CValtan_BT_Attack_WipeAssult_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio130 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	if (FAILED(pIf_Hp_UnderRatio130->AddChild(pWipeAssult))) return E_FAIL;
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio130)->Set_Ratio(130.f / 160.f);

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
	AnimationDesc.fAnimSpeed = 1.0f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fAnimSpeed = 1.15f;

	AnimationDesc.strAnimName = TEXT("att_battle_12_06");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.5f;
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
	AnimationDesc.fMaxLoopTime = 4.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_12_11");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//110줄 임포스터
	ActionDesc.strActionName = L"Action_Imposter";
	CBT_Action* pImposter = CValtan_BT_Attack_Imposter_Server::Create(&ActionDesc);
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio110 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio110)->Set_Ratio(110.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio110->AddChild(pImposter))) return E_FAIL;
	//0
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_16_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//1
	AnimationDesc.strAnimName = TEXT("att_battle_16_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 3.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//2
	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//3
	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//4
	AnimationDesc.strAnimName = TEXT("att_battle_20_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 0.7f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//5
	AnimationDesc.strAnimName = TEXT("att_battle_20_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//6
	AnimationDesc.strAnimName = TEXT("att_battle_11_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//7
	AnimationDesc.strAnimName = TEXT("att_battle_16_05");
	AnimationDesc.iStartFrame = 20;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//8
	AnimationDesc.strAnimName = TEXT("att_battle_16_06");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	//9
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//10
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//11
	AnimationDesc.strAnimName = TEXT("att_battle_16_08");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//12
	AnimationDesc.strAnimName = TEXT("att_battle_16_09");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//13
	AnimationDesc.strAnimName = TEXT("att_battle_16_10");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//88번 지형파괴
	ActionDesc.strActionName = L"Action_FirstTerrainDestruction";
	CBT_Action* pFirstTerrainDestruction = CValtan_BT_Attack_FirstTerrainDestruction_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio88 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio88)->Set_Ratio(88.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio88->AddChild(pFirstTerrainDestruction))) return E_FAIL;

	ActionDesc.vecAnimations.clear();
	//0
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_16_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//1
	AnimationDesc.strAnimName = TEXT("att_battle_16_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 3.5f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//2
	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//3
	AnimationDesc.strAnimName = TEXT("att_battle_20_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//4
	AnimationDesc.strAnimName = TEXT("att_battle_20_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 0.7f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//5
	AnimationDesc.strAnimName = TEXT("att_battle_20_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//6
	AnimationDesc.strAnimName = TEXT("att_battle_11_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//7
	AnimationDesc.strAnimName = TEXT("att_battle_16_05");
	AnimationDesc.iStartFrame = 20;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//8
	AnimationDesc.strAnimName = TEXT("att_battle_16_06");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	//9
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//10
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//11
	AnimationDesc.strAnimName = TEXT("att_battle_16_08");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//12
	AnimationDesc.strAnimName = TEXT("att_battle_16_09");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 4.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//13
	AnimationDesc.strAnimName = TEXT("att_battle_16_10");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//30줄 지형파괴
	ActionDesc.strActionName = L"Action_SecondTerrainDestruction";
	CBT_Action* pSecondTerrainDestruction = CValtan_BT_Attack_SecondTerrainDestruction_Server::Create(&ActionDesc);
	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio30 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio30)->Set_Ratio(30.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio30->AddChild(pSecondTerrainDestruction))) return E_FAIL;
	//0
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_12_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//1
	AnimationDesc.strAnimName = TEXT("att_battle_12_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//2
	AnimationDesc.strAnimName = TEXT("att_battle_12_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3
	AnimationDesc.strAnimName = TEXT("att_battle_13_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//4
	AnimationDesc.strAnimName = TEXT("att_battle_17_start");
	AnimationDesc.iStartFrame = 36;
	AnimationDesc.fChangeTime = 0.3f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//5
	AnimationDesc.strAnimName = TEXT("att_battle_17_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 1.5f;
	AnimationDesc.IsEndInstant = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	//6
	AnimationDesc.strAnimName = TEXT("att_battle_13_02-1");
	AnimationDesc.iStartFrame = 9;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//7
	AnimationDesc.strAnimName = TEXT("att_battle_13_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	AnimationDesc.IsEndInstant = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	//8
	AnimationDesc.strAnimName = TEXT("att_battle_13_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;

	//9
	AnimationDesc.strAnimName = TEXT("att_battle_13_05-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.IsRootRot = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.IsRootRot = false;

	//10
	AnimationDesc.strAnimName = TEXT("att_battle_13_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//11
	AnimationDesc.strAnimName = TEXT("att_battle_13_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//12
	AnimationDesc.strAnimName = TEXT("att_battle_13_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	AnimationDesc.IsEndInstant = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;

	//13
	AnimationDesc.strAnimName = TEXT("att_battle_13_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;

	//14
	AnimationDesc.strAnimName = TEXT("att_battle_13_05-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.IsRootRot = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.IsRootRot = false;

	//65번 버러지패턴
	ActionDesc.strActionName = L"Action_BugSmash";
	CBT_Action* pBugSmash = CValtan_BT_Attack_BugSmash_Server::Create(&ActionDesc);

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Hp_UnderRatio65 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
	static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio65)->Set_Ratio(65.f / 160.f);
	if (FAILED(pIf_Hp_UnderRatio65->AddChild(pBugSmash))) return E_FAIL;

	//0
	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_18_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//1
	AnimationDesc.strAnimName = TEXT("att_battle_18_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//2
	AnimationDesc.strAnimName = TEXT("att_battle_8_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;	
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	//3
	AnimationDesc.strAnimName = TEXT("att_battle_18_03-1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//4
	AnimationDesc.strAnimName = TEXT("att_battle_19_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//5
	AnimationDesc.strAnimName = TEXT("att_battle_19_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//6
	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//7
	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//8
	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//9
	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//10
	AnimationDesc.strAnimName = TEXT("att_battle_19_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	
	//11
	AnimationDesc.strAnimName = TEXT("att_battle_19_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//12
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//13
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	//14
	AnimationDesc.strAnimName = TEXT("att_battle_5_01_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//15
	AnimationDesc.strAnimName = TEXT("att_battle_19_05");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//16
	AnimationDesc.strAnimName = TEXT("att_battle_19_06");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);


	//16번 연환파신권
	ActionDesc.strActionName = L"Action_ChainDestructionFist";
	CBT_Action* pChainDestructionFist = CValtan_BT_Attack_ChainDestructionFist_Server::Create(&ActionDesc);

	if (FAILED(pSequenceLine16->AddChild(pChainDestructionFist))) return E_FAIL;
	if (FAILED(pSequenceLine16->AddChild(pPhase3))) return E_FAIL;

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_21_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.IsRootRot = true;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.IsRootRot = false;

	AnimationDesc.strAnimName = TEXT("att_battle_21_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_21_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 3.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

	AnimationDesc.strAnimName = TEXT("att_battle_21_04");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//유령 잡기

	ActionDesc.strActionName = L"Action_GhostGrab";
	CBT_Action* pGhostGrab = CValtan_BT_Attack_GhostGrab_Server::Create(&ActionDesc);



	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.3f;
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
		if (FAILED(pRepeat_99->AddChild(pWipeAssult))) return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);
		if (m_bTest == false)
		{
			//원래 기본 패턴
			if (FAILED(pSequenceNormalAttack->AddChild(pWhirlWind)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pRush)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pJumpSeismic)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pRush)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pDoubleSwingChopSwing)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pRainingAxe)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pRush)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pRainingAxe)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pWhirlWind)))
				return E_FAIL;
			if (FAILED(pSequenceNormalAttack->AddChild(pRush)))
				return E_FAIL;
		}
		else
		{
			// 테스트용
			//if (FAILED(pSequenceNormalAttack->AddChild(pSequenceTrippleCounterChop)))
			//if (FAILED(pSequenceNormalAttack->AddChild(pRushGrab)))
			if (FAILED(pSequenceNormalAttack->AddChild(pSwingSeismic)))
				return E_FAIL;
 
		}


		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Armor = CValtan_BT_IF_Armor_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_Armor->AddChild(pSequenceNormalAttack))) return E_FAIL;


		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNoArmor = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceNoArmor->AddChild(pWhirlWind)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pJumpSeismic)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pDoubleSwingChopSwing)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pRush)))
			return E_FAIL;
		if (FAILED(pSequenceNoArmor->AddChild(pRainingAxe)))
			return E_FAIL;
		pSequenceNoArmor->ShuffleChild();
		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_NoArmor = CValtan_BT_IF_NoArmor_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_NoArmor->AddChild(pSequenceNoArmor))) return E_FAIL;
		
		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorAttack = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSelectorAttack->AddChild(pIf_Armor)))
			return E_FAIL;
		if (FAILED(pSelectorAttack->AddChild(pIf_NoArmor)))
			return E_FAIL;
		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
		if (FAILED(pIfAttacked->AddChild(pSelectorAttack)))
			return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorNear = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSelectorNear->AddChild(pIf_Hp_UnderRatio130)))
			return E_FAIL;
		if (FAILED(pSelectorNear->AddChild(pIf_Hp_UnderRatio120)))
			return E_FAIL;
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

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Hp_UnderRatio88_Skill = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
		static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio88_Skill)->Set_Ratio(88.f / 160.f);
		if (FAILED(pIf_Hp_UnderRatio88_Skill->AddChild(pRushGrab))) return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceSkill = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSequenceSkill->AddChild(pSelectorCounterAttack)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pTeleportRush)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pImprisonment)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pIf_Hp_UnderRatio88_Skill)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Skill = CCommon_BT_IF_Skill_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_Skill->AddChild(pSequenceSkill))) return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);
		if (FAILED(pSequenceNormalAttack->AddChild(pRush)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pWhirlWind)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pFistSmashExplosion_Server)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pSwingSeismic)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pFrontBackWave)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pDoubleSwingChopSwing)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pDoubleJumpWave)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pJumpSeismic)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pJumpSeismic)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pJumpSeismic)))
			return E_FAIL;
		if (FAILED(pSequenceNormalAttack->AddChild(pMultipleChop)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pSilenceChop)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pRainingAxe)))
			return E_FAIL;

		pSequenceNormalAttack->ShuffleChild();
		pSequenceNormalAttack->ShuffleChild();

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
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
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio110)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio88)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio65)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio47)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio30)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio16)))
			return E_FAIL;
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

		if (FAILED(pSequenceSkill->AddChild(pSelectorCounterAttack)))
			return E_FAIL;

		if (FAILED(pSequenceSkill->AddChild(pDoubleSwingChopSwing)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Skill = CCommon_BT_IF_Skill_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
		if (FAILED(pIf_Skill->AddChild(pSequenceSkill))) return E_FAIL;



		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
		CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSequenceNormalAttack->AddChild(pWhirlWind)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pFrontBackWave)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pSwingSeismic)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pDoubleJumpWave)))
			return E_FAIL;

		if (FAILED(pSequenceNormalAttack->AddChild(pMultipleChop_1)))
			return E_FAIL;
		pSequenceNormalAttack->ShuffleChild();
		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIfAttacked = CCommon_BT_IF_Attacked_Server::Create(&DecoratorDesc);//공격을 했는가?
		if (FAILED(pIfAttacked->AddChild(pSequenceNormalAttack)))
			return E_FAIL;




		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelectorNear = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelectorNear->AddChild(pIfAttacked)))
			return E_FAIL;
		if (FAILED(pSelectorNear->AddChild(pBattleIdle)))
			return E_FAIL;

		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Hp_UnderRatio39 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
		static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio39)->Set_Ratio(39.f / 41.f);
		if (FAILED(pIf_Hp_UnderRatio39->AddChild(pGhostGrab))) return E_FAIL;
		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Hp_UnderRatio26 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
		static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio26)->Set_Ratio(26.f / 41.f);
		if (FAILED(pIf_Hp_UnderRatio26->AddChild(pGhostGrab))) return E_FAIL;
		DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
		CBT_Decorator* pIf_Hp_UnderRatio13 = CValtan_BT_IF_Hp_UnderRatio::Create(&DecoratorDesc);
		static_cast<CValtan_BT_IF_Hp_UnderRatio*>(pIf_Hp_UnderRatio13)->Set_Ratio(13.f / 41.f);
		if (FAILED(pIf_Hp_UnderRatio13->AddChild(pGhostGrab))) return E_FAIL;

		CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
		CBT_Composite* pSelector_Within_Range = CBT_Composite::Create(&CompositeDesc);

		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio39)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio26)))
			return E_FAIL;
		if (FAILED(pSelector_Within_Range->AddChild(pIf_Hp_UnderRatio13)))
			return E_FAIL;
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
	if (FAILED(pRoot->AddChild(pSelector_Phase)))
		return E_FAIL;

	m_pBehaviorTree->SetRoot(pRoot);

	return S_OK;
}

void CBoss_Valtan_Server::Find_NearTarget(_float fTimeDelta)
{
	m_fScantime += fTimeDelta;
	if (m_fScantime > 1.f)
	{
		m_fScantime = 0.f;
		_float fDistance = 99999.f;
		CGameObject* pNearTarget = nullptr;

		vector<CGameObject*> pTargets = CGameInstance::GetInstance()->Find_GameObjects(m_iCurrLevel, (_uint)LAYER_TYPE::LAYER_PLAYER);
		if (pTargets.empty())
			m_pNearTarget = nullptr;
		else
		{
			for (auto& pObject : pTargets)
			{
				if (pObject->Is_Dead() || pObject->Is_Active() == false|| TEXT("Grabbed") == pObject->Get_ServerState() ||(pObject)->Get_ServerState()== L"Dead_Start"|| (pObject)->Get_ServerState() == L"Dead_End")
					continue;

				if (pNearTarget == nullptr)
				{
					pNearTarget = pObject;
					Vec3 vCallObjectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
					Vec3 vObjectPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
					fDistance = (vCallObjectPos - vObjectPos).Length();
					continue;
				}

				Vec3 vCallObjectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
				Vec3 vObjectPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);

				_float fNewDistance = (vObjectPos - vCallObjectPos).Length();

				if (fNewDistance < fDistance)
				{
					pNearTarget = pObject;
					fDistance = fNewDistance;
				}
			}

		}
		if (pNearTarget != m_pNearTarget)
		{
			m_pNearTarget = pNearTarget;
			Send_NearTarget();
		}
	}

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
