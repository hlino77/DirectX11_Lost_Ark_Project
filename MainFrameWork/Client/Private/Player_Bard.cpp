#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Bard.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "NavigationMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Controller_MG.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

/* State */
#include "State_MG_Idle.h"
#include "State_MG_Run.h"
#include "State_MG_Dash.h"
#include "State_MG_Attack_1.h"
#include "State_MG_Attack_2.h"
#include "State_MG_Attack_3.h"
#include "State_MG_Identity.h"
#include "State_MG_Identity_Back.h"

#include "State_MG_Hit.h"
#include "State_MG_HitEnd.h"
#include "State_MG_Hit_Common.h"
#include "State_MG_Stand.h"
#include "State_MG_StandDash.h"
#include "State_MG_Grabbed.h"
#include "State_MG_Stop.h"

#include "State_MG_Dead_Start.h"
#include "State_MG_Dead_End.h"
#include "State_MG_Resurrect.h"

#include "State_MG_Fall.h"

#include "State_MG_Esther_Way.h"
#include "State_MG_Esther_Silian.h"
#include "State_MG_Esther_Bahuntur.h"

/* State_Skill */
#include "State_MG_SoundShock.h"
#include "State_MG_Sonatine.h"
#include "State_MG_SongOfStome.h"
#include "State_MG_SongOfLight_Loop.h"
#include "State_MG_SongOfLight_End.h"
#include "State_MG_SongOfWind.h"
#include "State_MG_SonicVibe_Start.h"
#include "State_MG_SonicVibe_End.h"
#include "State_MG_SongOfHeaven.h"
#include "State_MG_SongOfGuard.h"

/* 스킬 */
#include "Skill_MG_SoundShock.h"
#include "Skill_MG_Sonatine.h"
#include "Skill_MG_SongOfStome.h"
#include "Skill_MG_SongOfLight.h"
#include "Skill_MG_SongOfWind.h"
#include "Skill_MG_SonicVibe.h"
#include "Skill_MG_SongOfHeaven.h"
#include "Skill_MG_SongOfGuard.h"

#include "Skill.h"
#include "Boss.h"
#include "Item.h"

#include "Esther_Way.h"

CPlayer_Bard::CPlayer_Bard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Bard::CPlayer_Bard(const CPlayer_Bard& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Bard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Bard::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Item()))
		return E_FAIL;

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
		if (FAILED(Ready_SkillUI()))
			return E_FAIL;
	}


	m_fAttackMoveSpeed = 8.0f;

	m_vHairColor_1 = { 0.82f, 0.78f, 0.65f, 1.f };
	m_vHairColor_2 = { 0.82f, 0.78f, 0.65f, 1.f };

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

