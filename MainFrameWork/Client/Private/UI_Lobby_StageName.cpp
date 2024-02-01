#include "stdafx.h"
#include "UI_Lobby_StageName.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Lobby_NickName.h"
#include "TextBox.h"

CUI_Lobby_StageName::CUI_Lobby_StageName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Lobby_StageName::CUI_Lobby_StageName(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Lobby_StageName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Lobby_StageName::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		LOBBYSTAGE_DESC* UIDesc = static_cast<LOBBYSTAGE_DESC*>(pArg);
		_uint strIndex = UIDesc->iIndex;
		m_iCharacterIndex = strIndex;
		m_strUITag = TEXT("Lobby_StageName");
		m_strUITag += to_wstring(m_iCharacterIndex + 1);
		m_pNickName = UIDesc->pUI;
	}
	else
		m_strUITag = TEXT("Lobby_StageName");

	m_fSizeX = 205.f;
	m_fSizeY = 33.f;
	m_fX = m_pNickName->Get_UIDesc().fX;
	m_fY = 754.f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_TextBox()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Lobby_StageName::Initialize_TextBox()
{
	m_strFont = L"³Ø½¼Lv1°íµñBold";
	Ready_TextBox();
	m_pTextWnd->Set_Active(false);
	return S_OK;
}

void CUI_Lobby_StageName::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_Lobby_StageName::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Update_NickNameFrame();
}

HRESULT CUI_Lobby_StageName::Render()
{
	Print_TextBox();

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Lobby_StageName::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Current_TownFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Lobby_StageName::Bind_ShaderResources()
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


	return S_OK;
}

void CUI_Lobby_StageName::Update_NickNameFrame()
{
	if (1 == m_pNickName->Get_TextureIndex())
	{
		m_bRender = true;
		m_bTextOn = true;
		m_pTextWnd->Set_Active(true);
	}
	else
	{
		m_bRender = false;
		m_bTextOn = false;
		m_pTextWnd->Set_Active(false);
	}
}

void CUI_Lobby_StageName::Print_TextBox()
{
	if (!m_bTextOn)
		return;

	m_pTextWnd->Set_Active(true);
	m_pTextWnd->Clear_Text();
	m_pTextWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
	m_pTextWnd->Get_TransformCom()->Set_Scale(Vec3(205.f, 40.f, 0.f));
	m_pTextWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñ", m_strText);
	Vec2 vOrigin = vMeasure * 0.5f;

	m_pTextWnd->Set_Text(TEXT("StageNameWnd") + to_wstring(m_iCharacterIndex), m_strFont, m_strText, Vec2(102.0f, 20.0f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

HRESULT CUI_Lobby_StageName::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pTextWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("StageNameWnd") + to_wstring(m_iCharacterIndex);
		tTextDesc.vSize = Vec2(205.f, 40.0f);
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

void CUI_Lobby_StageName::Start_TextBox()
{
	m_strText.clear();
	m_pTextWnd->Set_Active(true);
}

void CUI_Lobby_StageName::End_TextBox()
{
	m_strText.clear();
	m_pTextWnd->Set_Active(false);
}

CUI_Lobby_StageName* CUI_Lobby_StageName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Lobby_StageName* pInstance = new CUI_Lobby_StageName(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Lobby_StageName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Lobby_StageName::Clone(void* pArg)
{
	CUI_Lobby_StageName* pInstance = new CUI_Lobby_StageName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Lobby_StageName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Lobby_StageName::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pTextWnd->Set_Dead(true);
}
