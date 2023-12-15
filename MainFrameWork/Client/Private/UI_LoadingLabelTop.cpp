#include "stdafx.h"
#include "UI_LoadingLabelTop.h"
#include "GameInstance.h"

CUI_LoadingLabelTop::CUI_LoadingLabelTop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_LoadingLabelTop::CUI_LoadingLabelTop(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_LoadingLabelTop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_LoadingLabelTop::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Loading_LabelTop");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 29.f;
    m_fSizeX = g_iWinSizeX * 0.5f;
    m_fSizeY = 100.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
    return S_OK;
}

void CUI_LoadingLabelTop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_LoadingLabelTop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_LoadingLabelTop::Render()
{
    __super::Render();

    Vec2 vPos(m_fX, m_fY + 4.f);
    Vec2 vScale(0.8f, 0.8f);
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"던파연마된칼날", m_strStage);
    Vec2 vOrigin = vMeasure * 0.5f;
    CGameInstance::GetInstance()->DrawFont(L"던파연마된칼날", m_strStage, vPos, Vec4(1.f, 245.f/255.f, 1.f, 1.0f), 0.0f, vOrigin, vScale);

    return S_OK;
}

void CUI_LoadingLabelTop::Set_StageName(const wstring& strStageName)
{
    m_strStage = strStageName;
}

HRESULT CUI_LoadingLabelTop::Ready_Components()
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

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Top"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    /* Com_Transform */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_LoadingLabelTop::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_LoadingLabelTop* CUI_LoadingLabelTop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_LoadingLabelTop* pInstance = new CUI_LoadingLabelTop(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_LoadingLabelTop");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_LoadingLabelTop::Clone(void* pArg)
{
    CUI_LoadingLabelTop* pInstance = new CUI_LoadingLabelTop(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_LoadingLabelTop");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_LoadingLabelTop::Free()
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
