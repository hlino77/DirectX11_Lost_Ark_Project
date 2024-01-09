#include "stdafx.h"
#include "UI_ServerEntranceButton.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_ServerEntranceButton::CUI_ServerEntranceButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ServerEntranceButton::CUI_ServerEntranceButton(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ServerEntranceButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ServerEntranceButton::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Server_EntranceButton");

	if (nullptr != pArg)
	{
		UIDESC* UIDesc = static_cast<UIDESC*>(pArg);
		m_fX = UIDesc->fX;
		m_fY = UIDesc->fY;
		m_fSizeX = UIDesc->fSizeX;
		m_fSizeY = UIDesc->fSizeY;

		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	}
	else
	{
		m_fX = g_iWinSizeX*0.5f;
		m_fY = (g_iWinSizeY * 0.5f) + 350.f;
		m_fSizeX = 160.f;
		m_fSizeY = 50.f;

		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	}

	return S_OK;
}

void CUI_ServerEntranceButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_bRender)
	{
		Moving_Rect();
		Picking_UI();
	}
	if ((m_bPick) && (KEY_AWAY(KEY::LBTN)))
	{
		if ((CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY, L"None"))))
			return;
	}
}

void CUI_ServerEntranceButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CUI_ServerEntranceButton::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();
	
	Vec2 vPos(m_fX, m_fY);
	Vec2 vScale(0.5f, 0.5f);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"ÇÑÄÄ°íµñ", m_strWord);
	Vec2 vOrigin = vMeasure * 0.5f;

	CGameInstance::GetInstance()->DrawFont(L"ÇÑÄÄ°íµñ", m_strWord, vPos, Vec4(1.f, 1.f, 1.f, 1.0f), 0.0f, vOrigin, vScale);

	return S_OK;
}

void CUI_ServerEntranceButton::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_ServerEntranceButton::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Server_EntranceButton"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ServerEntranceButton::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture",(_uint)m_bPick);

	return S_OK;
}

CUI_ServerEntranceButton* CUI_ServerEntranceButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ServerEntranceButton* pInstance = new CUI_ServerEntranceButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ServerEntranceButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ServerEntranceButton::Clone(void* pArg)
{
	CUI_ServerEntranceButton* pInstance = new CUI_ServerEntranceButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ServerEntranceButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ServerEntranceButton::Free()
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
