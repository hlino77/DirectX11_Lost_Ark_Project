#include "stdafx.h"
#include "UI_Inventory_ItemSlot.h"
#include "GameInstance.h"
#include "Player.h"
#include "Item.h"
#include "UI_Inventory.h"
#include "UI_Manager.h"
#include "UI_Mouse_Cursor.h"

CUI_Inventory_ItemSlot::CUI_Inventory_ItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Inventory_ItemSlot::CUI_Inventory_ItemSlot(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Inventory_ItemSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory_ItemSlot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_Inventory_ItemSlot");

	if (nullptr != pArg)
	{
		m_vecItems.clear();
		CUI_Inventory::INVEN_ITEMDESC* pInven_Desc = static_cast<CUI_Inventory::INVEN_ITEMDESC*>(pArg);
		m_vecItems = pInven_Desc->vecItemSlots.front().vecItems;
		m_pOwner = pInven_Desc->pPlayer;
		m_iSlotIndexX = pInven_Desc->iSlotIndexX;
		m_iSlotIndexY = pInven_Desc->iSlotIndexY;
		m_fX = pInven_Desc->fX + (56.f * (_float)m_iSlotIndexX);
		m_fY = pInven_Desc->fY - (56.f * (_float)m_iSlotIndexY);
		m_iSlotIndex = pInven_Desc->iSlotIndex;
		if (nullptr == m_pOwner)
			return E_FAIL;

		if (1 <= m_vecItems.size())//혹시나 모른 쓰레기값 대비
		{
			m_strItemName = m_vecItems.front()->Get_ItemName();
			m_strItemDescript = m_vecItems.front()->Get_ItemDescript();
			m_iItemType = m_vecItems.front()->Get_ItemType();
			m_iEquipType = m_vecItems.front()->Get_ItemType();
			m_iItemGrade = m_vecItems.front()->Get_ItemGrade();
			m_iItemCount = m_vecItems.size();
			m_pTexture_ItemIcon = 
				static_cast<CTexture*>(m_vecItems.front()->Get_ItemTexture()->Clone(nullptr, nullptr));
			Safe_AddRef(m_pTexture_ItemIcon);
			Set_Item_StringInfo(m_iItemType, m_iEquipType, m_iItemGrade);
		}
		else
		{
			m_strItemName = TEXT("");
			m_strItemDescript = TEXT("");
			m_strItemType = TEXT("");
			m_strItemGrade = TEXT("");
			m_iItemType = (_uint)CItem::TYPE::_END;
			m_iEquipType = (_uint)CItem::PART::_END;
			m_iItemGrade = (_uint)CItem::GRADE::_END;
			m_iItemCount = 0;
		}
	}
	m_fSizeX = 56.f * 0.8f;
	m_fSizeY = 56.f * 0.8f;

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	//m_pTransform_ItemIcon
	m_pTransform_ItemIcon->Set_Scale(Vec3(m_fSizeX * 0.9f , m_fSizeY * 0.9f, 1.f));
	m_pTransform_ItemIcon->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	m_bActive = false;

	return S_OK;
}

void CUI_Inventory_ItemSlot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Create_Rect();
	Picking_UI();
}

void CUI_Inventory_ItemSlot::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_bPick)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			if (!CUI_Manager::GetInstance()->Is_PickedIcon())
			{
				if(1 <= m_vecItems.size())
					CUI_Manager::GetInstance()->Picked_ItemIcon(m_vecItems.front()->Get_ObjectTag(), m_pTexture_ItemIcon, m_vecItems.front()->Get_ItemGrade());
			}
		}
		else if (KEY_AWAY(KEY::LBTN))
		{
			if (CUI_Manager::GetInstance()->Is_PickedIcon())
			{
				if (1 <= m_vecItems.size())
				{
					m_pOwner->Swap_Items_In_Inventory(CUI_Manager::GetInstance()->Get_PickedTag(), m_vecItems.front()->Get_ObjectTag());
					CUI_Manager::GetInstance()->Set_PickedTag(TEXT(""));
				}
				else
				{
					m_pOwner->Swap_Items_In_Inventory(CUI_Manager::
						GetInstance()->Get_PickedTag(), m_iSlotIndex);
					CUI_Manager::GetInstance()->Set_PickedTag(TEXT(""));
				}
			}
		}

		else if (KEY_TAP(KEY::RBTN))
		{
			if(1 <= m_vecItems.size())
				m_pOwner->Use_Item(m_vecItems.front()->Get_ObjectTag());
		}
	}
	Picking_Effect(fTimeDelta);
}

