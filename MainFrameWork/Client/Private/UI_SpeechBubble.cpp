#include "stdafx.h"
#include "UI_SpeechBubble.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "Chat_Manager.h"
#include "TextBox.h"
#include "Pool.h"

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
    _uint iIndex = *static_cast<_uint*>(pArg);


    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Speech_Bubble");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 200.f;
    m_fSizeY = 70.f;
    m_fAlpha = 0.4f;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));


    m_pTransformUp->Set_Scale(Vec3(m_fSizeX, 10.0f, 1.f));
    m_pTransformUp->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    m_pTransformDown->Set_Scale(Vec3(m_fSizeX, 19.0f, 1.f));
    m_pTransformDown->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));



    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    Set_Alpha(0.f);

    if (FAILED(Ready_TextBox(iIndex)))
        return E_FAIL;

    m_szFont = L"던파연마된칼날";
    m_vTextScale = Vec2(0.35f, 0.35f);

    Set_Active(false);

    return S_OK;
}

void CUI_SpeechBubble::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

   
    if (m_fDuration > 0.0f)
    {
        m_fDuration -= fTimeDelta;
        if (m_fDuration <= 0.0f)
        {
            Set_Active(false);
            m_pTextBox->Set_Active(false);
        }
    }
 
}

void CUI_SpeechBubble::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    Setting_HostPos();
}

HRESULT CUI_SpeechBubble::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformUp->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pTextureUp->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();
   
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformDown->Get_WorldMatrix())))
        return E_FAIL;

    if (FAILED(m_pTextureDown->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();


    return S_OK;
}

HRESULT CUI_SpeechBubble::Active_SpeechBuble(wstring szChat)
{
    Set_Active(true);
    m_fDuration = 3.0f;
    m_fAlpha = 0.4f;

    wstring szText;
    wstring szLine;
    for (_uint i = 0; i < szChat.length(); ++i)
    {
        szLine += szChat[i];

        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szLine);
        if (vMeasure.x * m_vTextScale.x > m_fSizeX * 0.9f)
        {
            szLine.pop_back();
            szLine += L"\n";
            szText += szLine;
            szLine.clear();
            --i;
        }
    }

    if (szLine.empty() == false)
        szText += szLine;

    
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szText);
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, vMeasure.y * m_vTextScale.y, 1.0f));

    m_pTextBox->Clear_Text();
    m_pTextBox->Set_Text(L"ChatWindow", m_szFont, szText, Vec2(0.0f, m_fSizeY * 3.f * 0.5f), m_vTextScale, Vec2(0.0f, vMeasure.y * 0.5f), 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_pTextBox->Set_Active(true);

    return S_OK;
}

HRESULT CUI_SpeechBubble::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Middle"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Up"),
        TEXT("Com_TextureUp"), (CComponent**)&m_pTextureUp)))
        return E_FAIL;

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Down"),
        TEXT("Com_TextureDown"), (CComponent**)&m_pTextureDown)))
        return E_FAIL;

    /* Com_Transform */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_TransformUp"), (CComponent**)&m_pTransformUp)))
        return E_FAIL;

    /* Com_Transform */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_TransformDown"), (CComponent**)&m_pTransformDown)))
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

    if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

void CUI_SpeechBubble::Setting_HostPos()
{
    if (nullptr != m_pHost)
    {
        Vec3 vHostPos = m_pHost->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

        vHostPos.y += 1.7f;

        Matrix ViewMatrix  = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
        Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);

        vHostPos = XMVector3TransformCoord(vHostPos, ViewMatrix);
        vHostPos = XMVector3TransformCoord(vHostPos, ProjMatrix);

        vHostPos.x *= g_iWinSizeX * 0.5f;
        vHostPos.y *= g_iWinSizeY * 0.5f;

        Vec3 vScale = m_pTransformCom->Get_Scale();

        m_pTransformCom->Set_State(CTransform::STATE_POSITION,
            Vec3(vHostPos.x - 20.f, vHostPos.y + (vScale.y * 0.5f) + 30.0f, 0.1f));

        Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        Vec3 vDownScale = m_pTransformDown->Get_Scale();
        Vec3 vUpScale = m_pTransformUp->Get_Scale();

        m_pTransformDown->Set_State(CTransform::STATE_POSITION,
            Vec3(vPos.x, vPos.y - (vScale.y * 0.5f) - (vDownScale.y * 0.5f), 0.1f));

        m_pTransformUp->Set_State(CTransform::STATE_POSITION,
            Vec3(vPos.x, vPos.y + (vScale.y * 0.5f) + (vUpScale.y * 0.5f), 0.1f));

        m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(vPos.x, vPos.y, 0.0f));
    }
}

HRESULT CUI_SpeechBubble::Ready_TextBox(_uint iIndex)
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    CTextBox::TEXTBOXDESC tTextDesc;
    tTextDesc.szTextBoxTag = L"SpeechBuble" + to_wstring(iIndex);
    tTextDesc.vSize = Vec2(m_fSizeX * 0.9f, m_fSizeY * 3.f);

    m_pTextBox = dynamic_cast<CTextBox*>(pGameInstance->Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
    if (m_pTextBox == nullptr)
    {
        Safe_Release(pGameInstance);
        return E_FAIL;
    }

    m_pTextBox->Set_ScaleUV(Vec2(1.0f, 1.0f));
    m_pTextBox->Set_Pos(0.0f, 0.0f);

    Safe_Release(pGameInstance);
    return S_OK;
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
