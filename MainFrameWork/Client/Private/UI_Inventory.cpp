#include "stdafx.h"
#include "UI_Inventory.h"
#include "GameInstance.h"
#include "UI_InventoryWnd.h"
#include "UI_Manager.h"
#include "UI_Inventory_ItemSlot.h"
#include "Player.h"
#include "Item.h"

CUI_Inventory::CUI_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Inventory::CUI_Inventory(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Inventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory::Initialize(void* pArg)
{
	
	if (nullptr != pArg)
	{
		m_pOwner = static_cast<CPlayer*>(pArg);
		if (nullptr == m_pOwner)
			return E_FAIL;
	}
	else 
	{
		m_pOwner = static_cast<CPlayer*>(CGameInstance::GetInstance()->Find_CtrlPlayer(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER));
		if (nullptr == m_pOwner)
			return E_FAIL;
	}

	if (FAILED(UI_SET()))
		return E_FAIL;

	return S_OK;
}

void CUI_Inventory::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::I))
	{
		Update_Used_Item();
		m_bActiveKey = !m_bActiveKey;
		
		for (auto& iter : m_vecUIParts)
		{
			iter->Set_Render(m_bActiveKey);
			iter->Set_Active(m_bActiveKey);
		}
	}
}

void CUI_Inventory::LateTick(_float fTimeDelta)
{
	if (m_bActiveKey)
	{
		Move_InventoryWNd();
		static_cast<CUI_InventoryWnd*>(m_pInventoryWnd)->Set_Player_Control(m_pOwner, m_bActiveKey);
	}
}

HRESULT CUI_Inventory::Render()
{
	return S_OK;
}

void CUI_Inventory::Update_Used_Item()
{
	if (!m_pOwner->Is_Control())
		return;

	for (auto& iter : m_vecUIParts)
	{
		if (TEXT("UI_InventoryWnd") == iter->Get_UITag())
			continue;
		static_cast<CUI_Inventory_ItemSlot*>(iter)->Clear_ItemSlot();
	}

	auto& iter = m_vecUIParts.begin();
	++iter;
	for (auto& iterator : m_pOwner->Get_Items())
	{
		if ((1 > iterator.vecItems.size()) || (nullptr == iterator.vecItems.front()))
		{
			++iter;
			continue;
		}
		static_cast<CUI_Inventory_ItemSlot*>((*iter))->Set_ItemInfo(iterator.vecItems);
		++iter;
	}
}

void CUI_Inventory::Update_ReSort_Inventory()
{
	if (!m_pOwner->Is_Control())
		return;

	for (auto& iter : m_vecUIParts)
	{
		if (TEXT("UI_InventoryWnd") == iter->Get_UITag())
			continue;
		static_cast<CUI_Inventory_ItemSlot*>(iter)->Clear_ItemSlot();
	}

	auto& iter = m_vecUIParts.begin();
	++iter;
	for (auto& iterator : m_pOwner->Get_Items())
	{
		if ((1 > iterator.vecItems.size()) || (iterator.vecItems.front()->Get_EquipWearing()))
		{
			++iter;
			continue;
		}
		static_cast<CUI_Inventory_ItemSlot*>((*iter))->Set_ItemInfo(iterator.vecItems);
		++iter;
	}
}

void CUI_Inventory::Move_InventoryWNd()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;

	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort);

	static_cast<CUI_InventoryWnd*>(m_pInventoryWnd)->Move_InventoryWnd(pt);
	_float fX = m_pInventoryWnd->Get_UIDesc().fX - (m_pInventoryWnd->Get_UIDesc().fSizeX * 0.5f) + 30.f;
	_float fY = m_pInventoryWnd->Get_UIDesc().fY - (m_pInventoryWnd->Get_UIDesc().fSizeY * 0.5f - 70.f);
	for (auto& iter : m_vecUIParts)
	{
		if(TEXT("UI_InventoryWnd") != iter->Get_UITag())
			static_cast<CUI_Inventory_ItemSlot*>(iter)->Following_InventortWnd(fX, fY);
	}
}

HRESULT CUI_Inventory::Ready_Components()
{
	return S_OK;
}

HRESULT CUI_Inventory::UI_SET()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pInventoryWnd = static_cast<CUI*>(pGameInstance->
		Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_InventoryWnd"), m_pOwner));

	if (nullptr == m_pInventoryWnd)
		return E_FAIL;
	else
		m_vecUIParts.push_back(m_pInventoryWnd);

	INVEN_ITEMDESC* pInvenDesc;
	ZeroMemory(&pInvenDesc, sizeof(INVEN_ITEMDESC*));
	pInvenDesc = new INVEN_ITEMDESC;
	pInvenDesc->pUIWnd = m_pInventoryWnd;
	pInvenDesc->pPlayer = m_pOwner;
	pInvenDesc->vecItemSlots = m_pOwner->Get_Items();
	pInvenDesc->fX = m_pInventoryWnd->Get_UIDesc().fX - (m_pInventoryWnd->Get_UIDesc().fSizeX * 0.5f) + 40.f;
	pInvenDesc->fY = m_pInventoryWnd->Get_UIDesc().fY - (m_pInventoryWnd->Get_UIDesc().fSizeY * 0.5f - 70.f);
	_uint iXIndex = 0;
	_uint iYIndex = 0;
	_uint iSlotIndex = 0;
	CUI* pUI = { nullptr };

	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			pInvenDesc->iSlotIndexX = j;
			pInvenDesc->iSlotIndexY = i;
			pInvenDesc->iSlotIndex = iSlotIndex;
			pUI = static_cast<CUI*>(pGameInstance->
				Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_Inventory_ItemSlot"),
					pInvenDesc));

			if (nullptr == pUI)
				return E_FAIL;
			else
			{
				pUI->Set_Active(false);
				m_vecUIParts.push_back(pUI);
			}
			++iSlotIndex;
		}
	}
	
	auto& iter = m_vecUIParts.begin();
	++iter;
	for (auto& iterator : m_pOwner->Get_Items())
	{
		if ((1 > iterator.vecItems.size())||(iterator.vecItems.front()->Get_EquipWearing()))
			continue;

		static_cast<CUI_Inventory_ItemSlot*>((*iter))->Set_ItemInfo(iterator.vecItems);
		++iter;
	}

	delete(pInvenDesc);
	Safe_Release(pGameInstance);
	return S_OK;
}

CUI_Inventory* CUI_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory* pInstance = new CUI_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory::Clone(void* pArg)
{
	CUI_Inventory* pInstance = new CUI_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory::Free()
{
	__super::Free();
}
