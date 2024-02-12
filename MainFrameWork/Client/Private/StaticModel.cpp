#include "stdafx.h"
#include "GameInstance.h"
#include "StaticModel.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "NavigationMgr.h"
#include "ServerSessionManager.h"
#include "Skill.h"
#include "Effect_Manager.h"
#include "Effect_Custom_BreakObject.h"

#include "Sound_Manager.h"

CStaticModel::CStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
	: CGameObject(pDevice, pContext, L"StaticModel", eObjType)
{
}

CStaticModel::CStaticModel(const CStaticModel& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStaticModel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
    return S_OK;
}

HRESULT CStaticModel::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_szModelName = Desc->strFileName;
	m_iLayer = Desc->iLayer;
	m_IsMapObject = Desc->IsMapObject;
	m_bInstance = Desc->bInstance;


	m_BloomColor = Desc->BloomColor; 

#pragma region Bern Grass Object

	// Basic
	if (m_szModelName == TEXT("Anh_Fiilage_Te_b73") || m_szModelName == TEXT("Atm_Foliage2_B94"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Flower
	if (m_szModelName == TEXT("Anh_Fiilage_Te_f02") || m_szModelName == TEXT("Anh_Fiilage_Te_f03") ||
		m_szModelName == TEXT("Anh_Fiilage_Te_f29") || m_szModelName == TEXT("Anh_Fiilage_Te_f30") ||
		m_szModelName == TEXT("Anh_Fiilage_Te_f01") || m_szModelName == TEXT("Atm_Foliage2_Flower02") ||
		m_szModelName == TEXT("Atm_Foliage2_Flower03"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Grass
	if (m_szModelName == TEXT("Anh_Fiilage_Te_b58") || m_szModelName == TEXT("Anh_Fiilage_Te_f26") ||
		m_szModelName == TEXT("Atm_Foliage2_B09") || m_szModelName == TEXT("Atm_Foliage2_Crops04") ||
		m_szModelName == TEXT("Atm_Foliage2_Filedgrass01") || m_szModelName == TEXT("Anh_Fiilage_Crops16"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Tree
	if (m_szModelName == TEXT("Anh_Fiilage_Te_t52"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Bush
	if (m_szModelName == TEXT("Atm_Foliage2_BushTree01") || m_szModelName == TEXT("Anh_Fiilage_Te_b50") ||
		m_szModelName == TEXT("Atm_Foliage2_BushTree02"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}

#pragma endregion

#pragma region Chaos3 Grass Object

	// Flower
	if (m_szModelName == TEXT("Vol_Foliage_Flower05_sky") || m_szModelName == TEXT("Vol_Foliage_Flower05a_sky"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Grass
	if (m_szModelName == TEXT("Vol_Add_LV_Vol_gjunglevol_Foilage01") || m_szModelName == TEXT("Vol_Add_LV_Vol_gjunglevol_Foilage02") ||
		m_szModelName == TEXT("Pap_Foliage_Tropic02") || m_szModelName == TEXT("Pap_Foliage_Tropic01") ||
		m_szModelName == TEXT("Pap_Foliage_Tropic01a") || m_szModelName == TEXT("Vol_Foliage_Flower03_sky") ||
		m_szModelName == TEXT("Vol_Tree_Leaf02"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Tree
	if (m_szModelName == TEXT("Ber_Stone_CherryBlossom01") || m_szModelName == TEXT("Ber_Stone_Ginkgo01") ||
		m_szModelName == TEXT("Ber_Stone_MapleTree01fbx"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}
	// Bush
	if (m_szModelName == TEXT("Vol_Add_LV_Vol_gjunglevol_Foilage08") || m_szModelName == TEXT("Atm_Foliage2_Flower07") ||
		m_szModelName == TEXT("Vol_Tree_Leaf01") || m_szModelName == TEXT("Atm_Foliage_glass02"))
	{
		m_IsGrass = true;
		m_iPass = 6;
	}

#pragma endregion

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPosition);
	m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;

	if (m_szModelName == TEXT("Elevator01d_Edit_Glass") || m_szModelName == TEXT("Ehgeiz_A_Bossfloor01b_Top"))
	{
		m_bInstance = false;
		m_iPass = 1;
	}

	if (m_bInstance)
	{
		if (m_pInstaceData->find(m_szModelName) == m_pInstaceData->end())
		{
			if (FAILED(Ready_Proto_InstanceBuffer()))
				return E_FAIL;
		}

		if (true == m_IsGrass)
		{
			m_iPass = 2;
		}
	}

	m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;
    return S_OK;

}

void CStaticModel::Tick(_float fTimeDelta)
{

#pragma region Specific Object Behavior

	// Chaos 2 Object
	{
		if (m_szModelName == TEXT("Ehrgeiz_A_gear01a") || m_szModelName == TEXT("Ehrgeiz_A_gear01c"))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f * fTimeDelta);
		}
		if (m_szModelName == TEXT("Ehrgeiz_A_gear01b") )
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), -2.f * fTimeDelta);
		}

	}

	// Chaos 3 Object 
	{
		if (m_szModelName == TEXT("Vol_ETC_C_Ship01f") || 
			m_szModelName == TEXT("Vol_Knaly_D_Decocore01h"))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 0.1f * fTimeDelta);
		}

		if (m_szModelName == TEXT("Vol_Add_Etc_Pattern01") ||
			m_szModelName == TEXT("Vol_Add_Etc_Pattern02"))
		{
			m_pTransformCom->Turn_Rotation_CurrentState(XMVectorSet(0.f, 1.f, 0.f, 1.f), 0.3f * fTimeDelta);
		}

		if (m_szModelName == TEXT("Vol_Add_Etc_Pattern02_001"))
		{
			m_pTransformCom->Turn_Rotation_CurrentState(XMVectorSet(0.f, 1.f, 0.f, 1.f), -0.3f * fTimeDelta);
		}

		if (m_szModelName == TEXT("Vol_Add_Gjunglevol_Deco01"))
		{
			m_pTransformCom->Turn_Rotation_CurrentState(XMVectorSet(0.f, 1.f, 0.f, 1.f), -2 * fTimeDelta);
		}

		if (m_szModelName == TEXT("Vol_Add_Knaly_Watergate01a"))
		{
			m_pTransformCom->Turn_Rotation_CurrentState(XMVectorSet(1.f, 0.f, 0.f, 1.f), 0.3f * fTimeDelta);
		}

		if (m_szModelName == TEXT("Vol_Add_Common_Elevator01"))
		{
			_float MaxY = 1.f;
			_float MinY = -16.f;

			Vec3 Position = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			if ((Position.y >= MaxY && bUp == true) ||
				(Position.y <= MinY && bUp == false))
			{
				bStop = true;
				bUp = !bUp;
			}
			// Stop Move
			if (true == bStop)
			{
				fAccTime += fTimeDelta;
				if (fAccTime >= 2.f)
				{
					fAccTime = 0.f;
					bStop = false;
				}
			}
			// Loop Move
			if (true == bUp && false == bStop)
			{
				m_pTransformCom->Go_Up(0.5f * fTimeDelta);
			}
			else if (false == bUp && false == bStop)
			{
				m_pTransformCom->Go_Down(0.5f * fTimeDelta);
			}
		}
	}

#pragma endregion


#pragma region For BreakAble Object 

	// BreakAble Object Active Setting
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::H))
	{
		if (m_StaticColliders.size() != 0)
		{
			Send_Collision(LEVEL_VALTANMAIN, true); // NaviCell Info Send to Server

			for (auto& CellIndex : m_NaviCellIndex)
			{
				CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, true);
			}
			//Set_Dead(true);
			//Set_Active(false);
			m_bBreak = true;

			for (auto& Collider : m_StaticColliders)
			{
				Collider->SetActive(false);
				//Collider->Get_Child()->SetActive(false);
			}
		}
	}


	// Collider Active Setting
	if (false == m_bActive)
	{

		for (auto& Collider : m_StaticColliders)
		{
			Collider->SetActive(false);
			//Collider->Get_Child()->SetActive(false);
		}
	}

#pragma endregion


#pragma region Grass Object

	if (true == m_IsGrass)
	{
		m_fWindChangeTime += fTimeDelta;

		if (m_fWindChangeTime >= 2.f)
		{

			m_TargetWindDir.x = ((float)rand() / RAND_MAX) * 1.4f - 0.7f; // Random Range -0.7 ~ 0.7
			m_TargetWindDir.z = ((float)rand() / RAND_MAX) * 1.4f - 0.7f; // Random Range -0.7 ~ 0.7
			m_TargetWindDir.y = 0;

			m_fTargetWindPower = ((float)rand() / RAND_MAX) * 1.8f - 0.9f; // Random Range -0.5 ~ 0.9

			m_fWindChangeTime = 0.f;

		}

		float lerpFactor = fTimeDelta * 0.3;

		m_WindDir = XMVectorLerp(XMLoadFloat3(&m_WindDir), XMLoadFloat3(&m_TargetWindDir), lerpFactor);
		m_fWindPower = m_fWindPower + (m_fTargetWindPower - m_fWindPower) * lerpFactor;

	}

#pragma endregion


}

void CStaticModel::LateTick(_float fTimeDelta)
{
	// BreakAble Object
	if (true == m_bBreak)
	{
		if (false == m_bBreakEffect)
		{
			CEffect_Manager::EFFECTPIVOTDESC tEffectDesc;
			tEffectDesc.pPivotMatrix = &m_pTransformCom->Get_WorldMatrix();
			EFFECT_START(L"DustCloud0", &tEffectDesc);
			EFFECT_START(L"DustCloud1", &tEffectDesc);
			EFFECT_START(L"DustCloud2", &tEffectDesc);

			m_bBreakEffect = true;
		}

		if (false == m_bBreakSound)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"HitWall.wav", 1.f);
			//CGameInstance::GetInstance()->PlaySoundFile(L"HitWall.wav", CHANNEL_EFFECT);

			m_bBreakSound = true;
		}

		if (false == m_bBreakCustomEffect)
		{
			m_bBreakCustomEffect = true;

			CEffect_Custom_BreakObject::BreakObjectDesc BreakDesc;
			BreakDesc.WorldMatrix = this->m_pTransformCom->Get_WorldMatrix();
			BreakDesc.pOwner = this;


#pragma region ITR_022306_GiYurk Break

			if (m_szModelName == TEXT("ITR_022306_GiYurk"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_000";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_002";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_003";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_232";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_268";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_451";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_761";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_919";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1090";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1367";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1511";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1786";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1847";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1848";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02306_Cell_1851";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

			}


#pragma endregion

#pragma region ITR_02307_Pillar_Small Break

			if (m_szModelName == TEXT("ITR_02307_Pillar_Small"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_000";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_287";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_431";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_748";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_782";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_783";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_785";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_790";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);
			}

#pragma endregion

#pragma region ITR_02308_Wall_Small Break

			if (m_szModelName == TEXT("ITR_02308_Wall_Small"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_000";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_287";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_431";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_748";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_782";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_783";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_785";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_789";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02307_Cell_790";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

			}

#pragma endregion

#pragma region ITR_02309_Wall Break

			if (m_szModelName == TEXT("ITR_02309_Wall"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_000";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_106";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1111";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1143";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1178";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1212";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1222";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1256";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1266";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1276";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1344";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1401";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1486";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1507";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1570";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1590";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02309_Cell_1696";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

			}

#pragma endregion

#pragma region ITR_02310_Cube Break

			if (m_szModelName == TEXT("ITR_02310_Cube"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_001";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_002";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_003";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_004";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_005";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_006";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_007";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_008";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_009";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_010";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_011";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_711";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_747";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_748";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_752";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_765";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_840";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_863";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_880";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_898";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_941";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_959";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_1150";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_1168";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_1179";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_1181";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02310_Cell_1236";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

			}

#pragma endregion

#pragma region ITR_02311_BigCube Break

			if (m_szModelName == TEXT("ITR_02311_BigCube"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_004";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_136";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_251";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_314";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_318";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_330";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_332";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_1152";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_1763";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_1832";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_1841";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_1860";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2019";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2080";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2107";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2159";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2194";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2243";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02311_Cell_2372";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);
			}


#pragma endregion

#pragma region ITR_02315_FrontWall1 Break

			if (m_szModelName == TEXT("ITR_02315_FrontWall1"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_000";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_022";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_063";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_102";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_108";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_160";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_162";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_201";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02315_Cell_240";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

			}
#pragma endregion

#pragma region ITR_02316_FrontWall2 Break

			if (m_szModelName == TEXT("ITR_02316_FrontWall2"))
			{
				CGameObject* pObject = nullptr;

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_000";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_247";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_444";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_508";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_565";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_619";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_821";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_871";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_876";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

				BreakDesc.strModelName = L"Prototype_Component_Model_Itr_02316_Cell_915";
				pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
					(_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_BreakObject", &BreakDesc);

			}

#pragma endregion


		}

		m_bRimLight = true;

		if (m_fRimLightTime <= 0.f)
		{
			Set_Active(false);
		}

	}


	// RimLight 
	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;

		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
			m_fRimLightColor = 0.f;
			//Set_Active(false);
		}
	}

	if (nullptr == m_pRendererCom)	
		return;

	if (m_bRender)
	{
		// NonBlend Render
		if (m_iPass == 0)
		{
			m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;

			if (m_bInstance)
			{
				m_pRendererCom->Add_InstanceRenderGroup(m_eRenderGroup, this);
			}
			else
			{
				m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
			}
		}
		else if (m_iPass == 1)
		{
			m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_ALPHABLEND;

			if (m_bInstance)
			{
				m_pRendererCom->Add_InstanceRenderGroup(m_eRenderGroup, this);
			}
			else
			{
				m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
			}
		}
		else if (m_iPass == 2)
		{
			m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;

			if (m_bInstance)
			{
				m_pRendererCom->Add_InstanceRenderGroup(m_eRenderGroup, this);
			}
			else
			{
				m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
			}
		}
		  
		// Draw Collider
		m_pRendererCom->Add_DebugObject(this);
	}

}

HRESULT CStaticModel::Render()
{

	if (m_iPass == 0)
	{
		if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
			return E_FAIL;

		if (FAILED(m_pShaderCom->Push_GlobalWVP()))
			return E_FAIL;


		if (true == m_bRimLight)
		{
			_float fRimLightColor = (_float)m_bRimLight;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}
		else
		{
			_float fRimLightColor = 0.f;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}
	
		if (FAILED(m_pModelCom->Render(m_pShaderCom)))
			return E_FAIL;

		// Init 
		if (true == m_bRimLight)
		{
			_float fRimLightColor = 0.f;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}

	}
	else if (m_iPass == 1)
	{
		if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
			return E_FAIL;

		if (FAILED(m_pShaderCom->Push_GlobalWVP()))
			return E_FAIL;

		if (true == m_bRimLight)
		{
			_float fRimLightColor = (_float)m_bRimLight;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}
		else
		{
			_float fRimLightColor = 0.f;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "Alpha")))
		{
			return E_FAIL;
		}

		// Init 
		if (true == m_bRimLight)
		{
			_float fRimLightColor = 0.f;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}


	}
	else if (m_iPass == 6)
	{
		if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
			return E_FAIL;

		if (FAILED(m_pShaderCom->Push_GlobalWVP()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vWind", &m_WindDir, sizeof(Vec3))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fPower", &m_fWindPower, sizeof(_float))))
			return E_FAIL;


		if (true == m_bRimLight)
		{
			_float fRimLightColor = (_float)m_bRimLight;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}
		else
		{
			_float fRimLightColor = 0.f;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "Grass")))
		{
			return E_FAIL;
		}

		// Init
		if (true == m_bRimLight)
		{
			_float fRimLightColor = 0.f;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;
		}

	}

    return S_OK;

}

