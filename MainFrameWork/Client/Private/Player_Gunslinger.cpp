#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Gunslinger.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Player_Controller_GN.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"

/* State */
#include "State_GN_Idle.h"
#include "State_GN_Run.h"
#include "NavigationMgr.h"
#include "State_GN_Dash.h"
#include "State_GN_Attack_Hand1.h"
#include "State_GN_Attack_Hand2.h"
#include "State_GN_Attack_Hand3.h"
#include "State_GN_Attack_Shot1.h"
#include "State_GN_Attack_Shot2.h"
#include "State_GN_Attack_Long1.h"
#include "State_GN_Attack_Long2.h"
#include "State_GN_Identity.h"
#include "State_GN_Identity_Back.h"
#include "State_GN_Run_Identity.h"
#include "State_GN_Run_Identity_Back.h"
#include "State_GN_Skill_Q.h"
#include "State_GN_Skill_W.h"
#include "State_GN_Skill_E.h"
#include "State_GN_Skill_R.h"
#include "State_GN_Skill_A.h"
#include "State_GN_Skill_S.h"
#include "State_GN_Skill_D.h"
#include "State_GN_Skill_F.h"

/* Skill */
#include "Skill_GN_FreeShooter.h"
#include "Skill_GN_FocusShot.h"
#include "Skill_GN_QuickStep.h"

CPlayer_Gunslinger::CPlayer_Gunslinger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Gunslinger::CPlayer_Gunslinger(const CPlayer_Gunslinger& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Gunslinger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Gunslinger::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	if (m_bControl)
	{
		if (FAILED(Ready_SkillUI()))
			return E_FAIL;
	}
	m_fAttackMoveSpeed = 8.0f;

	m_vHairColor_1 = { 0.78f, 0.78f, 0.78f, 1.f };
	m_vHairColor_2 = { 0.82f, 0.82f, 0.82f, 1.f };


	CPlayer_Controller_GN::GN_IDENTITY m_eIdentity = m_pController->Get_GN_Identity();
	Set_Weapon_RenderState(m_eIdentity);

	/* 플레이어 공통 요소 */
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_matTargetWorld = Desc->matWorld;
	m_vTargetPos = Desc->vTargetPos;

	Vec3 vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_WorldMatrix(Desc->matWorld);
	m_pTransformCom->Set_Scale(vScale);

	m_pStateMachine->Change_State(Desc->szState);

	CNavigationMgr::GetInstance()->Find_FirstCell(this);


	if (FAILED(Ready_Coliders()))
		return E_FAIL;


	return S_OK;
}

void CPlayer_Gunslinger::Tick(_float fTimeDelta)
{
	m_pStateMachine->Tick_State(fTimeDelta);
	m_pController->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CPlayer_Gunslinger::LateTick(_float fTimeDelta)
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

HRESULT CPlayer_Gunslinger::Render()
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

HRESULT CPlayer_Gunslinger::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{

			if (FAILED(m_pModelPartCom[i]->Render(m_pShaderCom, j, 3)))
				return S_OK;
		}
	}

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CPlayer_Gunslinger::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
		cout << "플레이어 Body : 몬스터 Body -> ENTER" << endl;

	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER && pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
		cout << "플레이어 Attack : 몬스터 Body -> ENTER" << endl;
}

void CPlayer_Gunslinger::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
		cout << "플레이어 Body : 몬스터 Body -> EXIT" << endl;

	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER && pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
		cout << "플레이어 Attack : 몬스터 Body -> EXIT" << endl;
}

void CPlayer_Gunslinger::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Gunslinger::Send_PlayerInfo()
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

void CPlayer_Gunslinger::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

void CPlayer_Gunslinger::Set_Weapon_RenderState(_uint iIndex, _bool Is_Shot2)
{
	switch (iIndex)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_Parts[CPartObject::PARTS::WEAPON_1]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_2]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_3]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_4]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(false);
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_Parts[CPartObject::PARTS::WEAPON_1]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_2]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_3]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_4]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(false);
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_Parts[CPartObject::PARTS::WEAPON_1]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_2]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_3]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_4]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(false);
		break;
	}

	if (true == Is_Shot2)
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(true);

}

void CPlayer_Gunslinger::Set_Several_Weapon_RenderState(CPartObject::PARTS ePart, _bool Is_Render)
{
	m_Parts[ePart]->Set_Render(Is_Render);
}

