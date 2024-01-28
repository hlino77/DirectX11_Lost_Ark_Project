#include "stdafx.h"
#include "UI_InventoryWnd.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "UI_Inventory_ItemSlot.h"
#include "Item_TestItem.h"
#include "TextBox.h"

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
	
	if (nullptr != pArg)
	{
		Initialize_TextBox(static_cast<CPlayer*>(pArg));
	}
	
	
	m_bActive = false;
	return S_OK;
}

HRESULT CUI_InventoryWnd::Initialize_TextBox(class CPlayer* pOwner)
{
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	Ready_MoneyTextBox(pOwner);
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

	if (m_bRender)
	{
		Print_Player_Money();
		m_pMoneyWnd->Render();
	}
	return S_OK;
}

void CUI_InventoryWnd::Move_InventoryWnd(POINT pt)
{
	Create_Rect_MoveWnd();
	Is_Picking_MoveWnd(pt);

	_float	MouseMoveX, MouseMoveY;
	MouseMoveX = CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);
	
	if ((m_bPick)&&(KEY_HOLD(KEY::LBTN)))
		m_bHolding = true;

	if (m_bHolding)
	{
		m_fX += MouseMoveX * 0.8f;
		m_fY = pt.y + 210.f;
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

		m_pMoneyWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

		if (KEY_AWAY(KEY::LBTN))
			m_bHolding = false;
	}

}

void CUI_InventoryWnd::Create_Rect_MoveWnd()
{
	m_rcMovingWnd.left = LONG(m_fX - (m_fSizeX / 2));
	m_rcMovingWnd.top = LONG((m_fY - 210.f) - (m_fSizeY * 0.049f / 2));
	m_rcMovingWnd.right = LONG(m_fX + (m_fSizeX / 2));
	m_rcMovingWnd.bottom = LONG((m_fY - 210.f) + (m_fSizeY * 0.049f / 2));
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

HRESULT CUI_InventoryWnd::Ready_MoneyTextBox(class CPlayer* pOwner)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pMoneyWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = to_wstring(pOwner->Get_ObjectID()) + TEXT("'s UI_Inventory");
		m_strWndTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(470.f * 0.8f, 560.f * 0.8f);
		m_pMoneyWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
		
		if (nullptr == m_pMoneyWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pMoneyWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pMoneyWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		m_pMoneyWnd->Set_Render(false);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_InventoryWnd::Print_Player_Money()
{
	if ((nullptr == m_pMoneyWnd)&&(!m_bRender))
		return;

	m_pMoneyWnd->Clear_Text();
	m_pMoneyWnd->Set_Alpha(1.f);
	m_pMoneyWnd->Get_TransformCom()->Set_Scale(Vec3(470.f * 0.8f, 560.f * 0.8f, 0.f));
	m_pMoneyWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(0.f, 0.f, 0.f));

	_uint iIndex = 1;

	//PlayerÇÑÅ× µ·¹Þ¾Æ¿À±â
	_uint iMoney = 1000000;
	m_strSilver = to_wstring(iMoney);
	for (_uint i = m_strSilver.length() - 1; i > 0; --i)
	{
		if ((iIndex % 3) == 0)
		{
			m_strSilver.insert(i, L",");
		}
		++iIndex;
	}
	iIndex = 1;
	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strSilver);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pMoneyWnd->Set_Text(m_strWndTag + TEXT("_Sliver"), m_strFont, m_strSilver, Vec2((470.f * 0.8f) *0.5f - 30.f, (560.f * 0.8f) - 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));

	m_strGold = to_wstring(iMoney);
	for (_uint i = m_strGold.length() - 1; i > 0; --i)
	{
		if ((iIndex % 3) == 0)
		{
			m_strGold.insert(i, L",");
		}
		++iIndex;
	}
	vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strGold);
	vOrigin = vMeasure * 0.5f;
	m_pMoneyWnd->Set_Text(m_strWndTag + TEXT("_Gold"), m_strFont, m_strGold, Vec2((470.f * 0.8f) * 0.5f + 30.f, (560.f * 0.8f) - 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 0.0f, 1.f));
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

	m_pMoneyWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
