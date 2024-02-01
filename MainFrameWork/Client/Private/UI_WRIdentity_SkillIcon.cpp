#include "stdafx.h"
#include "UI_WRIdentity_SkillIcon.h"
#include "GameInstance.h"
#include "UI_WRIdentity_Body.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"
#include "TextBox.h"

CUI_WRIdentity_SkillIcon::CUI_WRIdentity_SkillIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_WRIdentity_SkillIcon::CUI_WRIdentity_SkillIcon(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_WRIdentity_SkillIcon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_WRIdentity_SkillIcon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pBodyUI = static_cast<CUI*>(pArg);
    }


    m_strUITag = TEXT("WRIdentity_SkillIcon");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 700.f;
    m_fSizeX = 40.f;
    m_fSizeY = 40.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    if (nullptr != pPlayer)
        m_fCoolMaxTime = static_cast<CPlayer_Slayer*>(pPlayer)->
        Get_WR_Controller()->Get_IdenSkillCool();

    if (FAILED(Initialize_TextBox()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_WRIdentity_SkillIcon::Initialize_TextBox()
{
    m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
    if (FAILED(Ready_TextBox()))
        return E_FAIL;
    m_pCoolTimeWnd->Set_Active(false);

    return S_OK;
}

void CUI_WRIdentity_SkillIcon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    if (nullptr != pPlayer)
    {
            m_fCurrCool = static_cast<CController_WR*>(static_cast<CPlayer_Slayer*>(pPlayer)->
                Get_WR_Controller())->Get_IdentitySkill_CoolDown();
    }


    if ((CUI_WRIdentity_Body::WRIDENTITY_TRANSFORM_ON == static_cast<CUI_WRIdentity_Body*>(m_pBodyUI)->Get_Current_IdentityState())
        || (CUI_WRIdentity_Body::WRIDENTITY_ON == static_cast<CUI_WRIdentity_Body*>(m_pBodyUI)->Get_Current_IdentityState()))
    {
        m_fAlpha = 1.f;
        m_bSkillIcon_On = true;
    }
    else
    {
        m_fAlpha = 0.f;
        m_bSkillIcon_On = false;
    }

    m_fResultCool = m_fCoolMaxTime - m_fCurrCool;
}

void CUI_WRIdentity_SkillIcon::LateTick(_float fTimeDelta)
{
    if (m_bSkillIcon_On)
        __super::LateTick(fTimeDelta);
    if (m_fCoolMaxTime == m_fResultCool)
    {
        m_fCoolAngle = XM_PI;
        m_pCoolTimeWnd->Set_Active(false);
    }
    else
    {
        if (m_bSkillIcon_On)
        {
            m_fAlpha = 1.f;
            m_pCoolTimeWnd->Set_Active(true);
        }
        else
        {
            m_fAlpha = 0.f;
            m_pCoolTimeWnd->Set_Active(false);
        }
        m_fCoolRatio = 1.0f - (m_fResultCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
       
        Print_CoolTime();
    }
}

HRESULT CUI_WRIdentity_SkillIcon::Render()
{
 
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(20);
    m_pVIBufferCom->Render();

    if (m_fCoolMaxTime != m_fResultCool)
        m_pCoolTimeWnd->Render();

    return S_OK;
}

HRESULT CUI_WRIdentity_SkillIcon::Ready_Components()
{
    __super::Ready_Components();

        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_Identity_SkillFrame"),
            TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
            return E_FAIL;

    return S_OK;
}

HRESULT CUI_WRIdentity_SkillIcon::Bind_ShaderResources()
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

HRESULT CUI_WRIdentity_SkillIcon::Ready_TextBox()
{
    if (nullptr == m_pCoolTimeWnd)
    {
        CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
        Safe_AddRef(pGameInstance);

        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("Slayer_IdentitySkillCool");
        m_strTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(44.f, 44.0f);
        m_pCoolTimeWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pCoolTimeWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pCoolTimeWnd->Set_Render(false);

        m_pCoolTimeWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pCoolTimeWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);

        Safe_Release(pGameInstance);
    }

    return S_OK;
}

void CUI_WRIdentity_SkillIcon::Print_CoolTime()
{
    if ((nullptr == m_pCoolTimeWnd))
        return;

    m_pCoolTimeWnd->Clear_Text();

    m_pCoolTimeWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

    wstring strCool = to_wstring((_uint)m_fResultCool) + TEXT("s");

    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", strCool);
    Vec2 vOrigin = vMeasure * 0.5f;

    m_pCoolTimeWnd->Set_Text(m_strTag + TEXT("-1"), m_strFont, strCool, Vec2(22.f - 1.f, 22.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimeWnd->Set_Text(m_strTag + TEXT("-2"), m_strFont, strCool, Vec2(22.f + 1.f, 22.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimeWnd->Set_Text(m_strTag + TEXT("-3"), m_strFont, strCool, Vec2(22.f, 22.f - 1.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimeWnd->Set_Text(m_strTag + TEXT("-4"), m_strFont, strCool, Vec2(22.f, 22.f + 1.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));

    m_pCoolTimeWnd->Set_Text(m_strTag, m_strFont, strCool, Vec2(22.f, 22.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.f));
}

CUI_WRIdentity_SkillIcon* CUI_WRIdentity_SkillIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_WRIdentity_SkillIcon* pInstance = new CUI_WRIdentity_SkillIcon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_WRIdentity_SkillIcon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_WRIdentity_SkillIcon::Clone(void* pArg)
{
    CUI_WRIdentity_SkillIcon* pInstance = new CUI_WRIdentity_SkillIcon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_WRIdentity_SkillIcon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_WRIdentity_SkillIcon::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    m_pCoolTimeWnd->Set_Dead(true);
}
