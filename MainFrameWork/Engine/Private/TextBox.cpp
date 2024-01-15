#include "TextBox.h"
#include "GameInstance.h"
#include "Target_Manager.h"
#include "LockFree_Transform.h"
#include "ThreadManager.h"
#include "VIBuffer_Point.h"

CTextBox::CTextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"TextBox", -1)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CTextBox::CTextBox(const CTextBox& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTextBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTextBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	TEXTBOXDESC* tTextBoxDesc = static_cast<TEXTBOXDESC*>(pArg);

	m_vSize = tTextBoxDesc->vSize;
	m_strObjectTag = tTextBoxDesc->szTextBoxTag;

	m_fWinSizeX = 1600.0f;
	m_fWinSizeY = 900.0f;

	m_pTransformCom->Set_Scale(Vec3(m_vSize.x, m_vSize.y, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(0.0f, 0.0f, 0.f));


	if (FAILED(Ready_RenderTarget()))
		return E_FAIL;


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_fWinSizeX, m_fWinSizeY, 0.f, 1.f));

	m_vUV = Vec2(1.0f, 1.0f);

	m_bActive = false;

	m_fAlpha = 1.f;

	return S_OK;
}

void CTextBox::Tick(_float fTimeDelta)
{
}

void CTextBox::LateTick(_float fTimeDelta)
{
	if (m_bUpdate)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::UPDATE_TEXTBOX, this);
		m_bUpdate = false;
	}
	
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CTextBox::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vScaleUV", &m_vUV, sizeof(Vec2))))
		return E_FAIL;


	//Vec4 fColor = Vec4(1.0f, 5.f, 5.f, 1.f);
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &fColor, sizeof(Vec4))))
	//	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Bind_SRV(m_pShaderCom, m_szTargetTag, "g_TextBoxTexture")))
		return E_FAIL;


	m_pShaderCom->Begin(5);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CTextBox::Render_MakeSRV()
{
	if (m_TextList.empty())
		return S_OK;

	WRITE_LOCK

	CTarget_Manager::GetInstance()->Begin_MRT(m_pContext, m_szMRTTag);

	for (auto& Text : m_TextList)
	{
		CGameInstance::GetInstance()->DrawFont(Text.second.szFont, Text.second.szText, Text.second.vTextPos, Text.second.vTextColor, Text.second.fRotation, Text.second.vOrigin, Text.second.vTextScale);
	}

	CTarget_Manager::GetInstance()->End_MRT(m_pContext);
	
	return S_OK;
}



void CTextBox::Set_Pos(_float fX, _float fY)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(fX - m_fWinSizeX * 0.5f, -fY + m_fWinSizeY * 0.5f, 0.f));
}



void CTextBox::Set_Text(const wstring& szTextTag, const wstring& szFont, const wstring& szText, Vec2 vTextPos, Vec2 vScale, Vec2 vOrigin, _float fRotation, Vec4 vColor)
{
	WRITE_LOCK
	TEXTDESC& tText = m_TextList[szTextTag];

	tText.szFont = szFont;
	tText.szText = szText;
	tText.vTextPos = vTextPos;
	tText.vTextScale = vScale;
	tText.vOrigin = vOrigin;
	tText.fRotation = fRotation;
	tText.vTextColor = vColor;

	m_bUpdate = true;
}


HRESULT CTextBox::Ready_Components()
{
	m_pTransformCom = CLockFree_Transform::Create(m_pDevice, m_pContext);

	/* Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTextBox::Ready_RenderTarget()
{
	m_szTargetTag = L"Target_" + m_strObjectTag;
	m_szMRTTag = L"MRT_" + m_strObjectTag;

	/* For. Target_TextBox*/
	if (FAILED(CTarget_Manager::GetInstance()->Add_RenderTarget(m_pDevice, m_pContext, m_szTargetTag,
		m_vSize.x, m_vSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Add_MRT(m_szMRTTag, m_szTargetTag)))
		return E_FAIL;


	return S_OK;
}

CTextBox* CTextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTextBox* pInstance = new CTextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTextBox::Clone(void* pArg)
{
	CTextBox* pInstance = new CTextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTextBox::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTransformCom);

	CTarget_Manager::GetInstance()->Delete_RenderTarget(m_szTargetTag);
	CTarget_Manager::GetInstance()->Delete_MRT(m_szMRTTag);
}
