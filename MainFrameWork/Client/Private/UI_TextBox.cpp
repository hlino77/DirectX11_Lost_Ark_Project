#include "stdafx.h"
#include "UI_TextBox.h"
#include "GameInstance.h"
#include "TextBox.h"

CUI_TextBox::CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_TextBox::CUI_TextBox(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_TextBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_TextBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX * 1.0f;
	m_fSizeY = g_iWinSizeY * 1.0f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = (g_iWinSizeY * 0.5f);

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));


	m_vUV = Vec2(1.0f, 1.0f);


	CTextBox::TEXTBOXDESC tTextBoxDesc;
	tTextBoxDesc.szTextBoxTag = L"TestWindow";
	tTextBoxDesc.vSize = Vec2(900.0f, 900.0f);


	m_pTestTextBox = dynamic_cast<CTextBox*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, L"Prototype_GameObject_TextBox", &tTextBoxDesc));
	if(m_pTestTextBox == nullptr)
		return E_FAIL;

	return S_OK;
}

void CUI_TextBox::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_TextBox::LateTick(_float fTimeDelta)
{
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_TEXTBOX, this);
}

HRESULT CUI_TextBox::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = 1.0f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vScaleUV", &m_vUV, sizeof(Vec2))))
		return E_FAIL;


	//Vec4 fColor = Vec4(1.0f, 5.f, 5.f, 1.f);
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &fColor, sizeof(Vec4))))
	//	return E_FAIL;

	if (FAILED(m_pRendererCom->Bind_TextBoxSRV(m_pShaderCom)))
		return E_FAIL;


	m_pShaderCom->Begin(5);

	m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CUI_TextBox::Render_MakeSRV()
{
	if (m_TextList.empty())
		return S_OK;

	for (auto& Text : m_TextList)
	{
		CGameInstance::GetInstance()->DrawFont(Text.second.szFont, Text.second.szText, Text.second.vTextPos, Text.second.vTextColor, Text.second.fRotation, Text.second.vOrigin, Text.second.vTextScale);
	}
	

	return S_OK;
}


void CUI_TextBox::Set_Text(const wstring& szTextTag, const wstring& szFont, const wstring& szText, Vec2 vTextPos, Vec2 vScale, Vec2 vOrigin, _float fRotation, Vec4 vColor)
{
	TEXTDESC& tText = m_TextList[szTextTag];

	tText.szFont = szFont;
	tText.szText = szText;
	tText.vTextPos = vTextPos;
	tText.vTextScale = vScale;
	tText.vOrigin = vOrigin;
	tText.fRotation = fRotation;
	tText.vTextColor = vColor;

	m_pTestTextBox->Set_Text(szTextTag, szFont, szText, vTextPos, vScale, vOrigin, fRotation, vColor);
}

void CUI_TextBox::Set_Pos(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_pTestTextBox->Set_Pos(m_fX + 200.0f, m_fY);
}

void CUI_TextBox::Appear()
{
	m_bActive = true;
	m_eState = UISTATE::TICK;
	m_pTestTextBox->Set_Active(true);
}

void CUI_TextBox::Disappear()
{
	m_bActive = false;
	m_eState = UISTATE::DISAPPEAR;
	m_pTestTextBox->Set_Active(false);
}

HRESULT CUI_TextBox::Ready_Components()
{
	__super::Ready_Components();


	return S_OK;
}

HRESULT CUI_TextBox::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = 0.8f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	Vec4 fColor = Vec4(5.f, 5.f ,5.f, 1.f);	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &fColor, sizeof(Vec4))))
		return E_FAIL;


	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

CUI_TextBox* CUI_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_TextBox* pInstance = new CUI_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ServerWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_TextBox::Clone(void* pArg)
{
	CUI_TextBox* pInstance = new CUI_TextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ServerWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_TextBox::Free()
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
