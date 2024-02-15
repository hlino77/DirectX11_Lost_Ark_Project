#include "stdafx.h"
#include "GameInstance.h"
#include "Boss.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Pool.h"
#include "Zombie_BT_Attack2.h"
#include "Common_BT_Attack1.h"
#include "Common_BT_Chase.h"
#include "Common_BT_Damage1.h"
#include "Common_BT_Damage2.h"
#include "Common_BT_Dead.h"
#include "Common_BT_Idle.h"
#include "Common_BT_BattleIdle.h"
#include "Common_BT_Move.h"
#include "Common_BT_Spawn.h"
#include "BT_Composite.h"
#include "BehaviorTree.h"
#include "Projectile.h"
#include "UI_Boss_Hp.h"
#include "UI_Manager.h"
#include <Camera_Player.h>
#include "ServerSessionManager.h"
#include "Player.h"
#include "Item.h"
#include "Item_Manager.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
}

CBoss::CBoss(const CBoss& rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss::Initialize_Prototype()
{
	m_strObjectTag = L"Boss";
	m_iObjType = OBJ_TYPE::BOSS;
    return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_iCurrLevel = Desc->iLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_pRigidBody->Set_Gravity(false);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);
	m_vSpawnPosition = Desc->vPos;
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

	return S_OK;
}

void CBoss::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_pWeapon != nullptr)
		m_pWeapon->Tick(fTimeDelta);
	{
		/*if (KEY_HOLD(KEY::CTRL) && KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::Q))
			m_bTest = !m_bTest;
		if (KEY_TAP(KEY::P))
			cout << Get_Target_Distance() << endl << m_vDoughnutRadii.x << ": " << m_vDoughnutRadii.y << endl;
		if (KEY_HOLD(KEY::UP_ARROW))
			m_vDoughnutRadii.x += fTimeDelta;
		if (KEY_HOLD(KEY::DOWN_ARROW))
			m_vDoughnutRadii.x -= fTimeDelta;
		if (KEY_HOLD(KEY::LEFT_ARROW))
			m_vDoughnutRadii.y -= fTimeDelta;
		if (KEY_HOLD(KEY::RIGHT_ARROW))
			m_vDoughnutRadii.y += fTimeDelta;*/
	}
}

void CBoss::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_IsCounterSkill)
		Set_RimLight(0.05f, 0.9f);
	if (m_pWeapon != nullptr && m_bRender)
	{
		m_pWeapon->LateTick(fTimeDelta);
		if (m_IsCounterSkill)
			m_pWeapon->Set_RimLight(0.05f, 0.9f);
	}
}

HRESULT CBoss::Render()
{
	return 	__super::Render();
}

void CBoss::Set_SlowMotion(_bool bSlow)
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

HRESULT CBoss::Render_ShadowDepth()
{
	return __super::Render_ShadowDepth();
}

void CBoss::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	m_iHp -= iDamage;
	if (m_iHp < 1.f && m_iPhase == 2)
		m_iHp = 1;
	if (fDuration==1.f)
		m_iGroggyCount -= iGroggy;
	else
		m_iGroggyGauge -= iGroggy;
	if (m_iHp < 1)
		m_iHp = 0;


	if (m_iGroggyCount != fForce)
		m_iGroggyCount = fForce;
	if (m_iGroggyGauge != iStatusEffect)
		m_iGroggyGauge = iStatusEffect;
	if (m_iGroggyGauge < 1)
		m_iGroggyGauge = 0;
	if (m_iGroggyCount < 1)
	{
		m_iGroggyCount = 0;
		m_iMaxGroggyCount = 0;
	}
	if (iDamage == 0.f)
	{
		if (vHitPos.x == (_float)STATUSEFFECT::GROGGY)
		{

			Set_RimLight(0.05f, 1.f);
		}
	}
	else
		Set_RimLight(0.05f, 1.f);

}