HRESULT CStaticModel::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	m_pShaderCom->Push_StaticShadowWVP();

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;

		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
			return S_OK;
	}

	return S_OK;

}

HRESULT CStaticModel::Render_Instance(_uint iSize)
{

	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	{
		(*m_pInstaceData)[m_szModelName].Future_Instance.wait();
		if (FAILED((*m_pInstaceData)[m_szModelName].Future_Instance.get()))
			return E_FAIL;

		CGameInstance::GetInstance()->Execute_BeforeRenderCommandList((*m_pInstaceData)[m_szModelName].pInstanceContext);
		(*m_pInstaceData)[m_szModelName].pInstanceContext = nullptr;
	}

	if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Push_GlobalVP()))
		return E_FAIL;


	// For Grass 
	if (true == m_IsGrass)
	{
		if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Bind_RawValue("g_vWind", &m_WindDir, sizeof(Vec3))))
			return E_FAIL;

		if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Bind_RawValue("g_fPower", &m_fWindPower, sizeof(_float))))
			return E_FAIL;


		if (FAILED(m_pModelCom->Render_GrassInstance((*m_pInstaceData)[m_szModelName].pInstanceBuffer, iSize, (*m_pInstaceData)[m_szModelName].pInstanceShader, sizeof(Matrix))))
			return E_FAIL;
	}
	else
	{
		if (true == m_bRimLight)
		{

	/*		if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Bind_RawValue("g_fRimLight", &m_fRimLightColor, sizeof(_float))))
				return E_FAIL;*/

			_float fRimLightColor = (_float)m_bRimLight;
			if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render_Instance((*m_pInstaceData)[m_szModelName].pInstanceBuffer, iSize, (*m_pInstaceData)[m_szModelName].pInstanceShader, sizeof(Matrix))))
				return E_FAIL;

			fRimLightColor = 0.f;
			if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;

		}
		else
		{
			_float fRimLightColor = 0.f;
			if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render_Instance((*m_pInstaceData)[m_szModelName].pInstanceBuffer, iSize, (*m_pInstaceData)[m_szModelName].pInstanceShader, sizeof(Matrix))))
				return E_FAIL;
		}

	}

	return S_OK;
}

