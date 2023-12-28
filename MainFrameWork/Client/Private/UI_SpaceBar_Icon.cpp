#include "stdafx.h"
#include "UI_SpaceBar_Icon.h"
#include "GameInstance.h"
#include "Player.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"

CUI_SpaceBar_Icon::CUI_SpaceBar_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_SpaceBar_Icon::CUI_SpaceBar_Icon(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_SpaceBar_Icon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_SpaceBar_Icon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("SpaceBar_Icon");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 700.f;
    m_fSizeX = 40.f;
    m_fSizeY = 40.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_SpaceBar_Icon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

    if (nullptr != pPlayer)
    {
        if (nullptr != pPlayer && L"Gunslinger" == pPlayer->Get_ObjectTag())
        {
            m_fCoolMaxTime = static_cast<CPlayer_Gunslinger*>(pPlayer)->
                Get_GN_Controller()->Get_Skill_CoolTime(CPlayer_Controller::SKILL_KEY::SPACE);
            m_fCurrCool = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(pPlayer)->
                Get_GN_Controller())->Get_Skill_CoolDown(CPlayer_Controller::SKILL_KEY::SPACE);
        }

        else if (nullptr != pPlayer && L"WR" == pPlayer->Get_ObjectTag())
        {
            m_fCoolMaxTime = static_cast<CPlayer_Slayer*>(pPlayer)->
                Get_WR_Controller()->Get_Skill_CoolTime(CPlayer_Controller::SKILL_KEY::SPACE);
            m_fCurrCool = static_cast<CController_WR*>(static_cast<CPlayer_Slayer*>(pPlayer)->
                Get_WR_Controller())->Get_Skill_CoolDown(CPlayer_Controller::SKILL_KEY::SPACE);
        }
    }

    m_fResultCool = m_fCoolMaxTime - m_fCurrCool;
}

void CUI_SpaceBar_Icon::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_fCoolMaxTime == m_fResultCool)
    {
        m_fAlpha = 0.f;
        m_fCoolAngle = XM_PI;
    }
    else
    {
        m_fAlpha = 1.f;
        m_fCoolRatio = 1.0f - (m_fResultCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
    }
}

HRESULT CUI_SpaceBar_Icon::Render()
{
 
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    m_pShaderCom->Begin(8);
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_SpaceBar_Icon::Ready_Components()
{
    __super::Ready_Components();

    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    CTexture* pTexture = nullptr;

    /* Com_Texture*/
    if (nullptr != pPlayer && L"Gunslinger" == pPlayer->Get_ObjectTag())
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Space"),
            TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
            return E_FAIL;
    }
    else if (nullptr != pPlayer && L"WR" == pPlayer->Get_ObjectTag())
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_WRSpace"),
            TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CUI_SpaceBar_Icon::Bind_ShaderResources()
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

    return S_OK;
}

CUI_SpaceBar_Icon* CUI_SpaceBar_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_SpaceBar_Icon* pInstance = new CUI_SpaceBar_Icon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_SpaceBar_Icon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_SpaceBar_Icon::Clone(void* pArg)
{
    CUI_SpaceBar_Icon* pInstance = new CUI_SpaceBar_Icon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_SpaceBar_Icon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_SpaceBar_Icon::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pPlayer_Skill = nullptr;
    Safe_Release(m_pTextureCom);//¿¥ÇÁÆ¼
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
