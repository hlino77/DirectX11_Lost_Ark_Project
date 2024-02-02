#include "stdafx.h"
#include "Item_Manager.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Item.h"

//ITEM
#include "IT_GN_Body_Mococo.h"
#include "IT_GN_Helmet_Mococo.h"
#include "IT_GN_WP_Mococo.h"
#include "IT_GN_Body_Legend.h"
#include "IT_GN_Helmet_Legend.h"
#include "IT_GN_Leg_Legend.h"
#include "IT_GN_WP_Legend.h"

#include "IT_MG_Body_Mococo.h"
#include "IT_MG_Helmet_Mococo.h"
#include "IT_MG_WP_Mococo.h"
#include "IT_MG_WP_Legend.h"
#include "IT_MG_Body_Legend.h"
#include "IT_MG_Leg_Legend.h"
#include "IT_MG_Helmet_Legend.h"

#include "IT_WR_Body_Mococo.h"
#include "IT_WR_Helmet_Mococo.h"
#include "IT_WR_WP_Mococo.h"
#include "IT_WR_Body_Legend.h"
#include "IT_WR_Helmet_Legend.h"
#include "IT_WR_Leg_Legend.h"
#include "IT_WR_WP_Legend.h"

#include "IT_WDR_Body_Mococo.h"
#include "IT_WDR_Helmet_Mococo.h"
#include "IT_WDR_WP_Mococo.h"
#include "IT_WDR_Body_Legend.h"
#include "IT_WDR_Helmet_Legend.h"
#include "IT_WDR_WP_Legend.h"

#include "IT_SP_Body_Dino.h"
#include "IT_SP_Helmet_Dino.h"
#include "IT_SP_WP_Dino.h"
#include "IT_SP_Body_Legend.h"
#include "IT_SP_Helmet_Legend.h"
#include "IT_SP_Leg_Legend.h"
#include "IT_SP_WP_Legend.h"


IMPLEMENT_SINGLETON(CItem_Manager);


CItem_Manager::CItem_Manager()
{
}