HRESULT CUI_Inventory_ItemSlot::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ItemGrade()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ItemIcon()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Picked_Effect()))
		return E_FAIL;
	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Inventory_ItemSlot::Clear_ItemSlot()
{
	m_vecItems.clear();
	m_strItemName = TEXT("");
	m_strItemDescript = TEXT("");
	m_strItemType = TEXT("");
	m_strItemGrade = TEXT("");
	m_iItemType = (_uint)CItem::TYPE::_END;
	m_iEquipType = (_uint)CItem::PART::_END;
	m_iItemGrade = (_uint)CItem::GRADE::_END;
	m_iItemCount = 0;
	if(nullptr != m_pTexture_ItemIcon)
	{
		Safe_Release(m_pTexture_ItemIcon);
		m_pTexture_ItemIcon = nullptr;
	}
}

void CUI_Inventory_ItemSlot::Set_ItemInfo(vector<class CItem*> vecItems)
{
	Clear_ItemSlot();
	m_vecItems = vecItems;
	m_strItemName = m_vecItems.front()->Get_ItemName();
	m_strItemDescript = m_vecItems.front()->Get_ItemDescript();
	m_iItemType = m_vecItems.front()->Get_ItemType();
	m_iEquipType = m_vecItems.front()->Get_ItemType();
	m_iItemGrade = m_vecItems.front()->Get_ItemGrade();
	m_iItemCount = m_vecItems.size();

	m_pTexture_ItemIcon =
		static_cast<CTexture*>(m_vecItems.front()->Get_ItemTexture()->Clone(nullptr, nullptr));
	Safe_AddRef(m_pTexture_ItemIcon);
	Set_Item_StringInfo(m_iItemType, m_iEquipType, m_iItemGrade);

}

