#include "stdafx.h"
#include "UI_NPC_ChaosDungeon_NewWnd.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Player_Gunslinger.h"
#include "Player_Slayer.h"
#include "Player_Destroyer.h"
#include "Player_Bard.h"
#include "Player_Doaga.h"
#include "Player_Controller_GN.h"
#include "Controller_MG.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"
#include "Controller_SP.h"
#include "ServerSessionManager.h"

CUI_NPC_ChaosDungeon_NewWnd::CUI_NPC_ChaosDungeon_NewWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_NPC_ChaosDungeon_NewWnd::CUI_NPC_ChaosDungeon_NewWnd(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("NPC_UI_ChaosDungeon_NewWnd");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 400.f;
    m_fSizeX = 487.f;
    m_fSizeY = 224.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.1f));

    m_pTransform_AcceptButton->Set_Scale(Vec3(101.f, 34.f, 1.f));
    m_pTransform_AcceptButton->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 55.5f) - g_iWinSizeX * 0.5f, -(m_fY + 80.f) + g_iWinSizeY * 0.5f, 0.1f));
    Create_Rect_AcceptButton();
  
    m_pTransform_RefuseButton->Set_Scale(Vec3(101.f, 34.f, 1.f));
    m_pTransform_RefuseButton->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 55.5f) - g_iWinSizeX * 0.5f, -(m_fY + 80.f) + g_iWinSizeY * 0.5f, 0.1f));
    Create_Rect_RefuseButton();

    m_pTransform_Timer->Set_Scale(Vec3(294.f, 12.f, 1.f));
    m_pTransform_Timer->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.1f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    if (FAILED(Initialize_TextBox()))
        return E_FAIL;

    m_pTextBoxWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.f));
    m_pTimeCountWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.f));

    m_bClicked_Entrance = false;
            
    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Initialize_TextBox()
{
    m_strText = TEXT("ƒ´ø¿Ω∫ ¥¯¿¸ø° ¿‘¿Â«œΩ√∞⁄Ω¿¥œ±Ó?");
    m_strTimeCount = TEXT("10√  ≥≤æ“Ω¿¥œ¥Ÿ.");
    Ready_TextBox();
    Set_Active(false);

    return S_OK;
}

void CUI_NPC_ChaosDungeon_NewWnd::Tick(_float fTimeDelta)
{
    if (true == m_bDeActive)
    {
        m_fDeActiveAcc += fTimeDelta;
        if (m_fDeActiveAcc >= 0.5f)
        {
            m_bDeActive = false;
            Reset_Player_Control();
            Set_Active(false);
        }
    }

    if (true == m_bActive && false == m_bDeActive)
    {
        __super::Tick(fTimeDelta);

        if (0 < m_fCurrTimer)
        {
            m_fCurrTimer -= fTimeDelta;
            Set_Player_Control();
        }
        else if (0 > m_fCurrTimer)
        {
            m_fCurrTimer = 0.f;
            m_bDeActive = true;
            m_fDeActiveAcc = 0.0f;
            m_pTextBoxWnd->Set_Active(false);
            m_pTimeCountWnd->Set_Active(false);
        }
    }
}

void CUI_NPC_ChaosDungeon_NewWnd::LateTick(_float fTimeDelta)
{
    if (false == m_bDeActive)
    {
        if (true == m_IsClicked)
        {
            m_IsClicked = false;
            m_bDeActive = true;
            m_fDeActiveAcc = 0.0f;
            m_pTextBoxWnd->Set_Active(false);
            m_pTimeCountWnd->Set_Active(false);
   
            return;
        }
        Update_Button();

        __super::LateTick(fTimeDelta);

        m_fTimerRatio = m_fCurrTimer / m_fMaxTimer;
    }
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_AcceptButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_RefuseButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Timer()))
        return E_FAIL;
    m_pTexture_TimerEmpty->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Timer()))
        return E_FAIL;
    m_pTexture_TimerGauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(16);
    m_pVIBufferCom->Render();

    Print_Text();

    return S_OK;
}

void CUI_NPC_ChaosDungeon_NewWnd::Start_Text()
{
    m_strTimeCount.clear();
    m_strText.clear();
    m_pTextBoxWnd->Set_Active(true);
    m_pTimeCountWnd->Set_Active(true);
}

