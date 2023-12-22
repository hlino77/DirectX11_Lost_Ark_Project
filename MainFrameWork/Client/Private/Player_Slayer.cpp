#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Slayer.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "NavigationMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Controller_WR.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

/* State */
#include "State_WR_Idle.h"
#include "State_WR_Run.h"
#include "State_WR_Dash.h"

/* State_Skill */


/* 스킬 */


CPlayer_Slayer::CPlayer_Slayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Slayer::CPlayer_Slayer(const CPlayer_Slayer& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Slayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Slayer::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	if (FAILED(Ready_PhysxBoneBranch()))
		return E_FAIL;

	if (m_bControl)
	{
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

	CNavigationMgr::GetInstance()->Find_FirstCell(this);


	return S_OK;
}

void CPlayer_Slayer::Tick(_float fTimeDelta)
{
	m_pStateMachine->Tick_State(fTimeDelta);
	m_pController->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CPlayer_Slayer::LateTick(_float fTimeDelta)
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

HRESULT CPlayer_Slayer::Render()
{
	__super::Render();

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (i == 1/*hair*/ && j == 1/*m_iHairIndex*/)
			{
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_vHairColor_1, sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_vHairColor_2, sizeof(Vec4)))))
					return E_FAIL;

				if (FAILED(m_pModelPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &Vec4(), sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &Vec4(), sizeof(Vec4)))))
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

HRESULT CPlayer_Slayer::Render_ShadowDepth()
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

void CPlayer_Slayer::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CPlayer_Slayer::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Slayer::Send_PlayerInfo()
{
	Protocol::S_OBJECTINFO pkt;

	auto tPlayerInfo = pkt.add_tobject();

	tPlayerInfo->set_iobjectid(m_iObjectID);
	tPlayerInfo->set_ilevel(LEVEL_STATIC);
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

void CPlayer_Slayer::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

void CPlayer_Slayer::Set_Several_Weapon_RenderState(CPartObject::PARTS ePart, _bool Is_Render)
{
	m_Parts[ePart]->Set_Render(Is_Render);
}

HRESULT CPlayer_Slayer::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CController_WR::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_WR"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;

	/* 초기 장비 및 얼굴 설정 */
	wstring strComName = L"Prototype_Component_Model_WR_Head_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_WR_Body_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY])))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_Parts()
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
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_WP_WR_Base"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_WR_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Idle"), CState_WR_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Idle"), CState_WR_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_Skill()
{
	//CPlayer_Skill* pSkill = nullptr;
	//CPlayer_Skill::PLAYERSKILL_DESC SkillDesc;
	///* 핸드건 스킬 */
	//SkillDesc.pOwner = this;
	//SkillDesc.strSkill_StartName = TEXT("Skill_GN_QuickStep_Start");
	//SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_QuickStep_Start")));
	//SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_QuickStep_End")));
	//pSkill = CSkill_GN_QuickStep::Create(m_pDevice, m_pContext, this, &SkillDesc);
	//m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	//m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	//SkillDesc.State_Skills.clear();

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_Coliders()
{
	if (false == m_bControl)
		return S_OK;

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

HRESULT CPlayer_Slayer::Ready_PhysxBoneBranch()
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

HRESULT CPlayer_Slayer::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Slayer* CPlayer_Slayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Slayer* pInstance = new CPlayer_Slayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Slayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Slayer::Clone(void* pArg)
{
	CPlayer_Slayer* pInstance = new CPlayer_Slayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Slayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Slayer::Free()
{
	__super::Free();
}
