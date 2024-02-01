#include "stdafx.h"
#include "UI_HoldingFrame.h"
#include "GameInstance.h"
#include "State_Skill.h"
#include "Player_Skill.h"
#include "TextBox.h"
#include "Player.h"

CUI_HoldingFrame::CUI_HoldingFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_HoldingFrame::CUI_HoldingFrame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_HoldingFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_HoldingFrame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pSkill = static_cast<HOLDING_SKILL_DESC*>(pArg)->pSkill;
        m_strSkillName = static_cast<HOLDING_SKILL_DESC*>(pArg)->strSkillName;

        m_fSkillTimeAcc = 0.f;
        m_fSkillEndTime = static_cast<HOLDING_SKILL_DESC*>(pArg)->fSkillEndTime;
        m_fSkillSuccessTime_Max = static_cast<HOLDING_SKILL_DESC*>(pArg)->fSkillSuccessTime_Max;
        m_fSkillSuccessTime_Min = static_cast<HOLDING_SKILL_DESC*>(pArg)->fSkillSuccessTime_Min;
    }
    m_strUITag = TEXT("Holding_Frame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f + 150.f;
    m_fSizeX = 352.f * 0.8f;
    m_fSizeY = 22.f * 0.8f;
  
    //m_pTransformCom
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_Empty
    m_pTransform_Empty->Set_Scale(Vec3(382.f * 0.8f, 37.f * 0.8f, 1.f));
    m_pTransform_Empty->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_Frame
    m_pTransform_Frame->Set_Scale(Vec3(409.f * 0.8f, 43.f * 0.8f, 1.f));
    m_pTransform_Frame->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GaugeCut
    m_pTransform_GaugeCut->Set_Scale(Vec3(66.f * 0.8f, 43.f * 0.8f, 1.f));
    m_pTransform_GaugeCut->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - m_fSizeX) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_HoldingEnd
    m_fSizeX_HoldEnd = 375.f * 0.8f;
    m_fSizeY_HoldEnd = 43.f * 0.8f;
    m_pTransform_HoldingEnd->Set_Scale(Vec3(m_fSizeX_HoldEnd, m_fSizeY_HoldEnd, 1.f));
    m_pTransform_HoldingEnd->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    if (0.f < m_fSkillSuccessTime_Min)
    {
        //m_pTransform_PerfectZone
        _float  fX_SkillMax = m_fSkillSuccessTime_Max / m_fSkillEndTime;
        _float  fX_SkillMin = m_fSkillSuccessTime_Min / m_fSkillEndTime;
        m_fSizeX_PerfectZone = m_fSizeX * (fX_SkillMax - fX_SkillMin);
        m_fX_PerfectZone = (m_fX - (m_fSizeX * 0.5f)) + (m_fSizeX * fX_SkillMin) + (m_fSizeX_PerfectZone * 0.5f);
        m_pTransform_PerfectZone->Set_Scale(Vec3(m_fSizeX_PerfectZone, 80.f * 0.8f, 1.f));
        m_pTransform_PerfectZone->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX_PerfectZone - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    }
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    if (FAILED(Initialize_TextBox()))
        return E_FAIL;

    m_bActive = false;

    return S_OK;
}

HRESULT CUI_HoldingFrame::Initialize_TextBox()
{
    m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
    if (FAILED(Ready_TextBox()))
        return E_FAIL;
    Print_SkillName();
    m_pSkillNameWnd->Set_Active(false);
    return S_OK;
}

void CUI_HoldingFrame::Tick(_float fTimeDelta)
{

    Update_CurrGauge();
   __super::Tick(fTimeDelta);
}

void CUI_HoldingFrame::LateTick(_float fTimeDelta)
{

        Update_GaugeCut();
        Update_HoldingEnd(fTimeDelta);
        __super::LateTick(fTimeDelta);

}

