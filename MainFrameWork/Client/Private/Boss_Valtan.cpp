#include "stdafx.h"
#include "Boss_Valtan.h"
#include "BehaviorTree.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "Common_BT_Chase.h"
#include "Common_BT_Damage1.h"
#include "Common_BT_Damage2.h"
#include "Valtan_BT_Dead.h"
#include "Common_BT_Idle.h"
#include "Common_BT_BattleIdle.h"
#include "Common_BT_Move.h"
#include "Common_BT_Spawn.h"

#include "BehaviorTree.h"
#include "Valtan_BT_Spawn.h"
#include "Boss_BT_Counter.h"
#include "Boss_BT_Groggy.h"
#include "Valtan_BT_Attack_FirstTerrainDestruction.h"
#include "Valtan_BT_Attack_Rush.h"
#include "Valtan_BT_Attack_CounterAttack.h"
#include "Valtan_BT_Attack_WhirlWind.h"
#include "Valtan_BT_Attack_FistSmashExplosion.h"
#include "Valtan_BT_Attack_TeleportRush.h"
#include "Valtan_BT_Attack_TrunningPizza.h"
#include "Valtan_BT_Attack_FrontBackWave.h"
#include "Valtan_BT_Attack_DoubleSwingChopSwing.h"
#include "Valtan_BT_Attack_DoubleJumpWave.h"
#include "Valtan_BT_Attack_JumpSeismic.h"
#include "Valtan_BT_Attack_SwingSeismic.h"
#include "Valtan_BT_Attack_MultipleChop.h"
#include "Valtan_BT_Attack_SilenceChop.h"
#include "Valtan_BT_Attack_RainingAxe.h"
#include "Valtan_BT_Attack_Imprisonment.h"
#include "Valtan_BT_Attack_SecondTerrainDestruction.h"
#include "Valtan_BT_Attack_RushGrab.h"
#include "Valtan_BT_Attack_WipeAssult.h"
#include "Valtan_BT_Attack_Imposter.h"
#include "Valtan_BT_Attack_BugSmash.h"
#include "Valtan_BT_Attack_ChainDestructionFist.h"
#include "Valtan_BT_Attack_GhostGrab.h"
#include <Boss_BT_ArmorBreak.h>
#include <Valtan_BT_Phase2.h>
#include <Valtan_BT_Phase3.h>
#include <Valtan_BT_Attack_TrippleCounterChop_1.h>
#include <Valtan_BT_Attack_TrippleCounterChop_2.h>
#include <Valtan_BT_Attack_TrippleCounterChop_3.h>
#include <Valtan_BT_Attack_CounterAttack_1.h>
#include <Skill.h>
#include "ColliderOBB.h"
#include <Weapon_Boss_Valtan.h>


CBoss_Valtan::CBoss_Valtan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss(pDevice, pContext)
{
}

CBoss_Valtan::CBoss_Valtan(const CBoss_Valtan& rhs)
	: CBoss(rhs)
{
}

HRESULT CBoss_Valtan::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan::Initialize(void* pArg)
{
	m_eBossType =  BOSS_TYPE::VAlTAN;
	m_iMaxGroggyGauge = 2000;
	m_iGroggyGauge = m_iMaxGroggyGauge;
	m_iMaxHp = 8214139495;
	m_iHp = m_iMaxHp;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_tCullingSphere.Radius = 10.0f;

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
	m_pTransformCom->LookAt_Dir(Vec3(0.f, 0.f, 1.f));
	m_pTransformCom->Turn_Axis(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-1.f));
	m_bRender = false;
	m_pWeapon->Set_Render(false);
	Set_HpUIRender(false);
	Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
	m_fSummonTime = 0.f;
	m_vSummonPositions[0] = m_vSpawnPosition;
	m_vSummonPositions[0].z += 14.f;
	Vec3 vDir = { sqrtf(3.f),0.f,-1.f };
	vDir.Normalize();
	m_vSummonPositions[1] = m_vSpawnPosition + vDir * 14.f;
	vDir = { -sqrtf(3.f),0.f,-1.f };
	vDir.Normalize();
	m_vSummonPositions[2] = m_vSpawnPosition + vDir * 14.f;
	return S_OK;
}