HRESULT CItem_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	if (FAILED(Loading_Inventory_UI()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	/* Àåºñ */
	wstring strPath = L"../Bin/Resources/Meshes/PC_Part/";
	AutoLoad(strPath, LEVEL_STATIC);

	strPath = L"../Bin/Resources/Meshes/PC_Default/";
	AutoLoad(strPath, LEVEL_STATIC);

	strPath = L"../Bin/Resources/Meshes/PC_Weapon/";
	AutoLoad(strPath, LEVEL_STATIC, PivotMatrix);


	/* ITEM */

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_Body_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_Body_Mococo);
		m_Items.emplace(ITEMCODE::GN_Body_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_Helmet_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_Helmet_Mococo);
		m_Items.emplace(ITEMCODE::GN_Helmet_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_WP_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_WP_Mococo);
		m_Items.emplace(ITEMCODE::GN_WP_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_WP_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_WP_Legend);
		m_Items.emplace(ITEMCODE::GN_WP_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_Helmet_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_Helmet_Legend);
		m_Items.emplace(ITEMCODE::GN_Helmet_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_Body_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_Body_Legend);
		m_Items.emplace(ITEMCODE::GN_Body_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_GN_Leg_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::GN_Leg_Legend);
		m_Items.emplace(ITEMCODE::GN_Leg_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_Body_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_Body_Mococo);
		m_Items.emplace(ITEMCODE::MG_Body_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_Helmet_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_Helmet_Mococo);
		m_Items.emplace(ITEMCODE::MG_Helmet_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_WP_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_WP_Mococo);
		m_Items.emplace(ITEMCODE::MG_WP_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_Body_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_Body_Mococo);
		m_Items.emplace(ITEMCODE::WR_Body_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_Helmet_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_Helmet_Mococo);
		m_Items.emplace(ITEMCODE::WR_Helmet_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_WP_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_WP_Mococo);
		m_Items.emplace(ITEMCODE::WR_WP_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_Body_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_Body_Legend);
		m_Items.emplace(ITEMCODE::WR_Body_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_Helmet_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_Helmet_Legend);
		m_Items.emplace(ITEMCODE::WR_Helmet_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_Leg_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_Leg_Legend);
		m_Items.emplace(ITEMCODE::WR_Leg_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WR_WP_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WR_WP_Legend);
		m_Items.emplace(ITEMCODE::WR_WP_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WDR_Body_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WDR_Body_Mococo);
		m_Items.emplace(ITEMCODE::WDR_Body_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WDR_Helmet_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WDR_Helmet_Mococo);
		m_Items.emplace(ITEMCODE::WDR_Helmet_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WDR_WP_Mococo::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WDR_WP_Mococo);
		m_Items.emplace(ITEMCODE::WDR_WP_Mococo, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WDR_WP_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WDR_WP_Legend);
		m_Items.emplace(ITEMCODE::WDR_WP_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WDR_Body_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WDR_Body_Legend);
		m_Items.emplace(ITEMCODE::WDR_Body_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_WDR_Helmet_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::WDR_Helmet_Legend);
		m_Items.emplace(ITEMCODE::WDR_Helmet_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_Helmet_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_Helmet_Legend);
		m_Items.emplace(ITEMCODE::MG_Helmet_Legend, pItem);
	}
	
	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_Body_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_Body_Legend);
		m_Items.emplace(ITEMCODE::MG_Body_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_Leg_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_Leg_Legend);
		m_Items.emplace(ITEMCODE::MG_Leg_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_MG_WP_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::MG_WP_Legend);
		m_Items.emplace(ITEMCODE::MG_WP_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_WP_Dino::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_WP_Dino);
		m_Items.emplace(ITEMCODE::SP_WP_Dino, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_Body_Dino::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_Body_Dino);
		m_Items.emplace(ITEMCODE::SP_Body_Dino, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_Helmet_Dino::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_Helmet_Dino);
		m_Items.emplace(ITEMCODE::SP_Helmet_Dino, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_Helmet_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_Helmet_Legend);
		m_Items.emplace(ITEMCODE::SP_Helmet_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_Body_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_Body_Legend);
		m_Items.emplace(ITEMCODE::SP_Body_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_Leg_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_Leg_Legend);
		m_Items.emplace(ITEMCODE::SP_Leg_Legend, pItem);
	}

	{
		CItem* pItem = nullptr;
		pItem = CIT_SP_WP_Legend::Create(m_pDevice, m_pContext);
		if (nullptr == pItem)
			return E_FAIL;
		pItem->Initialize(nullptr);
		pItem->Set_ItemCode(ITEMCODE::SP_WP_Legend);
		m_Items.emplace(ITEMCODE::SP_WP_Legend, pItem);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

CItem* CItem_Manager::Get_Item(ITEMCODE eItem)
{
	auto& iter = m_Items.find(eItem);

	if (iter == m_Items.end())
		return nullptr;

	return (*iter).second;
}

HRESULT CItem_Manager::Loading_Inventory_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_InventoryWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Inventory/InventoryWnd.png"))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_ItemSlot"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Inventory/Inventory_ItemSlot.png"))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_PickedSlot_Effect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Inventory/PickedSlot_Effect.png"))))
		return E_FAIL;
	

	//Gunslinger
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_MococoHead"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_MococoHead.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_MococoBody"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_MococoBody.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Mococo_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_Mococo_Weapon.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Legend_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_Legend_Head.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Legend_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_Legend_Body.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Legend_Leg"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_Legend_Leg.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Legend_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Gunslinger/Gunslinger_Legend_Weapon.png"))))
			return E_FAIL;
		
	}

	//Destroyer
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Mococo_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Destroyer/Desrtoyer_Mococo_Head.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Mococo_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Destroyer/Desrtoyer_Mococo_Body.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Mococo_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Destroyer/Desrtoyer_Mococo_Weapon.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Legend_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Destroyer/Desrtoyer_Legend_Head.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Legend_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Destroyer/Desrtoyer_Legend_Body.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Legend_Weaopn"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Destroyer/Desrtoyer_Legend_Weaopn.png"))))
			return E_FAIL;
		
	}

	//Bard
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Mococo_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Mococo_Head.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Mococo_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Mococo_Body.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Mococo_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Mococo_Weapon.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Legend_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Legend_Head.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Legend_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Legend_Body.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Legend_Leg"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Legend_Leg.png"))))
			return E_FAIL;
		

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Legend_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Bard/Bard_Legend_Weapon.png"))))
			return E_FAIL;
		
	}

	//Slayer
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Mococo_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Mococo_Head.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Mococo_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Mococo_Body.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Mococo_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Mococo_Weapon.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Legend_Head"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Legend_Head.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Legend_Body"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Legend_Body.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Legend_Leg"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Legend_Leg.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Legend_Weapon"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Slayer/Slayer_Legend_Weapon.png"))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CItem_Manager::Loading_Item()
{
	
	return S_OK;
}

HRESULT CItem_Manager::AutoLoad(const filesystem::path& strPath, LEVELID eLevel, Matrix Pivot)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (const auto& entry : filesystem::directory_iterator(strPath))
	{
		if (filesystem::is_directory(entry))
		{
			{
				wstring strFileName = entry.path().filename();
				wstring strFilePath = entry.path().parent_path();
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(eLevel, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(eLevel, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath + L"/", strFileName, true, false, Pivot))))
						return E_FAIL;
				}
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


void CItem_Manager::Free()
{
	for (auto& Item : m_Items)
	{
		Safe_Release(Item.second);
	}
	m_Items.clear();
}