void CUI_NPC_ChaosDungeon_NewWnd::End_Text()
{
    m_strTimeCount.clear();
    m_strText.clear();
    m_pTextBoxWnd->Set_Active(false);
    m_pTimeCountWnd->Set_Active(false);
}

void CUI_NPC_ChaosDungeon_NewWnd::Print_Text()
{
    if (nullptr != m_pTextBoxWnd)
    {
        m_pTextBoxWnd->Clear_Text();
        m_pTextBoxWnd->Set_Alpha(1.f);
        m_pTextBoxWnd->Get_TransformCom()->Set_Scale(Vec3(487.f, 224.0f, 0.f));
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("≥ÿΩºLv1∞ÌµÒBold"), m_strText);
        Vec2 vOrigin = vMeasure * 0.5f;
        m_pTextBoxWnd->Set_Text(m_strTag, TEXT("≥ÿΩºLv1∞ÌµÒBold"), m_strText, Vec2(243.5f, 72.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
    }

    if (nullptr != m_pTimeCountWnd)
    {
        m_pTimeCountWnd->Clear_Text();
        m_pTimeCountWnd->Set_Alpha(1.f);
        m_pTimeCountWnd->Get_TransformCom()->Set_Scale(Vec3(487.f, 224.0f, 0.f));
        m_strTimeCount = to_wstring((_uint)m_fCurrTimer) + TEXT("√ ") + TEXT(" ≥≤æ“Ω¿¥œ¥Ÿ.");
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("≥ÿΩºLv1∞ÌµÒBold"), m_strTimeCount);
        Vec2 vOrigin = vMeasure * 0.5f;
        m_pTimeCountWnd->Set_Text(m_strTag_Timer, TEXT("≥ÿΩºLv1∞ÌµÒBold"), m_strTimeCount, Vec2(243.5f, 140.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
    }
}

void CUI_NPC_ChaosDungeon_NewWnd::Set_Player_Control()
{
    CPlayer* pPlayer = static_cast<CPlayer*>(CGameInstance::GetInstance()->Find_CtrlPlayer(LEVEL_BERN, (_uint)LAYER_TYPE::LAYER_PLAYER));
    if (nullptr == pPlayer)
        return;

    if (m_bPick)
    {
        if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Mouse_Active(false);
        }
        else if (TEXT("WR") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Mouse_Active(false);
        }
        else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Mouse_Active(false);
        }
        else if (TEXT("MG") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Bard*>(pPlayer)->Get_MG_Controller()->Set_Mouse_Active(false);
        }
        else if (TEXT("SP") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Mouse_Active(false);
        }
    }
    else if((!m_bPick))
    {
        if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Mouse_Active(true);
        }
        else if (TEXT("WR") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Mouse_Active(true);
        }
        else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Mouse_Active(true);
        }
        else if (TEXT("MG") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Bard*>(pPlayer)->Get_MG_Controller()->Set_Mouse_Active(true);
        }
        else if (TEXT("SP") == pPlayer->Get_ObjectTag())
        {
            static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Mouse_Active(true);
        }
    }
}



void CUI_NPC_ChaosDungeon_NewWnd::Set_Active(_bool bActive)
{
    if(false == bActive)
        m_fCurrTimer = m_fMaxTimer;

    m_bActive = bActive;
    m_pTextBoxWnd->Set_Active(bActive);
    m_pTimeCountWnd->Set_Active(bActive);
}

_bool CUI_NPC_ChaosDungeon_NewWnd::Is_Entered()
{
    return m_bClicked_Entrance; 
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);
    if (nullptr == m_pTextBoxWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("ChaosDungeon_Entrance_NewWnd");
        m_strTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(487.f, 224.0f);
        m_pTextBoxWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pTextBoxWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pTextBoxWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pTextBoxWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
    }

    if (nullptr == m_pTimeCountWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("ChaosDungeon_Entrance_TimeCounter");
        m_strTag_Timer = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(487.f, 224.0f);
        m_pTimeCountWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pTimeCountWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pTimeCountWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pTimeCountWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
    }
    Safe_Release(pGameInstance);
    return S_OK;
}