HRESULT CStaticModel::Add_ModelComponent(const wstring& strComName)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CStaticModel::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL && pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)
	{
		Send_Collision(LEVEL_VALTANMAIN, true);

		for (auto& CellIndex : m_NaviCellIndex)
		{
			CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, true);
		}

		for (auto& Collider : m_StaticColliders)
		{
			Collider->SetActive(false);
			Collider->Get_Child()->SetActive(false);
		}
		//Set_Active(false);
		m_bBreak = true; 

	}
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL && pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS)
	{
		if (static_cast<CSkill*> (pOther->Get_Owner())->Is_Destructive())
		{
			//Set_Dead(true);

			Send_Collision(LEVEL_VALTANMAIN, true);

			for (auto& CellIndex : m_NaviCellIndex)
			{
				CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, true);
			}
			//Set_Active(false);
			m_bBreak = true;

			for (auto& Collider : m_StaticColliders)
			{
				Collider->SetActive(false);
				//Collider->Get_Child()->SetActive(false);
			}
		}

	}
}

void CStaticModel::Disable_NaviCells()
{
	for (auto& CellIndex : m_NaviCellIndex)
	{
		CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, true);
	}

	for (auto& Collider : m_StaticColliders)
	{
		Collider->SetActive(false);
		Collider->Get_Child()->SetActive(false);
	}
}

