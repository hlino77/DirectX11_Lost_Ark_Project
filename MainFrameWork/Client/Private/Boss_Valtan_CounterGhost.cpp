#include "stdafx.h"
#include "Boss_Valtan_CounterGhost.h"
#include "BehaviorTree.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "PartObject.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "Common_BT_Chase.h"
#include "Common_BT_Damage1.h"
#include "Common_BT_Damage2.h"
#include "Common_BT_Dead.h"
#include "Common_BT_Idle.h"
#include "Common_BT_BattleIdle.h"
#include "Common_BT_Move.h"
#include "Common_BT_Spawn.h"
#include "BehaviorTree.h"
#include <Skill.h>
#include "ColliderOBB.h"
#include <Valtan_BT_Attack_Attack11.h>
#include <Valtan_BT_Attack_Attack1.h>
#include <Valtan_BT_BattleIdle.h>
#include <Valtan_BT_Attack_Attack9.h>
#include "RigidBody.h"
#include "NavigationMgr.h"

CBoss_Valtan_CounterGhost::CBoss_Valtan_CounterGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss(pDevice, pContext)
{
}

CBoss_Valtan_CounterGhost::CBoss_Valtan_CounterGhost(const CBoss_Valtan_CounterGhost& rhs)
	: CBoss(rhs)
{
}

HRESULT CBoss_Valtan_CounterGhost::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan_CounterGhost::Initialize(void* pArg)
{
	m_iMaxGroggyGauge = 1000;
	m_iGroggyGauge = m_iMaxGroggyGauge;
	m_iMaxHp = 1;
	m_iHp = m_iMaxHp;
	m_bDummy = true;
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_iCurrLevel = Desc->iLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_pRigidBody->Set_Gravity(false);
	 
	m_pTransformCom->LookAt_Dir( Desc->vPos);
	m_vSpawnPosition = CGameInstance::GetInstance()->Find_GameObject(m_iCurrLevel, (_uint)LAYER_TYPE::LAYER_BOSS, L"Boss_Valtan")->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSpawnPosition);
	CNavigationMgr::GetInstance()->Find_FirstCell(m_iCurrLevel, this);

	if (FAILED(Ready_DissolveTexture()))
		return E_FAIL;
	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	m_tCullingSphere.Radius = 2.0f;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;
	if (!m_bDummy)
		if (FAILED(Ready_HpUI()))
			return E_FAIL;
	m_vecAttackRanges.clear();
	m_fMoveSpeed = 4.f;
	m_vecAttackRanges.push_back(2.f);
	m_vecAttackRanges.push_back(3.5f);
	m_IsSuperArmor = true;
	m_fAttackRange = m_vecAttackRanges[0];
	m_fRootTargetDistance = 0.f;
	m_fNoticeRange = 150.f;
	m_iArmor = 2;
	m_iPhase = 1;
	m_fFontScale = 0.55f;
	Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
	return S_OK;
}

void CBoss_Valtan_CounterGhost::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pWeapon != nullptr)
		m_pWeapon->Tick(fTimeDelta);
}

void CBoss_Valtan_CounterGhost::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_iPhase == 3)
		Move_to_SpawnPosition();
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Center_ToBone();
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_BOSS]->Set_Center_ToBone();
	if (m_pWeapon != nullptr)
		m_pWeapon->LateTick(fTimeDelta);
}

HRESULT CBoss_Valtan_CounterGhost::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;
	_int	iDissolve = false;
	if (m_bDissolveIn || m_bDissolveOut)
	{
		iDissolve = true;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &iDissolve, sizeof(_int))))
			return E_FAIL;

		_float g_fDissolveAmount = m_fDissolvetime / m_fMaxDissolvetime;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &g_fDissolveAmount, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
			return E_FAIL;
	}


	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::GHOST]->Get_NumMeshes();

	for (_uint j = 0; j < iNumMeshes; ++j)
	{
		if (FAILED(m_pModelPartCom[(_uint)PARTS::GHOST]->Render_SingleMesh(m_pShaderCom, j)))
			return E_FAIL;

	}
	iDissolve = false;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &iDissolve, sizeof(_int))))
		return E_FAIL;
	return S_OK;
}


