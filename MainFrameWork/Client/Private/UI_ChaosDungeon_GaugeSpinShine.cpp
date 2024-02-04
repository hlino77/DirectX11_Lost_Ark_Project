#include "stdafx.h"
#include "UI_ChaosDungeon_GaugeSpinShine.h"
#include "GameInstance.h"

CUI_ChaosDungeon_GaugeSpinShine::CUI_ChaosDungeon_GaugeSpinShine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChaosDungeon_GaugeSpinShine::CUI_ChaosDungeon_GaugeSpinShine(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChaosDungeon_GaugeSpinShine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon_GaugeSpinShine::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("ChaosDungeon_GaugeSpinShine");

	m_fSizeX = 504;
	m_fSizeY = 335;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = (g_iWinSizeY * 0.5f) + 100.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_ChaosDungeon_GaugeSpinShine::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (76 <= m_fFrame)
		m_fFrame = 0.f;

	m_fFrame += 30.f * fTimeDelta;
	m_bRender = false;
}

void CUI_ChaosDungeon_GaugeSpinShine::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ChaosDungeon_GaugeSpinShine::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(18);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_ChaosDungeon_GaugeSpinShine::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gague_BackGround_SpinShine"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_GaugeSpinShine::Bind_ShaderResources()
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

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture",(_uint)m_fFrame);

	return S_OK;
}

CUI_ChaosDungeon_GaugeSpinShine* CUI_ChaosDungeon_GaugeSpinShine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChaosDungeon_GaugeSpinShine* pInstance = new CUI_ChaosDungeon_GaugeSpinShine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChaosDungeon_GaugeSpinShine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChaosDungeon_GaugeSpinShine::Clone(void* pArg)
{
	CUI_ChaosDungeon_GaugeSpinShine* pInstance = new CUI_ChaosDungeon_GaugeSpinShine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChaosDungeon_GaugeSpinShine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChaosDungeon_GaugeSpinShine::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