void CStaticModel::Add_Collider()
{
	CSphereCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CSphereCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<CSphereCollider*>(pComponent);
	if (nullptr == pCollider)
		return;

	m_StaticColliders.push_back(pCollider);

	Safe_Release(pGameInstance);
}

void CStaticModel::Add_ChildCollider(_uint iIndex)
{
	COBBCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<COBBCollider*>(pComponent);
	if (nullptr == pCollider)
		return;


	m_StaticColliders[iIndex]->Set_Child(pCollider);

	Safe_Release(pGameInstance);
}
void CStaticModel::Add_CollidersToManager()
{
	for (auto& Collider : m_StaticColliders)
	{
		if (Collider)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider);
		}
	}
}
HRESULT CStaticModel::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	//_vector vScale;
	//vScale.m128_f32[0] = 0.1f;
	//vScale.m128_f32[1] = 0.1f;
	//vScale.m128_f32[2] = 0.1f;

	//m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

void CStaticModel::Add_InstanceData(_uint iSize, _uint& iIndex)
{
	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(((*m_pInstaceData)[m_szModelName].pInstanceValue)->GetValue());


	Matrix matWorld = m_pTransformCom->Get_WorldMatrix();

	if (m_bRimLight)
	{
		matWorld.m[0][3] = (_float)m_fRimLightColor;
	}
	
	(*pInstanceValue)[iIndex] = matWorld;

	if (iSize - 1 == iIndex)
	{
		(*m_pInstaceData)[m_szModelName].Future_Instance = std::async(&CStaticModel::Ready_Instance_For_Render, this, iSize);
		 
		//ThreadManager::GetInstance()->EnqueueJob([=]()
		//	{
		//		promise<HRESULT> PromiseInstance;
		//		m_pInstaceData->Future_Instance = PromiseInstance.get_future();
		//		PromiseInstance.set_value(Ready_Instance_For_Render(iSize));
		//	});
	}
	else
	{
		++iIndex;
	}
		
}

