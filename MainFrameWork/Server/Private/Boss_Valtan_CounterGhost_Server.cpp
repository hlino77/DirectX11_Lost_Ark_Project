#include "stdafx.h"
#include "Boss_Valtan_CounterGhost_Server.h"
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
#include "Valtan_BT_BattleIdle_Server.h"
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
#include "Valtan_BT_Attack_Attack17_1_Server.h"
#include "Valtan_BT_Attack_Attack17_2_Server.h"
#include "Valtan_BT_Attack_Attack17_3_Server.h"
#include "Valtan_BT_Attack_Attack19_Server.h"
#include "Valtan_BT_Attack_Attack20_Server.h"
#include "Valtan_BT_Attack_Attack21_Server.h"
#include "Valtan_BT_Attack_Attack22_Server.h"
#include "Valtan_BT_Attack_Attack23_Server.h"
#include "Valtan_BT_Attack_Attack24_Server.h"
#include "Valtan_BT_IF_Phase2_Server.h"
#include "Valtan_BT_IF_Phase3_Server.h"
#include <Valtan_BT_IF_Hp_UnderRatio.h>
#include <Valtan_BT_IF_Armor_Server.h>
#include <Valtan_BT_Repeat_Server.h>
#include <Boss_BT_ArmorBreak_Server.h>
#include <Boss_BT_IF_ArmorBreak_Server.h>
#include <Valtan_BT_Phase3_Server.h>
#include <Valtan_BT_Phase2_Server.h>
#include <Valtan_BT_Attack_Attack2_1_Server.h>
#include <Player_Server.h>



CBoss_Valtan_CounterGhost_Server::CBoss_Valtan_CounterGhost_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss_Server(pDevice, pContext)
{
}

CBoss_Valtan_CounterGhost_Server::CBoss_Valtan_CounterGhost_Server(const CBoss_Valtan_CounterGhost_Server& rhs)
	: CBoss_Server(rhs)
{
}

HRESULT CBoss_Valtan_CounterGhost_Server::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan_CounterGhost_Server::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_vecAttackRanges.clear();
	m_fMoveSpeed = 4.f;;
	m_IsSuperArmor = true;
	m_iArmor = 0;
	m_iPhase = 1;
	m_fRootTargetDistance = 0.f;
	m_iMaxHp = 1;
	m_iHp = m_iMaxHp;
	m_iMaxArmorDurability = (_uint)(0);
	m_iArmorDurability = m_iMaxArmorDurability;
	m_fNoticeRange = 150.f;
	m_iMaxGroggyGauge = 0;
	m_iGroggyGauge = m_iMaxGroggyGauge;
	m_bDummy = true;
	return S_OK;
}

void CBoss_Valtan_CounterGhost_Server::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fSkillCoolDown = 0.f;
}

void CBoss_Valtan_CounterGhost_Server::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBoss_Valtan_CounterGhost_Server::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}


void CBoss_Valtan_CounterGhost_Server::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	WRITE_LOCK
		if (!m_IsInvincible)
		{
			if ((_uint)STATUSEFFECT::COUNTER == iStatusEffect && m_IsCounterSkill)
			{
				m_bSkipAction = true;
				m_IsHit = true;
				m_IsCounterSkill = false;
				m_IsCountered = true;
				m_iHp = 0;
			}
			Send_Collision(iDamage, vHitPos, iStatusEffect, fForce, fDuration, iGroggy);
		}
}

void CBoss_Valtan_CounterGhost_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_Valtan_CounterGhost_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_Valtan_CounterGhost_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}



void CBoss_Valtan_CounterGhost_Server::Set_Colliders(_float fTimeDelta)
{
	__super::Set_Colliders(fTimeDelta);
}

