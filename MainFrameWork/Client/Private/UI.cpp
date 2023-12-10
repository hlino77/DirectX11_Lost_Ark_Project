#include "stdafx.h"
#include "UI.h"
#include "VIBuffer_Point.h"
#include "GameInstance.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext, L"UI", OBJ_TYPE::UI)
{

}

CUI::CUI(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case UISTATE::APPEAR:
		UI_AppearTick(fTimeDelta);
		break;
	case UISTATE::TICK:
		UI_Tick(fTimeDelta);
		break;
	case UISTATE::DISAPPEAR:
		UI_DisappearTick(fTimeDelta);
		break;
	}

}

void CUI::LateTick(_float fTimeDelta)
{
	if(m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);	
}

HRESULT CUI::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	//CGameInstance::GetInstance()->DrawFont(L"125", szString, Vec2(0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, Vec2(0.0f, 0.0f), Vec2(0.5f, 0.5f));

	return S_OK;
}

void CUI::UI_AppearTick(_float fTimeDelta)
{
	m_eState = UISTATE::TICK;
}

void CUI::UI_DisappearTick(_float fTimeDelta)
{
	m_eState = UISTATE::READY;
	m_bActive = false;
}


void CUI::Appear()
{
	m_bActive = true;
	m_eState = UISTATE::APPEAR;
}

void CUI::Disappear()
{
	m_bActive = true;
	m_eState = UISTATE::DISAPPEAR;
}

HRESULT CUI::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* Com_Transform */
	CTransform::tagTransformDesc		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof TransformDesc);	

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &IdentityMatrix)))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
