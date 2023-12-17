#include "stdafx.h"
#include "UI_LoadingLabelBottom.h"
#include "GameInstance.h"

CUI_LoadingLabelBottom::CUI_LoadingLabelBottom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_LoadingLabelBottom::CUI_LoadingLabelBottom(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_LoadingLabelBottom::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_LoadingLabelBottom::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Loading_LabelBottom");

    m_fX = g_iWinSizeX * 0.5f - 9.f;
    m_fY = 800.f;
    m_fSizeX = g_iWinSizeX * 0.5f;
    m_fSizeY = 100.f;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
    return S_OK;
}

void CUI_LoadingLabelBottom::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_LoadingLabelBottom::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_LoadingLabelBottom::Render()
{
    __super::Render();

    Vec2 vPos(m_fX, m_fY - 20.f);
    Vec2 vScale(0.6f, 0.6f);
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"던파연마된칼날", m_strTip);
    Vec2 vOrigin = vMeasure * 0.5f;
    CGameInstance::GetInstance()->DrawFont(L"던파연마된칼날", m_strTip, vPos, Vec4(214.f / 255.f, 174.f/255.f, 90.f / 255.f, 1.0f), 0.0f, vOrigin, vScale);

    vPos = Vec2(m_fX, m_fY + 15.f);
    vScale = Vec2(0.5f, 0.5f);
    vMeasure = CGameInstance::GetInstance()->MeasureString(L"한컴고딕", m_strToolTips);
    vOrigin = vMeasure * 0.5f;
    CGameInstance::GetInstance()->DrawFont(L"한컴고딕", m_strToolTips, vPos, Vec4(1.f, 1.f, 1.f, 1.0f), 0.0f, vOrigin, vScale);
    return S_OK;
}

void CUI_LoadingLabelBottom::Set_ToolTip(const wstring& strToolTip)
{
    m_strToolTips = strToolTip;
}

HRESULT CUI_LoadingLabelBottom::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Bottom"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_LoadingLabelBottom::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_LoadingLabelBottom* CUI_LoadingLabelBottom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_LoadingLabelBottom* pInstance = new CUI_LoadingLabelBottom(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_LoadingLabelBottom");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_LoadingLabelBottom::Clone(void* pArg)
{
    CUI_LoadingLabelBottom* pInstance = new CUI_LoadingLabelBottom(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_LoadingLabelBottom");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_LoadingLabelBottom::Free()
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
