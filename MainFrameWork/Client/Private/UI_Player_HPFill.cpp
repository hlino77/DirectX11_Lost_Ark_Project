#include "stdafx.h"
#include "UI_Player_HPFill.h"
#include "GameInstance.h"
#include "Player.h"
#include "TextBox.h"

CUI_Player_HPFill::CUI_Player_HPFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Player_HPFill::CUI_Player_HPFill(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Player_HPFill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Player_HPFill::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        m_pPlayer = static_cast<CGameObject*>(pArg);
        m_iMaxHp = (_uint)static_cast<CPlayer*>(m_pPlayer)->Get_PlayerStat_Desc().iMaxHp;
        m_iPlayerHp = (_int)static_cast<CPlayer*>(m_pPlayer)->Get_PlayerStat_Desc().iCurHp;
        m_fCurrentRatio = (_float)m_iPlayerHp / (_float)m_iMaxHp;
        m_fPreRatio = m_fCurrentRatio;
    }

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Player_HPFill");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 400.f;
    m_fSizeY = 20.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    if (FAILED(Initialize_TextBox()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_HPFill::Initialize_TextBox()
{
    Ready_TextBox();
    Set_Active(true);
    Print_Hp();
    return S_OK;
}

void CUI_Player_HPFill::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Player_HPFill::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (nullptr != m_pPlayer)
    {
        m_iMaxHp = (_uint)static_cast<CPlayer*>(m_pPlayer)->Get_PlayerStat_Desc().iMaxHp;
        m_iPlayerHp = (_int)static_cast<CPlayer*>(m_pPlayer)->Get_PlayerStat_Desc().iCurHp;
        m_fCurrentRatio = (_float)m_iPlayerHp / (_float)m_iMaxHp;

        if (m_fCurrentRatio != m_fPreRatio)
        {
            m_fPreRatio = m_fCurrentRatio;
            Print_Hp();
        }
    }
  
}

HRESULT CUI_Player_HPFill::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
 
    m_pShaderCom->Begin(7);

    m_pVIBufferCom->Render();

    m_pPlayerHpWnd->Render();

    return S_OK;
}

HRESULT CUI_Player_HPFill::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Fill"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_HPFill::Bind_ShaderResources()
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

void CUI_Player_HPFill::Print_Hp()
{
    if (nullptr != m_pPlayerHpWnd)
    {
        m_pPlayerHpWnd->Set_Active(true);
        m_pPlayerHpWnd->Clear_Text();
        m_pPlayerHpWnd->Set_Alpha(1.f);
        m_pPlayerHpWnd->Get_TransformCom()->Set_Scale(Vec3(240.f, 16.0f, 0.f));
        m_pPlayerHpWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3(590.f - g_iWinSizeX * 0.5f, -790.f + g_iWinSizeY * 0.5f, 0.2f));
        if (0 >= m_iPlayerHp)
            m_iPlayerHp = 0;
        wstring strPlayerHp = to_wstring(m_iPlayerHp) + TEXT("/") + to_wstring(m_iMaxHp);
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("³Ø½¼Lv1°íµñBold"), strPlayerHp);
        Vec2 vOrigin = vMeasure * 0.5f;
        m_pPlayerHpWnd->Set_Text(m_strWndTag, TEXT("³Ø½¼Lv1°íµñBold"), strPlayerHp, Vec2(120.f, 8.f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
    }
}

void CUI_Player_HPFill::Set_Active(_bool bActive)
{
    m_pPlayerHpWnd->Set_Active(bActive);
}

HRESULT CUI_Player_HPFill::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);

    if (nullptr == m_pPlayerHpWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("Clinet_Player_HpUI");
        m_strWndTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(240.f, 16.0f);
        m_pPlayerHpWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pPlayerHpWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pPlayerHpWnd->Set_Render(false);
        m_pPlayerHpWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pPlayerHpWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
    }

    Safe_Release(pGameInstance);
    return S_OK;
}

CUI_Player_HPFill* CUI_Player_HPFill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Player_HPFill* pInstance = new CUI_Player_HPFill(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Player_HPFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player_HPFill::Clone(void* pArg)
{
    CUI_Player_HPFill* pInstance = new CUI_Player_HPFill(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Player_HPFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Player_HPFill::Free()
{
    __super::Free();
    m_pPlayer = nullptr;
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pPlayerHpWnd->Set_Dead(true);

}
