#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Doaga.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "NavigationMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Controller_SP.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

/* State */
#include "State_SP_Idle.h"
#include "State_SP_Run.h"
#include "State_SP_Dash.h"
#include "State_SP_Fall.h"
#include "State_SP_Dead_Start.h"
#include "State_SP_Dead_End.h"
#include "State_SP_Esther_Bahuntur.h"
#include "State_SP_Esther_Silian.h"
#include "State_SP_Esther_Way.h"
#include "State_Sp_Grabbed.h"
#include "State_SP_Hit.h"
#include "State_SP_Hit_Common.h"
#include "State_SP_HitEnd.h"
#include "State_SP_Resurrect.h"
#include "State_SP_Stand.h"
#include "State_SP_StandDash.h"
#include "State_SP_Stop.h"

#include "State_SP_Attack1.h"
#include "State_SP_Attack2.h"
#include "State_SP_Attack3.h"
#include "State_SP_Attack4.h"

#include "State_SP_Identity_Sun.h"
#include "State_SP_Identity_Moon.h"
#include "State_SP_Identity_Moon_End.h"

/* State_Skill */
#include "State_SP_Inkpaddle.h"
#include "State_SP_DimensionalShift_Start.h"
#include "State_SP_DimensionalShift_End.h"
#include "State_SP_Inkshot.h"
#include "State_SP_Onestroke.h"
#include "State_SP_Moondrawing_Start.h"
#include "State_SP_Moondrawing_End.h"
#include "State_SP_SkyKongKong_Start.h"
#include "State_SP_SkyKongKong_Loop.h"
#include "State_SP_SkyKongKong_End.h"
#include "State_SP_SpiritHarmony.h"
#include "State_SP_Flyheaven.h"

/* 스킬 */
#include "Skill_SP_Inkpaddle.h"
#include "Skill_SP_DimensionalShift.h"
#include "Skill_SP_Inkshot.h"
#include "Skill_SP_Onestroke.h"
#include "Skill_SP_Moondrawing.h"
#include "Skill_SP_SkyKongKong.h"
#include "Skill_SP_SpiritHarmony.h"
#include "Skill_SP_Flyheaven.h"


#include "Skill.h"
#include "Boss.h"
#include "Item.h"

#include "Esther_Way.h"
#include "Item_Manager.h"
#include "Effect_Manager.h"
#include "Effect.h"

CPlayer_Doaga::CPlayer_Doaga(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Doaga::CPlayer_Doaga(const CPlayer_Doaga& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Doaga::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Doaga::Initialize(void* pArg)
{
	__super::Initialize(pArg);

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

	m_tPCStatDesc.iMaxHp = 80000 + CGameInstance::GetInstance()->Random_Int(0,7000);
	m_tPCStatDesc.iCurHp = m_tPCStatDesc.iMaxHp;


	m_tPCStatDesc.iMaxMp = 1200;
	m_tPCStatDesc.iCurMp = m_tPCStatDesc.iMaxMp;

	return S_OK;
}

vector<CEffect*> Effects;

void CPlayer_Doaga::Tick(_float fTimeDelta)
{
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

void CPlayer_Doaga::LateTick(_float fTimeDelta)
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

HRESULT CPlayer_Doaga::Render()
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

HRESULT CPlayer_Doaga::Render_ShadowDepth()
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

void CPlayer_Doaga::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
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
				m_IsSafeZone = true;
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
				if (false == Is_Invincible())
				{
					if (false == m_IsSafeZone || true == static_cast<CSkill*>(pOther->Get_Owner())->Is_SafeZonePierce())
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

			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				if (TEXT("Skill_Crystal") == pOther->Get_Owner()->Get_ObjectTag())
				{
					Add_CollisionStay((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pOther);
				}
			}

			if ((_uint)LAYER_COLLIDER::LAYER_BODY_NPC == pOther->Get_ColLayer() && true == m_IsClickNpc)
			{

				m_pController->Set_Control_Active(false);
				Set_State(TEXT("Idle"));
			}
		}

		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(0.1f);
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(1.f);
			}
			
		}
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			if (TEXT("Identity_Moon_End") != Get_State())
			{
				if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
				{
					m_pController->Increase_IdenGage(0.8f);
				}
				else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
				{
					m_pController->Increase_IdenGage(8.f);
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
	}
}

void CPlayer_Doaga::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
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

void CPlayer_Doaga::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	if (true == m_bControl)
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_SAFEZONE == pOther->Get_ColLayer())
			{
				m_IsSafeZone = false;
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

			if ((_uint)LAYER_COLLIDER::LAYER_BODY_NPC == pOther->Get_ColLayer())
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
				Delete_CollisionStay((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, pOther);
			}
		}
	}
}

