#include "stdafx.h"
#include "UI_LoadingFill.h"
#include "GameInstance.h"

float CUI_LoadingFill::m_fLoadingSizeX = 0.f;

CUI_LoadingFill::CUI_LoadingFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_LoadingFill::CUI_LoadingFill(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_LoadingFill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_LoadingFill::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_strObjectTag = TEXT("Loading_Fill");

    m_fX = 0.f;
    m_fY = 850.f;
    m_fSizeX = 100.f;
    m_fSizeY = 30;
 
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX*0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
   
    return S_OK;
}

void CUI_LoadingFill::Tick(_float fTimeDelta)
{
    Change_SizeX(1.f);
}

void CUI_LoadingFill::LateTick(_float fTimeDelta)
{
    if (m_bActive)
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_LoadingFill::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_LoadingFill::Change_SizeX(_float fSizeX)
{
    m_fSizeX = (_float)m_fLoadingSizeX;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

HRESULT CUI_LoadingFill::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_FillBar"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_LoadingFill::Bind_ShaderResources()
{
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

CUI_LoadingFill* CUI_LoadingFill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_LoadingFill* pInstance = new CUI_LoadingFill(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_LoadingFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_LoadingFill::Clone(void* pArg)
{
    CUI_LoadingFill* pInstance = new CUI_LoadingFill(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_LoadingFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_LoadingFill::Free()
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
