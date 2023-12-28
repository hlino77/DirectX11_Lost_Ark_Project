#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Select_WR.h"
#include "Key_Manager.h"
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

CPlayer_Select_WR::CPlayer_Select_WR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer_Select(pDevice, pContext)
{
}

CPlayer_Select_WR::CPlayer_Select_WR(const CPlayer_Select_WR& rhs)
	: CPlayer_Select(rhs)
{
}

HRESULT CPlayer_Select_WR::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Select_WR::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_PhysxBoneBranch()))
		return E_FAIL;


	return S_OK;
}

void CPlayer_Select_WR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPlayer_Select_WR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Set_Colliders(fTimeDelta);

}

HRESULT CPlayer_Select_WR::Render()
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

HRESULT CPlayer_Select_WR::Render_ShadowDepth()
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

void CPlayer_Select_WR::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

HRESULT CPlayer_Select_WR::Ready_Components()
{
	__super::Ready_Components();

	/* 초기 장비 및 얼굴 설정 */
	wstring strComName = L"Prototype_Component_Model_WR_Head_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_WR_Body_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY])))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Select_WR::Ready_Coliders()
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

HRESULT CPlayer_Select_WR::Ready_PhysxBoneBranch()
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

CPlayer_Select_WR* CPlayer_Select_WR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Select_WR* pInstance = new CPlayer_Select_WR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Select_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Select_WR::Clone(void* pArg)
{
	CPlayer_Select_WR* pInstance = new CPlayer_Select_WR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Select_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Select_WR::Free()
{
	__super::Free();
}