void CPlayer_Doaga::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Doaga::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Doaga::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Doaga::Send_PlayerInfo()
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

void CPlayer_Doaga::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

_bool CPlayer_Doaga::Get_CellPickingPos(Vec3& vPickPos)
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

HRESULT CPlayer_Doaga::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CController_SP::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_SP"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;

	/* 초기 장비 및 얼굴 설정 */
	wstring strComName = L"Prototype_Component_Model_SP_Face";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pModelPartCom[(_uint)PART::FACE])))
		return E_FAIL;

	/* 디폴트 장비 설정 */
	strComName = L"Prototype_Component_Model_SP_Body_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::BODY])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_SP_Shoulder_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Shoulder_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::SHOULDER])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_SP_Arm_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Arm_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::ARM])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_SP_Leg_Default";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Leg_Default"), (CComponent**)&m_pDefaultModel[(_uint)PART::LEG])))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer_Doaga::Ready_Parts()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Weapon_1 */
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_SP_WP_Base"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Doaga::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_SP_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Run"), CState_SP_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash"), CState_SP_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Fall"), CState_SP_Fall::Create(TEXT("Fall"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dead_Start"), CState_SP_Dead_Start::Create(TEXT("Dead_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dead_End"), CState_SP_Dead_End::Create(TEXT("Dead_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Way"), CState_SP_Esther_Way::Create(TEXT("Esther_Way"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Silian"), CState_SP_Esther_Silian::Create(TEXT("Esther_Silian"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Esther_Bahuntur"), CState_SP_Esther_Bahuntur::Create(TEXT("Esther_Bahuntur"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Grabbed"), CState_Sp_Grabbed::Create(TEXT("Grabbed"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Hit"), CState_SP_Hit::Create(TEXT("Hit"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Hit_Common"), CState_SP_Hit_Common::Create(TEXT("Hit_Common"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("HitEnd"), CState_SP_HitEnd::Create(TEXT("HitEnd"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Resurrect"), CState_SP_Resurrect::Create(TEXT("Resurrect"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Stand"), CState_SP_Stand::Create(TEXT("Stand"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("StandDash"), CState_SP_StandDash::Create(TEXT("StandDash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Stop"), CState_SP_Stop::Create(TEXT("Stop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_1"), CState_SP_Attack1::Create(TEXT("Attack_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_2"), CState_SP_Attack2::Create(TEXT("Attack_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_3"), CState_SP_Attack3::Create(TEXT("Attack_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_4"), CState_SP_Attack4::Create(TEXT("Attack_4"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Sun"), CState_SP_Identity_Sun::Create(TEXT("Identity_Sun"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Moon"), CState_SP_Identity_Moon::Create(TEXT("Identity_Moon"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Moon_End"), CState_SP_Identity_Moon_End::Create(TEXT("Identity_Moon_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	/* 스킬 스테이트 */
	m_pStateMachine->Add_State(TEXT("Skill_SP_Inkpaddle"), CState_SP_Inkpaddle::Create(TEXT("Skill_SP_Inkpaddle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_DimensionalShift_Start"), CState_SP_DimensionalShift_Start::Create(TEXT("Skill_SP_DimensionalShift_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_DimensionalShift_End"), CState_SP_DimensionalShift_End::Create(TEXT("Skill_SP_DimensionalShift_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_Inkshot"), CState_SP_Inkshot::Create(TEXT("Skill_SP_Inkshot"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_Onestroke"), CState_SP_Onestroke::Create(TEXT("Skill_SP_Onestroke"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_Moondrawing_Start"), CState_SP_Moondrawing_Start::Create(TEXT("Skill_SP_Moondrawing_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_Moondrawing_End"), CState_SP_Moondrawing_End::Create(TEXT("Skill_SP_Moondrawing_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_SkyKongKong_Start"), CState_SP_SkyKongKong_Start::Create(TEXT("Skill_SP_SkyKongKong_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_SkyKongKong_Loop"), CState_SP_SkyKongKong_Loop::Create(TEXT("Skill_SP_SkyKongKong_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_SkyKongKong_End"), CState_SP_SkyKongKong_End::Create(TEXT("Skill_SP_SkyKongKong_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_SpiritHarmony"), CState_SP_SpiritHarmony::Create(TEXT("Skill_SP_SpiritHarmony"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_SP_Flyheaven"), CState_SP_Flyheaven::Create(TEXT("Skill_SP_Flyheaven"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	return S_OK;
}

HRESULT CPlayer_Doaga::Ready_Skill()
{
	CPlayer_Skill* pSkill = nullptr;
	CPlayer_Skill::PLAYERSKILL_DESC SkillDesc;

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_Inkpaddle");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_Inkpaddle")));
	pSkill = CSkill_SP_Inkpaddle::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_Inkshot");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_Inkshot")));
	pSkill = CSkill_SP_Inkshot::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::W, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_SkyKongKong_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_SkyKongKong_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_SkyKongKong_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_SkyKongKong_End")));
	pSkill = CSkill_SP_SkyKongKong::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::E, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_Flyheaven");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_Flyheaven")));
	pSkill = CSkill_SP_Flyheaven::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::R, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_Onestroke");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_Onestroke")));
	pSkill = CSkill_SP_Onestroke::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::A, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_Moondrawing_Start//Banish");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_Moondrawing_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_Moondrawing_End")));
	pSkill = CSkill_SP_Moondrawing::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::S, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_SpiritHarmony//Banish");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_SpiritHarmony")));
	pSkill = CSkill_SP_SpiritHarmony::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::D, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_SP_DimensionalShift_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_DimensionalShift_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_SP_DimensionalShift_End")));
	pSkill = CSkill_SP_DimensionalShift::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::F, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();



	return S_OK;
}

HRESULT CPlayer_Doaga::Ready_Coliders()
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

HRESULT CPlayer_Doaga::Ready_PhysxBoneBranch()
{
	//m_pModelCom->Play_Animation(10.0f);


	//CPhysXMgr::GetInstance()->Add_Player(this);

	//{
	//	vector<_uint> Bones;

	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_01"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_02"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_03"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_04"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_05"));

	//	CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	//}

	//{
	//	vector<_uint> Bones;

	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_01"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_02"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_03"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_04"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_05"));

	//	CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	//}

	//{
	//	vector<_uint> Bones;

	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_01"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_02"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_03"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_04"));

	//	CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	//}

	//{
	//	vector<_uint> Bones;

	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_01"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_02"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_03"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_04"));

	//	CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	//}

	//{
	//	vector<_uint> Bones;

	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_01"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_02"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_03"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_04"));

	//	CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	//}

	///*{
	//	vector<_uint> Bones;

	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_01"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_02"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_03"));
	//	Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_04"));

	//	CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	//}*/

	return S_OK;
}

HRESULT CPlayer_Doaga::Ready_Item()
{
	CItem* pItem = nullptr;
	CItem_Manager* pItemManager = GET_INSTANCE(CItem_Manager);
	
	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_Helmet_Dino);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	Use_Item(pItem->Get_ObjectTag(), 1, false);

	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_Body_Dino);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	Use_Item(pItem->Get_ObjectTag(), 1, false);

	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_WP_Dino);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);
	Use_Item(pItem->Get_ObjectTag(), 1, false);

	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_WP_Legend);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_Helmet_Legend);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_Body_Legend);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	pItem = pItemManager->GetInstance()->Get_Item(ITEMCODE::SP_Leg_Legend);
	if (nullptr == pItem)
		return E_FAIL;

	Add_Item(pItem->Get_ObjectTag(), pItem);

	Safe_Release(pItemManager);
	return S_OK;
}

HRESULT CPlayer_Doaga::Ready_Esther()
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

HRESULT CPlayer_Doaga::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Doaga* CPlayer_Doaga::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Doaga* pInstance = new CPlayer_Doaga(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Doaga");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Doaga::Clone(void* pArg)
{
	CPlayer_Doaga* pInstance = new CPlayer_Doaga(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Doaga");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Doaga::Free()
{
	__super::Free();
}
