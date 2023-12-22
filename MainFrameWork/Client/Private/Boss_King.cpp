#include "stdafx.h"
#include "GameInstance.h"
#include "Boss_King.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Pool.h"
#include "Common_BT_Attack1.h"
#include "Zombie_BT_Attack2.h"
#include "Common_BT_Chase.h"
#include "Common_BT_DamageLeft.h"
#include "Common_BT_DamageRight.h"
#include "Common_BT_Dead.h"
#include "Common_BT_Idle.h"
#include "Common_BT_BattleIdle.h"
#include "Common_BT_Move.h"
#include "Common_BT_Spawn.h"
#include "BehaviorTree.h"
#include "BindShaderDesc.h"
#include "King_BT_Attack_Attack2.h"
#include "King_BT_Attack_Attack3.h"
#include "King_BT_Attack_Attack4.h"
#include "King_BT_Attack_Charge_Swing.h"
#include "King_BT_Attack_Erruption.h"
#include "PartObject.h"

CBoss_King::CBoss_King(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss(pDevice, pContext)
{
}

CBoss_King::CBoss_King(const CBoss_King& rhs)
	: CBoss(rhs)
{
}

HRESULT CBoss_King::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
    return S_OK;
}

HRESULT CBoss_King::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_fRootTargetDistance = 1.f;
	return S_OK;
}

void CBoss_King::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pWeapon != nullptr)
		m_pWeapon->Tick(fTimeDelta);

}

void CBoss_King::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_pWeapon != nullptr)
		m_pWeapon->LateTick(fTimeDelta);
}


HRESULT CBoss_King::Render()
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

HRESULT CBoss_King::Render_ShadowDepth()
{
	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "ShadowPass")))
			return S_OK;
	}

	return S_OK;
}


void CBoss_King::Set_SlowMotion(_bool bSlow)
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



HRESULT CBoss_King::Ready_Components()
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


	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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

	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	m_pWeapon = dynamic_cast<CPartObject*>( pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Weapon_Boss_King"), &PartDesc_Weapon));
	if (nullptr == m_pWeapon)
		return E_FAIL;

	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

HRESULT CBoss_King::Ready_BehaviourTree()
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
	CBT_Action* pDead = CCommon_BT_Dead::Create(&ActionDesc);



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
	ActionDesc.strActionName = L"Action_Damage";
	CBT_Action* pDamageLeft = CCommon_BT_DamageLeft::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_7_01");
	AnimationDesc.iStartFrame = 60;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 60;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Respawn";
	CBT_Action* pSpawn = CCommon_BT_Spawn::Create(&ActionDesc);



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

	AnimationDesc.strAnimName = TEXT("att_battle_2_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_2_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.1f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_2_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.1f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Charge_Swing";
	ActionDesc.iLoopAnimationIndex = 1;
	ActionDesc.fMaxLoopTime = 1.f;
	CBT_Action* pSkill1 = CKing_BT_Attack_Charge_Swing::Create(&ActionDesc);
	ActionDesc.iLoopAnimationIndex = -1;
	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_10_start");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_10_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_10_end");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Erruption";
	CBT_Action* pSkill2 = CKing_BT_Attack_Erruption::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_1_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fAnimSpeed = 1.4f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack1";
	CBT_Action* pAttack1 = CCommon_BT_Attack1::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_3_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fAnimSpeed = 1.4f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fAnimSpeed = 1.2f;
	AnimationDesc.strAnimName = TEXT("att_battle_3_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.strAnimName = TEXT("att_battle_3_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Attack2";
	CBT_Action* pAttack2 = CKing_BT_Attack_Attack2::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_4_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_4_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_4_03");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fAnimSpeed = 1.4f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fAnimSpeed = 1.2f;
	ActionDesc.strActionName = L"Action_Attack3";
	CBT_Action* pAttack3 = CKing_BT_Attack_Attack3::Create(&ActionDesc);
	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("att_battle_8_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_8_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.5f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.fAnimSpeed = 1.4f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fAnimSpeed = 1.2f;
	ActionDesc.strActionName = L"Action_Attack4";
	CBT_Action* pAttack4 = CKing_BT_Attack_Attack4::Create(&ActionDesc);



	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CCommon_BT_BattleIdle::Create(&ActionDesc);





	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Idle_0";
	CBT_Action* pIdle_0 = CCommon_BT_Idle::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("walk_normal_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_Move";
	CBT_Action* pMove = CCommon_BT_Move::Create(&ActionDesc);


	m_pBehaviorTree->Init_PreviousAction(L"Action_Respawn");
	return S_OK;
}


CBoss_King* CBoss_King::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_King* pInstance = new CBoss_King(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_King");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_King::Clone(void* pArg)
{
	CBoss_King* pInstance = new CBoss_King(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_King");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_King::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBehaviorTree);
}