void CUI_NPC_ChaosDungeon_NewWnd::Update_MovingWnd(POINT pt)
{
   if (true == Is_Picking_MovingWnd(pt))
   {
       if (KEY_HOLD(KEY::LBTN))
           m_bHolding = true;
   }

   if (m_bHolding)
   {
       _float	MouseMoveX, MouseMoveY;
       MouseMoveX = CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);

       m_fX += MouseMoveX * 0.8f;
       m_fY = pt.y + 94.f;

       m_pTransformCom->Set_State(CTransform::STATE_POSITION,
           Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.1f));
       m_pTransform_AcceptButton->Set_State(CTransform::STATE_POSITION,
           Vec3((m_fX - 55.5f) - g_iWinSizeX * 0.5f, -(m_fY + 80.f) + g_iWinSizeY * 0.5f, 0.1f));
       m_pTransform_RefuseButton->Set_State(CTransform::STATE_POSITION,
           Vec3((m_fX + 55.5f) - g_iWinSizeX * 0.5f, -(m_fY + 80.f) + g_iWinSizeY * 0.5f, 0.1f));
       m_pTransform_Timer->Set_State(CTransform::STATE_POSITION,
           Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.1f));
       m_pTextBoxWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
           Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.f));
       m_pTimeCountWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
           Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.f));

       if (KEY_AWAY(KEY::LBTN))
           m_bHolding = false;
   }
}

void CUI_NPC_ChaosDungeon_NewWnd::Update_Button()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);  // ≈¨∂Û¿Ãæ∆Æ ≥ªø° ∏∂øÏΩ∫ ∆˜¿Œ≈Õ ∞°¡Æø¿±‚ 
    _uint ViewPortIndex = 1;
    CD3D11_VIEWPORT ViewPort;
    m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // ∫‰∆˜∆Æ ∞°¡Æø¿±‚ 

    Create_Rect_AcceptButton();
    Create_Rect_RefuseButton();
    Create_Rect_MovingWnd();

    Update_AcceptButton(pt);
    Update_RefuseButton(pt);
    Update_MovingWnd(pt);
}

void CUI_NPC_ChaosDungeon_NewWnd::Update_AcceptButton(POINT pt)
{
    if (true == Is_Picking_AcceptButton(pt))
    {
        if (KEY_TAP(KEY::LBTN))
        {
            m_bClicked_Entrance = true;
            m_IsClicked = true;
        }
        m_iTextureIndex_AcceptButton = 1;
    }
    else
        m_iTextureIndex_AcceptButton = 0;
}

void CUI_NPC_ChaosDungeon_NewWnd::Update_RefuseButton(POINT pt)
{
    if (true == Is_Picking_RefuseButton(pt))
    {
        if (KEY_TAP(KEY::LBTN))
        {
            m_bClicked_Entrance = false;
            m_IsClicked = true;
        }
        m_iTextureIndex_RefuseButton = 1;
    }
    else
        m_iTextureIndex_RefuseButton = 0;
}

void CUI_NPC_ChaosDungeon_NewWnd::Create_Rect_AcceptButton()
{
    m_rcAcceptButton.left = LONG((m_fX - 55.5f) - (101.f / 2));
    m_rcAcceptButton.top = LONG((m_fY + 80.f) - (34.f / 2));
    m_rcAcceptButton.right = LONG((m_fX - 55.5f) + (101.f / 2));
    m_rcAcceptButton.bottom = LONG((m_fY + 80.f) + (34.f / 2));

}

void CUI_NPC_ChaosDungeon_NewWnd::Create_Rect_RefuseButton()
{
    m_rcRefuseButton.left = LONG((m_fX + 55.5f) - (101.f / 2));
    m_rcRefuseButton.top = LONG((m_fY + 80.f) - (34.f / 2));
    m_rcRefuseButton.right = LONG((m_fX + 55.5f) + (101.f / 2));
    m_rcRefuseButton.bottom = LONG((m_fY + 80.f) + (34.f / 2));
}

_bool CUI_NPC_ChaosDungeon_NewWnd::Is_Picking_AcceptButton(POINT pt)
{
    if (PtInRect(&m_rcAcceptButton, pt))
        return true;//m_bPicking_AcceptButton = true;
    else
        return false;//m_bPicking_AcceptButton = false;
}