void CBoss_Valtan::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (m_iPhase == 3 && m_iHp != 0)
	{
		m_fSummonTime += fTimeDelta;
		if (m_fSummonTime > 10.f)
		{
			m_fSummonTime = 0.f;
			for (_int i = 0; i < 3; i++)
			{
				wstring szComponentName = L"Boss_Valtan_RunningGhost";
				CBoss::MODELDESC Desc;
				Desc.strFileName = L"Boss_Valtan_RunningGhost";
				Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
				Desc.iObjectID = -1;
				Desc.iLevel = m_iCurrLevel;
				Desc.vPos = m_vSummonPositions[i];
				wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
				CBoss* pGhost = dynamic_cast<CBoss*>(CGameInstance::GetInstance()->Add_GameObject(Desc.iLevel, Desc.iLayer, szMonsterName, &Desc));
				_int iNext = i + 1;
				if (iNext == 3)
					iNext = 0;
				pGhost->Get_TransformCom()->LookAt_ForLandObject(m_vSummonPositions[iNext]);
				pGhost->Set_TargetPos(m_vSummonPositions[iNext]);
			}
		}
	}
}

void CBoss_Valtan::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_iPhase == 3)
		Move_to_SpawnPosition();
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Center_ToBone();
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_BOSS]->Set_Center_ToBone();
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS]->Set_Center_ToBone();
}

HRESULT CBoss_Valtan::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_int	iDissolve = false;
	if (!m_bRenderPostValtan && m_bDissolveIn || m_bDissolveOut)
	{
			_int   bDissolve = true;
			_int  bReverseDissovle = false;
			_float fDissolveDensity = 3.f;
			_float fDissolveValue = 0.1f;
			_float fDissolveColorValue = 0.02f;
			_int  bDissolveEmissive = true;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
				return E_FAIL;

			_float g_fDissolveAmount = m_fDissolvetime / m_fMaxDissolvetime;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &g_fDissolveAmount, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
				return E_FAIL;

	}
	if (m_bRimLight)
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &m_fRimLightColor, sizeof(_float))))
			return E_FAIL;

	Color vValtanBloom = Color(0.4f, 1.6f, 1.3f, 1.f);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vValtanBloom, sizeof(Color))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	if (m_iArmor > 1 && m_iPhase != 3&& !m_bRenderPostValtan)
	{
		_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::PART1]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pModelPartCom[(_uint)PARTS::PART1]->Render_SingleMesh(m_pShaderCom, j)))
				return E_FAIL;
		}
	}
	if (m_iArmor > 0 && m_iPhase != 3 && !m_bRenderPostValtan)
	{
		_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::PART2]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pModelPartCom[(_uint)PARTS::PART2]->Render_SingleMesh(m_pShaderCom, j)))
				return E_FAIL;
		}
	}
	if (m_iPhase == 3)
	{
		_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::GHOST]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pModelPartCom[(_uint)PARTS::GHOST]->Render_SingleMesh(m_pShaderCom, j)))
				return E_FAIL;
		}
	}
	else if (m_bRenderPostValtan)
	{
		if (m_bDissolveIn)
		{
			iDissolve = true;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &iDissolve, sizeof(_int))))
				return E_FAIL;
			_int   bDissolve = true;
			_int  bReverseDissovle = false;
			_float fDissolveDensity = 3.f;
			_float fDissolveValue = 0.1f;
			_float fDissolveColorValue = 0.02f;
			_int  bDissolveEmissive = true;
			
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
				return E_FAIL;

			_float fDissolveAmount = 1 - m_fDissolvetime / m_fMaxDissolvetime;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
				return E_FAIL;

			if (FAILED(m_pModelPartCom[(_uint)PARTS::POSTDEATH]->Render(m_pShaderCom)))
				return E_FAIL;

			iDissolve = true;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &iDissolve, sizeof(_int))))
				return E_FAIL;

			fDissolveAmount = (m_fDissolvetime / m_fMaxDissolvetime);
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pModelPartCom[(_uint)PARTS::BODY]->Render(m_pShaderCom)))
				return E_FAIL;

			if (FAILED(m_pModelPartCom[(_uint)PARTS::PART1]->Render(m_pShaderCom)))
				return E_FAIL;

			if (FAILED(m_pModelPartCom[(_uint)PARTS::PART2]->Render(m_pShaderCom)))
				return E_FAIL;

			iDissolve = false;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &iDissolve, sizeof(_int))))
				return E_FAIL;
		}
		else
		{
			_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::POSTDEATH]->Get_NumMeshes();

			for (_uint j = 0; j < iNumMeshes; ++j)
			{
				if (FAILED(m_pModelPartCom[(_uint)PARTS::POSTDEATH]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;
			}
		}

	}
	else
	{
		_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::BODY]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pModelPartCom[(_uint)PARTS::BODY]->Render_SingleMesh(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	if (m_bDissolveIn || m_bDissolveOut)
	{
		_int bDissolve = false;
		_int   bReverseDissovle = false;
		_float fDissolveDensity = 1.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.02f;
		_int   bDissolveEmissive = false;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		Vec4 vDissolveColor = Vec4::One;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vDissolveColor, sizeof(Vec4))))
			return E_FAIL;
	}
	_float fRimLightColor = 0.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