void CPlayer_Bard::Tick(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::O))
	{
		Use_Item(TEXT("IT_MG_WP_Mococo"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::P))
	{
		Use_Item(TEXT("IT_MG_WP_Legend"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::J))
	{
		Use_Item(TEXT("IT_MG_Helmet_Legend"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::K))
	{
		Use_Item(TEXT("IT_MG_Body_Legend"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::L))
	{
		Use_Item(TEXT("IT_MG_Leg_Legend"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::N))
	{
		Use_Item(TEXT("IT_MG_Helmet_Mococo"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::M))
	{
		Use_Item(TEXT("IT_MG_Body_Mococo"));
	}
	if (KEY_HOLD(KEY::ALT) && KEY_TAP(KEY::X) &&
		TEXT("Dead_End") == Get_State())
	{
		Set_State(TEXT("Resurrect"));
	}

	m_pRigidBody->Tick(fTimeDelta);
	m_pStateMachine->Tick_State(fTimeDelta);
	m_pController->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CPlayer_Bard::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pController->LateTick(fTimeDelta);

	Set_Colliders(fTimeDelta);

	if (m_bControl)
	{
		m_fSendInfoTime += fTimeDelta;
		if (m_fSendInfoTime >= 0.05f)
		{
			m_fSendInfoTime = 0.0f;
			Send_PlayerInfo();
		}
	}
}

HRESULT CPlayer_Bard::Render()
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

HRESULT CPlayer_Bard::Render_ShadowDepth()
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

void CPlayer_Bard::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (true == m_bControl)
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS == pOther->Get_ColLayer())
			{
				if (false == m_pController->Is_GrabState())
				{
					m_pController->Get_GrabMessage(pOther->Get_Owner());
				}
			}

			if ((_uint)LAYER_COLLIDER::LAYER_SAFEZONE == pOther->Get_ColLayer())
			{
				Set_Invincible(true);
			}

			if ((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER == pOther->Get_ColLayer())
			{
				if (false == Is_Invincible())
					m_pController->Get_HitMessage(static_cast<CMonster*>(pOther->Get_Owner())->Get_Atk(), 0.f);
			}
			if ((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS == pOther->Get_ColLayer())
			{
				if (false == Is_Invincible())
				{
					Vec3 vPos = static_cast<CBoss*>(pOther->Get_Owner())->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

					m_pController->Get_HitMessage(static_cast<CBoss*>(pOther->Get_Owner())->Get_Atk(), static_cast<CBoss*>(pOther->Get_Owner())->Get_Force(), vPos);
				}
			}
			if ((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS == pOther->Get_ColLayer())
			{
				if (false == Is_Invincible() || true == static_cast<CSkill*>(pOther->Get_Owner())->Is_SafeZonePierce())
				{
					Vec3 vCenter;
					if (true == static_cast<CSkill*>(pOther->Get_Owner())->Get_Collider_Center(pOther->GetID(), &vCenter))
					{
						if (false == m_pController->Is_HitState())
						{
							if (true == static_cast<CBoss*>((static_cast<CSkill*>(pOther->Get_Owner())->Get_SkillOwner()))->Is_Dummy())
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

			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
				{
					Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
				}
			}
		}

		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(1);
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(1);
			}
		}
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(10);
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(10);
			}
		}
	}
	else
	{
		if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
		{
			if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
			{
				Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
			}
		}
	}
}

void CPlayer_Bard::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
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
	}
}

void CPlayer_Bard::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	if (true == m_bControl)
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_SAFEZONE == pOther->Get_ColLayer())
			{
				Set_Invincible(false);
			}

			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
				{
					Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, pOther);
				}

				if (TEXT("Stop") == Get_State())
				{
					Set_State(TEXT("Idle"));
				}
			}
		}
	}
	else
	{
		if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
		{
			if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
			{
				Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, pOther);
			}
		}
	}
}

void CPlayer_Bard::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Bard::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Bard::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Bard::Send_PlayerInfo()
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

void CPlayer_Bard::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

_bool CPlayer_Bard::Get_CellPickingPos(Vec3& vPickPos)
{
	_bool IsPick = __super::Get_CellPickingPos(vPickPos);

	if (true == m_IsClickNpc)
	{
		m_pController->Get_MoveToNpcMessage();
	}
	else
	{
		m_pController->Get_MoveToCellMessage();
	}

	return IsPick;
}