HRESULT CBoss_Valtan_CounterGhost_Server::Ready_Coliders()
{
	if (FAILED(__super::Ready_Coliders()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBoss_Valtan_CounterGhost_Server::Ready_Components()
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

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Boss_Valtan";
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	Safe_Release(pGameInstance);
	Vec3 vScale;
	vScale.x = 0.012f;
	vScale.y = 0.012f;
	vScale.z = 0.012f;

	m_pTransformCom->Set_Scale(vScale);
	return S_OK;
}

HRESULT CBoss_Valtan_CounterGhost_Server::Ready_BehaviourTree()
{
	if (FAILED(__super::Ready_BehaviourTree()))
		return E_FAIL;

	CBT_Action::ACTION_DESC ActionDesc = {};
	ActionDesc.pBehaviorTree = m_pBehaviorTree;
	ActionDesc.pGameObject = this;
	ActionDesc.vecAnimations.clear();
	CBT_Action::ANIMATION_DESC AnimationDesc = {};
	AnimationDesc.strAnimName = TEXT("dmg_critical_start_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 1.5f;
	AnimationDesc.fAnimSpeed = 1.15f;
	AnimationDesc.bIsLoop = false;
	AnimationDesc.IsEndInstant = false;
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

	CBT_Composite::COMPOSITE_DESC CompositeDesc = {};
	CompositeDesc.pGameObject = this;
	CompositeDesc.pBehaviorTree = m_pBehaviorTree;
	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pSelector_Hit = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pSelector_Hit->AddChild(pIfDead))) return E_FAIL;

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIfHit = CCommon_BT_IF_Hit_Server::Create(&DecoratorDesc);//맞았는가
	if (FAILED(pIfHit->AddChild(pSelector_Hit)))
		return E_FAIL;

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.3f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CValtan_BT_BattleIdle_Server::Create(&ActionDesc);

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
	ActionDesc.strActionName = L"Action_Attack1";
	CBT_Action* pAttack1 = CValtan_BT_Attack_Attack1_Server::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_7_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fRootDist = 0.f;
	AnimationDesc.fAnimSpeed = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;
	AnimationDesc.fAnimSpeed = 1.15f;

	AnimationDesc.strAnimName = TEXT("att_battle_7_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 찍기
	ActionDesc.strActionName = L"Action_Attack9";
	CBT_Action* pAttack9 = CValtan_BT_Attack_Attack9_Server::Create(&ActionDesc);


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
	ActionDesc.strActionName = L"Action_Attack11";
	CBT_Action* pAttack11 = CValtan_BT_Attack_Attack11_Server::Create(&ActionDesc);




	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SEQUENCE;
	CBT_Composite* pSequenceNormalAttack = CBT_Composite::Create(&CompositeDesc);


	if (FAILED(pSequenceNormalAttack->AddChild(pAttack1)))
		return E_FAIL;
	
	if (FAILED(pSequenceNormalAttack->AddChild(pBattleIdle)))
		return E_FAIL;

	if (CGameInstance::GetInstance()->Random_Coin(0.5f))
	{
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack9)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pSequenceNormalAttack->AddChild(pAttack11)))
			return E_FAIL;
	}

	DecoratorDesc.eDecoratorType = CBT_Decorator::DecoratorType::IF;
	CBT_Decorator* pIf_Phase1 = CValtan_BT_IF_Phase1_Server::Create(&DecoratorDesc);//플레이어와 가까운가?
	if (FAILED(pIf_Phase1->AddChild(pSequenceNormalAttack)))
		return E_FAIL;

	CompositeDesc.eCompositeType = CBT_Composite::CompositeType::SELECTOR;
	CBT_Composite* pRoot = CBT_Composite::Create(&CompositeDesc);
	if (FAILED(pRoot->AddChild(pIfHit)))
		return E_FAIL;
	if (FAILED(pRoot->AddChild(pIf_Phase1)))
		return E_FAIL;

	m_pBehaviorTree->SetRoot(pRoot);

	return S_OK;
}

void CBoss_Valtan_CounterGhost_Server::Find_NearTarget(_float fTimeDelta)
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
			for (auto& Object : pTargets)
			{
				_bool	Is_Grabbed = false;
				_int iObjectID = Object->Get_ObjectID();
				for (auto iID : m_vecGrabbedPlayerIDs)
					if (iID == iObjectID)
					{
						Is_Grabbed = true;
						break;
					}
				if (Object->Is_Dead() || Object->Is_Active() == false|| Is_Grabbed==true ||(Object)->Get_ServerState()== L"Dead_Start"|| (Object)->Get_ServerState() == L"Dead_End")
					continue;

				if (pNearTarget == nullptr)
				{
					pNearTarget = Object;
					Vec3 vCallObjectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
					Vec3 vObjectPos = Object->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
					fDistance = (vCallObjectPos - vObjectPos).Length();
					continue;
				}

				Vec3 vCallObjectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
				Vec3 vObjectPos = Object->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);

				_float fNewDistance = (vObjectPos - vCallObjectPos).Length();

				if (fNewDistance < fDistance)
				{
					pNearTarget = Object;
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

void CBoss_Valtan_CounterGhost_Server::Clear_GrabbedPlayerIDs()
{
	m_vecGrabbedPlayerIDs.clear();
}

CBoss_Server* CBoss_Valtan_CounterGhost_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Valtan_CounterGhost_Server* pInstance = new CBoss_Valtan_CounterGhost_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Valtan_CounterGhost_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Valtan_CounterGhost_Server::Clone(void* pArg)
{
	CBoss_Valtan_CounterGhost_Server* pInstance = new CBoss_Valtan_CounterGhost_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_Valtan_CounterGhost_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_Valtan_CounterGhost_Server::Free()
{
	__super::Free();
}
