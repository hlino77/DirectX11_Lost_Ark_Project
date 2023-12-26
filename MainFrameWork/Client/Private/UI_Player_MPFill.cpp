#include "stdafx.h"
#include "UI_Player_MPFill.h"
#include "GameInstance.h"

CUI_Player_MPFill::CUI_Player_MPFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Player_MPFill::CUI_Player_MPFill(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Player_MPFill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Player_MPFill::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        m_pPlayer = static_cast<CGameObject*>(pArg);
    }

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Player_HPFrame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 400.f;
    m_fSizeY = 50.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_Player_MPFill::Tick(const _float& fTimeDelta)
{
    __super::Tick(fTimeDelta);
    if (nullptr != m_pPlayer)
    {
        _float fMaxHp = m_pPlayer->Get_MaxHp();
        _float fCurrHp = m_pPlayer->Get_Hp();
        m_fCurrentRatio = fCurrHp / fMaxHp * 100.f;
    }
}

void CUI_Player_MPFill::LateTick(const _float& fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_Player_MPFill::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(7);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_Player_MPFill::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Fill"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_MPFill::Bind_ShaderResources()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCurrentRatio, sizeof(_float))))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_Player_MPFill* CUI_Player_MPFill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Player_MPFill* pInstance = new CUI_Player_MPFill(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Player_MPFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player_MPFill::Clone(void* pArg)
{
    CUI_Player_MPFill* pInstance = new CUI_Player_MPFill(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Player_MPFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Player_MPFill::Free()
{
    __super::Free();
    m_pPlayer = nullptr;
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
