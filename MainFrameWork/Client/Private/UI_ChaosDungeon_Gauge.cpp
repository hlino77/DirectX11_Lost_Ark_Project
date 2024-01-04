#include "stdafx.h"
#include "UI_ChaosDungeon_Gauge.h"
#include "GameInstance.h"
#include "TextBox.h"

CUI_ChaosDungeon_Gauge::CUI_ChaosDungeon_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChaosDungeon_Gauge::CUI_ChaosDungeon_Gauge(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChaosDungeon_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon_Gauge::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("ChaosDungeon_Gauge");

	m_fSizeX = 504;
	m_fSizeY = 335;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = (g_iWinSizeY * 0.5f) + 100.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	//if (FAILED(Initialize_Percent()))
		//return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Gauge::Initialize_Percent()
{
	m_szFont = L"ºûÀÇ°è½ÂÀÚ";
	Ready_TextBox();

	Set_Active(true);

	return S_OK;
}

void CUI_ChaosDungeon_Gauge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (!m_bTextOn)
	{
		m_bTextOn = true;
		Print_Stage_Percent();
	}
}

void CUI_ChaosDungeon_Gauge::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ChaosDungeon_Gauge::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Gauge::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gauge"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Gauge::Bind_ShaderResources()
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

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

void CUI_ChaosDungeon_Gauge::Start_Stage_Name()
{
	m_strPercent.clear();
	m_pPercentWnd->Set_Active(true);
}

void CUI_ChaosDungeon_Gauge::End_Stage_Name()
{
	m_strPercent.clear();
	m_pPercentWnd->Set_Active(false);
}

void CUI_ChaosDungeon_Gauge::Print_Stage_Percent()
{
	if (nullptr == m_pPercentWnd)
		return;

	m_pPercentWnd->Set_Active(true);
	m_pPercentWnd->Clear_Text();
	m_pPercentWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
	m_pPercentWnd->Get_TransformCom()->Set_Scale(Vec3(100.f, 40.f, 0.f));
	m_pPercentWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"ºûÀÇ°è½ÂÀÚ", m_strPercent);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pPercentWnd->Set_Text(L"ChaosPercentWnd", m_szFont, m_strPercent, Vec2(100.f, 40.f), Vec2(1.0f, 1.0f), vOrigin, 0.f, m_vColor);
}

void CUI_ChaosDungeon_Gauge::Set_Active(_bool bActive)
{
	m_pPercentWnd->Set_Active(bActive);
}

HRESULT CUI_ChaosDungeon_Gauge::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);
	if (nullptr == m_pPercentWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("ChaosPercentWnd");
		tTextDesc.vSize = Vec2(200.f, 80.0f);
		m_pPercentWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pPercentWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pPercentWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pPercentWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}
	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_ChaosDungeon_Gauge::Set_Percent(const _uint& iGague)
{
	 m_strPercent.clear();
	 m_strPercent += to_wstring(iGague);
	 m_strPercent += TEXT("%");
}

CUI_ChaosDungeon_Gauge* CUI_ChaosDungeon_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChaosDungeon_Gauge* pInstance = new CUI_ChaosDungeon_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChaosDungeon_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChaosDungeon_Gauge::Clone(void* pArg)
{
	CUI_ChaosDungeon_Gauge* pInstance = new CUI_ChaosDungeon_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChaosDungeon_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChaosDungeon_Gauge::Free()
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