HRESULT CPlayer_Gunslinger::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CPlayer_Controller_GN::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_GN"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;

	/* 초기 장비 및 얼굴 설정 */
	wstring strComName = L"Prototype_Component_Model_GN_Legend_Helmet";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_Legend_Body";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_Legend_Leg";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Leg"), (CComponent**)&m_pModelPartCom[(_uint)PART::LEG])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_Face";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pModelPartCom[(_uint)PART::FACE])))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_Parts()
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
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Hand"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	/* For.Part_Weapon_2 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_2;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Hand_2"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_2, pParts);

	/* For.Part_Weapon_3 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_3;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Long"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_3, pParts);

	/* For.Part_Weapon_4 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_4;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Shot"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_4, pParts);

	/* For.Part_Weapon_5 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_5;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Shot"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_5, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_GN_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Run"), CState_GN_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash"), CState_GN_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN"), CState_GN_Identity::Create(TEXT("Identity_GN"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN_Back"), CState_GN_Identity_Back::Create(TEXT("Identity_GN_Back"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN_Run"), CState_GN_Run_Identity::Create(TEXT("Identity_GN_Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN_Run_Back"), CState_GN_Run_Identity_Back::Create(TEXT("Identity_GN_Run_Back"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Hand_1"), CState_GN_Attack_Hand1::Create(TEXT("Attack_Hand_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Hand_2"), CState_GN_Attack_Hand2::Create(TEXT("Attack_Hand_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Hand_3"), CState_GN_Attack_Hand3::Create(TEXT("Attack_Hand_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Shot_1"), CState_GN_Attack_Shot1::Create(TEXT("Attack_Shot_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Shot_2"), CState_GN_Attack_Shot2::Create(TEXT("Attack_Shot_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Long_1"), CState_GN_Attack_Long1::Create(TEXT("Attack_Long_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Long_2"), CState_GN_Attack_Long2::Create(TEXT("Attack_Long_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_Q"), CState_GN_Skill_Q::Create(TEXT("Skill_Q"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_W"), CState_GN_Skill_W::Create(TEXT("Skill_W"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_E"), CState_GN_Skill_E::Create(TEXT("Skill_E"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_R"), CState_GN_Skill_R::Create(TEXT("Skill_R"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_A"), CState_GN_Skill_A::Create(TEXT("Skill_A"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_S"), CState_GN_Skill_S::Create(TEXT("Skill_S"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_D"), CState_GN_Skill_D::Create(TEXT("Skill_D"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_F"), CState_GN_Skill_F::Create(TEXT("Skill_F"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));


	m_pStateMachine->Change_State(TEXT("Idle"));

	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_Skill()
{
	/* 핸드건 */
	m_pController->Bind_HandSkill(CPlayer_Controller_GN::SKILL_KEY::Q, CSkill_GN_QuickStep::Create(this));

	/* 샷건 */
	m_pController->Bind_ShotSkill(CPlayer_Controller_GN::SKILL_KEY::Q, CSkill_GN_FreeShooter::Create(this));

	/* 장총 */
	m_pController->Bind_LongSkill(CPlayer_Controller_GN::SKILL_KEY::Q, CSkill_GN_FocusShot::Create(this));


	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_Coliders()
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


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = true;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
				CSphereColliderGroup* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColliderGroup"), TEXT("Com_ColliderAttackChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER, pCollider);
		}
	}

	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER]->SetActive(true);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER]->Set_Radius(6.0f);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER]->Set_Offset(Vec3(0.0f, 0.2f, 0.0f));


		CSphereColliderGroup* pGroup = dynamic_cast<CSphereColliderGroup*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER]->Get_Child());


		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		Vec3 vOffset(0.0f, 0.0f, 4.0f);
		Quaternion vQuat = Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(360.0f / 100.0f));

		for (_uint i = 0; i < 100; ++i)
		{
			CCollider::ColliderInfo tColliderInfo;
			tColliderInfo.m_bActive = true;
			tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;

			CSphereCollider* pCollider = dynamic_cast<CSphereCollider*>(pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), &tColliderInfo));
			if (nullptr == pCollider)
				return E_FAIL;

			vOffset = XMVector3Rotate(vOffset, vQuat);

			pCollider->SetActive(true);
			pCollider->Set_Radius(0.6f);
			pCollider->Set_Offset(vOffset);


			pGroup->Add_Collider(pCollider);
		}


		Safe_Release(pGameInstance);
	}





	//m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK]->Set_Radius(0.5f);
	//m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK]->SetActive(false);
	//m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK]->Set_Offset(Vec3(0.0f, 0.7f, 1.0f));


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

HRESULT CPlayer_Gunslinger::Ready_PhysxBoneBranch()
{
	return E_NOTIMPL;
}

HRESULT CPlayer_Gunslinger::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Gunslinger* CPlayer_Gunslinger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Gunslinger* pInstance = new CPlayer_Gunslinger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Gunslinger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Gunslinger::Clone(void* pArg)
{
	CPlayer_Gunslinger* pInstance = new CPlayer_Gunslinger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Gunslinger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Gunslinger::Free()
{
	__super::Free();
}