HRESULT CUI_HoldingFrame::Render()
{
    if (FAILED(Bind_ShaderResources_Empty()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Gauge()))
        return E_FAIL;
    m_pShaderCom->Begin(16);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Frame()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GaugeCut()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_PerfectZone()))
    return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HoldingEnd()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    m_pSkillNameWnd->Render();

    return S_OK;
}

void CUI_HoldingFrame::Set_SkillOn(_bool bSkillOn)
{
    if (bSkillOn)
    {
        m_fAlpha = 1.f;
        m_fSizeX_HoldEnd = 375.f * 0.8f;
        m_fSizeY_HoldEnd = 43.f * 0.8f;
        m_pTransform_HoldingEnd->Set_Scale(Vec3(m_fSizeX_HoldEnd, m_fSizeY_HoldEnd, 0.f));
        m_pSkillNameWnd->Set_Alpha(m_fAlpha);
        m_pSkillNameWnd->Set_Active(true);
        Print_SkillName();
        m_bRender = true;
        m_bIsSkillEnd = false;
        Set_Active(true);
    }
    else
    {
        m_bIsSkillEnd = true;
    }
}

void CUI_HoldingFrame::Update_CurrGauge()
{
    m_fHoldRatio = m_fSkillTimeAcc / m_fSkillEndTime;
    Update_GaugeCut();
}

void CUI_HoldingFrame::Update_GaugeCut()
{
    m_pTransform_GaugeCut->Set_Scale(Vec3(66.f * 0.8f, 43.f * 0.8f, 1.f));
    m_pTransform_GaugeCut->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - m_fSizeX* 0.5f) + (m_fSizeX * m_fHoldRatio)) - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.f));
}

void CUI_HoldingFrame::Update_HoldingEnd(_float fTimeDelta)
{
    if (!m_bIsSkillEnd)
        return;

    m_fAlpha -= 3.f * fTimeDelta;
    m_pSkillNameWnd->Set_Alpha(m_fAlpha);
    m_pTransform_HoldingEnd->Set_Scale(Vec3(m_fSizeX_HoldEnd += (200.f * fTimeDelta), m_fSizeY_HoldEnd += (200.f * fTimeDelta), 0.f));
   
    if (0.f >= m_fAlpha)
    {
        m_fAlpha = 1.f;
        m_fSkillTimeAcc = 0.f;
        m_fHoldRatio = 0.f;
        m_fSizeX_HoldEnd = 375.f * 0.8f;
        m_fSizeY_HoldEnd = 43.f * 0.8f;
        m_pTransform_HoldingEnd->Set_Scale(Vec3(m_fSizeX_HoldEnd, m_fSizeY_HoldEnd, 0.f));
        m_bRender = false;
        m_pSkillNameWnd->Set_Alpha(m_fAlpha);
        m_pSkillNameWnd->Set_Active(false);
        m_bIsSkillEnd = false;
        Set_Active(false);
    }
}

