#include "stdafx.h"
#include "UI_Boss_SpecialGroggy.h"
#include "GameInstance.h"
#include "Boss.h"

CUI_Boss_SpecialGroggy::CUI_Boss_SpecialGroggy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_Boss_SpecialGroggy::CUI_Boss_SpecialGroggy(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_SpecialGroggy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Boss_SpecialGroggy::Initialize(void* pArg)
{
	if (nullptr != pArg)
		m_pOwner = static_cast<CGameObject*>(pArg);
	else
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 158.f;
	m_fSizeY = 13.f;
	m_fX = 0.5f * g_iWinSizeX;
	m_fY = 0.5f * g_iWinSizeY;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	
	m_bActive = false;

	return S_OK;
}

void CUI_Boss_SpecialGroggy::Tick(_float fTimeDelta)
{

}

void CUI_Boss_SpecialGroggy::LateTick(_float fTimeDelta)
{
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_WORLDUI, this);

	Update_Position();
	Update_GroggyCount();
}

HRESULT CUI_Boss_SpecialGroggy::Render()
{
	if (0 == m_fMaxGroggyCount)
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Fill()))
		return E_FAIL;
	m_pShaderCom->Begin("PixTexturePassX_NoGamma");
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Boss_SpecialGroggy::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_GroggyWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_GroggyFill"),
		TEXT("Com_Texture_Fill"), (CComponent**)&m_pTexture_Fill)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_SpecialGroggy::Bind_ShaderResources()
{
	m_fSizeY = 13.f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_Boss_SpecialGroggy::Bind_ShaderResources_Fill()
{
	m_fSizeY = 9.f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fRatio, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTexture_Fill->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	return S_OK;
}

void CUI_Boss_SpecialGroggy::Update_GroggyCount()
{
	if (nullptr == m_pOwner)
		return;

	m_fCurrGroggyCount = static_cast<CBoss*>(m_pOwner)->Get_GroggyCount();
	m_fMaxGroggyCount = static_cast<CBoss*>(m_pOwner)->Get_MaxGroggyCount();
	if(0 != m_fMaxGroggyCount)
		m_fRatio = m_fCurrGroggyCount / m_fMaxGroggyCount;
}

void CUI_Boss_SpecialGroggy::Update_Position()
{
	if (nullptr != m_pOwner)
	{
		Vec3 vHostPos = static_cast<CBoss*>(m_pOwner)->Get_GroggyPos();
		m_vEffectPos = vHostPos;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(vHostPos.x * g_iWinSizeX * 0.5f, vHostPos.y * g_iWinSizeY * 0.5f, 0.2f));
	}
}

CUI_Boss_SpecialGroggy* CUI_Boss_SpecialGroggy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_SpecialGroggy* pInstance = new CUI_Boss_SpecialGroggy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Boss_SpecialGroggy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Boss_SpecialGroggy::Clone(void* pArg)
{
	CUI_Boss_SpecialGroggy* pInstance = new CUI_Boss_SpecialGroggy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Boss_SpecialGroggy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_SpecialGroggy::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_Fill);
}
