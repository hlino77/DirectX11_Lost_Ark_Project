#include "stdafx.h"
#include "UI_SpeechBubble.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "Chat_Manager.h"

CUI_SpeechBubble::CUI_SpeechBubble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_SpeechBubble::CUI_SpeechBubble(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_SpeechBubble::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_SpeechBubble::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        m_pHost = static_cast<CGameObject*>(pArg);
    }


    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Speech_Bubble");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 200.f;
    m_fSizeY = 100.f;
    m_fAlpha = 0.4;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    Set_Alpha(0.f);

    return S_OK;
}

void CUI_SpeechBubble::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    Setting_HostPos();

    if (!CChat_Manager::GetInstance()->Is_Chat())
        m_bChatOff = false;
    else
    {
        m_bChatting = true;
        Set_Alpha(0.4f);
    }
    if ((!m_bChatOff) && (m_bChatting))
    {
        m_fDuration -= fTimeDelta;
        if (1.f >= m_fDuration)
        {
            Decrease_Alpha(fTimeDelta);
        }
        else if (0 >= m_fDuration)
        {
            m_bChatting = false;
            m_fDuration = 3.f;
        }
    }
}

void CUI_SpeechBubble::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_SpeechBubble::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_SpeechBubble::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SpeechBubble::Bind_ShaderResources()
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

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

void CUI_SpeechBubble::Setting_HostPos()
{
    if (nullptr != m_pHost)
    {
        Vec3 vHostPos = m_pHost->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
        Matrix ViewMatrix  = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
        Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);

        vHostPos = XMVector3TransformCoord(vHostPos, ViewMatrix);
        vHostPos = XMVector3TransformCoord(vHostPos, ProjMatrix);

        m_pTransformCom->Set_State(CTransform::STATE_POSITION,
            Vec3(vHostPos.x - 20.f, vHostPos.y + 150.f, 0.0f));
    }
}

CUI_SpeechBubble* CUI_SpeechBubble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_SpeechBubble* pInstance = new CUI_SpeechBubble(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_SpeechBubble");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_SpeechBubble::Clone(void* pArg)
{
    CUI_SpeechBubble* pInstance = new CUI_SpeechBubble(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_SpeechBubble");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_SpeechBubble::Free()
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