void CBoss_Valtan::Update_Dissolve(_float fTimeDelta)
{
	if (m_bDissolveOut)
	{
		m_fDissolvetime += fTimeDelta;
		if (m_fDissolvetime > m_fMaxDissolvetime)
		{
			m_fDissolvetime = m_fMaxDissolvetime;
			Set_Dead(true);
		}
	}
	else if (m_bDissolveIn)
	{
		m_fDissolvetime -= fTimeDelta;
		if (m_fDissolvetime < 0.0f)
		{
			m_fDissolvetime = m_fMaxDissolvetime;
			m_bDissolveIn = false;
			m_bRenderPostValtan = false;
		}
	}
}

HRESULT CBoss_Valtan::Ready_Coliders()
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
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderGrab"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS, pCollider);
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

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS]->Set_Radius(1.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS]->Set_Offset(Vec3(0.f, 1.3f, -1.1f));
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS]->Set_BoneIndex(m_pModelCom->Find_BoneIndex(TEXT("bip001-l-hand")));

	return S_OK;
}




void CBoss_Valtan::Load_GrabMatrix(Matrix& matWorld)
{
	_uint iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("bip001-l-hand"));
	Matrix BoneMatrix = m_pModelCom->Get_CombinedMatrix(iBoneIndex) * m_pModelCom->Get_PivotMatrix();

	BoneMatrix.Right(XMVector3Normalize(BoneMatrix.Right()));
	BoneMatrix.Up(XMVector3Normalize(BoneMatrix.Up()));
	BoneMatrix.Backward(XMVector3Normalize(BoneMatrix.Backward()));

	BoneMatrix *= m_pTransformCom->Get_WorldMatrix();

	matWorld.Translation(BoneMatrix.Translation() + BoneMatrix.Up() * 0.5f);
}

HRESULT CBoss_Valtan::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	wstring strComName = L"Prototype_Component_Model_Boss_Valtan";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::BODY])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_Boss_Valtan_Parts1";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Valtan_Parts1"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::PART1])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_Boss_Valtan_Parts2";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Valtan_Parts2"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::PART2])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_Boss_Valtan_Color_Ghost";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Valtan_Ghost"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::GHOST])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_Boss_Valtan_PostDeath";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Valtan_PostDeath"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::POSTDEATH])))
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

	m_vOriginScale.x = 1.f;
	m_vOriginScale.y = 1.f;
	m_vOriginScale.z = 1.f;

	m_pTransformCom->Set_Scale(m_vOriginScale);
	return S_OK;
}

