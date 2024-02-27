#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Destroyer.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "NavigationMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Controller_WDR.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

/* State */
#include "State_WDR_Idle.h"
#include "State_WDR_Run.h"
#include "State_WDR_Dash.h"
#include "State_WDR_Attack_1.h"
#include "State_WDR_Attack_2.h"
#include "State_WDR_Attack_3.h"
#include "State_WDR_Identity.h"
#include "State_WDR_Iden_Idle.h"
#include "State_WDR_Iden_Attack_1.h"
#include "State_WDR_Iden_Attack_2.h"
#include "State_WDR_Iden_Attack_3.h"
#include "State_WDR_Iden_Attack_4.h"
#include "State_WDR_Iden_Skill.h"
#include "State_WDR_Hit.h"
#include "State_WDR_HitEnd.h"
#include "State_WDR_Hit_Common.h"
#include "State_WDR_Stand.h"
#include "State_WDR_StandDash.h"
#include "State_WDR_Grabbed.h"
#include "State_WDR_Stop.h"
#include "State_WDR_Resurrect.h"
#include "State_WDR_Dead_Start.h"
#include "State_WDR_Dead_End.h"
#include "State_WDR_Fall.h"
#include "State_WDR_Esther_Way.h"
#include "State_WDR_Esther_Silian.h"
#include "State_WDR_Esther_Bahuntur.h"

/* State_Skill */
#include "State_WDR_EndurePain.h"
#include "State_WDR_EarthEater.h"
#include "State_WDR_FullSwing_Start.h"
#include "State_WDR_FullSwing_Loop.h"
#include "State_WDR_FullSwing_Success.h"
#include "State_WDR_FullSwing_Fail.h"
#include "State_WDR_HeavyCrush.h"
#include "State_WDR_PerfectSwing_Start.h"
#include "State_WDR_PerfectSwing_Loop.h"
#include "State_WDR_PerfectSwing_Success.h"
#include "State_WDR_PerfectSwing_Fail.h"
#include "State_WDR_PowerShoulder_Start.h"
#include "State_WDR_PowerShoulder_Loop.h"
#include "State_WDR_PowerShoulder_End.h"
#include "State_WDR_PowerStrike_Start.h"
#include "State_WDR_PowerStrike_Loop.h"
#include "State_WDR_PowerStrike_End.h"
#include "State_WDR_SizemicHammer.h"

/* 스킬 */
#include "Skill_WDR_EndurePain.h"
#include "Skill_WDR_EarthEater.h"
#include "Skill_WDR_FullSwing.h"
#include "Skill_WDR_HeavyCrush.h"
#include "Skill_WDR_PerfectSwing.h"
#include "Skill_WDR_PowerShoulder.h"
#include "Skill_WDR_PowerStrike.h"
#include "Skill_WDR_SizemicHammer.h"

#include "Skill.h"
#include "Boss.h"
#include "Item.h"

#include "Esther_Way.h"

#include "Item_Manager.h"
#include "Projectile.h"

CPlayer_Destroyer::CPlayer_Destroyer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Destroyer::CPlayer_Destroyer(const CPlayer_Destroyer& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Destroyer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Destroyer::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_tPCStatDesc.iMaxHp = 120000 + CGameInstance::GetInstance()->Random_Int(0, 7000);
	m_tPCStatDesc.iCurHp = m_tPCStatDesc.iMaxHp;

	m_tPCStatDesc.iMaxMp = 800;
	m_tPCStatDesc.iCurMp = m_tPCStatDesc.iMaxMp;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	if (FAILED(Ready_Esther()))
		return E_FAIL;

	if (FAILED(Ready_PhysxBoneBranch()))
		return E_FAIL;

	if (m_bControl)
	{
		if (FAILED(Ready_Item()))
			return E_FAIL;

		if (FAILED(Ready_SkillUI()))
			return E_FAIL;
	}


	m_fAttackMoveSpeed = 8.0f;

	m_vHairColor_1 = { 0.78f, 0.78f, 0.78f, 1.f };
	m_vHairColor_2 = { 0.82f, 0.82f, 0.82f, 1.f };

	/* 플레이어 공통 요소 */
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_matTargetWorld = Desc->matWorld;
	m_vTargetPos = Desc->vTargetPos;

	Vec3 vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_WorldMatrix(Desc->matWorld);
	m_pTransformCom->Set_Scale(vScale);

	m_pStateMachine->Change_State(Desc->szState);

	CNavigationMgr::GetInstance()->Find_FirstCell(m_iCurrLevel, this);

	return S_OK;
}

