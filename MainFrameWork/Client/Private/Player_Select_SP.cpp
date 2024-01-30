#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Select_SP.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

CPlayer_Select_SP::CPlayer_Select_SP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer_Select(pDevice, pContext)
{
}

CPlayer_Select_SP::CPlayer_Select_SP(const CPlayer_Select_SP& rhs)
	: CPlayer_Select(rhs)
{
}

HRESULT CPlayer_Select_SP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Select_SP::Initialize(void* pArg)
{
	m_strObjectTag = TEXT("SP");

	__super::Initialize(pArg);

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_Part()))
		return E_FAIL;

	m_vSelectPos = Vec3(1.05f, 0.04f, 0.877f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSelectPos);
	m_pTransformCom->My_Rotation(Vec3(0.f, 190.f, 0.f));

	m_iSelectAnim_Normal = m_pModelCom->Initailize_FindAnimation(L"characterdeselection_normal_loop_4", 1.0f);
	if (m_iSelectAnim_Normal == -1)
		return E_FAIL;

	m_iSelectAnim_Start = m_pModelCom->Initailize_FindAnimation(L"characterselection_normal_start_4", 1.0f);
	if (m_iSelectAnim_Start == -1)
		return E_FAIL;

	m_iSelectAnim_Loop = m_pModelCom->Initailize_FindAnimation(L"characterselection_normal_loop_4", 1.0f);
	if (m_iSelectAnim_Start == -1)
		return E_FAIL;

	m_iSelectAnim_End = m_pModelCom->Initailize_FindAnimation(L"characterselection_normal_end_4", 1.0f);
	if (m_iSelectAnim_Start == -1)
		return E_FAIL;

	m_iSelectAnim = m_iSelectAnim_Normal;
	m_pModelCom->Set_CurrAnim(m_iSelectAnim);

	m_vHairColor_1 = { 0.78f, 0.78f, 0.78f, 1.f };
	m_vHairColor_2 = { 0.82f, 0.82f, 0.82f, 1.f };

	return S_OK;
}

void CPlayer_Select_SP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pPart->Tick(fTimeDelta);
}

void CPlayer_Select_SP::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Set_Colliders(fTimeDelta);

	m_pPart->LateTick(fTimeDelta);
}

HRESULT CPlayer_Select_SP::Render()
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

HRESULT CPlayer_Select_SP::Render_ShadowDepth()
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

void CPlayer_Select_SP::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

HRESULT CPlayer_Select_SP::Ready_Components()
{
	__super::Ready_Components();

	/* 초기 장비 및 얼굴 설정 */
	CModel::CHANGECOLOR pChangeColor;
	pChangeColor.vColor_R = Vec4(0.089888f * 0.679775f, 1.f * 0.679775f, 0.673617f * 0.679775f, 1.f);
	pChangeColor.vColor_G = Vec4(0.981382f, 0.2061f, 0.171599f, 1.f);
	pChangeColor.vColor_B = Vec4(1.44715f, 0.268293f, 0.34582f, 1.f);

	wstring strComName = L"Prototype_Component_Model_SP_Helmet_Dino";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET], &pChangeColor)))
		return E_FAIL;

	m_IsHair = m_pModelPartCom[(_uint)PART::HELMET]->Is_HairTexture();

	/* 초기 장비 및 얼굴 설정 */
	strComName = L"Prototype_Component_Model_SP_Face";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pModelPartCom[(_uint)PART::FACE])))
		return E_FAIL;

	pChangeColor.vColor_R = Vec4(0.089888f * 0.679775f, 1.f * 0.679775f, 0.673617f * 0.679775f, 1.f);
	pChangeColor.vColor_G = Vec4(1.30214f, 1.12166f, 0.373086f, 1.f);
	pChangeColor.vColor_B = Vec4(2.11905f, 1.31934f, 0.344261f, 1.f);

	strComName = L"Prototype_Component_Model_SP_Body_Dino";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY], &pChangeColor)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Select_SP::Ready_Coliders()
{
	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->SetActive(true);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Radius(0.5f);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Offset(Vec3(0.0f, 0.4f, 0.0f));


		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.15f, 0.4f, 0.15f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.4f, 0.0f));
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

HRESULT CPlayer_Select_SP::Ready_Part()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Weapon_1 */
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_5;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Tea"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_pPart = pParts;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CPlayer_Select_SP* CPlayer_Select_SP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Select_SP* pInstance = new CPlayer_Select_SP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Select_SP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Select_SP::Clone(void* pArg)
{
	CPlayer_Select_SP* pInstance = new CPlayer_Select_SP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Select_SP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Select_SP::Free()
{
	__super::Free();
}