HRESULT CBoss_Valtan_CounterGhost::Ready_Coliders()
{

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS;
		CSphereCollider* pCollider = nullptr;
	
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderBody"), (CComponent**)&pCollider, &tColliderInfo)))
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
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, pCollider);
	}
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_BOSS]->Set_Radius(1.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_BOSS]->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_BOSS]->Set_BoneIndex(m_pModelCom->Find_BoneIndex(TEXT("bip001-spine")));

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Radius(2.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_BoneIndex(m_pModelCom->Find_BoneIndex(TEXT("b_wp_r_01")));

return S_OK;
}


void CBoss_Valtan_CounterGhost::Reserve_WeaponAnimation(wstring strAnimName, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fAnimspeed)
{
	if (m_pWeapon->Get_ModelCom()->Initailize_FindAnimation(strAnimName, fAnimspeed) > m_pWeapon->Get_ModelCom()->Get_MaxAnimIndex())
		strAnimName = L"att_battle_8_01_loop";
	m_pWeapon->Get_ModelCom()->Reserve_NextAnimation(m_pWeapon->Get_ModelCom()->Find_AnimIndex(strAnimName), fChangeTime,
		iStartFrame, iChangeFrame);
	m_pWeapon->Get_ModelCom()->Set_Anim_Speed(m_pWeapon->Get_ModelCom()->Find_AnimIndex(strAnimName) ,fAnimspeed);
}

void CBoss_Valtan_CounterGhost::Set_Weapon_Render(_bool IsRender)
{
	m_pWeapon->Set_Render(IsRender);
}

HRESULT CBoss_Valtan_CounterGhost::Ready_Components()
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

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Boss_Valtan";
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	strComName = L"Prototype_Component_Model_Boss_Valtan_Ghost";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Valtan_Ghost"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::GHOST])))
		return E_FAIL;

	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_r_01"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	m_pWeapon = dynamic_cast<CPartObject*>(CGameInstance::GetInstance()->Clone_GameObject(TEXT("Prototype_GameObject_Weapon_Boss_Valtan"), &PartDesc_Weapon));
	if (nullptr == m_pWeapon)
		return E_FAIL;

	m_vOriginScale.x = 1.2f;
	m_vOriginScale.y = 1.2f;
	m_vOriginScale.z = 1.2f;

	m_pTransformCom->Set_Scale(m_vOriginScale);
	return S_OK;
}

HRESULT CBoss_Valtan_CounterGhost::Ready_BehaviourTree()
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
	CBT_Action* pDead = CCommon_BT_Dead::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.3f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CValtan_BT_BattleIdle::Create(&ActionDesc);

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
	CBT_Action* pAttack1 = CValtan_BT_Attack_Attack1::Create(&ActionDesc);

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
	CBT_Action* pAttack9 = CValtan_BT_Attack_Attack9::Create(&ActionDesc);

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
	CBT_Action* pAttack11 = CValtan_BT_Attack_Attack11::Create(&ActionDesc);

	m_pBehaviorTree->Init_PreviousAction(L"Action_BattleIdle", 0);
	m_bRender = false;
	return S_OK;
}

CBoss* CBoss_Valtan_CounterGhost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Valtan_CounterGhost* pInstance = new CBoss_Valtan_CounterGhost(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Valtan_CounterGhost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Valtan_CounterGhost::Clone(void* pArg)
{
	CBoss_Valtan_CounterGhost* pInstance = new CBoss_Valtan_CounterGhost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_Valtan_CounterGhost" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_Valtan_CounterGhost::Free()
{
	__super::Free();
	for (auto& Collider : m_Coliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider.second);
}