void CUI_HoldingFrame::Print_SkillName()
{
    m_pSkillNameWnd->Clear_Text();
    m_pSkillNameWnd->Set_Alpha(1.f);

    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strSkillName);
    Vec2 vOrigin = vMeasure * 0.5f;


    m_pSkillNameWnd->Set_Text(m_strWndTag + TEXT("-1"), m_strFont, m_strSkillName, Vec2((382.f * 0.8f) * 0.5f + 1.f, (37.f * 0.8f) * 0.5f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
    m_pSkillNameWnd->Set_Text(m_strWndTag + TEXT("-2"), m_strFont, m_strSkillName, Vec2((382.f * 0.8f) * 0.5f - 1.f, (37.f * 0.8f) * 0.5f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
    m_pSkillNameWnd->Set_Text(m_strWndTag + TEXT("-3"), m_strFont, m_strSkillName, Vec2((382.f * 0.8f) * 0.5f, (37.f * 0.8f) * 0.5f + 1.f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
    m_pSkillNameWnd->Set_Text(m_strWndTag + TEXT("-4"), m_strFont, m_strSkillName, Vec2((382.f * 0.8f) * 0.5f, (37.f * 0.8f) * 0.5f - 1.f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
    m_pSkillNameWnd->Set_Text(m_strWndTag, m_strFont, m_strSkillName, Vec2((382.f * 0.8f) * 0.5f, (37.f * 0.8f) * 0.5f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
}


HRESULT CUI_HoldingFrame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Fill"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Frame"),
        TEXT("Com_Texture_Frame"), (CComponent**)&m_pTexture_Frame)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Empty"),
        TEXT("Com_Texture_Empty"), (CComponent**)&m_pTextureCom_Empty)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Shine"),
        TEXT("Com_Texture_GaugeCut"), (CComponent**)&m_pTexture_GaugeCut)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Perfect_Zone"),
        TEXT("Com_Texture_PerfectZone"), (CComponent**)&m_pTexture_PerfectZone)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_End"),
        TEXT("Com_Texture_HoldingEnd"), (CComponent**)&m_pTexture_HoldingEnd)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_None"),
        TEXT("Com_Texture_None"), (CComponent**)&m_pTexture_None)))
        return E_FAIL;


    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Empty"), (CComponent**)&m_pTransform_Empty)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Frame"), (CComponent**)&m_pTransform_Frame)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GaugeCut"), (CComponent**)&m_pTransform_GaugeCut)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_PerfectZone"), (CComponent**)&m_pTransform_PerfectZone)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_HoldineEnd"), (CComponent**)&m_pTransform_HoldingEnd)))
        return E_FAIL;
 
    return S_OK;
}

HRESULT CUI_HoldingFrame::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);

    if (nullptr == m_pSkillNameWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = m_strSkillName + TEXT("_HoldingUI");
        m_strWndTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(382.f * 0.8f, 37.f * 0.8f);
        m_pSkillNameWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pSkillNameWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pSkillNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pSkillNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);

        m_pSkillNameWnd->Get_TransformCom()->Set_Scale(Vec3(382.f * 0.8f, 37.f * 0.8f, 0.f));
        m_pSkillNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

        m_pSkillNameWnd->Set_Render(false);
    }
    Safe_Release(pGameInstance);
    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources_Gauge()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fHoldRatio, sizeof(_float))))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources_Empty()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Empty->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    m_pTextureCom_Empty->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources_Frame()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Frame->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    m_pTexture_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources_GaugeCut()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GaugeCut->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if((0 < m_fHoldRatio)&&(0.99f > m_fHoldRatio))
        m_pTexture_GaugeCut->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources_PerfectZone()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_PerfectZone->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (0.f < m_fSkillSuccessTime_Min)
        m_pTexture_PerfectZone->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_HoldingFrame::Bind_ShaderResources_HoldingEnd()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_HoldingEnd->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    
    if(m_bIsSkillEnd)
        m_pTexture_HoldingEnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

CUI_HoldingFrame* CUI_HoldingFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_HoldingFrame* pInstance = new CUI_HoldingFrame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_HoldingFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_HoldingFrame::Clone(void* pArg)
{
    CUI_HoldingFrame* pInstance = new CUI_HoldingFrame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_HoldingFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_HoldingFrame::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    if (nullptr != m_pSkillNameWnd)
        m_pSkillNameWnd->Set_Dead(true);

    Safe_Release(m_pTextureCom_Empty);
    Safe_Release(m_pTexture_Frame);
    Safe_Release(m_pTexture_GaugeCut);
    Safe_Release(m_pTexture_PerfectZone);
    Safe_Release(m_pTexture_HoldingEnd);
    Safe_Release(m_pTexture_None);

    Safe_Release(m_pTransform_Empty);
    Safe_Release(m_pTransform_Frame);
    Safe_Release(m_pTransform_GaugeCut);
    Safe_Release(m_pTransform_PerfectZone);
    Safe_Release(m_pTransform_HoldingEnd);
}