HRESULT CBoss_Valtan::Ready_BehaviourTree()
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
	AnimationDesc.fAnimSpeed = 1.15f;
	AnimationDesc.fRootDist = 0.f;
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
	CBT_Action* pDead = CValtan_BT_Dead::Create(&ActionDesc);
	AnimationDesc.fRootDist = 1.5f;
	AnimationDesc.fAnimSpeed = 1.15f;

	ActionDesc.vecAnimations.clear();
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
	ActionDesc.strActionName = L"Action_Counter";

	CBT_Action* pCounter = CBoss_BT_Counter::Create(&ActionDesc);
	ActionDesc.vecAnimations.clear();



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
	CBT_Action* pGroggy = CBoss_BT_Groggy::Create(&ActionDesc);
	ActionDesc.vecAnimations.clear();


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
	CBT_Action* pArmorBreak = CBoss_BT_ArmorBreak::Create(&ActionDesc);



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
	AnimationDesc.fMaxLoopTime = 4.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;
	AnimationDesc.fRootDist = 1.5f;

	ActionDesc.strActionName = L"Action_Respawn";
	CBT_Action* pSpawn = CValtan_BT_Spawn::Create(&ActionDesc);

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

	ActionDesc.strActionName = L"Action_Phase2";
	CBT_Action* pPhase2 = CValtan_BT_Phase2::Create(&ActionDesc);
	

	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("respawn_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	//65줄 유령페이즈 시작 모션
	ActionDesc.strActionName = L"Action_Phase3";
	CBT_Action* pPhase3 = CValtan_BT_Phase3::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//돌진&1패 이후 카운터 돌진
	ActionDesc.strActionName = L"Action_Rush";
	CBT_Action* pRush = CValtan_BT_Attack_Rush::Create(&ActionDesc);


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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//반격
	ActionDesc.strActionName = L"Action_CounterAttack";
	CBT_Action* pCounterAttack = CValtan_BT_Attack_CounterAttack::Create(&ActionDesc);

	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_9_01_end-2");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.1f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.fRootDist = 1.5f;
	ActionDesc.strActionName = L"Action_CounterAttack_1";
	CBT_Action* pCounterAttack_1 = CValtan_BT_Attack_CounterAttack_1::Create(&ActionDesc);


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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//휠윈드
	ActionDesc.strActionName = L"Action_WhirlWind";
	CBT_Action* pWhirlWind = CValtan_BT_Attack_WhirlWind::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	// 양손 찍고 폭발
	ActionDesc.strActionName = L"Action_FistSmashExplosion";
	CBT_Action* pFistSmashExplosion = CValtan_BT_Attack_FistSmashExplosion::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	// 어디로든지문 돌격
	ActionDesc.strActionName = L"Action_TeleportRush";
	CBT_Action* pTeleportRush = CValtan_BT_Attack_TeleportRush::Create(&ActionDesc);

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
	CBT_Action* pTrunningPizza = CValtan_BT_Attack_TrunningPizza::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//앞뒤앞 찍기
	ActionDesc.strActionName = L"Action_FrontBackWave";
	CBT_Action* pFrontBackWave = CValtan_BT_Attack_FrontBackWave::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//도끼찍고 휘두르기
	ActionDesc.strActionName = L"Action_DoubleSwingChopSwing";
	CBT_Action* pDoubleSwingChopSwing = CValtan_BT_Attack_DoubleSwingChopSwing::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 찍기
	ActionDesc.strActionName = L"Action_DoubleJumpWave";
	CBT_Action* pDoubleJumpWave = CValtan_BT_Attack_DoubleJumpWave::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 십자 찍기
	ActionDesc.strActionName = L"Action_JumpSeismic";
	CBT_Action* pJumpSeismic = CValtan_BT_Attack_JumpSeismic::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//십자 찍기
	ActionDesc.strActionName = L"Action_SwingSeismic";
	CBT_Action* pSwingSeismic = CValtan_BT_Attack_SwingSeismic::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//4번 찍기
	ActionDesc.strActionName = L"Action_MultipleChop";
	CBT_Action* pMultipleChop = CValtan_BT_Attack_MultipleChop::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3번 찍기 페이즈3
	ActionDesc.strActionName = L"Action_MultipleChop_1";
	CBT_Action* pMultipleChop_1 = CValtan_BT_Attack_MultipleChop::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//침묵 찍기
	ActionDesc.strActionName = L"Action_SilenceChop";
	CBT_Action* pSilenceChop = CValtan_BT_Attack_SilenceChop::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//점프 후 비
	ActionDesc.strActionName = L"Action_RainingAxe";
	CBT_Action* pRainingAxe = CValtan_BT_Attack_RainingAxe::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//감금
	ActionDesc.strActionName = L"Action_Imprisonment";
	CBT_Action* pImprisonment = CValtan_BT_Attack_Imprisonment::Create(&ActionDesc);


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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3카운터-1
	ActionDesc.strActionName = L"Action_TrippleCounterChop_1";
	CBT_Action* pTrippleCounterChop_1 = CValtan_BT_Attack_TrippleCounterChop_1::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3카운터-2
	ActionDesc.strActionName = L"Action_TrippleCounterChop_2";
	CBT_Action* pTrippleCounterChop_2 = CValtan_BT_Attack_TrippleCounterChop_2::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//3카운터-3
	ActionDesc.strActionName = L"Action_TrippleCounterChop_3";
	CBT_Action* pTrippleCounterChop_3 = CValtan_BT_Attack_TrippleCounterChop_3::Create(&ActionDesc);



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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//잡기
	ActionDesc.strActionName = L"Action_RushGrab";
	CBT_Action* pRushGrab = CValtan_BT_Attack_RushGrab::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//130줄 전멸기
	ActionDesc.strActionName = L"Action_WipeAssult";
	CBT_Action* pWipeAssult = CValtan_BT_Attack_WipeAssult::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//110줄 임포스터
	ActionDesc.strActionName = L"Action_Imposter";
	CBT_Action* pImposter = CValtan_BT_Attack_Imposter::Create(&ActionDesc);

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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//88번 지형파괴
	ActionDesc.strActionName = L"Action_FirstTerrainDestruction";
	CBT_Action* pFirstTerrainDestruction = CValtan_BT_Attack_FirstTerrainDestruction::Create(&ActionDesc);


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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//30줄 지형파괴
	ActionDesc.strActionName = L"Action_SecondTerrainDestruction";
	CBT_Action* pSecondTerrainDestruction = CValtan_BT_Attack_SecondTerrainDestruction::Create(&ActionDesc);

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
	CBT_Action* pBugSmash = CValtan_BT_Attack_BugSmash::Create(&ActionDesc);


	ActionDesc.vecAnimations.clear();
	AnimationDesc.strAnimName = TEXT("att_battle_18_01");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_18_02");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);

	AnimationDesc.strAnimName = TEXT("att_battle_8_01_loop");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.f;
	AnimationDesc.iChangeFrame = 0;
	AnimationDesc.bIsLoop = true;
	AnimationDesc.fMaxLoopTime = 2.f;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	AnimationDesc.bIsLoop = false;

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

	//12
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
	ActionDesc.strActionName = L"Action_ChainDestructionFist";
	CBT_Action* pChainDestructionFist = CValtan_BT_Attack_ChainDestructionFist::Create(&ActionDesc);


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

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.2f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	//유령 잡기
	ActionDesc.strActionName = L"Action_GhostGrab";
	CBT_Action* pValtan_BT_Attack_GhostGrab = CValtan_BT_Attack_GhostGrab::Create(&ActionDesc);



	ActionDesc.vecAnimations.clear();

	AnimationDesc.strAnimName = TEXT("idle_battle_1");
	AnimationDesc.iStartFrame = 0;
	AnimationDesc.fChangeTime = 0.3f;
	AnimationDesc.iChangeFrame = 0;
	ActionDesc.vecAnimations.push_back(AnimationDesc);
	ActionDesc.strActionName = L"Action_BattleIdle";
	CBT_Action* pBattleIdle = CCommon_BT_BattleIdle::Create(&ActionDesc);

	m_pBehaviorTree->Init_PreviousAction(L"Action_BattleIdle");
	return S_OK;
}

CBoss* CBoss_Valtan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Valtan* pInstance = new CBoss_Valtan(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Valtan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Valtan::Clone(void* pArg)
{
	CBoss_Valtan* pInstance = new CBoss_Valtan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_Valtan" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_Valtan::Free()
{
	__super::Free();
}