_bool CUI_NPC_ChaosDungeon_NewWnd::Is_Picking_RefuseButton(POINT pt)
{
    if (PtInRect(&m_rcRefuseButton, pt))
        return true;//m_bPicking_RefuseButton = true;
    else
        return false;//m_bPicking_RefuseButton = false;
}

void CUI_NPC_ChaosDungeon_NewWnd::Create_Rect_MovingWnd()
{
    m_rcMovingWnd.left = LONG(m_fX - (101.f / 2));
    m_rcMovingWnd.top = LONG((m_fY - 94.f ) - (36.f / 2));
    m_rcMovingWnd.right = LONG(m_fX + (101.f / 2));
    m_rcMovingWnd.bottom = LONG((m_fY - 94.f) + (36.f / 2));
}

_bool CUI_NPC_ChaosDungeon_NewWnd::Is_Picking_MovingWnd(POINT pt)
{
    if (PtInRect(&m_rcMovingWnd, pt))
        return true;
    else
        return false;
}

void CUI_NPC_ChaosDungeon_NewWnd::Reset_Player_Control()
{
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    if (nullptr == pPlayer)
        return;

    if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
    {
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Control_Active(true);
    }
    else if (TEXT("WR") == pPlayer->Get_ObjectTag())
    {
        static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Control_Active(true);
    }
    else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
    {
        static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Control_Active(true);
    }
    else if (TEXT("MG") == pPlayer->Get_ObjectTag())
    {
        static_cast<CPlayer_Bard*>(pPlayer)->Get_MG_Controller()->Set_Control_Active(true);
    }
    else if (TEXT("SP") == pPlayer->Get_ObjectTag())
    {
        static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Control_Active(true);
    }

}


HRESULT CUI_NPC_ChaosDungeon_NewWnd::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_NewWnd_Text"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Accept_Button"),
        TEXT("Com_Texture_AcceptButton"), (CComponent**)&m_pTexture_AcceptButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Refuse_Button"),
        TEXT("Com_Texture_RefuseButton"), (CComponent**)&m_pTexture_RefuseButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Timer_Empty"),
        TEXT("Com_Texture_TimerEmpty"), (CComponent**)&m_pTexture_TimerEmpty)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Timer"),
        TEXT("Com_Texture_Timer"), (CComponent**)&m_pTexture_TimerGauge)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_AcceptButton"), (CComponent**)&m_pTransform_AcceptButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_RefuseButton"), (CComponent**)&m_pTransform_RefuseButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Timer"), (CComponent**)&m_pTransform_Timer)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Bind_ShaderResources()
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

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Bind_ShaderResources_AcceptButton()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_AcceptButton->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    m_pTexture_AcceptButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_AcceptButton);

    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Bind_ShaderResources_RefuseButton()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_RefuseButton->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    m_pTexture_RefuseButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_RefuseButton);

    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon_NewWnd::Bind_ShaderResources_Timer()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Timer->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fTimerRatio, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CUI_NPC_ChaosDungeon_NewWnd* CUI_NPC_ChaosDungeon_NewWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_NPC_ChaosDungeon_NewWnd* pInstance = new CUI_NPC_ChaosDungeon_NewWnd(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_NPC_ChaosDungeon_NewWnd");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_NPC_ChaosDungeon_NewWnd::Clone(void* pArg)
{
    CUI_NPC_ChaosDungeon_NewWnd* pInstance = new CUI_NPC_ChaosDungeon_NewWnd(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_NPC_ChaosDungeon_NewWnd");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_NPC_ChaosDungeon_NewWnd::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pTextBoxWnd->Set_Dead(true);
    m_pTimeCountWnd->Set_Dead(true);

    Safe_Release(m_pTexture_AcceptButton);
    Safe_Release(m_pTexture_RefuseButton);
    Safe_Release(m_pTexture_TimerEmpty);
    Safe_Release(m_pTexture_TimerGauge);
    Safe_Release(m_pTransform_AcceptButton);
    Safe_Release(m_pTransform_RefuseButton);
    Safe_Release(m_pTransform_Timer);
}