void CBoss::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			_int iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			Vec3 vPos = {};
			if (static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().bUseProjPos)
			{
				vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			else
			{
				vPos = static_cast<CProjectile*>(pOther->Get_Owner())->Get_AttackOwner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			_float fForce = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion;
			 iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			int iCritical = rand() % 10;

			iDamage = (CGameInstance::GetInstance()->Random_Int(iDamage, _int((_float)iDamage * 1.4f)) + 1) * 26789;
			_bool IsCritical = false;
			if (CGameInstance::GetInstance()->Random_Coin(0.3f))
			{
				IsCritical = true;
				iDamage *= 2;
			}
			if(m_bTest)
				iDamage *= 10;
			Send_Collision(iDamage, vPos, STATUSEFFECT::EFFECTEND, fForce, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration,0);
			Show_Damage(iDamage, IsCritical);
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{

		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			_int iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			Vec3 vPos = {};
			if (static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().bUseProjPos)
			{
				vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			else
			{
				vPos = static_cast<CProjectile*>(pOther->Get_Owner())->Get_AttackOwner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			_float fForce = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion;


			int iCritical = rand() % 10;

			iDamage = (CGameInstance::GetInstance()->Random_Int(iDamage, _int((_float)iDamage * 1.5f)) + 1) * 26789;
			_bool IsCritical = false;
			if (CGameInstance::GetInstance()->Random_Coin(0.3f))
			{
				IsCritical = true;
				iDamage *= 2;
			}		
			if (m_bTest)
				iDamage *= 10;
			STATUSEFFECT eStatusEffect = STATUSEFFECT::EFFECTEND;
			if (static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().IsCounter)
				eStatusEffect = STATUSEFFECT::COUNTER;
			Send_Collision(iDamage, vPos, eStatusEffect,  fForce, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStagger);
			Show_Damage(iDamage, IsCritical);
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{

		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_ESTHER)
		{
			_int iDamage = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iDamage;
			Vec3 vPos = {};
			if (static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().bUseProjPos)
			{
				vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			else
			{
				vPos = static_cast<CProjectile*>(pOther->Get_Owner())->Get_AttackOwner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			}
			_float fForce = static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion;


			iDamage = (CGameInstance::GetInstance()->Random_Int(iDamage, _int((_float)iDamage * 1.5f)) + 1) * 26789;
			_bool IsCritical = true;
			iDamage *= 2;
			if (m_bTest)
				iDamage *= 10;
			STATUSEFFECT eStatusEffect = STATUSEFFECT::EFFECTEND;
			Send_Collision(iDamage, vPos, eStatusEffect, fForce, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration, static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStagger);
			Show_Damage(iDamage, IsCritical);
		}
	}
	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL && iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)
	{
		if (pOther->Get_Owner()->Get_ModelName() == L"ITR_02307_Pillar_Small"|| pOther->Get_Owner()->Get_ModelName() == L"ITR_022306_GiYurk")
		{
			Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(3.f);
			Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(2.46f, 0.f, -1.65f));
			Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pModelCom->Find_BoneIndex(TEXT("bip001-spine")));
			m_iAtk=0;
			m_fForce=0.f;
			Send_Collision(0, Vec3(), STATUSEFFECT::GROGGY, 0, 0.f, 0);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.3f, 100.0f, 1.5f, 9.0f);
		}
	}
}

void CBoss::Follow_ServerPos(_float fDistance, _float fLerpSpeed)
{
	Vec3 vCurrPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	Matrix matTargetWorld = m_matTargetWorld;
	Vec3 vServerPos(matTargetWorld.m[3]);

	Vec3 vDistance = vServerPos - vCurrPos;
	if (vDistance.Length() > fDistance)
	{
		vCurrPos = Vec3::Lerp(vCurrPos, vServerPos, fLerpSpeed);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCurrPos);
	}
}

void CBoss::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp_ForLand(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}

void CBoss::Add_Colider(_int iColIndex , CSphereCollider* pCollider)
{
	m_Coliders.emplace(iColIndex, pCollider);
}

void CBoss::Set_EffectPos()
{
	_uint iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_effectname"));//b_effectworldzero
	Matrix matEffect; 
	if (TEXT("Boss_Valtan") == m_strObjectTag)
	{
		matEffect = m_pModelCom->Get_CombinedMatrix(iBoneIndex) * XMMatrixScaling(0.012f, 0.012f, 0.012f);
	}
	else
	{
		matEffect = m_pModelCom->Get_CombinedMatrix(iBoneIndex) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	}
	matEffect *= m_pTransformCom->Get_WorldMatrix();
	memcpy(&m_vEffectPos, matEffect.m[3], sizeof(Vec3));
	Matrix ViewMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
	m_vEffectPos = XMVector3TransformCoord(m_vEffectPos, ViewMatrix);
	m_vEffectPos = XMVector3TransformCoord(m_vEffectPos, ProjMatrix);
}

