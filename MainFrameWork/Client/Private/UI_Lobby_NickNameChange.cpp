#include "stdafx.h"
#include "UI_Lobby_NickNameChange.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Lobby_NickName.h"
#include "TextBox.h"
#include "Chat_Manager.h"

CUI_Lobby_NickNameChange::CUI_Lobby_NickNameChange(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Lobby_NickNameChange::CUI_Lobby_NickNameChange(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Lobby_NickNameChange::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Lobby_NickNameChange::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Lobby_NameChanger");

	m_fSizeX = 42.f;
	m_fSizeY = 49.f;
	m_fX = 1550.f;
	m_fY = 866.f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	m_pTransformCom_NewWnd->Set_Scale(Vec3(457.f, 90.f, 1.f));
	m_pTransformCom_NewWnd->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX * 0.5f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	Initialize_Textbox();
	return S_OK;
}

HRESULT CUI_Lobby_NickNameChange::Initialize_Textbox()
{
	m_strFont = L"´øÆÄ¿¬¸¶µÈÄ®³¯";
	Ready_TextBox();
	m_pInputWnd->Set_Active(false);
	return S_OK;
}

void CUI_Lobby_NickNameChange::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_NickNameFrame();
	Update_NickNameChangeWnd();
}

void CUI_Lobby_NickNameChange::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


}

HRESULT CUI_Lobby_NickNameChange::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	Print_NickNameChange();

	if (FAILED(Bind_ShaderResources_NewWnd()))
		return E_FAIL;
	m_pTextureCom_NewWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	//Render_InputString();
	
	return S_OK;
}


const wstring CUI_Lobby_NickNameChange::Get_NickName()
{
	if (nullptr != m_pNickName)
		return static_cast<CUI_Lobby_NickName*>(m_pNickName)->Get_NickName();
}

HRESULT CUI_Lobby_NickNameChange::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Changer"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Change"),
		TEXT("Com_Texture_NewWnd"), (CComponent**)&m_pTextureCom_NewWnd)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_NewWnd"), (CComponent**)&m_pTransformCom_NewWnd)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Lobby_NickNameChange::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Lobby_NickNameChange::Bind_ShaderResources_NewWnd()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_NewWnd->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	_float fAlpha = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

void CUI_Lobby_NickNameChange::Render_InputString()
{
	if (!m_bOnWnd)
		return;

	Vec2 vPos((g_iWinSizeX * 0.5f), (g_iWinSizeY * 0.5f) + 25.f);
	Vec2 vScale(0.7f, 0.7f);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñ", 
		static_cast<CUI_Lobby_NickName*>(m_pNickName)->Get_NickName());
	Vec2 vOrigin = vMeasure * 0.5f;

	CGameInstance::GetInstance()->DrawFont(L"³Ø½¼Lv1°íµñ", static_cast<CUI_Lobby_NickName*>(m_pNickName)->Get_NickName(), vPos, Vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, vOrigin, vScale);
}

void CUI_Lobby_NickNameChange::Update_NickNameFrame()
{
	Create_Rect();
	Picking_UI();

	if (m_bPick)
		m_fAlpha = 1.f;
	else 
		m_fAlpha = 0.5f;

	if ((m_bPick) && (KEY_TAP(KEY::LBTN)))
	{
		if (nullptr == m_pNickName)
			return;
		m_bOnWnd = true;
		m_iTextureIndex = 1;
		CChat_Manager::GetInstance()->Add_InputTextObject(this);
	}
	
	if (!m_bOnWnd)
		m_iTextureIndex = 0;
}

void CUI_Lobby_NickNameChange::Update_NickNameChangeWnd()
{
	if ((nullptr != m_pNickName)&&(true == m_bOnWnd))
	{
		wstring strCurrNickName = static_cast<CUI_Lobby_NickName*>(m_pNickName)->Get_NickName();
		wstring strNewNickName = CChat_Manager::GetInstance()->Get_InputText(this);
		if(strCurrNickName != strNewNickName)
			m_strNickName = strNewNickName;

		if (KEY_TAP(KEY::ENTER))
		{
			static_cast<CUI_Lobby_NickName*>(m_pNickName)->Set_NickName(m_strNickName);
			m_bOnWnd = false;
			m_iTextureIndex = 0;
			CChat_Manager::GetInstance()->Delete_InputTextObject(this);
			m_pInputWnd->Set_Active(false);
		}
		else if (KEY_TAP(KEY::ESC))
		{
			static_cast<CUI_Lobby_NickName*>(m_pNickName)->Set_NickName(strCurrNickName);
			m_bOnWnd = false;
			m_iTextureIndex = 0;
			CChat_Manager::GetInstance()->Delete_InputTextObject(this);
			m_pInputWnd->Set_Active(false);
		}
		//static_cast<CUI_Lobby_NickName*>(m_pNickName)->Set_NickName();
	}
}

void CUI_Lobby_NickNameChange::Update_InputString()
{
	if (!m_bOnWnd)
		return;
}

void CUI_Lobby_NickNameChange::Print_NickNameChange()
{
	if ((nullptr == m_pInputWnd)||(!m_bOnWnd))
		return;

	m_pInputWnd->Set_Active(true);
	m_pInputWnd->Clear_Text();
	m_pInputWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(800.f - g_iWinSizeX * 0.5f, -470.f + g_iWinSizeY * 0.5f, 0.f);
	m_pInputWnd->Get_TransformCom()->Set_Scale(Vec3(458.f, 90.f, 0.f));
	m_pInputWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"ºûÀÇ°è½ÂÀÚ", m_strNickName);
	Vec2 vOrigin = vMeasure * 0.5f;

	m_pInputWnd->Set_Text(TEXT("InputNickNameWnd1"), m_strFont, m_strNickName, Vec2(458.f * 0.5f, 45.f), Vec2(0.6f, 0.6f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
	m_pInputWnd->Set_Text(TEXT("InputNickNameWnd2"), m_strFont, m_strNickName, Vec2(458.f * 0.5f, 45.f), Vec2(0.6f, 0.6f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
	m_pInputWnd->Set_Text(TEXT("InputNickNameWnd"), m_strFont, m_strNickName, Vec2(458.f * 0.5f, 45.f), Vec2(0.6f, 0.6f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

HRESULT CUI_Lobby_NickNameChange::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pInputWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("InputNickNameWnd");
		tTextDesc.vSize = Vec2(458.f, 90.0f);
		m_pInputWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pInputWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pInputWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pInputWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_Lobby_NickNameChange::Start_NickNameText()
{
	m_strNickName.clear();
	m_pInputWnd->Set_Active(true);
}

void CUI_Lobby_NickNameChange::End_NickNameText()
{
	m_strNickName.clear();
	m_pInputWnd->Set_Active(false);
}

CUI_Lobby_NickNameChange* CUI_Lobby_NickNameChange::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Lobby_NickNameChange* pInstance = new CUI_Lobby_NickNameChange(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Lobby_NickNameChange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Lobby_NickNameChange::Clone(void* pArg)
{
	CUI_Lobby_NickNameChange* pInstance = new CUI_Lobby_NickNameChange(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Lobby_NickNameChange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Lobby_NickNameChange::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pInputWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_NewWnd);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransformCom_NewWnd);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
