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

void CUI::Picking_UI()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);  // 클라이언트 내에 마우스 포인터 가져오기 

	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;

	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // 뷰포트 가져오기 

	if (PtInRect(&m_rcUI, pt))   //  PtInRect(네모주소, 마우스 포인트) 이 함수가 개꿀 함수입니다 그냥 네모 구역에 마우스 있는지 바로 인식해줌
		m_bPick = true;
	else
		m_bPick = false;
}

void CUI::Create_Rect(void* pArg)
{
	if (nullptr != pArg)
	{
		UIDESC* pDesc = static_cast<UIDESC*>(pArg);

		m_rcUI.left = LONG(pDesc->fX - (pDesc->fSizeX / 2));
		m_rcUI.top = LONG(pDesc->fY - (pDesc->fSizeY / 2));
		m_rcUI.right = LONG(pDesc->fX + (pDesc->fSizeX / 2));
		m_rcUI.bottom = LONG(pDesc->fY + (pDesc->fSizeY / 2));
	}
	else
	{
		m_rcUI.left = LONG(m_fX - (m_fSizeX / 2));
		m_rcUI.top = LONG(m_fY - (m_fSizeY / 2));
		m_rcUI.right = LONG(m_fX + (m_fSizeX / 2));
		m_rcUI.bottom = LONG(m_fY + (m_fSizeY / 2));
	}
}

void CUI::Moving_Rect()
{
	m_rcUI.left = LONG(m_fX - (m_fSizeX / 2));
	m_rcUI.top = LONG(m_fY - (m_fSizeY / 2));
	m_rcUI.right = LONG(m_fX + (m_fSizeX / 2));
	m_rcUI.bottom = LONG(m_fY + (m_fSizeY / 2));
}

void CUI::RemoveDeadItem_And_ReOrder(vector<CGameObject*>& vec)
{
	auto newEnd = std::remove_if(vec.begin(), vec.end(), [](CGameObject* item) { return item->Is_Die(); });
	vec.erase(newEnd, vec.end());
}

CUI* CUI::Get_UIPart(const wstring& strPartTag)
{
	for (auto& iter : m_vecUIParts)
	{
		if (strPartTag == iter->Get_UITag());
			return	 iter;
	}

	return nullptr;
}

vector<CUI*> CUI::Get_UIParts()
{
	return m_vecUIParts;
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

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