HRESULT CStaticModel::Render_Debug()
{
	for (auto& Collider : m_StaticColliders)
	{
		if (nullptr != Collider)
		{
			Collider->DebugRender();
		}
	}

	return S_OK;
}

HRESULT CStaticModel::Ready_Proto_InstanceBuffer()
{
	(*m_pInstaceData)[m_szModelName].iMaxInstanceCount = 500;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_StaticModelInstace"),
		TEXT("Com_InstanceShader"), (CComponent**)&(*m_pInstaceData)[m_szModelName].pInstanceShader)))
		return E_FAIL;

	(*m_pInstaceData)[m_szModelName].pInstanceValue = new tagTypeLessData<vector<Matrix>>();

	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(((*m_pInstaceData)[m_szModelName].pInstanceValue)->GetValue());
	
	pInstanceValue->resize((*m_pInstaceData)[m_szModelName].iMaxInstanceCount, XMMatrixIdentity());

	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(Matrix) * ((*m_pInstaceData)[m_szModelName].iMaxInstanceCount);
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(Matrix);

		D3D11_SUBRESOURCE_DATA		InitialData;

		InitialData.pSysMem = pInstanceValue->data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &(*m_pInstaceData)[m_szModelName].pInstanceBuffer)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStaticModel::Ready_Instance_For_Render(_uint iSize)
{
	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(((*m_pInstaceData)[m_szModelName].pInstanceValue)->GetValue());

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	(*m_pInstaceData)[m_szModelName].pInstanceContext = CGameInstance::GetInstance()->Get_BeforeRenderContext();

	if ((*m_pInstaceData)[m_szModelName].pInstanceContext == nullptr)
		return E_FAIL;

	if(FAILED((*m_pInstaceData)[m_szModelName].pInstanceContext->Map(((*m_pInstaceData)[m_szModelName].pInstanceBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		return E_FAIL;

	memcpy(SubResource.pData, pInstanceValue->data(), sizeof(Matrix) * iSize);

	(*m_pInstaceData)[m_szModelName].pInstanceContext->Unmap((*m_pInstaceData)[m_szModelName].pInstanceBuffer, 0);

	return S_OK;
}

void CStaticModel::Send_Collision(_uint iLevel, _bool bActive)
{
	Protocol::S_NAVIGATION pkt;

	pkt.set_ilevel(iLevel);
	pkt.set_bactive(bActive);

	auto Indices = pkt.mutable_iindex();
	Indices->Resize((_int)m_NaviCellIndex.size(), -1);
	memcpy(Indices->mutable_data(), m_NaviCellIndex.data(), sizeof(_uint) * m_NaviCellIndex.size());

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

}

CStaticModel* CStaticModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
{
	CStaticModel* pInstance = new CStaticModel(pDevice, pContext, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStaticModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaticModel::Clone(void* pArg)
{
	CStaticModel* pInstance = new CStaticModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStaticModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticModel::Free()
{
	__super::Free();

	for (auto& Collider : m_StaticColliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
