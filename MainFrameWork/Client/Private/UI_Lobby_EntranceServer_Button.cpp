#include "stdafx.h"
#include "UI_Lobby_EntranceServer_Button.h"
#include "GameInstance.h"
#include "TextBox.h"

CUILobby_Entrance_to_ServrerButton::CUILobby_Entrance_to_ServrerButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUILobby_Entrance_to_ServrerButton::CUILobby_Entrance_to_ServrerButton(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUILobby_Entrance_to_ServrerButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUILobby_Entrance_to_ServrerButton::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Button_Entrance_to_Server");

	m_fSizeX = 200.f;
	m_fSizeY = 60.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = 865.f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if(FAILED(Initialize_TextBox()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUILobby_Entrance_to_ServrerButton::Initialize_TextBox()
{
	m_strFont = L"ÇÑÄÄ°íµñ";
	Ready_TextBox();
	m_pTextWnd->Set_Active(false);
	return S_OK;
}

void CUILobby_Entrance_to_ServrerButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_Buttton();
}

void CUILobby_Entrance_to_ServrerButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUILobby_Entrance_to_ServrerButton::Render()
{
	Print_TextBox();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUILobby_Entrance_to_ServrerButton::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Server_EntranceButton"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUILobby_Entrance_to_ServrerButton::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex);

	return S_OK;
}

void CUILobby_Entrance_to_ServrerButton::Update_Buttton()
{
	Create_Rect();
	Picking_UI();

	if ((BUTTON_STATE::BUTTON_NORMAL == m_iButtonState) && (m_bPick))
	{
		m_bEntrance = false;
		m_iButtonState = (_uint)BUTTON_STATE::BUTTON_PICKING;
	}
	else if ((BUTTON_STATE::BUTTON_PICKING == m_iButtonState) && ((KEY_TAP(KEY::LBTN)) || (KEY_HOLD(KEY::LBTN))))
	{
		m_bEntrance = false;
		m_iButtonState = (_uint)BUTTON_STATE::BUTTON_PICKED;
	}
	else if ((m_iButtonState == (_uint)BUTTON_STATE::BUTTON_PICKED) && (KEY_AWAY(KEY::LBTN) && (m_bPick)))
	{
		m_bEntrance = true;
		m_iButtonState = (_uint)BUTTON_STATE::BUTTON_NORMAL;
	}

	if (!m_bPick)
	{
		m_bEntrance = false;
		m_iButtonState = BUTTON_STATE::BUTTON_NORMAL;
	}
	Update_Button_Texture();
}

void CUILobby_Entrance_to_ServrerButton::Update_Button_Texture()
{
	m_vColor = Vec4{ 1.f, 1.f ,1.f, 1.f };
	if ((_uint)BUTTON_PICKED == m_iButtonState)
		m_vColor *= Vec4{ 0.6f, 0.6f ,0.6f, 1.f };
	else if((_uint)BUTTON_PICKING == m_iButtonState)
		m_vColor *= Vec4{ 3.f, 3.f ,3.f, 1.f };
	else if((_uint)BUTTON_NORMAL == m_iButtonState)
		m_vColor = Vec4{ 1.f, 1.f ,1.f, 1.f };

}

void CUILobby_Entrance_to_ServrerButton::Print_TextBox()
{
	m_pTextWnd->Set_Active(true);
	m_pTextWnd->Clear_Text();
	m_pTextWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
	m_pTextWnd->Get_TransformCom()->Set_Scale(Vec3(200.f, 60.f, 0.f));
	m_pTextWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"ºûÀÇ°è½ÂÀÚ", m_strText);
	Vec2 vOrigin = vMeasure * 0.5f;

	m_pTextWnd->Set_Text(TEXT("EntranceWnd"), m_strFont, m_strText, Vec2(100.0f, 30.0f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

HRESULT CUILobby_Entrance_to_ServrerButton::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pTextWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("EntranceWnd");
		tTextDesc.vSize = Vec2(200.f, 60.0f);
		m_pTextWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pTextWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pTextWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pTextWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CUILobby_Entrance_to_ServrerButton::Start_TextBox()
{
	m_strText.clear();
	m_pTextWnd->Set_Active(true);
}

void CUILobby_Entrance_to_ServrerButton::End_TextBox()
{
	m_strText.clear();
	m_pTextWnd->Set_Active(false);
}

CUILobby_Entrance_to_ServrerButton* CUILobby_Entrance_to_ServrerButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUILobby_Entrance_to_ServrerButton* pInstance = new CUILobby_Entrance_to_ServrerButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUILobby_Entrance_to_ServrerButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUILobby_Entrance_to_ServrerButton::Clone(void* pArg)
{
	CUILobby_Entrance_to_ServrerButton* pInstance = new CUILobby_Entrance_to_ServrerButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUILobby_Entrance_to_ServrerButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUILobby_Entrance_to_ServrerButton::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pTextWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
