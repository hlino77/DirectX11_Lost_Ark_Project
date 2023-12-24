#include "stdafx.h"
#include "UI_HoldingFill.h"
#include "GameInstance.h"
#include "Player.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"


CUI_HoldingFill::CUI_HoldingFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_HoldingFill::CUI_HoldingFill(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_HoldingFill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_HoldingFill::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Holding_Fill");

    m_fX = 0.f;
    m_fY = 850.f;
    m_fSizeX = 77.f;
    m_fSizeY = 30.f;
 
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX*0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
   
    return S_OK;
}

void CUI_HoldingFill::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    /*CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

    if (nullptr != pPlayer)
    {
        if ((_uint)CHR_CLASS::GUNSLINGER == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectType())
        {
            m_pPlayerSkill = static_cast<CPlayer_Gunslinger*>(pPlayer)->
                Get_GN_Controller()->Get_PlayerSkill(static_cast<CPlayer_Gunslinger*>(pPlayer)->
                    Get_GN_Controller()->Get_Selected_Skill());

            if (CPlayer_Skill::SKILL_CTRLTYPE::HOLD == m_pPlayerSkill->Get_Skill_CtrlType())
            {
                m_bHoldOn = true;
            }
        }
    }*/
}

void CUI_HoldingFill::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_HoldingFill::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_HoldingFill::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Fill"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_HoldingFill::Bind_ShaderResources()
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

CUI_HoldingFill* CUI_HoldingFill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_HoldingFill* pInstance = new CUI_HoldingFill(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_HoldingFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_HoldingFill::Clone(void* pArg)
{
    CUI_HoldingFill* pInstance = new CUI_HoldingFill(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_HoldingFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_HoldingFill::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pPlayerSkill);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
