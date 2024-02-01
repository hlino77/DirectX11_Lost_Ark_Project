#include "stdafx.h"
#include "UI_WRIdentity_Gauge.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"
#include "UI_WRIdentity_Body.h"

CUI_WRIdentity_Gauge::CUI_WRIdentity_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_WRIdentity_Gauge::CUI_WRIdentity_Gauge(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_WRIdentity_Gauge::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_WRIdentity_Gauge::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("WRIdentity_Gauge");

    m_fX = 800.f;
    m_fY = 826.f;
    m_fSizeX = 240.f;
    m_fSizeY = 160.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    if(nullptr != pPlayer)
    m_fIdentity_MaxGauge = static_cast<CPlayer_Slayer*>(pPlayer)->
        Get_WR_Controller()->Get_IdenMaxGauge();

    return S_OK;
}

void CUI_WRIdentity_Gauge::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    Identity_Gauge();
}

void CUI_WRIdentity_Gauge::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_WRIdentity_Gauge::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(9);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_WRIdentity_Gauge::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Gauge"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_WRIdentity_Gauge::Bind_ShaderResources()
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

    //3.15(시작)  0(절반)   -3.15(꽉참)
    //_float fAngle = 1.5f;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fGaugeAngle, sizeof(_float))))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

void CUI_WRIdentity_Gauge::Identity_Gauge()
{
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    if (nullptr != pPlayer)
    {
        m_fIdentity_Gauge = static_cast<CPlayer_Slayer*>(pPlayer)->
            Get_WR_Controller()->Get_IdenGage();

        if (m_fIdentity_MaxGauge == m_fIdentity_Gauge)
        {
            m_fGaugeAngle = -(XM_PI);
        }
        else
        {
            //현탐때 m_iIdentity_Gauge 쓰레기값들어옴
            m_fGaugeRatio = (_float)m_fIdentity_Gauge / (_float)m_fIdentity_MaxGauge;
            m_fGaugeAngle = XM_PI - (2.f * XM_PI * m_fGaugeRatio);
            if (1.f < m_fGaugeRatio)
                m_fGaugeAngle = XM_PI;
        }
    }
}

CUI_WRIdentity_Gauge* CUI_WRIdentity_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_WRIdentity_Gauge* pInstance = new CUI_WRIdentity_Gauge(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_WRIdentity_Gauge");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_WRIdentity_Gauge::Clone(void* pArg)
{
    CUI_WRIdentity_Gauge* pInstance = new CUI_WRIdentity_Gauge(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_WRIdentity_Gauge");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_WRIdentity_Gauge::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