void CPlayer_Destroyer::Tick(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::X) &&
		TEXT("Dead_End") == Get_State())
	{
		Set_State(TEXT("Resurrect"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::B))
	{
		CItem* pItem = nullptr;
		CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

		pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_Helmet_Legend);
		if (nullptr == pItem)
			return;

		Add_Item(pItem->Get_ObjectTag(), pItem);

		pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_Body_Legend);
		if (nullptr == pItem)
			return;

		Add_Item(pItem->Get_ObjectTag(), pItem);

		pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_WP_Legend);
		if (nullptr == pItem)
			return;

		Add_Item(pItem->Get_ObjectTag(), pItem);

		RELEASE_INSTANCE(CItem_Manager);
	}

	m_pRigidBody->Tick(fTimeDelta);
	m_pStateMachine->Tick_State(fTimeDelta);
	m_pController->Tick(fTimeDelta);


	__super::Tick(fTimeDelta);
}

void CPlayer_Destroyer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pController->LateTick(fTimeDelta);

	Set_Colliders(fTimeDelta);

	if (m_bControl && m_bPlayerInfoSend)
	{
		m_fSendInfoTime += fTimeDelta;
		if (m_fSendInfoTime >= 0.05f)
		{
			m_fSendInfoTime = 0.0f;
			Send_PlayerInfo();
		}
	}
}

HRESULT CPlayer_Destroyer::Render()
{
	__super::Render();

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (i == (_uint)PART::HELMET && j == m_IsHair)
			{
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_vHairColor_1, sizeof(Vec4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_vHairColor_2, sizeof(Vec4))))
					return E_FAIL;

				if (FAILED(m_pModelPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;

				Vec4 vResetColor;
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &vResetColor, sizeof(Vec4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &vResetColor, sizeof(Vec4))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pModelPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CPlayer_Destroyer::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{

			if (FAILED(m_pModelPartCom[i]->Render(m_pShaderCom, j, "ShadowPass")))
				return S_OK;
		}
	}

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CPlayer_Destroyer::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (true == m_bControl)
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS == pOther->Get_ColLayer())
			{
				if (false == m_pController->Is_GrabState() && false == m_pController->Is_Dead())
				{
					m_IsSafeZone = false;
					m_pController->Get_GrabMessage(pOther->Get_Owner());
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_SAFEZONE == pOther->Get_ColLayer())
			{
				m_IsSafeZone = true;
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BUFF_PLAYER == pOther->Get_ColLayer())
			{
				m_pController->Get_BuffMessage(static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStatusEffect,
					static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion,
					static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration);

				if ((_uint)CPlayer_Controller::BUFFEFFECT::HPREFILL == static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStatusEffect)
				{
					Send_Hp();
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BUFF_ESTHER == pOther->Get_ColLayer())
			{
				m_pController->Get_BuffMessage(static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStatusEffect,
					static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fRepulsion,
					static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().fStatusDuration);

				if ((_uint)CPlayer_Controller::BUFFEFFECT::HPREFILL == static_cast<CProjectile*>(pOther->Get_Owner())->Get_ProjInfo().iStatusEffect)
				{
					Send_Hp();
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER == pOther->Get_ColLayer())
			{
				if (false == Is_Invincible())
				{
					m_pController->Get_HitMessage(static_cast<CMonster*>(pOther->Get_Owner())->Get_Atk(), 0.f);
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS == pOther->Get_ColLayer())
			{
				if (false == Is_Invincible())
				{
					Vec3 vPos = static_cast<CBoss*>(pOther->Get_Owner())->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

					m_pController->Get_HitMessage(static_cast<CBoss*>(pOther->Get_Owner())->Get_Atk(), static_cast<CBoss*>(pOther->Get_Owner())->Get_Force(), vPos);
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS == pOther->Get_ColLayer())
			{
				if (false == Is_Invincible())
				{
					if (false == m_IsSafeZone || true == (m_IsSafeZonePierce = static_cast<CSkill*>(pOther->Get_Owner())->Is_SafeZonePierce()))
					{
						Vec3 vCenter;
						if (true == static_cast<CSkill*>(pOther->Get_Owner())->Get_Collider_Center(pOther->GetID(), &vCenter))
						{
							if (false == m_pController->Is_HitState())
							{
								if (true == m_pController->Is_GrabState() && true == static_cast<CBoss*>((static_cast<CSkill*>(pOther->Get_Owner())->Get_SkillOwner()))->Is_Dummy())
								{
									m_pController->Get_HitMessage(static_cast<CSkill*>(pOther->Get_Owner())->Get_Atk(), 0, vCenter);
								}
								else
								{
									m_pController->Get_HitMessage(static_cast<CSkill*>(pOther->Get_Owner())->Get_Atk(), static_cast<CSkill*>(pOther->Get_Owner())->Get_Force(), vCenter);
								}
							}
						}
						m_pController->Get_StatusEffectMessage((_uint)static_cast<CSkill*>(pOther->Get_Owner())->Get_StatusEffect(), static_cast<CSkill*>(pOther->Get_Owner())->Get_StatusEffectDuration());
					}
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
				{
					Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer() && TEXT("Dead_End") != Get_State())
			{
				if (false == static_cast<CBoss*>(pOther->Get_Owner())->Is_Dummy())
				{
					Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS, pOther);
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_NPC == pOther->Get_ColLayer() && true == m_IsClickNpc)
			{
				m_pController->Set_Control_Active(false);
				Set_State(TEXT("Idle"));
			}
		}
		else if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			//CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WDR_Hitted_82.wav"), 0.2f);
		}
		else if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			//CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WDR_Hitted_82.wav"), 0.2f);

			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				m_pController->Get_HitMarbleMessage();
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				m_pController->Get_HitMarbleMessage();
			}
		}

		if (TEXT("Skill_WDR_PowerShoulder_Start") == Get_State())
		{
			if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
			{
				if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
				{
					Set_TargetPos(Vec3(0.f, -1.f, 0.f));
				}
				else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
				{
					Set_TargetPos(Vec3(0.f, -1.f, 0.f));
				}
			}
		}
	}
	else
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER && (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
		{
			if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
			{
				Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
			}
		}
		else if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER && (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer()
			&& TEXT("Dead_End") != Get_State())
		{
			if (false == static_cast<CBoss*>(pOther->Get_Owner())->Is_Dummy())
			{
				Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS, pOther);
			}
		}
	}
}

