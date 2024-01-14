#include "stdafx.h"
#include "UI_LoadingFill.h"
#include "GameInstance.h"

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

    m_strUITag = TEXT("Loading_Fill");

    m_fX = 0.f;
    m_fY = 850.f;
    m_fSizeX = g_iWinSizeX;
    m_fSizeY = 30.f;
 
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX*0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

    m_fX_Arrow = 50.f;
    m_fY_Arrow = 850.f;
    m_fSizeX_Arrow = 200.f;
    m_fSizeY_Arrow = 30.f;
    m_pTransform_Arrow->Set_Scale(Vec3(m_fSizeX_Arrow, m_fSizeY_Arrow, 1.f));
    m_pTransform_Arrow->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX_Arrow - (g_iWinSizeX * 0.5f) + (m_fSizeX_Arrow * 0.5f), -m_fY_Arrow + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
   
    return S_OK;
}

void CUI_LoadingFill::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_LoadingFill::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    //Update_Loading();
}

HRESULT CUI_LoadingFill::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pShaderCom->Begin(0);//0
    m_pVIBufferCom->Render();            

    /*if (FAILED(Bind_ShaderResources_Arrow()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();*/

    return S_OK;
}

void CUI_LoadingFill::Change_SizeX(_float fSizeX)
{
    m_fSizeX = fSizeX;

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

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BarArrow"),
        TEXT("Com_Texture_Arrow"), (CComponent**)&m_pTexture_Arrow)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Arrow"), (CComponent**)&m_pTransform_Arrow)))
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
    //if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fFilesRatio, sizeof(_float))))
        //return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

HRESULT CUI_LoadingFill::Bind_ShaderResources_Arrow()
{

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    m_pTexture_Arrow->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

void CUI_LoadingFill::Update_Loading()
{
    m_fFilesRatio = (_float)m_iCurrFiles / (_float)m_iMaxFiles;

    _float fArrow_X = (_float)g_iWinSizeX - (g_iWinSizeX * m_fFilesRatio);
    m_pTransform_Arrow->Set_State(CTransform::STATE_POSITION,
        Vec3(fArrow_X - g_iWinSizeX * 0.5f, -m_fY_Arrow + g_iWinSizeY * 0.5f, 0.f));
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
    Safe_Release(m_pTexture_Arrow);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTransform_Arrow);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