void CUI_Inventory_ItemSlot::Following_InventortWnd(_float fX, _float fY)
{
	m_fX = fX + (56.f * (0.8f) * (_float)m_iSlotIndexX);
	m_fY = fY + (56.f * (0.8f) * (_float)m_iSlotIndexY);

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	//m_pTransform_ItemIcon
	m_pTransform_ItemIcon->Set_Scale(Vec3(m_fSizeX * 0.9f, m_fSizeY * 0.9f, 1.f));
	m_pTransform_ItemIcon->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

HRESULT CUI_Inventory_ItemSlot::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_ItemSlot"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Rank"),
		TEXT("Com_Texture_ItemGrade"), (CComponent**)&m_pTexture_ItemGrade)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_None"),
		TEXT("Com_Texture_None"), (CComponent**)&m_pTexture_None)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_PickedSlot_Effect"),
		TEXT("Com_Texture_Picked_Effect"), (CComponent**)&m_pTexture_Picked_Effect)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ItemIcon"), (CComponent**)&m_pTransform_ItemIcon)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Inventory_ItemSlot::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if(FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_Inventory_ItemSlot::Bind_ShaderResources_ItemGrade()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if ((nullptr != m_pTexture_ItemIcon)&&(7 > (_uint)m_iItemGrade))
	{
		if (FAILED(m_pTexture_ItemGrade->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iItemGrade)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Inventory_ItemSlot::Bind_ShaderResources_ItemIcon()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if (1 <= m_vecItems.size())
	{
		if (FAILED(m_pTexture_ItemIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Inventory_ItemSlot::Bind_ShaderResources_Picked_Effect()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fPickedAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pTexture_Picked_Effect->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_Inventory_ItemSlot::Set_Item_StringInfo(const _uint& iItemType, const _uint& iEquipType, const _uint& iItemGrade)
{
	wstring strItemType, strItemGrade, strItemPart;
	
	switch (iItemGrade)
	{
	case (_uint)CItem::GRADE::WHITE:
		strItemGrade = TEXT("일반");
		break;
	case (_uint)CItem::GRADE::GREEN:
		strItemGrade = TEXT("고급");
		break;
	case (_uint)CItem::GRADE::BLUE:
		strItemGrade = TEXT("희귀");
		break;
	case (_uint)CItem::GRADE::PURPLE:
		strItemGrade = TEXT("영웅");
		break;
	case (_uint)CItem::GRADE::ORANGE:
		strItemGrade = TEXT("전설");
		break;
	case (_uint)CItem::GRADE::RED:
		strItemGrade = TEXT("유물");
		break;
	case (_uint)CItem::GRADE::IVORY:
		strItemGrade = TEXT("고대");
		break;
	}

	switch (iItemType)
	{
	case (_uint)CItem::TYPE::EQUIP:
	{
		switch (iEquipType)
		{
			case (_uint)CItem::PART::FACE:
				strItemPart = TEXT("얼굴");
				strItemType = TEXT("방어구");
				break;
			case (_uint)CItem::PART::HELMET:
				strItemPart = TEXT("머리");
				strItemType = TEXT("방어구");
				break;
			case (_uint)CItem::PART::SHOULDER:
				strItemPart = TEXT("어깨");
				strItemType = TEXT("방어구");
				break;
			case (_uint)CItem::PART::BODY:
				strItemPart = TEXT("상의");
				strItemType = TEXT("방어구");
				break;
			case (_uint)CItem::PART::ARM:
				strItemPart = TEXT("장갑");
				strItemType = TEXT("방어구");
				break;
			case (_uint)CItem::PART::LEG:
				strItemPart = TEXT("하의");
				strItemType = TEXT("방어구");
				break;
			case (_uint)CItem::PART::WEAPON:
				strItemPart = TEXT("");
				if(wstring::npos != m_strItemName.find(TEXT("망치")))
					strItemType = TEXT("망치");
				else if (wstring::npos != m_strItemName.find(TEXT("하프")))
					strItemType = TEXT("하프");
				else if (wstring::npos != m_strItemName.find(TEXT("총")))
					strItemType = TEXT("총");
				else if (wstring::npos != m_strItemName.find(TEXT("대검")))
					strItemType = TEXT("대검");
				break;
		}
		m_strItemGrade = strItemGrade + TEXT(" ") + strItemPart + +TEXT(" ") + strItemType;
	}
	break;

	case (_uint)CItem::TYPE::CONSUM:
		strItemPart = TEXT("");
		strItemType = TEXT("배틀아이템");
		m_strItemGrade = strItemGrade + TEXT(" ") + strItemType;
		break;
	}
}

void CUI_Inventory_ItemSlot::Picking_Effect(_float fTimeDelta)
{
	if (!m_bPick)
	{
		if (0.f < m_fPickedAlpha)
		{
			m_fPickedAlpha -= 3.f * fTimeDelta;
		}
		if (0.f > m_fPickedAlpha)
			m_fPickedAlpha = 0.f;
	}
	else
	{
		if (1.f > m_fPickedAlpha)
		{
			m_fPickedAlpha += 3.f * fTimeDelta;
		}
		if (1.f < m_fPickedAlpha)
			m_fPickedAlpha = 1.f;
	}
}

CUI_Inventory_ItemSlot* CUI_Inventory_ItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory_ItemSlot* pInstance = new CUI_Inventory_ItemSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Inventory_ItemSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory_ItemSlot::Clone(void* pArg)
{
	CUI_Inventory_ItemSlot* pInstance = new CUI_Inventory_ItemSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Inventory_ItemSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory_ItemSlot::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexture_ItemIcon);
	Safe_Release(m_pTexture_ItemGrade);
	Safe_Release(m_pTexture_None);
	Safe_Release(m_pTexture_Picked_Effect);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransform_ItemIcon);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