void CPlayer_Destroyer::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
	if (true == m_bControl)
	{
		if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
		{
			if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
			{
				m_pController->Get_CheckLengthMessage(1.f, pOther->Get_Owner());
			}
		}
		else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
		{
			if (false == static_cast<CBoss*>(pOther->Get_Owner())->Is_Dummy() &&
				TEXT("Grabbed") != Get_State())
			{
				m_pController->Get_CheckLengthMessage(1.f, pOther->Get_Owner());
			}
		}
	}
	else
	{
		if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
		{
			if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
			{
				m_pController->Get_CheckLengthMessage(1.f, pOther->Get_Owner());
			}
		}
		else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
		{
			if (false == static_cast<CBoss*>(pOther->Get_Owner())->Is_Dummy() &&
				TEXT("Grabbed") != Get_State())
			{
				m_pController->Get_CheckLengthMessage(1.f, pOther->Get_Owner());
			}
		}
	}
}

void CPlayer_Destroyer::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	if (true == m_bControl)
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_SAFEZONE == pOther->Get_ColLayer())
			{
				m_IsSafeZone = false;
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
				{
					Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
				}

				if (TEXT("Stop") == Get_State())
				{
					if (true == m_pController->Is_In_Identity())
					{
						Set_State(TEXT("Iden_Idle"));
					}
					else
					{
						Set_State(TEXT("Idle"));
					}
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				if (false == static_cast<CBoss*>(pOther->Get_Owner())->Is_Dummy())
				{
					Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS, pOther);
				}
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_NPC == pOther->Get_ColLayer())
			{
				m_pController->Set_Control_Active(true);
			}
		}
	}
	else
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER && (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
		{
			if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
			{
				Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
			}
		}
		else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
		{
			if (false == static_cast<CBoss*>(pOther->Get_Owner())->Is_Dummy())
			{
				Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS, pOther);
			}
		}
	}
}

void CPlayer_Destroyer::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Destroyer::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Destroyer::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Destroyer::Send_PlayerInfo()
{
	Protocol::S_OBJECTINFO pkt;

	auto tPlayerInfo = pkt.add_tobject();

	tPlayerInfo->set_iobjectid(m_iObjectID);
	tPlayerInfo->set_ilevel(m_iCurrLevel);
	tPlayerInfo->set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);


	auto vTargetPos = tPlayerInfo->mutable_vtargetpos();
	vTargetPos->Resize(3, 0.0f);
	Vec3 vPlayerTargetPos = m_vTargetPos.load();
	memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));


	auto matWorld = tPlayerInfo->mutable_matworld();
	matWorld->Resize(16, 0.0f);
	Matrix matPlayerWorld = m_pTransformCom->Get_WorldMatrix();
	memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));


	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CPlayer_Destroyer::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