HRESULT CPlayer_Bard::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CController_MG::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_MG"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;

	/* 초기 장비 및 얼굴 설정 */
	wstring strComName = L"Prototype_Component_Model_MG_Face";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pModelPartCom[(_uint)PART::FACE])))
		return E_FAIL;

	/* 디폴트 장비 설정 */
	strComName = L"Prototype_Component_Model_MG_Body_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::BODY])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_MG_Shoulder_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Shoulder_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::SHOULDER])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_MG_Arm_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Arm_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::ARM])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_MG_Leg_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Leg_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::LEG])))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer_Bard::Ready_Parts()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Weapon_1 */
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_lhand"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_MG_WP_Base"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Bard::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_MG_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Run"), CState_MG_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash"), CState_MG_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_1"), CState_MG_Attack_1::Create(TEXT("Attack_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_2"), CState_MG_Attack_2::Create(TEXT("Attack_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_3"), CState_MG_Attack_3::Create(TEXT("Attack_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_MG"), CState_MG_Identity::Create(TEXT("Identity_MG"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_MG_Back"), CState_MG_Identity_Back::Create(TEXT("Identity_MG_Back"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SoundShock"), CState_MG_SoundShock::Create(TEXT("Skill_MG_SoundShock"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_Sonatine"), CState_MG_Sonatine::Create(TEXT("Skill_MG_Sonatine"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SongOfStome"), CState_MG_SongOfStome::Create(TEXT("Skill_MG_SongOfStome"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SongOfLight_Loop"), CState_MG_SongOfLight_Loop::Create(TEXT("Skill_MG_SongOfLight_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SongOfLight_End"), CState_MG_SongOfLight_End::Create(TEXT("Skill_MG_SongOfLight_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SongOfWind"), CState_MG_SongOfWind::Create(TEXT("Skill_MG_SongOfWind"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SonicVibe_Start"), CState_MG_SonicVibe_Start::Create(TEXT("Skill_MG_SonicVibe_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SonicVibe_End"), CState_MG_SonicVibe_End::Create(TEXT("Skill_MG_SonicVibe_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SonicVibe_End"), CState_MG_SonicVibe_End::Create(TEXT("Skill_MG_SonicVibe_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SongOfHeaven"), CState_MG_SongOfHeaven::Create(TEXT("Skill_MG_SongOfHeaven"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_MG_SongOfGuard"), CState_MG_SongOfGuard::Create(TEXT("Skill_MG_SongOfGuard"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Hit"), CState_MG_Hit::Create(TEXT("Hit"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("HitEnd"), CState_MG_HitEnd::Create(TEXT("HitEnd"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Hit_Common"), CState_MG_Hit_Common::Create(TEXT("Hit_Common"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Stand"), CState_MG_Stand::Create(TEXT("Stand"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("StandDash"), CState_MG_StandDash::Create(TEXT("StandDash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Grabbed"), CState_MG_Grabbed::Create(TEXT("Grabbed"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Stop"), CState_MG_Stop::Create(TEXT("Stop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dead_Start"), CState_MG_Dead_Start::Create(TEXT("Dead_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dead_End"), CState_MG_Dead_End::Create(TEXT("Dead_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Resurrect"), CState_MG_Resurrect::Create(TEXT("Resurrect"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Fall"), CState_MG_Fall::Create(TEXT("TEXT"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Way"), CState_MG_Esther_Way::Create(TEXT("Esther_Way"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Silian"), CState_MG_Esther_Silian::Create(TEXT("Esther_Silian"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Bahuntur"), CState_MG_Esther_Bahuntur::Create(TEXT("Esther_Bahuntur"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	return S_OK;
}

HRESULT CPlayer_Bard::Ready_Skill()
{
	CPlayer_Skill* pSkill = nullptr;
	CPlayer_Skill::PLAYERSKILL_DESC SkillDesc;

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SoundShock");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SoundShock")));
	pSkill = CSkill_MG_SoundShock::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_Sonatine");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_Sonatine")));
	pSkill = CSkill_MG_Sonatine::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::W, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SongOfStome");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SongOfStome")));
	pSkill = CSkill_MG_SongOfStome::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::E, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SongOfWind");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SongOfWind")));
	pSkill = CSkill_MG_SongOfWind::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::R, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SongOfLight_Loop");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SongOfLight_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SongOfLight_End")));
	pSkill = CSkill_MG_SongOfLight::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::A, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SonicVibe_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SonicVibe_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SonicVibe_End")));
	pSkill = CSkill_MG_SonicVibe::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::S, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SongOfHeaven");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SongOfHeaven")));
	pSkill = CSkill_MG_SongOfHeaven::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::D, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_MG_SongOfGuard");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_MG_SongOfGuard")));
	pSkill = CSkill_MG_SongOfGuard::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::F, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();


	return S_OK;
}

HRESULT CPlayer_Bard::Ready_Coliders()
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

HRESULT CPlayer_Bard::Ready_PhysxBoneBranch()
{


	return S_OK;
}

HRESULT CPlayer_Bard::Ready_Item()
{
	CItem* pItem = nullptr;

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_Helmet_Mococo")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	pItem->Use_Item(this);

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_Body_Mococo")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	pItem->Use_Item(this);

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_WP_Mococo")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	pItem->Use_Item(this);

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_WP_Legend")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_Helmet_Legend")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_Body_Legend")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	pItem = static_cast<CItem*>(m_pGameInstance->Find_GameObject(LEVELID::LEVEL_STATIC,
		(_uint)LAYER_TYPE::LAYER_ITEM, TEXT("IT_MG_Leg_Legend")));
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	return S_OK;
}

HRESULT CPlayer_Bard::Ready_Esther()
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

HRESULT CPlayer_Bard::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Bard* CPlayer_Bard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Bard* pInstance = new CPlayer_Bard(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Bard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Bard::Clone(void* pArg)
{
	CPlayer_Bard* pInstance = new CPlayer_Bard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Bard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Bard::Free()
{
	__super::Free();
}
