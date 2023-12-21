#include "stdafx.h"
#include "UI_SkillIcon_Frame.h"
#include "GameInstance.h"

CUI_SkillIcon_Frame::CUI_SkillIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_SkillIcon_Frame::CUI_SkillIcon_Frame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_SkillIcon_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        wstring* strIndex = static_cast<wstring*>(pArg);
        m_strUITag = TEXT("SkillIcon_Frame");
        m_strUITag += *strIndex;
    }
    else
        m_strUITag = TEXT("SkillIcon_Frame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 64.f;
    m_fSizeY = 64.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_SkillIcon_Frame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
}

void CUI_SkillIcon_Frame::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_fCurrCool == m_fCoolMaxTime)
        m_fCoolAngle = XM_PI;
    else
    {
        m_fCoolRatio = 1.0f - (m_fCurrCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
    }
}

HRESULT CUI_SkillIcon_Frame::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(8);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkillFrame"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_SkillIcon_Frame* CUI_SkillIcon_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_SkillIcon_Frame* pInstance = new CUI_SkillIcon_Frame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_SkillIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_SkillIcon_Frame::Clone(void* pArg)
{
    CUI_SkillIcon_Frame* pInstance = new CUI_SkillIcon_Frame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_SkillIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_SkillIcon_Frame::Free()
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