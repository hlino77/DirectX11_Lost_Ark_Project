#include "stdafx.h"
#include "UI_WRIdentity_Body.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"

CUI_WRIdentity_Body::CUI_WRIdentity_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_WRIdentity_Body::CUI_WRIdentity_Body(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_WRIdentity_Body::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_WRIdentity_Body::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("WRIdentity_Body");

    m_fX = 800.f;
    m_fY = 826.f;
    m_fSizeX = 240.f;
    m_fSizeY = 160.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 0.1f));

    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    m_iIdentity_MaxGauge = static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Get_IdenMaxGauge();
    m_fIdentity_MaxCool = static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Get_IdenCoolTime();

    return S_OK;
}

void CUI_WRIdentity_Body::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    Get_Identity_State(pPlayer);
    Identity_NormalState(fTimeDelta);
    Transform_Identity(fTimeDelta);
    Identity_CoolState(fTimeDelta);
}

void CUI_WRIdentity_Body::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_WRIdentity_Body::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_WRIdentity_Body::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_WRIdentity_Body::Bind_ShaderResources()
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

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);

    return S_OK;
}

void CUI_WRIdentity_Body::Get_Identity_State(CPlayer* pPlayer)
{
    if (nullptr != pPlayer)
    {
        m_iIdentity_Gauge = static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Get_IdenGage();

        m_bIdentity = static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Is_In_Identity();

        m_fIdentity_CurrCool = static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Get_IdenCoolAcc();

        if ((!m_bIdentity) && (m_iIdentity_Gauge != m_iIdentity_MaxGauge))
            m_iIdentity_CurrState = (_int)WRIDENTITY_NORMAL;

        else if ((!m_bIdentity) && (m_iIdentity_CurrState == (_int)WRIDENTITY_NORMAL)&&(m_iIdentity_Gauge == m_iIdentity_MaxGauge))
            m_iIdentity_CurrState = (_int)WRIDENTITY_MAX;

        else if ((m_bIdentity) && (m_iIdentity_CurrState == (_int)WRIDENTITY_MAX))
        {
            m_bTransfrom = true;
            m_iIdentity_CurrState = (_int)WRIDENTITY_TRANSFORM_ON;
        }
        else if ((!m_bTransfrom) && (m_bIdentity) && (m_iIdentity_CurrState == (_int)WRIDENTITY_TRANSFORM_ON))
            m_iIdentity_CurrState = (_int)WRIDENTITY_ON;

        else if ((!m_bIdentity) && (m_iIdentity_CurrState == (_int)WRIDENTITY_ON) && (0 == m_iIdentity_Gauge))
        {
            m_bTransfrom = true;
            m_iIdentity_CurrState = (_int)WRIDENTITY_TRANSFORM_OFF;
        }

        else if ((!m_bTransfrom) && (m_iIdentity_CurrState == (_int)WRIDENTITY_TRANSFORM_OFF))
            m_iIdentity_CurrState = (_int)WRIDENTITY_COOL;

        else if ((m_fIdentity_CurrCool >= m_fIdentity_MaxCool)&&(m_iIdentity_CurrState == (_int)WRIDENTITY_COOL))
            m_iIdentity_CurrState = (_int)WRIDENTITY_NORMAL;
    }
}

void CUI_WRIdentity_Body::Identity_NormalState(_float fTimeDelta)
{
    if (m_iIdentity_CurrState != (_int)WRIDENTITY_NORMAL)
        return;

    if (12 <= m_iTextureIndex)
        m_iTextureIndex = 0;

    m_iTextureIndex += (_int)(5.f * fTimeDelta);

}

void CUI_WRIdentity_Body::Transform_Identity(_float fTimeDelta)
{
    if (!m_bTransfrom)
        return; 

    if (m_iIdentity_CurrState == (_int)WRIDENTITY_TRANSFORM_ON)
    {   
        m_iTextureIndex += 5.f * fTimeDelta;
        if(24 < m_iTextureIndex)
            m_bTransfrom = false;
    }

    else if (m_iIdentity_CurrState == (_int)WRIDENTITY_TRANSFORM_OFF)
    {
        m_iTextureIndex += 5.f * fTimeDelta;
        if (28 < m_iTextureIndex)
            m_bTransfrom = false;
    }
}

void CUI_WRIdentity_Body::Identity_CoolState(_float fTimeDelta)
{
}

CUI_WRIdentity_Body* CUI_WRIdentity_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_WRIdentity_Body* pInstance = new CUI_WRIdentity_Body(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_WRIdentity_Body");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_WRIdentity_Body::Clone(void* pArg)
{
    CUI_WRIdentity_Body* pInstance = new CUI_WRIdentity_Body(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_WRIdentity_Body");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_WRIdentity_Body::Free()
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
