#include "stdafx.h"
#include "UI_WDRIdentity_Bubble.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Controller_WDR.h"
#include "Player_Destroyer.h"

CUI_WDRIdentity_Bubble::CUI_WDRIdentity_Bubble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WDRIdentity_Bubble::CUI_WDRIdentity_Bubble(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WDRIdentity_Bubble::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity_Bubble::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		 m_iBubbleIndex= *static_cast<_uint*>(pArg) + 1;
		wstring strIndex = to_wstring(m_iBubbleIndex);
		m_strUITag = TEXT("IdentityWDR_Bubble");
		m_strUITag += strIndex;
	}
	else
		m_strUITag = TEXT("IdentityWDR_Bubble");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;
	m_vColor = Vec4(100.f, 100.f, 100.f, 1.f);
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));


	m_pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != m_pPlayer)
		m_iBubbleCurrCount = static_cast<CPlayer_Destroyer*>(m_pPlayer)->
		Get_WDR_Controller()->Get_MarbleCnt();

	m_iBubblePreCount = m_iBubbleCurrCount;

	return S_OK;
}

void CUI_WDRIdentity_Bubble::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_Bubbles(fTimeDelta);
}

void CUI_WDRIdentity_Bubble::LateTick(_float fTimeDelta)
{
	if(m_iBubbleCurrCount >= m_iBubbleIndex)
		__super::LateTick(fTimeDelta);
}

HRESULT CUI_WDRIdentity_Bubble::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_WDRIdentity_Bubble::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Bubble"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WDRIdentity_Bubble::Bind_ShaderResources()
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

void CUI_WDRIdentity_Bubble::Update_Bubbles(_float fTimeDelta)
{
	m_iBubbleCurrCount = static_cast<CPlayer_Destroyer*>(m_pPlayer)->
		Get_WDR_Controller()->Get_MarbleCnt();

	if (m_iBubbleCurrCount != m_iBubblePreCount)
	{
		m_bSpawnBubbleAnim = false;
		m_iBubblePreCount = m_iBubbleCurrCount;
		m_vColor = Vec4(100.f, 100.f, 100.f, 1.f);
	}

	Spawan_BubbleAnim(m_iBubbleCurrCount, fTimeDelta);
}

void CUI_WDRIdentity_Bubble::Spawan_BubbleAnim(_uint iBubbleCount, _float fTimeDelta)
{
	if ((0 == iBubbleCount)||(m_bSpawnBubbleAnim))
		return;

	if (iBubbleCount >= m_iBubbleIndex)
	{
		m_vColor -= (m_vColor - Vec4(1.f, 1.f, 1.f, 1.f)) *  (20.f *fTimeDelta);

		if (1.f >= m_vColor.x)
		{
			m_vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			m_bSpawnBubbleAnim = true;
		}
	}

}

CUI_WDRIdentity_Bubble* CUI_WDRIdentity_Bubble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WDRIdentity_Bubble* pInstance = new CUI_WDRIdentity_Bubble(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WDRIdentity_Bubble");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WDRIdentity_Bubble::Clone(void* pArg)
{
	CUI_WDRIdentity_Bubble* pInstance = new CUI_WDRIdentity_Bubble(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WDRIdentity_Bubble");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WDRIdentity_Bubble::Free()
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