_bool CPlayer_Destroyer::Get_CellPickingPos(Vec3& vPickPos)
{
	_bool IsPick = __super::Get_CellPickingPos(vPickPos);

	if (true == m_IsClickNpc)
	{
		//m_pController->Get_MoveToNpcMessage();
		m_pController->Get_MoveToCellMessage();
	}
	else
	{
		m_pController->Get_MoveToCellMessage();
	}

	return IsPick;
}

HRESULT CPlayer_Destroyer::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CController_WDR::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_WDR"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;


	/* 디폴트 장비 설정 */
	wstring strComName = L"Prototype_Component_Model_WDR_Body_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::BODY])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_WDR_Shoulder_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Shoulder_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::SHOULDER])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_WDR_Arm_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Arm_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::ARM])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_WDR_Leg_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Leg_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::LEG])))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_Parts()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Weapon_1 */
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_rhand"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_WDR_WP_Base"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_WDR_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Run"), CState_WDR_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash"), CState_WDR_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_1"), CState_WDR_Attack_1::Create(TEXT("Attack_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_2"), CState_WDR_Attack_2::Create(TEXT("Attack_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_3"), CState_WDR_Attack_3::Create(TEXT("Attack_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("WDR_Identity"), CState_WDR_Identity::Create(TEXT("WDR_Identity"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Iden_Idle"), CState_WDR_Iden_Idle::Create(TEXT("Iden_Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Iden_Attack_1"), CState_WDR_Iden_Attack_1::Create(TEXT("Iden_Attack_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Iden_Attack_2"), CState_WDR_Iden_Attack_2::Create(TEXT("Iden_Attack_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Iden_Attack_3"), CState_WDR_Iden_Attack_3::Create(TEXT("Iden_Attack_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Iden_Attack_4"), CState_WDR_Iden_Attack_4::Create(TEXT("Iden_Attack_4"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Iden_Skill"), CState_WDR_Iden_Skill::Create(TEXT("Iden_Skill"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_EndurePain"), CState_WDR_EndurePain::Create(TEXT("Skill_WDR_EndurePain"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_EarthEater"), CState_WDR_EarthEater::Create(TEXT("Skill_WDR_EarthEater"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_FullSwing_Start"), CState_WDR_FullSwing_Start::Create(TEXT("Skill_WDR_FullSwing_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_FullSwing_Loop"), CState_WDR_FullSwing_Loop::Create(TEXT("Skill_WDR_FullSwing_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_FullSwing_Success"), CState_WDR_FullSwing_Success::Create(TEXT("Skill_WDR_FullSwing_Success"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_FullSwing_Fail"), CState_WDR_FullSwing_Fail::Create(TEXT("Skill_WDR_FullSwing_Fail"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_HeavyCrush"), CState_WDR_HeavyCrush::Create(TEXT("Skill_WDR_HeavyCrush"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PerfectSwing_Start"), CState_WDR_PerfectSwing_Start::Create(TEXT("Skill_WDR_PerfectSwing_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PerfectSwing_Loop"), CState_WDR_PerfectSwing_Loop::Create(TEXT("Skill_WDR_PerfectSwing_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PerfectSwing_Success"), CState_WDR_PerfectSwing_Success::Create(TEXT("Skill_WDR_PerfectSwing_Success"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PerfectSwing_Fail"), CState_WDR_PerfectSwing_Fail::Create(TEXT("Skill_WDR_PerfectSwing_Fail"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PowerShoulder_Start"), CState_WDR_PowerShoulder_Start::Create(TEXT("Skill_WDR_PowerShoulder_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PowerShoulder_Loop"), CState_WDR_PowerShoulder_Loop::Create(TEXT("Skill_WDR_PowerShoulder_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PowerShoulder_End"), CState_WDR_PowerShoulder_End::Create(TEXT("Skill_WDR_PowerShoulder_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PowerStrike_Start"), CState_WDR_PowerStrike_Start::Create(TEXT("Skill_WDR_PowerStrike_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PowerStrike_Loop"), CState_WDR_PowerStrike_Loop::Create(TEXT("Skill_WDR_PowerStrike_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_PowerStrike_End"), CState_WDR_PowerStrike_End::Create(TEXT("Skill_WDR_PowerStrike_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WDR_SizemicHammer"), CState_WDR_SizemicHammer::Create(TEXT("Skill_WDR_SizemicHammer"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Hit"), CState_WDR_Hit::Create(TEXT("Hit"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("HitEnd"), CState_WDR_HitEnd::Create(TEXT("HitEnd"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Hit_Common"), CState_WDR_Hit_Common::Create(TEXT("Hit_Common"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Stand"), CState_WDR_Stand::Create(TEXT("Stand"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("StandDash"), CState_WDR_StandDash::Create(TEXT("StandDash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Grabbed"), CState_WDR_Grabbed::Create(TEXT("Grabbed"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Stop"), CState_WDR_Stop::Create(TEXT("Stop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dead_Start"), CState_WDR_Dead_Start::Create(TEXT("Dead_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dead_End"), CState_WDR_Dead_End::Create(TEXT("Dead_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Resurrect"), CState_WDR_Resurrect::Create(TEXT("Resurrect"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Fall"), CState_WDR_Fall::Create(TEXT("Fall"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Way"), CState_WDR_Esther_Way::Create(TEXT("Esther_Way"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Silian"), CState_WDR_Esther_Silian::Create(TEXT("Esther_Silian"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Bahuntur"), CState_WDR_Esther_Bahuntur::Create(TEXT("Esther_Bahuntur"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_Skill()
{
	CPlayer_Skill* pSkill = nullptr;
	CPlayer_Skill::PLAYERSKILL_DESC SkillDesc;

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_EndurePain");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_EndurePain")));
	pSkill = CSkill_WDR_EndurePain::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_EarthEater");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_EarthEater")));
	pSkill = CSkill_WDR_EarthEater::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::W, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_FullSwing_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_FullSwing_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_FullSwing_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_FullSwing_Success")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_FullSwing_Fail")));
	pSkill = CSkill_WDR_FullSwing::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::E, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_HeavyCrush");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_HeavyCrush")));
	pSkill = CSkill_WDR_HeavyCrush::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::A, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_PerfectSwing_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PerfectSwing_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PerfectSwing_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PerfectSwing_Success")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PerfectSwing_Fail")));
	pSkill = CSkill_WDR_PerfectSwing::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::D, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_PowerShoulder_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PowerShoulder_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PowerShoulder_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PowerShoulder_End")));
	pSkill = CSkill_WDR_PowerShoulder::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::F, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_PowerStrike_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PowerStrike_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PowerStrike_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_PowerStrike_End")));
	pSkill = CSkill_WDR_PowerStrike::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::S, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WDR_SizemicHammer");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WDR_SizemicHammer")));
	pSkill = CSkill_WDR_SizemicHammer::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::R, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_Coliders()
{
	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->SetActive(true);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Radius(0.7f);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));


		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.2f, 0.6f, 0.2f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
		pChildCollider->SetActive(true);
	}

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			Collider.second->Update_Collider();
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_PhysxBoneBranch()
{
	m_pModelCom->Play_Animation(10.0f);


	CPhysXMgr::GetInstance()->Add_Player(this);

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_04"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_05"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_04"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_05"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	/*{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}*/

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_Item()
{
	CItem* pItem = nullptr;

	CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

	pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_Helmet_Mococo);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	Use_Item(pItem->Get_ObjectTag(), 1, false);

	pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_Body_Mococo);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	Use_Item(pItem->Get_ObjectTag(), 1, false);

	pItem = pItem_Manager->Get_Item(ITEMCODE::WDR_WP_Mococo);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	Use_Item(pItem->Get_ObjectTag(), 1, false);

	Safe_Release(pItem_Manager);

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_Esther()
{
	CGameObject* pEsther = nullptr;

	CEsther::ESTHERDESC tEstherDesc;
	tEstherDesc.pLeaderPlayer = this;

	pEsther = m_pGameInstance->Add_GameObject((_uint)LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_ESTHER, TEXT("Prototype_GameObject_Esther_Way"), &tEstherDesc);
	if (nullptr == pEsther)
		return E_FAIL;

	m_pController->Set_Esther(pEsther);

	pEsther = m_pGameInstance->Add_GameObject((_uint)LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_ESTHER, TEXT("Prototype_GameObject_Esther_Silian"), &tEstherDesc);
	if (nullptr == pEsther)
		return E_FAIL;

	m_pController->Set_Esther(pEsther);

	pEsther = m_pGameInstance->Add_GameObject((_uint)LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_ESTHER, TEXT("Prototype_GameObject_Esther_Bahuntur"), &tEstherDesc);
	if (nullptr == pEsther)
		return E_FAIL;

	m_pController->Set_Esther(pEsther);

	return S_OK;
}

HRESULT CPlayer_Destroyer::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Destroyer* CPlayer_Destroyer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Destroyer* pInstance = new CPlayer_Destroyer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Destroyer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Destroyer::Clone(void* pArg)
{
	CPlayer_Destroyer* pInstance = new CPlayer_Destroyer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Destroyer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Destroyer::Free()
{
	__super::Free();
}