void CBoss::Set_Die(_float fTime)
{
	for (auto& Collider : m_Coliders)
		Collider.second->SetActive(false);

	if (m_bDissolveOut == false)
		Set_DissolveOut(fTime);
	if (m_pBossHpUI != nullptr)
	{
		if (LEVELID::LEVEL_CHAOS_3 == (_uint)CGameInstance::GetInstance()->Get_CurrLevelIndex())
		{
			CUI_Manager::GetInstance()->Clear_ChaosGate();

			CItem* pItem = nullptr;
			
			CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

			CGameObject::STATDESC tStat = pPlayer->Get_PlayerStat_Desc();
			tStat.iCurHp = tStat.iMaxHp;
			tStat.iCurMp = tStat.iMaxMp;
			pPlayer->Set_PlayerStat_Desc(tStat);

			wstring ObjectTag = pPlayer->Get_ObjectTag();
			if (TEXT("Gunslinger") == ObjectTag)
			{
				CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

				pItem = pItem_Manager->Get_Item(ITEMCODE::GN_Helmet_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::GN_Body_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::GN_Leg_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::GN_WP_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				Safe_Release(pItem_Manager);
			}
			else if (TEXT("WR") == ObjectTag)
			{
				CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WR_Helmet_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WR_Body_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WR_Leg_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WR_WP_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				Safe_Release(pItem_Manager);
			}
			else if (TEXT("WDR") == ObjectTag)
			{
				CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_Helmet_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_Body_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_WP_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				Safe_Release(pItem_Manager);
			}
			else if (TEXT("SP") == ObjectTag)
			{
				CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

				pItem = pItem_Manager->Get_Item(ITEMCODE::SP_Helmet_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::SP_Body_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::SP_Leg_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				pItem = pItem_Manager->Get_Item(ITEMCODE::SP_WP_Legend);
				if (nullptr == pItem)
					return;

				pPlayer->Add_Item(pItem->Get_ObjectTag(), pItem);

				Safe_Release(pItem_Manager);
			}
		}

		m_pBossHpUI->Set_Dead_BossHpUI();
		m_pBossHpUI = nullptr;
	}
}

void CBoss::Set_HpUIRender(_bool bRender)
{
	if (m_pBossHpUI != nullptr)
	{
		m_pBossHpUI->Set_Active(bRender);
	}
}

void CBoss::Disable_HpUI()
{
	if (m_pBossHpUI != nullptr)
	{
		m_pBossHpUI->Set_Dead_BossHpUI();
		m_pBossHpUI = nullptr;
	}
}

void CBoss::Set_HpUI(_int iHpCount, _int  iMaxHp, wstring strBossName)
{
	if (m_pBossHpUI != nullptr)
	{
		m_pBossHpUI->Set_HpCount(iHpCount);
		m_pBossHpUI->Set_MaxHp(iMaxHp);
		m_pBossHpUI->Set_CurrHpColor(0);
		m_pBossHpUI->Set_BossName(strBossName);
	}
}

void CBoss::Move_to_SpawnPosition()
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSpawnPosition);
}

void CBoss::Load_WorldMatrix(Matrix& matWorld)
{
	matWorld = m_pTransformCom->Get_WorldMatrix();
}

void CBoss::Set_Weapon_Render(_bool IsRender)
{
	if (m_pWeapon != nullptr)
	{
		m_pWeapon->Set_Render(IsRender);
	}
}

void CBoss::Set_Weapon_RimLight(_float fTime, _float fColor)
{
	if (m_pWeapon != nullptr)
	{
		m_pWeapon->Set_RimLight(fTime, fColor);
	}
}

void CBoss::Reserve_WeaponAnimation(wstring strAnimName, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fAnimspeed)
{
	if (m_pWeapon != nullptr)
	{
		if (m_pWeapon->Get_ModelCom()->Initailize_FindAnimation(strAnimName, fAnimspeed) > m_pWeapon->Get_ModelCom()->Get_MaxAnimIndex())
			strAnimName = L"att_battle_8_01_loop";
		m_pWeapon->Get_ModelCom()->Reserve_NextAnimation(m_pWeapon->Get_ModelCom()->Find_AnimIndex(strAnimName), fChangeTime,
			iStartFrame, iChangeFrame);
		m_pWeapon->Get_ModelCom()->Set_Anim_Speed(m_pWeapon->Get_ModelCom()->Find_AnimIndex(strAnimName), fAnimspeed);
	}
}

HRESULT CBoss::Ready_Components()
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
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	Safe_Release(pGameInstance);

	
	m_vOriginScale.x = 0.01f;
	m_vOriginScale.y = 0.01f;
	m_vOriginScale.z = 0.01f;

	m_pTransformCom->Set_Scale(m_vOriginScale);

    return S_OK;
}

void CBoss::CullingObject()
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}
		
}

HRESULT CBoss::Ready_BehaviourTree()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"), TEXT("Com_Behavior"), (CComponent**)&m_pBehaviorTree)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss::Ready_HpUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pBossHpUI = static_cast<CUI_Boss_Hp*>(pGameInstance->Add_GameObject(m_iCurrLevel,
		(_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_BossHpUI"), this));

	if (nullptr == m_pBossHpUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Set_CurrHPUI(m_pBossHpUI);

	Safe_Release(pGameInstance);
	return S_OK;
}

void CBoss::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame)
{
	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame);
}

CGameObject* CBoss::Clone(void* pArg)
{
	CBoss* pInstance = new CBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss::Free()
{
	__super::Free();
}
