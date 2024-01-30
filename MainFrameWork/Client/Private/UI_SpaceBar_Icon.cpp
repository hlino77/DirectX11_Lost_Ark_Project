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
#include "Controller_WDR.h"
#include "Player_Destroyer.h"
#include "Controller_MG.h"
#include "Player_Bard.h"
#include "TextBox.h"

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
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    if (FAILED(Initialize_Percent()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SpaceBar_Icon::Initialize_Percent()
{
    m_szFont = L"³Ø½¼Lv1°íµñBold";
    Ready_TextBox();

    Set_Active(false);

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

        else if (nullptr != pPlayer && L"WDR" == pPlayer->Get_ObjectTag())
        {
            m_fCoolMaxTime = static_cast<CPlayer_Destroyer*>(pPlayer)->
                Get_WDR_Controller()->Get_Skill_CoolTime(CPlayer_Controller::SKILL_KEY::SPACE);
            m_fCurrCool = static_cast<CController_WDR*>(static_cast<CPlayer_Destroyer*>(pPlayer)->
                Get_WDR_Controller())->Get_Skill_CoolDown(CPlayer_Controller::SKILL_KEY::SPACE);
        }

        else if (nullptr != pPlayer && L"MG" == pPlayer->Get_ObjectTag())
        {
            m_fCoolMaxTime = static_cast<CPlayer_Bard*>(pPlayer)->
                Get_MG_Controller()->Get_Skill_CoolTime(CPlayer_Controller::SKILL_KEY::SPACE);
            m_fCurrCool = static_cast<CController_MG*>(static_cast<CPlayer_Bard*>(pPlayer)->
                Get_MG_Controller())->Get_Skill_CoolDown(CPlayer_Controller::SKILL_KEY::SPACE);
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
        if (nullptr != m_pCoolTimetWnd)
            m_pCoolTimetWnd->Set_Active(false);
    }
    else
    {
        m_fAlpha = 1.f;
        m_fCoolRatio = 1.0f - (m_fResultCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
        Print_CoolTime();
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
    else if (nullptr != pPlayer && L"WDR" == pPlayer->Get_ObjectTag())
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_WDRSpace"),
            TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
            return E_FAIL;
    }
    else if (nullptr != pPlayer && L"MG" == pPlayer->Get_ObjectTag())
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_MGSpace"),
            TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_MGSpace"),
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

void CUI_SpaceBar_Icon::Print_CoolTime()
{
    if (nullptr == m_pCoolTimetWnd)
        return;

    m_pCoolTimetWnd->Set_Active(true);
    m_pCoolTimetWnd->Clear_Text();
    m_pCoolTimetWnd->Set_Alpha(1.f);

    Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
    m_pCoolTimetWnd->Get_TransformCom()->Set_Scale(Vec3(22.f, 22.f, 0.f));
    m_pCoolTimetWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

    Set_StringCoolTime();
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"ºûÀÇ°è½ÂÀÚ", m_strCoolTime);
    Vec2 vOrigin = vMeasure * 0.5f;
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-1"), m_szFont, m_strCoolTime, Vec2(22.f - 1.f, 22.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-2"), m_szFont, m_strCoolTime, Vec2(22.f + 1.f, 22.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-3"), m_szFont, m_strCoolTime, Vec2(22.f, 22.f - 1.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-4"), m_szFont, m_strCoolTime, Vec2(22.f, 22.f + 1.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));

    m_pCoolTimetWnd->Set_Text(m_strWndName, m_szFont, m_strCoolTime, Vec2(22.f, 22.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.f));
}

void CUI_SpaceBar_Icon::Set_Active(_bool bActive)
{
    m_pCoolTimetWnd->Set_Active(bActive);
}

HRESULT CUI_SpaceBar_Icon::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);
    if (nullptr == m_pCoolTimetWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("SpaceCoolWnd");
        m_strWndName = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(44.f, 44.0f);
        m_pCoolTimetWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pCoolTimetWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pCoolTimetWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pCoolTimetWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
    }
    Safe_Release(pGameInstance);
    return S_OK;
}

void CUI_SpaceBar_Icon::Start_CoolTimeText()
{
    m_strCoolTime.clear();
    m_pCoolTimetWnd->Set_Active(true);
}

void CUI_SpaceBar_Icon::End_CoolTimeText()
{
    m_strCoolTime.clear();
    m_pCoolTimetWnd->Set_Active(false);
}

void CUI_SpaceBar_Icon::Set_StringCoolTime()
{
    wstring strCool = to_wstring((_uint)m_fResultCool);
    m_strCoolTime.clear();
    m_strCoolTime = strCool + TEXT("s");
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

    m_pCoolTimetWnd->Set_Dead(true);
    Safe_Release(m_pTextureCom);//¿¥ÇÁÆ¼
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
