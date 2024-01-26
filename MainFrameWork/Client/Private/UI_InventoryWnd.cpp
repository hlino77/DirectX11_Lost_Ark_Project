#include "stdafx.h"
#include "UI_InventoryWnd.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "UI_Inventory_ItemSlot.h"
#include "Item_TestItem.h"

CUI_InventoryWnd::CUI_InventoryWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_InventoryWnd::CUI_InventoryWnd(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_InventoryWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_InventoryWnd::Initialize(void* pArg)


{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_InventoryWnd");
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fSizeX = 470.f * 0.8f;
	m_fSizeY = 560.f * 0.8f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_InventoryWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_InventoryWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_InventoryWnd::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();
	return S_OK;
}

void CUI_InventoryWnd::Move_InventoryWnd(POINT pt)
{
	Create_Rect_MoveWnd();
	Is_Picking_MoveWnd(pt);

	_float	MouseMoveX, MouseMoveY;
	MouseMoveX = CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);
	
	if ((m_bPick)&&(KEY_HOLD(KEY::LBTN)))
	{
		m_fX += MouseMoveX;
		m_fY = pt.y + 210.f;
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	}
}

void CUI_InventoryWnd::Create_Rect_MoveWnd()
{
	m_rcMovingWnd.left = LONG(m_fX - (m_fSizeX / 2));
	m_rcMovingWnd.top = LONG((m_fY - 210.f) - (m_fSizeY * 0.43f / 2));
	m_rcMovingWnd.right = LONG(m_fX + (m_fSizeX / 2));
	m_rcMovingWnd.bottom = LONG((m_fY - 210.f) + (m_fSizeY * 0.43f / 2));
}

void CUI_InventoryWnd::Is_Picking_MoveWnd(POINT pt)
{
	if (PtInRect(&m_rcMovingWnd, pt))  
		m_bPick = true;
	else
		m_bPick = false;
}

HRESULT CUI_InventoryWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_InventoryWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InventoryWnd::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if(FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	return S_OK;
}

CUI_InventoryWnd* CUI_InventoryWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InventoryWnd* pInstance = new CUI_InventoryWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_InventoryWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InventoryWnd::Clone(void* pArg)
{
	CUI_InventoryWnd* pInstance = new CUI_InventoryWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_InventoryWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InventoryWnd::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
