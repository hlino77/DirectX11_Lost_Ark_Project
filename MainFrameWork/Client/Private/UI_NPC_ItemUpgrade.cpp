#include "stdafx.h"
#include "UI_NPC_ItemUpgrade.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Player.h"
#include "Item.h"
#include "ServerSessionManager.h"
#include "Camera_Player.h"
#include "UI_Manager.h"
#include "Sound_Manager.h"

CUI_NPC_ItemUpgrade::CUI_NPC_ItemUpgrade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_NPC_ItemUpgrade::CUI_NPC_ItemUpgrade(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("NPC_UI_ItemUpgrade");

    if (FAILED(Initialize_Transform_BaseWnd()))
        return E_FAIL;

    if (FAILED(Initialize_Transform_SidePannel_L()))
        return E_FAIL;

    if (FAILED(Initialize_Transform_SidePannel_R()))
        return E_FAIL;

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    for (size_t i = 0; i < SELECTED_END; i++)
    {
        m_pEquips[i] = { nullptr };
    }

    if (FAILED(Initialize_TextBox()))
        return E_FAIL;
    Set_Active_UpGrade(true);

    m_bActive = false;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_Transform_BaseWnd()
{
    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 664.f;
    m_fSizeY = 728.f;

    //m_pTransformCom
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX * 0.8f, m_fSizeY * 0.8f, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_HammerCap
    m_pTransform_HammerCap->Set_Scale(Vec3(423.f * 0.8f, 207.f * 0.8f, 1.f));
    m_pTransform_HammerCap->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 30.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_HammerEffect
    m_pTransform_HammerEffect->Set_Scale(Vec3(556.f * 0.8f, 374.f * 0.8f, 1.f));
    m_pTransform_HammerEffect->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 10.f) - g_iWinSizeX * 0.5f, -((m_fY - 88.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_HammerAura
    m_pTransform_HammerAura->Set_Scale(Vec3(669.f * 0.8f, 417.f * 0.8f, 1.f));
    m_pTransform_HammerAura->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 72.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_Gauge
    m_pTransform_Gauge->Set_Scale(Vec3(110.f * 0.8f, 110.f * 0.8f, 1.f));
    m_pTransform_Gauge->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_GaugeEffect
    m_pTransform_GaugeEffect->Set_Scale(Vec3(200.f * 0.8f, 200.f * 0.8f, 1.f));
    m_pTransform_GaugeEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 45.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_GaugeSmoke
    m_pTransform_GaugeSmoke->Set_Scale(Vec3(152.f * 0.8f, 152.f * 0.8f, 1.f));
    m_pTransform_GaugeSmoke->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_GrowthButton
    m_pTransform_GrowthButton->Set_Scale(Vec3(108.f * 0.8f, 44.f * 0.8f, 1.f));
    m_pTransform_GrowthButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 30.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_ItemName
    m_pTransform_ItemName->Set_Scale(Vec3(566.f * 0.8f, 98.f * 0.8f, 1.f));
    m_pTransform_ItemName->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 4.f) - g_iWinSizeX * 0.5f, -(m_fY - 140.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_NextLevelIcon
    m_pTransform_NextLevelIcon->Set_Scale(Vec3(46.f * 0.8f, 34.f * 0.8f, 1.f));
    m_pTransform_NextLevelIcon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX) - g_iWinSizeX * 0.5f, -(m_fY - 140.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_Ingredients
    m_pTransform_Ingredients->Set_Scale(Vec3(478.f * 0.8f, 189.f * 0.8f, 1.f));
    m_pTransform_Ingredients->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 4.f) - g_iWinSizeX * 0.5f, -((m_fY + 160.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_UpgradeButton
    m_pTransform_UpgradeButton->Set_Scale(Vec3(221.f * 0.8f, 42.f * 0.8f, 1.f));
    m_pTransform_UpgradeButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 264.f)) +g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_ItemSlot
    m_pTransform_ItemSlot->Set_Scale(Vec3(64.f * 0.8f, 64.f * 0.8f, 1.f));
    m_pTransform_ItemSlot->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 220.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_Item
    m_pTransform_Item->Set_Scale(Vec3(64.f * 0.7f, 64.f * 0.7f, 1.f));
    m_pTransform_Item->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 220.f) +g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_IngredientSlotL
    m_pTransform_IngredientSlotL->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlotL->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - 100.f)) - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_IngredientSlot
    m_pTransform_IngredientSlot->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlot->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_IngredientSlotR
    m_pTransform_IngredientSlotR->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlotR->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 100.f)) - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.03f));
    for (size_t i = 0; i < 3; i++)
    {
        m_pTransform_Ingredient[i]->Set_Scale(Vec3(64.f * 0.7f, 64.f * 0.7f, 1.f));
        m_pTransform_Ingredient[i]->Set_State(CTransform::STATE_POSITION,
            Vec3(((m_fX - 100.f) + 100.f * i) - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.03f));

        m_pTransform_IngredientWnd[i]->Set_Scale(Vec3(64.f * 0.8f, 64.f * 0.8f, 1.f));
        m_pTransform_IngredientWnd[i]->Set_State(CTransform::STATE_POSITION,
            Vec3(((m_fX - 100.f) + 100.f * i) - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.03f));
    }
    //m_pTransform_SidePannel
    m_pTransform_SidePannel->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
    m_pTransform_SidePannel->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - 366.f)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_SideWnd
    m_pTransform_SideWnd->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
    m_pTransform_SideWnd->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 366.f)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_NpcSpeech_BackGround
    m_pTransform_NpcSpeech_BackGround->Set_Scale(Vec3(1620.f, 200.f, 1.f));
    m_pTransform_NpcSpeech_BackGround->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY - 80.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_NpcSpeech_Wnd
    m_pTransform_NpcSpeech_Wnd->Set_Scale(Vec3(g_iWinSizeX * 0.5f, 80.f * 0.8f, 1.f));
    m_pTransform_NpcSpeech_Wnd->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 10.f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY - 80.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_QuitButton
    m_pTransform_QuitButton->Set_Scale(Vec3(205.f * 0.8f, 48.f * 0.8f, 1.f));
    m_pTransform_QuitButton->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 700.f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY - 50.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_Result
    m_pTransform_Result->Set_Scale(Vec3(1092.f, 581.f, 1.f));
    m_pTransform_Result->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_MaxGaugeEffect
    m_pTransform_MaxGaugeEffect->Set_Scale(Vec3(200.f * 0.8f, 200.f * 0.8f, 1.f));
    m_pTransform_MaxGaugeEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_ResultEffect
    m_pTransform_ResultEffect->Set_Scale(Vec3(500.f, 500.f, 1.f));
    m_pTransform_ResultEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ResultWnd
    m_pTransform_ResultWnd->Set_Scale(Vec3(1092.f, 581.f, 1.f));
    m_pTransform_ResultWnd->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.01f));
    //m_pTransform_ResultTextEffect
    m_pTransform_ResultTextEffect->Set_Scale(Vec3(204.f * 1.5f, 84.f * 1.5f, 1.f));
    m_pTransform_ResultTextEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.01f));
    //m_pTransform_ResultItemEffect
    m_pTransform_ResultItemEffect->Set_Scale(Vec3(64.f * 5.f, 60.f * 5.f, 1.f));
    m_pTransform_ResultItemEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 98.f) + g_iWinSizeY * 0.5f, 0.01f));
    //m_pTransform_ResultItemSlot
    m_pTransform_ResultItemSlot->Set_Scale(Vec3(64.f * 0.8f, 64.f * 0.8f, 1.f));
    m_pTransform_ResultItemSlot->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 2.f) - g_iWinSizeX * 0.5f, -(m_fY - 98.f) + g_iWinSizeY * 0.5f, 0.01f));
    //m_pTransform_ResultItem
    m_pTransform_ResultItem->Set_Scale(Vec3(64.f * 0.7f, 64.f * 0.7f, 1.f));
    m_pTransform_ResultItem->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 2.f) - g_iWinSizeX * 0.5f, -(m_fY - 98.f) + g_iWinSizeY * 0.5f, 0.01f));
    //m_pTransform_ResultCheckButton
    m_pTransform_ResultCheckButton->Set_Scale(Vec3(221.f * 0.8f, 42.f * 0.8f, 1.f));
    m_pTransform_ResultCheckButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 264.f)) + g_iWinSizeY * 0.5f, 0.01f));

    m_fDeActiveAcc = 0.0f;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_Transform_SidePannel_L()
{
    //m_pTransform_EquipItemWnd_Face
    m_pTransform_EquipItemWnd_Face->Set_Scale(Vec3(323.f*0.9f , 60.f, 1.f));
    m_pTransform_EquipItemWnd_Face->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 200.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_ItemIcon_Face
    m_pTransform_ItemIcon_Face->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Face->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY - 200.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipIcon_Face
    m_pTransform_EquipIcon_Face->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Face->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 215.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_UpgradeIcon_Face
    m_pTransform_UpgradeIcon_Face->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Face->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 185.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_EquipItemWnd_Helemt
    m_pTransform_EquipItemWnd_Helemt->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Helemt->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 135.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_ItemIcon_Helemt
    m_pTransform_ItemIcon_Helemt->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Helemt->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY - 135.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_UpgradeIcon_Helemt
    m_pTransform_UpgradeIcon_Helemt->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Helemt->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 120.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_EquipIcon_Helemt
    m_pTransform_EquipIcon_Helemt->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Helemt->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 150.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_EquipItemWnd_Shoulder
    m_pTransform_EquipItemWnd_Shoulder->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Shoulder->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 75.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_ItemIcon_Shoulder
    m_pTransform_ItemIcon_Shoulder->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Shoulder->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY - 75.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_UpgradeIcon_Shoulder
    m_pTransform_UpgradeIcon_Shoulder->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Shoulder->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 60.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_EquipIcon_Shoulder
    m_pTransform_EquipIcon_Shoulder->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Shoulder->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 90.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_EquipItemWnd_Body
    m_pTransform_EquipItemWnd_Body->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 10.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_ItemIcon_Body
    m_pTransform_ItemIcon_Body->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY - 10.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_UpgradeIcon_Body
    m_pTransform_UpgradeIcon_Body->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 5.f) + g_iWinSizeY * 0.5f, 0.03f));
   //m_pTransform_EquipIcon_Body
    m_pTransform_EquipIcon_Body->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 25.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipItemWnd_Arm
    m_pTransform_EquipItemWnd_Arm->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Arm->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY + 55.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_ItemIcon_Arm
    m_pTransform_ItemIcon_Arm->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Arm->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY + 55.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_UpgradeIcon_Arm
    m_pTransform_UpgradeIcon_Arm->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Arm->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 70) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipIcon_Arm
    m_pTransform_EquipIcon_Arm->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Arm->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 40.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipItemWnd_Leg
    m_pTransform_EquipItemWnd_Leg->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY + 120.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_ItemIcon_Leg
    m_pTransform_ItemIcon_Leg->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY +120.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_UpgradeIcon_Leg
    m_pTransform_UpgradeIcon_Leg->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 135) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipIcon_Leg
    m_pTransform_EquipIcon_Leg->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 105.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipItemWnd_Weapon
    m_pTransform_EquipItemWnd_Weapon->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY + 185.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_ItemIcon_Weapon
    m_pTransform_ItemIcon_Weapon->Set_Scale(Vec3(64.f * 0.9f, 64.f * 0.9f, 1.f));
    m_pTransform_ItemIcon_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 480.f) - g_iWinSizeX * 0.5f, -(m_fY + 185.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_UpgradeIcon_Weapon
    m_pTransform_UpgradeIcon_Weapon->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.9f, 1.f));
    m_pTransform_UpgradeIcon_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 200.f) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_EquipIcon_Weapon
    m_pTransform_EquipIcon_Weapon->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY + 170.f) + g_iWinSizeY * 0.5f, 0.03f));
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_Transform_SidePannel_R()
{
    for (size_t i = 0; i < 7; i++)
    {
        m_pTransform_GradeEffect[i]->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
        m_pTransform_GradeEffect[i]->Set_State(CTransform::STATE_POSITION,
            Vec3(((m_fX + 370.f)) - g_iWinSizeX * 0.5f, -(m_fY - 190.f + (65 * i)) + g_iWinSizeY * 0.5f, 0.03f));
    }

    //m_pTransform_DragLine
    m_pTransform_DragLine->Set_Scale(Vec3(22.f * 0.9f, 199.f * 2.2f, 1.f));
    m_pTransform_DragLine->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 530.f)) - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.03f));
    //m_pTransform_DragBar
    m_pTransform_DragBar->Set_Scale(Vec3(18 * 0.8f, 199.f, 1.f));
    m_pTransform_DragBar->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 530.f)) - g_iWinSizeX * 0.5f, -(m_fY)+g_iWinSizeY * 0.5f, 0.03f));

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_TextBox()
{
    m_strFont = TEXT("´øÆÄ¿¬¸¶µÈÄ®³¯");
    if (FAILED(Ready_TextBox()))
        return E_FAIL;
    m_pCurrItemNameWnd->Set_Active(true);
    m_pItemNameWnd->Set_Active(true);
    m_pCurrGaugeWnd->Set_Active(true);
    m_pResultWnd->Set_Active(false);
    m_pNpcScriptWnd->Set_Active(true);
    Print_FaceItemNameWnd();
    Print_FaceItemGradeLevelWnd();
    Print_HelemetItemNameWnd();
    Print_HelmetItemGradeLevelWnd();
    Print_ShoulderItemNameWnd();
    Print_ShoulderItemGradeLevelWnd();
    Print_BodyItemNameWnd();
    Print_BodyItemGradeLevelWnd();
    Print_ArmItemNameWnd();
    Print_ArmItemGradeLevelWnd();
    Print_LegItemNameWnd();
    Print_LegItemGradeLevelWnd();
    Print_WeaponItemNameWnd();
    Print_WeaponItemGradeLevelWnd();
    Print_CurrGauge();
    Print_Bahuntrr();
    return S_OK;
}

void CUI_NPC_ItemUpgrade::Tick(_float fTimeDelta)
{
    if (nullptr == m_pUsingPlayer)
    {
        m_pUsingPlayer = CServerSessionManager::GetInstance()->Get_Player();
        
    }
        
    __super::Tick(fTimeDelta);

    if (true == m_bDeActive)
    {
        m_fDeActiveAcc += fTimeDelta;
        if (0.5f <= m_fDeActiveAcc)
        {
            CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_FadeInOut(1.f, false);
            CUI* pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(), TEXT("UI_Chat"));
            for (auto& iter : pUI->Get_UIParts())
            {
                iter->Set_Active(true);
            }

            m_fDeActiveAcc = 0.0f;
            m_bDeActive = false;
            Reset_Player_Control();
            Set_Active(false);
        }
    }

    if (m_fItemGrowthMaxGauge <= m_fItemGrowthCurrGauge)
        m_bMaxGauge = true;
    else
        m_bMaxGauge = false;

    m_fItemGrowthRatio = m_fItemGrowthCurrGauge / m_fItemGrowthMaxGauge;
}

void CUI_NPC_ItemUpgrade::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    Update_Button(fTimeDelta);
    Update_Hammer_Effects(fTimeDelta);
    Update_Gague_Smoke(fTimeDelta);
    Update_Gauge_Effect(fTimeDelta);
    Update_Gauge_Spin(fTimeDelta);
    Update_Max_Gauge_Effect(fTimeDelta);
    Update_ResultWaiting(fTimeDelta);
    Update_ResultEffect(fTimeDelta);
}

HRESULT CUI_NPC_ItemUpgrade::Render()
{
    if (FAILED(Bind_ShaderResources_SidePannel_L()))
        return E_FAIL;

    if (FAILED(m_pItemNameWnd->Render()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources_SidePannel_R()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerAura()))
        return E_FAIL;
    if((m_bMaxGauge)&&(!m_bDecreaseEffect))
        m_pShaderCom->Begin(18);
    else
        m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerCap()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerEffect()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Gauge()))
        return E_FAIL;
    m_pShaderCom->Begin(17);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GaugeSpin()))
        return E_FAIL;
    if ((m_bMaxGauge)&&(!m_bDecreaseEffect))
        m_pShaderCom->Begin(18);
    else
        m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GaugeEffect()))
        return E_FAIL;
    if (m_bMaxGauge)
        m_pShaderCom->Begin(3);
    else
        m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_GaugeSmoke()))
        return E_FAIL;
    m_pShaderCom->Begin(3);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GrowthButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ItemName()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_NextLevelIcon()))
        return E_FAIL;
        m_pShaderCom->Begin(0);
        m_pVIBufferCom->Render();

    if (FAILED(m_pCurrItemNameWnd->Render()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources_Ingredients()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_UpgradeButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_UpgradeButtonEffect()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
  
    if (FAILED(Bind_ShaderResources_ItemFrame()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_CurrUpgradeItem_Grade()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_CurrUpgradeItemIcon()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_IngredientWnd_L()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_IngredientWnd()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_IngredientWnd_R()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Ingredient_L()))
        return E_FAIL;
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Ingredient()))
        return E_FAIL;
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Ingredient_R()))
        return E_FAIL;
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_IngredientSlotL()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_IngredientSlot()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_IngredientSlotR()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_MaxGaugeEffect()))
        return E_FAIL;
    if (m_bMaxGaugeEffect)
        m_pShaderCom->Begin(18);
    else
        m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if(FAILED(m_pCurrGaugeWnd->Render()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources_CurrItem()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_NpcSpeech_BackGround()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_NpcSpeech_Wnd()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(m_pNpcScriptWnd->Render()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources_QuitButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultWnd()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultItemEffect()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultTextEffect()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if(FAILED(m_pResultWnd->Render()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources_ResultItemSlot()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultItemGrade()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultItemIcon()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultCheckButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultWaiting()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ResultEffect()))
        return E_FAIL;
    if(m_bResult)
        m_pShaderCom->Begin(18);
    else
        m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_NPC_ItemUpgrade::Get_PlayerInformation()
{
}

void CUI_NPC_ItemUpgrade::Set_Active_UpGrade(_bool  IsUpgrade, CPlayer* pPlayer)
{
    Set_Active(IsUpgrade);
    if ((true == IsUpgrade)&&(nullptr != pPlayer))
    {
        m_pUsingPlayer = static_cast<CPlayer*>(pPlayer);
        if (nullptr == m_pUsingPlayer)
            return;
        Update_Items();

    }
    else if ((true == IsUpgrade)&&(nullptr == pPlayer))
    {
        m_pUsingPlayer = CServerSessionManager::GetInstance()->Get_Player();
        if (nullptr == m_pUsingPlayer)
            return;
        Update_Items();

        m_pCurrItemNameWnd->Set_Active(true);
        m_pItemNameWnd->Set_Active(true);
        m_pCurrGaugeWnd->Set_Active(true);
        m_pResultWnd->Set_Active(false);
        m_pNpcScriptWnd->Set_Active(true);
        Print_FaceItemNameWnd();
        Print_FaceItemGradeLevelWnd();
        Print_HelemetItemNameWnd();
        Print_HelmetItemGradeLevelWnd();
        Print_ShoulderItemNameWnd();
        Print_ShoulderItemGradeLevelWnd();
        Print_BodyItemNameWnd();
        Print_BodyItemGradeLevelWnd();
        Print_ArmItemNameWnd();
        Print_ArmItemGradeLevelWnd();
        Print_LegItemNameWnd();
        Print_LegItemGradeLevelWnd();
        Print_WeaponItemNameWnd();
        Print_WeaponItemGradeLevelWnd();
        Print_CurrGauge();
    }
    CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_FadeInOut(1.f, false);
    CUI* pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(), TEXT("UI_Chat"));
    if (nullptr != pUI)
    {
        for (auto& iter : pUI->Get_UIParts())
        {
            iter->Set_Active(false);
        }
    }
}

void CUI_NPC_ItemUpgrade::Update_Items()
{
    _uint iIndex = 0;
    CItem* pEquips[SELECTED_END];
    for (size_t i = 0; i < SELECTED_END; i++)
    {
        pEquips[i] = nullptr;
        m_pEquips[i] = nullptr;
    }

    for (size_t i = 0; i < (_uint)(CItem::PART::_END); i++)
    {
        pEquips[i] = static_cast<CPlayer*>(m_pUsingPlayer)->Get_EquipItem((_uint)i);
        if (nullptr != pEquips[i])
        {
            m_strItemsName[i] = TEXT("");

            if (i == pEquips[i]->Get_EquipType())
            {
                m_pTexture_ItemIcon[iIndex] = static_cast<CTexture*>(pEquips[i]->Get_ItemTexture());
                m_iSidePannel_L_Wnd_TextureIndex[iIndex] = pEquips[i]->Get_ItemGrade();
                m_strItemsName[iIndex] = pEquips[i]->Get_ItemName();
                m_pEquips[iIndex] = pEquips[i];
                iIndex++;
            }
        }
    }
    Print_FaceItemNameWnd();
    Print_FaceItemGradeLevelWnd();
    Print_HelemetItemNameWnd();
    Print_HelmetItemGradeLevelWnd();
    Print_ShoulderItemNameWnd();
    Print_ShoulderItemGradeLevelWnd();
    Print_BodyItemNameWnd();
    Print_BodyItemGradeLevelWnd();
    Print_ArmItemNameWnd();
    Print_ArmItemGradeLevelWnd();
    Print_LegItemNameWnd();
    Print_LegItemGradeLevelWnd();
    Print_WeaponItemNameWnd();
    Print_WeaponItemGradeLevelWnd();
    Print_CurrItemNameWnd();
    Print_CurrGauge();
    Print_Bahuntrr();
}

void CUI_NPC_ItemUpgrade::Update_Button(_float fTimeDelta)
{
    Create_Rect_GrowthButton();
    Create_Rect_UpgradeButton();
    Create_Rect_QuitButton();

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);  // Å¬¶óÀÌ¾ðÆ® ³»¿¡ ¸¶¿ì½º Æ÷ÀÎÅÍ °¡Á®¿À±â 
    _uint ViewPortIndex = 1;
    CD3D11_VIEWPORT ViewPort;
    m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // ºäÆ÷Æ® °¡Á®¿À±â 

    Upadate_GrowthButton(pt, fTimeDelta);
    Update_UpgradeButton(pt, fTimeDelta);
    Update_QuitButton(pt);
    Update_ResultCheckButton(pt);
    LateTcik_Items(pt);
}

void CUI_NPC_ItemUpgrade::Upadate_GrowthButton(POINT pt, _float fTimeDelta)
{
    if (nullptr != m_pCurrUpgradeItem)
    {
        if (m_fItemGrowthMaxGauge <= m_pCurrUpgradeItem->Get_UpgradeGauge())
        {
            m_bGrowthOn = false;
            m_fItemGrowthCurrGauge = m_pCurrUpgradeItem->Get_UpgradeGauge();
        }
    }
    if ((m_bMaxGauge) || (m_fItemGrowthCurrGauge == m_fItemGrowthMaxGauge))
    {
        m_iGrowthButton_TextureIndex = 2;
        return;
    }
    Update_GrowthGauge(fTimeDelta);
    Is_Picking_GrowthButton(pt, fTimeDelta);
    if ((KEY_TAP(KEY::LBTN)&&(m_bGrowthButton)))
    {
        if (m_bGrowthOn)
            return;
        CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        m_bGrowthOn = true;
        m_bMaxGaugeEffect = true;
        CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Item_GrowthUp.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
    }
}

void CUI_NPC_ItemUpgrade::Update_UpgradeButton(POINT pt, _float fTimeDelta)
{
    if ((!m_bMaxGauge) || (m_fItemGrowthMaxGauge > m_fItemGrowthCurrGauge))
    {
        m_iUpgradeButton_TextureIndex = 2;
        return;
    }
    Is_Picking_UpgradeButton(pt);
    if ((m_bUpgradeButton) && (KEY_TAP(KEY::LBTN)))
    {
        CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        if (nullptr != m_pCurrUpgradeItem)
        {
            m_pCurrUpgradeItem->Set_UpgradeGauge(0.f);
            Print_CurrGauge();
        }
        if (!m_bResultWaiting)
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Item_Upgrade_Wating.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_bResultWaiting = true;
            _uint SuccessPercent = 0;
            SuccessPercent = CGameInstance::GetInstance()->Random_Int(0, 100);
            if (50 < SuccessPercent)
            {
                CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

                m_pCurrUpgradeItem->Disuse_Item(pPlayer, true);
                m_pCurrUpgradeItem->Upgrade_Item();
                m_pCurrUpgradeItem->Use_Item(pPlayer);

                m_pCurrUpgradeItem->Growth_UpgradeLevel();
                Print_FaceItemGradeLevelWnd();
                Print_HelmetItemGradeLevelWnd();
                Print_ShoulderItemGradeLevelWnd();
                Print_BodyItemGradeLevelWnd();
                Print_ArmItemGradeLevelWnd();
                Print_LegItemGradeLevelWnd();
                Print_WeaponItemGradeLevelWnd();
                Print_CurrGauge();
                m_bResultSuccess = true;
            }
            else
            {
                m_bResultSuccess = false;
            }
        }
    }

    if (!m_bDecreaseUpgradeEffect)
    {
        if (1.f > m_fAlpha_UpgradeButton_Effect)
            m_fAlpha_UpgradeButton_Effect += fTimeDelta;
        else
            m_bDecreaseUpgradeEffect = true;
    }
    else if (m_bDecreaseUpgradeEffect)
    {
        if (0.f < m_fAlpha_UpgradeButton_Effect)
            m_fAlpha_UpgradeButton_Effect -= fTimeDelta;
        else
            m_bDecreaseUpgradeEffect = false;
    }

}

void CUI_NPC_ItemUpgrade::Update_QuitButton(POINT pt)
{
    Is_Picking_QuitButton(pt);
    if ((m_bQuitButton) && (KEY_TAP(KEY::LBTN)))
    {
        m_pItemNameWnd->Set_Active(false);
        m_pCurrItemNameWnd->Set_Active(false);
        m_pItemNameWnd->Set_Active(false);
        m_pResultWnd->Set_Active(false);
        m_pNpcScriptWnd->Set_Active(false);
        m_bDeActive = true;
        CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        CSound_Manager::GetInstance()->Stop_Channel_Sound(L"RomanticWeapon");
        CSound_Manager::GetInstance()->Resume_Channel(L"CastleBern");
    }
}

void CUI_NPC_ItemUpgrade::Update_ResultCheckButton(POINT pt)
{
    if (!m_bResultWnd)
        return;
    Is_Picking_UpgradeButton(pt);
    if ((m_bUpgradeButton) && (KEY_TAP(KEY::LBTN)))
    {
        CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        CSound_Manager::GetInstance()->Resume_Channel(L"RomanticWeapon");
        m_bResultWnd = false;
        Print_ResultWnd();
        Print_Bahuntrr();
    }
}

void CUI_NPC_ItemUpgrade::Create_Rect_GrowthButton()
{
    m_rcGrowthButton.left = LONG(m_fX - ((108.f * 0.8f) / 2));
    m_rcGrowthButton.top = LONG((m_fY + 30.f) - ((44.f * 0.8f) / 2));
    m_rcGrowthButton.right = LONG(m_fX + ((108.f * 0.8f) / 2));
    m_rcGrowthButton.bottom = LONG((m_fY + 30.f) + ((44.f * 0.8f) / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_GrowthButton(POINT pt,_float fTimeDelta)
{
    if (PtInRect(&m_rcGrowthButton, pt))
    {
        if (!m_bSound[0])
        {
            m_bSound[0] = true;
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        }
        m_iGrowthButton_TextureIndex = 1;
        m_bGrowthButton = true;
    }
    else
    {
        m_iGrowthButton_TextureIndex = 0;
        m_bGrowthButton = false;
        m_bSound[0] = false;
    }
}

void CUI_NPC_ItemUpgrade::Create_Rect_UpgradeButton()
{
    m_rcUpgradeButton.left = LONG(m_fX - ((221.f * 0.8f) / 2));
    m_rcUpgradeButton.top = LONG((m_fY + 264.f) - ((42.f * 0.8f) / 2));
    m_rcUpgradeButton.right = LONG(m_fX + ((221.f * 0.8f) / 2));
    m_rcUpgradeButton.bottom = LONG((m_fY + 264.f) + ((42.f * 0.8f) / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_UpgradeButton(POINT pt)
{
    if (PtInRect(&m_rcUpgradeButton, pt))
    {
        if (!m_bSound[1])
        {
            m_bSound[1] = true;
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        }
        m_iUpgradeButton_TextureIndex = 1;
        m_bUpgradeButton = true;
    }
    else
    {
        m_iUpgradeButton_TextureIndex = 0;
        m_bUpgradeButton = false;
        m_bSound[1] = false;
    }
}

void CUI_NPC_ItemUpgrade::Create_Rect_QuitButton()
{
    m_rcQuitButton.left = LONG((m_fX + 700.f) - ((205.f * 0.8f) / 2));
    m_rcQuitButton.top = LONG((g_iWinSizeY - 50.f) - ((48.f * 0.8f) / 2));
    m_rcQuitButton.right = LONG((m_fX + 700.f) + ((205.f * 0.8f) / 2));
    m_rcQuitButton.bottom = LONG((g_iWinSizeY - 50.f) + ((48.f * 0.8f) / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_QuitButton(POINT pt)
{
    if (PtInRect(&m_rcQuitButton, pt))
    {
        if (!m_bSound[2])
        {
            m_bSound[2] = true;
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        }
        m_iQuitButton_TextureIndex = 1;
        m_bQuitButton = true;
    }
    else
    {
        m_iQuitButton_TextureIndex = 0;
        m_bQuitButton = false;
        m_bSound[2] = false;
    }
}

void CUI_NPC_ItemUpgrade::LateTcik_Items(POINT pt)
{
    Create_Rect_FaceItem();
    Create_Rect_HelemtItem();
    Create_Rect_ShoulderItem();
    Create_Rect_BodyItem();
    Create_Rect_ArmItem();
    Create_Rect_LegItem();

    Is_Picking_FaceItem(pt);
    Is_Picking_HelemtItem(pt);
    Is_Picking_ShoulderItem(pt);
    Is_Picking_BodyItem(pt);
    Is_Picking_ArmItem(pt);
    Is_Picking_LegItem(pt);
}

void CUI_NPC_ItemUpgrade::Create_Rect_FaceItem()
{
    m_rcFaceItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcFaceItem.top = LONG((m_fY - 200.f) - (60.f / 2));
    m_rcFaceItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcFaceItem.bottom = LONG((m_fY - 200.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_FaceItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_FACE])
        return;
    if (PtInRect(&m_rcFaceItem, pt))
    {
        if(m_iCurrItem != SELECTED_FACE)
            m_vColorFaceItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_FACE;
            Update_ItemIcon();
        }
    }
    else
        m_vColorFaceItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_HelemtItem()
{
    m_rcHelemtItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcHelemtItem.top = LONG((m_fY - 135.f) - (60.f / 2));
    m_rcHelemtItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcHelemtItem.bottom = LONG((m_fY - 135.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_HelemtItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_HELMET])
        return;
    if (PtInRect(&m_rcHelemtItem, pt))
    {
        if (m_iCurrItem != SELECTED_HELMET)
            m_vColorHelemtItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_HELMET;
            Update_ItemIcon();
        }
    }
    else
        m_vColorHelemtItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_ShoulderItem()
{
    m_rcShoulderItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcShoulderItem.top = LONG((m_fY - 75.f) - (60.f / 2));
    m_rcShoulderItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcShoulderItem.bottom = LONG((m_fY - 75.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_ShoulderItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_SHOULDER])
        return;
    if (PtInRect(&m_rcShoulderItem, pt))
    {
        if (m_iCurrItem != SELECTED_SHOULDER)
            m_vColorShoulderItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_SHOULDER;
            Update_ItemIcon();
        }
    }
    else
        m_vColorShoulderItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_BodyItem()
{
    m_rcBodyItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcBodyItem.top = LONG((m_fY - 15.f) - (60.f / 2));
    m_rcBodyItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcBodyItem.bottom = LONG((m_fY - 15.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_BodyItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_BODY])
        return;
    if (PtInRect(&m_rcBodyItem, pt))
    {
        if (m_iCurrItem != SELECTED_BODY)
            m_vColorBodyItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_BODY;
            Update_ItemIcon();
        }
    }
    else
        m_vColorBodyItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_ArmItem()
{
    m_rcArmItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcArmItem.top = LONG((m_fY + 55.f) - (60.f / 2));
    m_rcArmItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcArmItem.bottom = LONG((m_fY + 55.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_ArmItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_ARM])
        return;
    if (PtInRect(&m_rcArmItem, pt))
    {
        if (m_iCurrItem != SELECTED_ARM)
            m_vColorArmItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_ARM;
            Update_ItemIcon();
        }
    }
    else
        m_vColorArmItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_LegItem()
{
    m_rcLegItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcLegItem.top = LONG((m_fY + 120.f) - (60.f / 2));
    m_rcLegItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcLegItem.bottom = LONG((m_fY + 120.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_LegItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_LEG])
        return;
    if (PtInRect(&m_rcLegItem, pt))
    {
        if (m_iCurrItem != SELECTED_LEG)
            m_vColorLegItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_LEG;
            Update_ItemIcon();
        }
    }
    else
        m_vColorLegItem = Vec4(1.f, 1.f, 1.f, 1.f);
  
}

void CUI_NPC_ItemUpgrade::Create_Rect_WeaponItem()
{
    m_rcLegItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcLegItem.top = LONG((m_fY + 185.f) - (60.f / 2));
    m_rcLegItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcLegItem.bottom = LONG((m_fY + 185.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_WeaponItem(POINT pt)
{
    if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_WEAPON])
        return;
    if (PtInRect(&m_rcWeaponItem, pt))
    {
        if (m_iCurrItem != SELECTED_WEAPON)
            m_vColorWeaponItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            m_iCurrItem = SELECTED_WEAPON;
            Update_ItemIcon();
        }
    }
    else
        m_vColorWeaponItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Update_GrowthGauge(_float fTimeDelta)
{
    if (!m_bGrowthOn)
        return;

    if (nullptr != m_pCurrUpgradeItem)
    {
        if (m_fItemGrowthMaxGauge > m_pCurrUpgradeItem->Get_UpgradeGauge())
        {
            m_pCurrUpgradeItem->Add_UpgradeGauge(60.f * fTimeDelta);
            m_fItemGrowthCurrGauge = m_pCurrUpgradeItem->Get_UpgradeGauge();
            Print_CurrGauge();
        }
    }
}

void CUI_NPC_ItemUpgrade::Update_Hammer_Effects(_float fTimeDelta)
{
    Update_Hammer_Effect(fTimeDelta);
    Update_Hammer_Aura(fTimeDelta);
}

void CUI_NPC_ItemUpgrade::Update_Hammer_Effect(_float fTimeDelta)
{
    if (!m_bMaxGauge)
    {
        m_fHammer_Effect_Alpha = 0.f;
        m_bDecreaseEffect = true;
        return;
    }
    if (m_bDecreaseEffect)
    {
        if (0.f < m_fHammer_Effect_Alpha)
        {
            m_fHammer_Effect_Alpha -= fTimeDelta;
        }
        else
            m_bDecreaseEffect = false;
    }
    else
    {
        if (1.f > m_fHammer_Effect_Alpha)
            m_fHammer_Effect_Alpha += fTimeDelta;
        else if (1.f < m_fHammer_Effect_Alpha)
            m_fHammer_Effect_Alpha = 1.f;
    }
}

void CUI_NPC_ItemUpgrade::Update_Hammer_Aura(_float fTimeDelta)
{
    if (!m_bMaxGauge)
        return;

    if (33.f < m_fFrame)
        m_fFrame = 0.f;
    else if (33.f >= m_fFrame)
        m_fFrame += 33.f * fTimeDelta;
}

void CUI_NPC_ItemUpgrade::Update_Gague_Smoke(_float fTimeDelta)
{
    if (55.f >= m_fFrame_Smoke)
        m_fFrame_Smoke += min(1.0f, 20.f * fTimeDelta);

    if (55.f < m_fFrame_Smoke)
        m_fFrame_Smoke = 0.f;
}

void CUI_NPC_ItemUpgrade::Update_Gauge_Effect(_float fTimeDelta)
{
    if (!m_bMaxGauge)
        return;
  if (9.f >= m_fFrame_GaugeEffect)
        m_fFrame_GaugeEffect += 9.f * fTimeDelta;
  if (9.f < m_fFrame_GaugeEffect)
      m_fFrame_GaugeEffect = 0.f;
}

void CUI_NPC_ItemUpgrade::Update_Gauge_Spin(_float fTimeDelta)
{
    if (m_bDecreaseEffect)
        return;
   if (78.f > m_fFrame_GaugeSpin)
        m_fFrame_GaugeSpin += 40.f * fTimeDelta;

    if (78.f <= m_fFrame_GaugeSpin)
        m_fFrame_GaugeSpin = 0.f;
}

void CUI_NPC_ItemUpgrade::Update_Max_Gauge_Effect(_float fTimeDelta)
{
    if (!m_bMaxGaugeEffect)
    {
        m_fFrame_MaxGaugeEffect = 0.f;
        return;
    }

    if (24.f >= m_fFrame_MaxGaugeEffect)
    {
        m_fFrame_MaxGaugeEffect += 20.f * fTimeDelta;
    }

    if (24.f < m_fFrame_MaxGaugeEffect)
    {
        m_bMaxGaugeEffect = false;
        m_fFrame_MaxGaugeEffect = 0.f;
    }
}

void CUI_NPC_ItemUpgrade::Update_ItemIcon()
{
    switch(m_iCurrItem)
    {
    case SELECTED_FACE:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_FACE])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_FACE];    
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_FACE];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_FACE];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_FACE];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_FACE]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_FACE];
        break;
    case SELECTED_HELMET:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_HELMET])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_HELMET];  
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_HELMET];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_HELMET];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_HELMET];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_HELMET]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_HELMET];
        break;
    case SELECTED_SHOULDER:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_SHOULDER])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_SHOULDER];    
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_SHOULDER];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_SHOULDER];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_SHOULDER];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_SHOULDER]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_SHOULDER];
        break;
    case SELECTED_BODY:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_BODY])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_BODY];    
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_BODY];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_BODY];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_BODY];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_BODY]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_BODY];
        break;
    case SELECTED_ARM:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_ARM])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_ARM];    
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_ARM];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_ARM];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_ARM];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_ARM]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_ARM];
        break;
    case SELECTED_LEG:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_LEG])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_LEG];    
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_LEG];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_LEG];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_LEG];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_LEG]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_LEG];
        break;

    case SELECTED_WEAPON:
        if (nullptr == m_pTexture_ItemIcon[(_uint)SELECTED_WEAPON])
            return;
        m_pTexture_Item = m_pTexture_ItemIcon[(_uint)SELECTED_WEAPON];
        m_pTexture_ResultItem = m_pTexture_ItemIcon[(_uint)SELECTED_WEAPON];
        m_iCurrItemGrade = m_iSidePannel_L_Wnd_TextureIndex[(_uint)SELECTED_WEAPON];
        m_strCurrItemName = m_strItemsName[(_uint)SELECTED_WEAPON];
        m_fItemGrowthCurrGauge = m_pEquips[(_uint)SELECTED_WEAPON]->Get_UpgradeGauge();
        m_pCurrUpgradeItem = m_pEquips[(_uint)SELECTED_WEAPON];
        break;
    }

    if (nullptr != m_pCurrUpgradeItem)
    {
        if (100.f > m_pCurrUpgradeItem->Get_UpgradeGauge())
        {
            m_fHammer_Effect_Alpha = 0.f;
            m_bDecreaseEffect = true;
        }
        m_bMaxGaugeEffect = false;
        m_fFrame = 0.f;
        m_fFrame_GaugeEffect = 0.f;
        m_fFrame_Smoke = 0.f;
        m_fFrame_MaxGaugeEffect = 0.f;
        m_fFrame_GaugeSpin = 0.f;
        m_fFrame_ResultWaiting = 0.f;
        m_fFrame_ResultEffect = 0.f;
        Print_CurrItemNameWnd();
        Print_CurrGauge();
    }
}

void CUI_NPC_ItemUpgrade::Update_ResultWaiting(_float fTimeDelta)
{
    if (!m_bResultWaiting)
        return; 
    if (77.f > m_fFrame_ResultWaiting)
        m_fFrame_ResultWaiting += 30.f * fTimeDelta;

    if (77.0f < m_fFrame_ResultWaiting)
    {
        m_bResult = true;

        m_fFrame_ResultWaiting = 77.f;
        m_bMaxGauge = false;
        m_bMaxGaugeEffect = false;
        m_bDecreaseEffect = true;
        m_bResultWnd = true;
        m_fItemGrowthCurrGauge = 0.f;
        m_fFrame = 0.f;
        m_fFrame_GaugeEffect = 0.f;
        m_fFrame_MaxGaugeEffect = 0.f;
        m_fFrame_GaugeSpin = 0.f;
        m_fAlpha_UpgradeButton_Effect = 0.f;
        m_fHammer_Effect_Alpha = 0.f;
        m_pResultWnd->Set_Active(true);
        Print_ResultWnd();
        Print_Bahuntrr();
        m_pCurrUpgradeItem->Set_UpgradeGauge(0.f);
        Print_CurrItemNameWnd();
        Print_CurrGauge();

        if (true == m_bResultSuccess)
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Item_Upgrade_Succese.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));

            switch (CGameInstance::GetInstance()->Random_Int(0, 2))
            {
            case 0:
                CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ItemUpgrade_Success0.wav", 1.f);
                break;
            case 1:
                CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ItemUpgrade_Success1.wav", 1.f);
                break;
            case 2:
                CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ItemUpgrade_Success2.wav", 1.f);
                break;
            }
        }

        else if (false == m_bResultSuccess)
        {
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Item_Upgrade_Failed.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
            switch (CGameInstance::GetInstance()->Random_Int(0, 2))
            {
            case 0:
                CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ItemUpgrade_Failed0.wav", 1.f);
                break;
            case 1:
                CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ItemUpgrade_Failed1.wav", 1.f);
                break;
            case 2:
                CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ItemUpgrade_Failed2.wav", 1.f);
                break;
            }
        }
    }
}

void CUI_NPC_ItemUpgrade::Update_ResultEffect(_float fTimeDelta)
{
    if (!m_bResult)
        return;

    if (22.f > m_fFrame_ResultEffect)
    {
        m_fFrame_ResultEffect += 30.f * fTimeDelta;
        m_fAlpha_ResultWaiting -= 3.f * fTimeDelta;
    }

    if (22.f <= m_fFrame_ResultEffect)
    {
        m_bResult = false;
        m_bResultWaiting = false;
        m_fFrame_ResultEffect = 0.f;
        m_fFrame_ResultWaiting = 0.f;
        m_fAlpha_ResultWaiting = 1.f;
    }
}

HRESULT CUI_NPC_ItemUpgrade::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);

    if (nullptr == m_pCurrItemNameWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("ItemUpgrade_CurrName");
        m_strTagNameWnd = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(566.f * 0.8f, 98.f * 0.8f);
        m_pCurrItemNameWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        m_pCurrItemNameWnd->Set_Render(false);
        if (nullptr == m_pCurrItemNameWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pCurrItemNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pCurrItemNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3((m_fX - 4.f) - g_iWinSizeX * 0.5f, -(m_fY - 140.f)+ g_iWinSizeY * 0.5f, 0.02f));
    }

    if (nullptr == m_pItemNameWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("ItemUpgrade_ItemsName");
        m_strTagItemsNameWnd = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(450.f * 0.8f, 728.f * 0.8f);
        m_pItemNameWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        m_pItemNameWnd->Set_Render(false);
        if (nullptr == m_pItemNameWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pItemNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pItemNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3((m_fX - 366.f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.02f));
    }

    if (nullptr == m_pCurrGaugeWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("ItemUpgrade_Gauge");
        m_strUpgradeGagueTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(110.f * 0.8f, 110.f * 0.8f);
        m_pCurrGaugeWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        m_pCurrGaugeWnd->Set_Render(false);
        if (nullptr == m_pCurrGaugeWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pCurrGaugeWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pCurrGaugeWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.02f));
    }

    if (nullptr == m_pResultWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("ResultWnd");
        m_strResultWndTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(1092.f, 581.f);
        m_pResultWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        m_pResultWnd->Set_Render(false);
        if (nullptr == m_pResultWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pResultWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pResultWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.01f));
    }

    if (nullptr == m_pNpcScriptWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("Bahunturr_Script");
        m_strScriptWndTag = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(g_iWinSizeX, g_iWinSizeY);
        m_pNpcScriptWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        m_pNpcScriptWnd->Set_Render(false);
        if (nullptr == m_pNpcScriptWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }
        m_pNpcScriptWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pNpcScriptWnd->Get_TransformCom()->Set_Scale(Vec3(g_iWinSizeX, g_iWinSizeY, 0.f));
        m_pNpcScriptWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.01f));
    }
    Safe_Release(pGameInstance);
    return S_OK;
}

void CUI_NPC_ItemUpgrade::Print_CurrItemNameWnd()
{
    if (nullptr != m_pCurrItemNameWnd && nullptr != m_pCurrUpgradeItem)
    {
        m_pCurrItemNameWnd->Clear_Text();
        m_pCurrItemNameWnd->Set_Alpha(1.f);
        m_pCurrItemNameWnd->Get_TransformCom()->Set_Scale(Vec3(566.f * 0.6f, 98.f * 0.6f, 0.f));
        m_pCurrItemNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3((m_fX - 4.f) - g_iWinSizeX * 0.5f, -(m_fY - 150.f)+ g_iWinSizeY * 0.5f, 0.02f));

        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strCurrItemName);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_iCurrItemGrade)
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd + TEXT("-1"), m_strFont, m_strCurrItemName, Vec2((566.f * 0.8f) * 0.5f -1.f, ((98.f * 0.8f) * 0.5f) - 10.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd + TEXT("-2"), m_strFont, m_strCurrItemName, Vec2((566.f * 0.8f) * 0.5f +1.f, ((98.f * 0.8f) * 0.5f) - 10.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd + TEXT("-3"), m_strFont, m_strCurrItemName, Vec2((566.f * 0.8f) * 0.5f, ((98.f * 0.8f) * 0.5f) - 9.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd + TEXT("-4"), m_strFont, m_strCurrItemName, Vec2((566.f * 0.8f) * 0.5f, ((98.f * 0.8f) * 0.5f) - 11.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd, m_strFont, m_strCurrItemName, Vec2((566.f * 0.8f) * 0.5f, ((98.f * 0.8f) * 0.5f) - 10.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, vGradeColor);

        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd + TEXT("_CurrLevel"), m_strFont, to_wstring(m_pCurrUpgradeItem->Get_UpgradeLevel()) +TEXT(" ´Ü°è"), Vec2((566.f * 0.8f) * 0.5f - 100.f, ((98.f * 0.8f) * 0.5f)), Vec2(0.5f, 0.5f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
        m_pCurrItemNameWnd->Set_Text(m_strTagNameWnd + TEXT("_NextLevel"), m_strFont, to_wstring(m_pCurrUpgradeItem->Get_UpgradeLevel() + 1) + TEXT(" ´Ü°è"), Vec2((566.f * 0.8f) * 0.5f + 50.f, ((98.f * 0.8f) * 0.5f)), Vec2(0.5f, 0.5f), Vec2(0.f, 0.f), 0.f, Vec4(0.67f, 0.85f, 0.18f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_FaceItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_FACE]) && (nullptr != m_pItemNameWnd))
    {
        m_pItemNameWnd->Clear_Text();
        m_pItemNameWnd->Set_Alpha(1.f);
        m_pItemNameWnd->Get_TransformCom()->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
        m_pItemNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3((m_fX - 366.f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.02f));
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::FACE]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::FACE]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_FaceItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::FACE], Vec2((323.f * 0.9f) * 0.4f -1, (30.f * 3.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_FaceItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::FACE], Vec2((323.f * 0.9f) * 0.4f +1, (30.f * 3.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_FaceItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::FACE], Vec2((323.f * 0.9f) * 0.4f, (30.f * 3.f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_FaceItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::FACE], Vec2((323.f * 0.9f) * 0.4f, (30.f * 3.f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_FaceItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::FACE], Vec2((323.f * 0.9f) * 0.4f, (30.f * 3.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_FaceItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::FACE]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_FACE])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_FACE]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_FaceGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.4f, (30.f * 2.5f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_HelemetItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_HELMET]) && (nullptr != m_pItemNameWnd))
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::HELMET]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::HELMET]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_HelmetItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::HELMET], Vec2((323.f * 0.9f) * 0.4f +1.f, (30.f * 5.2f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_HelmetItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::HELMET], Vec2((323.f * 0.9f) * 0.4f -1.f, (30.f * 5.2f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_HelmetItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::HELMET], Vec2((323.f * 0.9f) * 0.4f, (30.f * 5.2f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_HelmetItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::HELMET], Vec2((323.f * 0.9f) * 0.4f, (30.f * 5.2f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_HelmetItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::HELMET], Vec2((323.f * 0.9f) * 0.4f, (30.f * 5.2f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_HelmetItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::HELMET]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_HELMET])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_HELMET]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_HelmetGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.4f, (30.f * 4.7f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_ShoulderItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_SHOULDER]) && (nullptr != m_pItemNameWnd))
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::SHOULDER]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::SHOULDER]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ShoulderItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::SHOULDER], Vec2((323.f * 0.9f) * 0.4f +1.f, (30.f * 7.1f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ShoulderItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::SHOULDER], Vec2((323.f * 0.9f) * 0.4f -1.f, (30.f * 7.1f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ShoulderItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::SHOULDER], Vec2((323.f * 0.9f) * 0.4f, (30.f * 7.1f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ShoulderItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::SHOULDER], Vec2((323.f * 0.9f) * 0.4f, (30.f * 7.1f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ShoulderItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::SHOULDER], Vec2((323.f * 0.9f) * 0.4f, (30.f * 7.1f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_ShoulderItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::SHOULDER]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_SHOULDER])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_SHOULDER]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ShoulderGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.4f, (30.f * 6.6f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_BodyItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_BODY]) && (nullptr != m_pItemNameWnd))
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::BODY]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::BODY]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_BodyItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::BODY], Vec2((323.f * 0.9f) * 0.4f +1.f, (30.f * 9.3f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_BodyItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::BODY], Vec2((323.f * 0.9f) * 0.4f -1.f, (30.f * 9.3f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_BodyItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::BODY], Vec2((323.f * 0.9f) * 0.4f, (30.f * 9.3f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_BodyItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::BODY], Vec2((323.f * 0.9f) * 0.4f, (30.f * 9.3f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_BodyItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::BODY], Vec2((323.f * 0.9f) * 0.4f, (30.f * 9.3f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_BodyItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::BODY]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_BODY])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_BODY]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_BodyGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.4f, (30.f * 8.8f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_ArmItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_ARM]) && (nullptr != m_pItemNameWnd))
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::BODY]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::ARM]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ArmItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::ARM], Vec2((323.f * 0.9f) * 0.4f +1.f, (30.f * 12.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ArmItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::ARM], Vec2((323.f * 0.9f) * 0.4f -1.f, (30.f * 12.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ArmItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::ARM], Vec2((323.f * 0.9f) * 0.4f, (30.f * 12.f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ArmItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::ARM], Vec2((323.f * 0.9f) * 0.4f, (30.f * 12.f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ArmItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::ARM], Vec2((323.f * 0.9f) * 0.4f, (30.f * 12.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_ArmItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::ARM]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_ARM])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_ARM]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_ArmGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.4f, (30.f * 11.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_LegItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_LEG]) && (nullptr != m_pItemNameWnd))
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::LEG]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::LEG]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_LegItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f + 1.f, (30.f * 14.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_LegItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f - 1.f, (30.f * 14.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_LegItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f, (30.f * 14.f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_LegItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f, (30.f * 14.f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_LegItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f, (30.f * 14.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_LegItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::LEG]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_LEG])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_LEG]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_LegGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.4f, (30.f * 13.2f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_WeaponItemNameWnd()
{
    if ((nullptr != m_pEquips[SELECTED_WEAPON]) && (nullptr != m_pItemNameWnd))
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::WEAPON]);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec4 vGradeColor;
        switch (m_pEquips[(_uint)CItem::PART::WEAPON]->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_WeaponItem-1"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f + 1.f, (30.f * 5.f) + 15.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_WeaponItem-2"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f - 1.f, (30.f * 5.f) + 15.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_WeaponItem-3"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f, (30.f * 15.f) + 4.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_WeaponItem-4"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f, (30.f * 15.f) + 6.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_WeaponItem"), TEXT("³Ø½¼Lv1°íµñBold"), m_strItemsName[(_uint)CItem::PART::LEG], Vec2((323.f * 0.9f) * 0.4f, (30.f * 15.f) + 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, vGradeColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_WeaponItemGradeLevelWnd()
{
    if (nullptr != m_pItemNameWnd)
    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, m_strItemsName[(_uint)CItem::PART::WEAPON]);
        Vec2 vOrigin = vMeasure * 0.5f;
        wstring strItemGradeLevel;
        if (nullptr != m_pEquips[SELECTED_WEAPON])
            strItemGradeLevel = to_wstring(m_pEquips[SELECTED_WEAPON]->Get_UpgradeLevel()) + TEXT("´Ü°è");
        else
            strItemGradeLevel = TEXT("");

        m_pItemNameWnd->Set_Text(m_strTagItemsNameWnd + TEXT("_WeaponGradeLevel"), TEXT("³Ø½¼Lv1°íµñBold"), strItemGradeLevel, Vec2((323.f * 0.9f) * 0.5f, (30.f * 15.f) - 5.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.86f, 0.65f, 0.3f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_CurrGauge()
{
    if (nullptr != m_pCurrGaugeWnd)
    {
        m_pCurrGaugeWnd->Clear_Text();
        m_pCurrGaugeWnd->Set_Alpha(1.f);
        m_pCurrGaugeWnd->Get_TransformCom()->Set_Scale(Vec3(110.f * 0.8f, 110.f * 0.8f, 0.f));
        m_pCurrGaugeWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.02f));

        wstring fUpgradeGauge;
        if (nullptr != m_pCurrUpgradeItem)
            fUpgradeGauge = to_wstring((_uint)m_pCurrUpgradeItem->Get_UpgradeGauge()) + TEXT("%");
        else
            fUpgradeGauge = TEXT("");

        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, fUpgradeGauge);
        Vec2 vOrigin = vMeasure * 0.5f;

        m_pCurrGaugeWnd->Set_Text(m_strUpgradeGagueTag + TEXT("-1"), m_strFont, fUpgradeGauge, Vec2((110.f * 0.8f) * 0.5f - 1.f, (110.f * 0.8f) * 0.5f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.73f, 0.5f, 0.26f, 1.f));
        m_pCurrGaugeWnd->Set_Text(m_strUpgradeGagueTag + TEXT("-2"), m_strFont, fUpgradeGauge, Vec2((110.f * 0.8f) * 0.5f + 1.f, (110.f * 0.8f) * 0.5f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.73f, 0.5f, 0.26f, 1.f));
        m_pCurrGaugeWnd->Set_Text(m_strUpgradeGagueTag + TEXT("-3"), m_strFont, fUpgradeGauge, Vec2((110.f * 0.8f) * 0.5f , (110.f * 0.8f) * 0.5f - 1.f) , Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.73f, 0.5f, 0.26f, 1.f));
        m_pCurrGaugeWnd->Set_Text(m_strUpgradeGagueTag + TEXT("-4"), m_strFont, fUpgradeGauge, Vec2((110.f * 0.8f) * 0.5f , (110.f * 0.8f) * 0.5f + 1.f) , Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.73f, 0.5f, 0.26f, 1.f));
        m_pCurrGaugeWnd->Set_Text(m_strUpgradeGagueTag, m_strFont, fUpgradeGauge , Vec2((110.f * 0.8f)*0.5f, (110.f * 0.8f) * 0.5f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.93f, 0.85f, 0.76f, 1.f));
    }
}

void CUI_NPC_ItemUpgrade::Print_ResultWnd()
{
    if (nullptr != m_pResultWnd)
    {
        m_pResultWnd->Clear_Text();
        m_pResultWnd->Set_Alpha(1.f);
        m_pResultWnd->Get_TransformCom()->Set_Scale(Vec3(1092.f, 581.f, 0.f));
        m_pResultWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.01f));

        wstring fUpgradeGauge;
        wstring strGradeLevel;
        wstring strUpgradeCheck;
        Vec4    vResultColor;
        if ((nullptr != m_pCurrUpgradeItem) && (m_bResultWnd))
        {
            fUpgradeGauge = m_pCurrUpgradeItem->Get_ItemName();
            if (m_bResultSuccess)
            {
                strGradeLevel = to_wstring(m_pCurrUpgradeItem->Get_UpgradeLevel()) + TEXT("´Ü°è");
                strUpgradeCheck = TEXT("Àç·Ã ¼º°ø");
                vResultColor = Vec4(1.f, 1.f, 1.f, 1.f);
            }
            else
            {
                strGradeLevel = TEXT("");
                strUpgradeCheck = TEXT("Àç·Ã ½ÇÆÐ");
                vResultColor = Vec4(0.69f, 0.28f, 0.27f, 1.f);
                CSound_Manager::GetInstance()->Pause_Channel(L"RomanticWeapon");
            }
        }
        else
        {
            fUpgradeGauge = TEXT("");
            strGradeLevel = TEXT("");
            strUpgradeCheck = TEXT("");
        }
        Vec4 vGradeColor;
        switch (m_pCurrUpgradeItem->Get_ItemGrade())
        {
        case (_uint)CItem::GRADE::WHITE:
            vGradeColor = Vec4(1.f, 1.f, 1.f, 1.f);
            break;
        case (_uint)CItem::GRADE::GREEN:
            vGradeColor = Vec4(0.29f, 0.85f, 0.3f, 1.f);
            break;
        case (_uint)CItem::GRADE::BLUE:
            vGradeColor = Vec4(0.29f, 0.53f, 0.85f, 1.f);
            break;
        case (_uint)CItem::GRADE::PURPLE:
            vGradeColor = Vec4(0.53f, 0.25f, 0.61f, 1.f);
            break;
        case (_uint)CItem::GRADE::ORANGE:
            vGradeColor = Vec4(0.83f, 0.49f, 0.f, 1.f);
            break;
        case (_uint)CItem::GRADE::RED:
            vGradeColor = Vec4(0.83f, 0.24f, 0.f, 1.f);
            break;
        }

        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, fUpgradeGauge);
        Vec2 vOrigin = vMeasure * 0.5f;
        Vec2 vGradeLevelOrigin = CGameInstance::GetInstance()->MeasureString(m_strFont, strGradeLevel) * 0.5f;
        Vec2 vResultText = CGameInstance::GetInstance()->MeasureString(m_strFont, strUpgradeCheck) * 0.5f;
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemName-1"), m_strFont, fUpgradeGauge, Vec2(1092.f * 0.5f + 1.f, (581.f * 0.5f) - 50.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemName-2"), m_strFont, fUpgradeGauge, Vec2(1092.f * 0.5f - 1.f, (581.f * 0.5f) - 50.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemName-3"), m_strFont, fUpgradeGauge, Vec2(1092.f * 0.5f, (581.f * 0.5f) - 51.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemName-4"), m_strFont, fUpgradeGauge, Vec2(1092.f * 0.5f, (581.f * 0.5f) - 49.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemName"), m_strFont, fUpgradeGauge, Vec2(1092.f * 0.5f, (581.f * 0.5f) - 50.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, vGradeColor);

        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeLevel-1"), m_strFont, strGradeLevel, Vec2(1092.f * 0.5f+1.f, (581.f * 0.5f) - 30.f), Vec2(0.4f, 0.4f), vGradeLevelOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeLevel-2"), m_strFont, strGradeLevel, Vec2(1092.f * 0.5f-1.f, (581.f * 0.5f) - 30.f), Vec2(0.4f, 0.4f), vGradeLevelOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeLevel-3"), m_strFont, strGradeLevel, Vec2(1092.f * 0.5f, (581.f * 0.5f) - 31.f), Vec2(0.4f, 0.4f), vGradeLevelOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeLevel-4"), m_strFont, strGradeLevel, Vec2(1092.f * 0.5f, (581.f * 0.5f) - 29.f), Vec2(0.4f, 0.4f), vGradeLevelOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeLevel"), m_strFont, strGradeLevel, Vec2(1092.f * 0.5f, (581.f * 0.5f) - 30.f), Vec2(0.4f, 0.4f), vGradeLevelOrigin, 0.f, Vec4(0.29f, 0.85f, 0.3f, 1.f));
       
        
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeCheck-1"), m_strFont, strUpgradeCheck, Vec2(1092.f * 0.5f -1.f, (581.f * 0.5f)), Vec2(0.5f, 0.5f), vResultText, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeCheck-2"), m_strFont, strUpgradeCheck, Vec2(1092.f * 0.5f +1.f, (581.f * 0.5f)), Vec2(0.5f, 0.5f), vResultText, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeCheck-3"), m_strFont, strUpgradeCheck, Vec2(1092.f * 0.5f, (581.f * 0.5f) -1.f), Vec2(0.5f, 0.5f), vResultText, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeCheck-4"), m_strFont, strUpgradeCheck, Vec2(1092.f * 0.5f, (581.f * 0.5f) + 1.f), Vec2(0.5f, 0.5f), vResultText, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
        m_pResultWnd->Set_Text(m_strUpgradeGagueTag + TEXT("_ItemGradeCheck"), m_strFont, strUpgradeCheck, Vec2(1092.f * 0.5f, (581.f * 0.5f)), Vec2(0.5f, 0.5f), vResultText, 0.f, vResultColor);
    }
}

void CUI_NPC_ItemUpgrade::Print_Bahuntrr()
{
    if (nullptr == m_pNpcScriptWnd)
        return;

    m_pNpcScriptWnd->Clear_Text();
    m_pNpcScriptWnd->Set_Alpha(1.f);

    if ((m_bResultSuccess) && (m_bResultWnd))
        m_strNpcScript = TEXT("ÇÏÇÏÇÏ! Àß ºÃÁö! ÀÌ°Ô¹Ù·Î ¹ÙÈÆÅõ¸£´ÔÀÇ ¿¡¼¾½º¶ó°í!");
    else if ((!m_bResultSuccess) && (m_bResultWnd))
        m_strNpcScript = TEXT("¾îÀÌÄí! Å©Èì...Àú..¾Æ¿ì´Ô, ¼ÕÀÌ ¹Ì²ô·¯Á®¼­ ±×¸¸..");
    else if (!m_bResultWnd)
        m_strNpcScript = TEXT("¾Æ¿ì´Ô, Àåºñ¸¸ °®°í¿À¶ó°í! ³»°¡ ´õ ¸ÚÁö°Ô ÇØÁÙÅ×´Ï±ñ!");
  
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("³Ø½¼Lv1°íµñBold"), m_strNpcScript);
    Vec2 vOrigin = vMeasure * 0.5f;
    m_pNpcScriptWnd->Set_Text(m_strScriptWndTag, TEXT("³Ø½¼Lv1°íµñBold"), m_strNpcScript, Vec2(g_iWinSizeX * 0.5f, g_iWinSizeY - 50.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.f));
    
    vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("³Ø½¼Lv1°íµñBold"), TEXT("¹ÙÈÆÅõ¸£"));
    vOrigin = vMeasure * 0.5f;
    m_pNpcScriptWnd->Set_Text(m_strScriptWndTag + TEXT("NPC_Name"), TEXT("´øÆÄ¿¬¸¶µÈÄ®³¯"), TEXT("¹ÙÈÆÅõ¸£"), Vec2(g_iWinSizeX * 0.5f, g_iWinSizeY - 100.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.84f, 0.68f, 0.35f, 1.f));
}

HRESULT CUI_NPC_ItemUpgrade::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_None"),
        TEXT("Com_Texture_None"), (CComponent**)&m_pTexture_None)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer_Cap"),
        TEXT("Com_Texture_HammerCap"), (CComponent**)&m_pTexture_HammerCap)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer_Effect"),
        TEXT("Com_Texture_HammerEffect"), (CComponent**)&m_pTexture_HammerAura)))
        return E_FAIL; 
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer_Shine"),
        TEXT("Com_Texture_HammerAura"), (CComponent**)&m_pTexture_HammerEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Gauge"),
        TEXT("Com_Texture_Gauge"), (CComponent**)&m_pTexture_Gauge)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Gauge_Effect"),
        TEXT("Com_Texture_GaugeEffect"), (CComponent**)&m_pTexture_GaugeEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Smoking_Gauge"),
        TEXT("Com_Texture_GaugeSmoke"), (CComponent**)&m_pTexture_GaugeSmoke)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Growth_Item"),
        TEXT("Com_Texture_GrowthButton"), (CComponent**)&m_pTexture_GrowthButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_NameFrame"),
        TEXT("Com_Texture_ItemName"), (CComponent**)&m_pTexture_ItemName)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Next_Grade"),
        TEXT("Com_Texture_NextLevelIcon"), (CComponent**)&m_pTexture_NextLevelIcon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Ingredients_Frame"),
        TEXT("Com_Texture_Ingredients"), (CComponent**)&m_pTexture_Item_Ingredients)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Button"),
        TEXT("Com_Texture_UpgradeButton"), (CComponent**)&m_pTexture_UpgradeButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_Slot"),
        TEXT("Com_Texture_ItemFrame"), (CComponent**)&m_pTexture_ItemSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Rank"),
        TEXT("Com_Texture_ItemGrade"), (CComponent**)&m_pTexture_ItemGrade)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Ingredient"),
        TEXT("Com_Texture_IngredientSlot"), (CComponent**)&m_pTexture_IngredientSlot)))
        return E_FAIL;
    //m_pTexture_UpgradeStone
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_stone"),
        TEXT("Com_Texture_UpgradeStone"), (CComponent**)&m_pTexture_UpgradeStone)))
        return E_FAIL;
    //m_pTexture_Fragment
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Fragment"),
        TEXT("Com_Texture_Fragment"), (CComponent**)&m_pTexture_Fragment)))
        return E_FAIL;
    //m_pTexture_FusionStone
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Fusion_stone"),
        TEXT("Com_Texture_FusionStone"), (CComponent**)&m_pTexture_FusionStone)))
        return E_FAIL;
    //m_pTexture_LeapStone
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_leap_stone"),
        TEXT("Com_Texture_LeapStone"), (CComponent**)&m_pTexture_LeapStone)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Side_Pannel"),
        TEXT("Com_Texture_SidePannel"), (CComponent**)&m_pTexture_SidePannel)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Side_Wnd"),
        TEXT("Com_Texture_SideWnd"), (CComponent**)&m_pTexture_SideWnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_NPC_Speech_BackWnd"),
        TEXT("Com_Texture_NpcSpeech_BackGround"), (CComponent**)&m_pTexture_NpcSpeech_BackGround)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Npc_SpeechWnd"),
        TEXT("Com_Texture_NpcSpeech_Wnd"), (CComponent**)&m_pTexture_NpcSpeech_Wnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Quit_Button"),
        TEXT("Com_Texture_QuitButton"), (CComponent**)&m_pTexture_QuitButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Max_GaugeEffect"),
        TEXT("Com_Texturem_MaxGaugeEffect"), (CComponent**)&m_pTexture_MaxGaugeEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_ItemBuild"),
        TEXT("Com_Texturem_GaugeSpin"), (CComponent**)&m_pTexture_GaugeSpin)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Result_Waiting"),
        TEXT("Com_Texturem_ResultWaiting"), (CComponent**)&m_pTexture_ResultWaiting)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Mask_Effect0"),
        TEXT("Com_Texturem_ResultEffect_Success"), (CComponent**)&m_pTexture_ResultEffect_Success)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Mask_Effect1"),
        TEXT("Com_Texturem_ResultEffect_Failed"), (CComponent**)&m_pTexture_ResultEffect_Failed)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Button_Effect"),
        TEXT("Com_Texturem_UpgradeButton_Effect"), (CComponent**)&m_pTexture_UpgradeButton_Effect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Succese_Frame"),
        TEXT("Com_Texturem_ResultWnd"), (CComponent**)&m_pTexture_ResultWnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_Result_Effect"),
        TEXT("Com_Texturem_ResultItemEffect"), (CComponent**)&m_pTexture_ResultItemEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_Slot"),
        TEXT("Com_Texturem_ResultItemSlot"), (CComponent**)&m_pTexture_ResultItemSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_ResultCheck_Button"),
        TEXT("Com_Texturem_ResultCheckButton"), (CComponent**)&m_pTexture_ResultCheckButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_ResultText_Effect"),
        TEXT("Com_Texturem_ResultTextEffect"), (CComponent**)&m_pTexture_ResultTextEffect)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_HammerCap"), (CComponent**)&m_pTransform_HammerCap)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_HammerEffect"), (CComponent**)&m_pTransform_HammerEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_HammerAura"), (CComponent**)&m_pTransform_HammerAura)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Gauge"), (CComponent**)&m_pTransform_Gauge)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GaugeSmoke"), (CComponent**)&m_pTransform_GaugeSmoke)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GaugeEffect"), (CComponent**)&m_pTransform_GaugeEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GrowthButton"), (CComponent**)&m_pTransform_GrowthButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemName"), (CComponent**)&m_pTransform_ItemName)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_NextLevelIcon"), (CComponent**)&m_pTransform_NextLevelIcon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Ingredients"), (CComponent**)&m_pTransform_Ingredients)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeButton"), (CComponent**)&m_pTransform_UpgradeButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemSlot"), (CComponent**)&m_pTransform_ItemSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Item"), (CComponent**)&m_pTransform_Item)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_IngredientSlotL"), (CComponent**)&m_pTransform_IngredientSlotL)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_IngredientSlot"), (CComponent**)&m_pTransform_IngredientSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_IngredientSlotR"), (CComponent**)&m_pTransform_IngredientSlotR)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_SidePannel"), (CComponent**)&m_pTransform_SidePannel)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_SideWnd"), (CComponent**)&m_pTransform_SideWnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_BackGround"), (CComponent**)&m_pTransform_NpcSpeech_BackGround)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_NpcSpeech_Wnd"), (CComponent**)&m_pTransform_NpcSpeech_Wnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_QuitButton"), (CComponent**)&m_pTransform_QuitButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_Result"), (CComponent**)&m_pTransform_Result)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_MaxGaugeEffect"), (CComponent**)&m_pTransform_MaxGaugeEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultEffect"), (CComponent**)&m_pTransform_ResultEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultWnd"), (CComponent**)&m_pTransform_ResultWnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultItemEffect"), (CComponent**)&m_pTransform_ResultItemEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultItemSlot"), (CComponent**)&m_pTransform_ResultItemSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultItem"), (CComponent**)&m_pTransform_ResultItem)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultCheckButton"), (CComponent**)&m_pTransform_ResultCheckButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ResultTextEffect"), (CComponent**)&m_pTransform_ResultTextEffect)))
        return E_FAIL;
    //m_pTransform_Ingredient
    //m_pTransform_IngredientWnd
    for (size_t i = 0; i < 3; i++)
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
            TEXT("Com_Transform_Ingredient") + to_wstring(i), (CComponent**)&m_pTransform_Ingredient[i])))
            return E_FAIL;

        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
            TEXT("Com_Transform_IngredientWnd") + to_wstring(i), (CComponent**)&m_pTransform_IngredientWnd[i])))
            return E_FAIL;
    }

    if (FAILED(Ready_Components_SidePannel_L()))
        return E_FAIL;

    if (FAILED(Ready_Components_SidePannel_R()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Ready_Components_SidePannel_L()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Rank"),
        TEXT("Com_Texturem_EquipItemWnd"), (CComponent**)&m_pTexture_EquipItemWnd)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Icon"),
        TEXT("Com_Texturem_UpgradeIcon"), (CComponent**)&m_pTexture_UpgradeIcon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Equipment_Icon"),
        TEXT("Com_Texturem_EquipIcon"), (CComponent**)&m_pTexture_EquipIcon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Selected_Item"),
        TEXT("Com_Texturem_CurrItem"), (CComponent**)&m_pTexture_CurrItemSlot)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Face"), (CComponent**)&m_pTransform_EquipItemWnd_Face)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Face"), (CComponent**)&m_pTransform_ItemIcon_Face)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Face"), (CComponent**)&m_pTransform_UpgradeIcon_Face)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Face"), (CComponent**)&m_pTransform_EquipIcon_Face)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Helemt"), (CComponent**)&m_pTransform_EquipItemWnd_Helemt)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Helemt"), (CComponent**)&m_pTransform_ItemIcon_Helemt)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Helemt"), (CComponent**)&m_pTransform_UpgradeIcon_Helemt)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Helemt"), (CComponent**)&m_pTransform_EquipIcon_Helemt)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Shoulder"), (CComponent**)&m_pTransform_EquipItemWnd_Shoulder)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Shoulder"), (CComponent**)&m_pTransform_ItemIcon_Shoulder)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Shoulder"), (CComponent**)&m_pTransform_UpgradeIcon_Shoulder)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Shoulder"), (CComponent**)&m_pTransform_EquipIcon_Shoulder)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Body"), (CComponent**)&m_pTransform_EquipItemWnd_Body)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Body"), (CComponent**)&m_pTransform_ItemIcon_Body)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Body"), (CComponent**)&m_pTransform_UpgradeIcon_Body)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Body"), (CComponent**)&m_pTransform_EquipIcon_Body)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Arm"), (CComponent**)&m_pTransform_EquipItemWnd_Arm)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Arm"), (CComponent**)&m_pTransform_ItemIcon_Arm)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Arm"), (CComponent**)&m_pTransform_UpgradeIcon_Arm)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Arm"), (CComponent**)&m_pTransform_EquipIcon_Arm)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Leg"), (CComponent**)&m_pTransform_EquipItemWnd_Leg)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Leg"), (CComponent**)&m_pTransform_ItemIcon_Leg)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Leg"), (CComponent**)&m_pTransform_UpgradeIcon_Leg)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Leg"), (CComponent**)&m_pTransform_EquipIcon_Leg)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipItemWnd_Weapon"), (CComponent**)&m_pTransform_EquipItemWnd_Weapon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Weapon"), (CComponent**)&m_pTransform_ItemIcon_Weapon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Weapon"), (CComponent**)&m_pTransform_UpgradeIcon_Weapon)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Weapon"), (CComponent**)&m_pTransform_EquipIcon_Weapon)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Ready_Components_SidePannel_R()
{
    //m_pTexture_GradeEffectWnd
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_ListFrame"),
        TEXT("Com_Texturem_GradeEffectWnd"), (CComponent**)&m_pTexture_GradeEffectWnd)))
        return E_FAIL;
    //m_pTexture_DragLine
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_DragLine"),
        TEXT("Com_Texturem_DragLine"), (CComponent**)&m_pTexture_DragLine)))
        return E_FAIL;
    //m_pTexture_DragBar
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_DragBar"),
        TEXT("Com_Texturem_DragBar"), (CComponent**)&m_pTexture_DragBar)))
        return E_FAIL;
  
    //m_pTransform_GradeEffect
    for (size_t i = 0; i < 7; i++)
    {
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
            TEXT("Com_Transform_GradeEffect") + to_wstring(i), (CComponent**)&m_pTransform_GradeEffect[i])))
            return E_FAIL;
    }
    //m_pTransform_DragLine
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_DragLine"), (CComponent**)&m_pTransform_DragLine)))
        return E_FAIL;
    //m_pTransform_DragBar
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_DragBar"), (CComponent**)&m_pTransform_DragBar)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources()
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

    if(FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_HammerCap()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_HammerCap->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_HammerCap->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_HammerAura()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_HammerEffect->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
  
        if ((m_bMaxGauge)&&(!m_bDecreaseEffect))
        {
            if (FAILED(m_pTexture_HammerAura->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame)))
                return E_FAIL;
        }
        else
        {
            if(FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
                return E_FAIL;
        }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_HammerEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_HammerAura->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fHammer_Effect_Alpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (m_bMaxGauge)
    {
        if (FAILED(m_pTexture_HammerEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_Gauge()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Gauge->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fItemGrowthRatio, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pTexture_Gauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GaugeEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GaugeEffect->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(m_bMaxGauge)
    {
        if (FAILED(m_pTexture_GaugeEffect->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_GaugeEffect)))
            return E_FAIL;
    }
    else
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GaugeSmoke()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GaugeSmoke->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_GaugeSmoke->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_Smoke)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GaugeSpin()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GaugeSmoke->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    _float fAlpha = 0.8f;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
        return E_FAIL;
    Vec4 fColor = Vec4(2.f, 2.f, 1.f, 1.f);
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &fColor, sizeof(Vec4))))
        return E_FAIL;
    if ((m_bMaxGauge)&&(!m_bDecreaseEffect))
    {
        if (FAILED(m_pTexture_GaugeSpin->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_GaugeSpin)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GrowthButton()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GrowthButton->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if(FAILED(m_pTexture_GrowthButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iGrowthButton_TextureIndex)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemName()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemName->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if(FAILED(m_pTexture_ItemName->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_NextLevelIcon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_NextLevelIcon->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (nullptr != m_pCurrUpgradeItem)
    {
        if (FAILED(m_pTexture_NextLevelIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_Ingredients()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Ingredients->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_Item_Ingredients->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeButton()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeButton->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_UpgradeButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iUpgradeButton_TextureIndex)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeButtonEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeButton->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha_UpgradeButton_Effect, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_UpgradeButton_Effect->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemFrame()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemSlot->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_ItemSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_CurrUpgradeItem_Grade()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Item->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_Item)
    {
        if(FAILED(m_pTexture_ItemGrade->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iCurrItemGrade)))
            return E_FAIL;
    }
    else
    {
        if(FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_CurrUpgradeItemIcon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Item->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_Item)
    {
        if(FAILED(m_pTexture_Item->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_MaxGaugeEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_MaxGaugeEffect->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (m_bMaxGaugeEffect)
    {
        if(FAILED(m_pTexture_MaxGaugeEffect->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_MaxGaugeEffect)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_IngredientWnd_L()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IngredientWnd[0]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_ItemGrade->Set_SRV(m_pShaderCom, "g_DiffuseTexture",2)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_IngredientWnd()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IngredientWnd[1]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_ItemGrade->Set_SRV(m_pShaderCom, "g_DiffuseTexture",2)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_IngredientWnd_R()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IngredientWnd[2]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_ItemGrade->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 2)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_Ingredient_L()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Ingredient[0]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_UpgradeStone->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_Ingredient()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Ingredient[1]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_LeapStone->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_Ingredient_R()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Ingredient[2]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_FusionStone->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_IngredientSlotL()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IngredientSlotL->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_IngredientSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_IngredientSlot()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IngredientSlot->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_IngredientSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_IngredientSlotR()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IngredientSlotR->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_IngredientSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_SidePannel()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_SidePannel->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_SidePannel->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_SideWnd()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_SideWnd->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_SideWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_NpcSpeech_BackGround()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_NpcSpeech_BackGround->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_NpcSpeech_BackGround->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_NpcSpeech_Wnd()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_NpcSpeech_Wnd->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_NpcSpeech_Wnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_QuitButton()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_QuitButton->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_QuitButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iQuitButton_TextureIndex)))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultWaiting()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Result->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha_ResultWaiting, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (m_bResultWaiting)
    {
        if (FAILED(m_pTexture_ResultWaiting->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame_ResultWaiting)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultEffect->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    Vec4 vColor = Vec4(2.f, 2.f, 2.f, 1.f);
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((m_bResult) && (m_bResultSuccess))
    {
        if (FAILED(m_pTexture_ResultEffect_Success->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_ResultEffect)))
            return E_FAIL;
    }
    else if ((m_bResult) && (!m_bResultSuccess))
    {
        if (FAILED(m_pTexture_ResultEffect_Failed->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_ResultEffect)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultWnd()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultWnd->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (m_bResultWnd)
    {
        if (FAILED(m_pTexture_ResultWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultItemEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultItemEffect->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (m_bResultWnd)
    {
        if (FAILED(m_pTexture_ResultItemEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_bResultSuccess)))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultItemSlot()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultItemSlot->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (m_bResultWnd)
    {
        if (FAILED(m_pTexture_ResultItemSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultItemGrade()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultItem->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((m_bResultWnd)&&(nullptr != m_pTexture_Item))
    {
        if (FAILED(m_pTexture_ItemGrade->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iCurrItemGrade)))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultItemIcon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultItem->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((m_bResultWnd) && (nullptr != m_pTexture_Item))
    {
        if (FAILED(m_pTexture_Item->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultCheckButton()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultCheckButton->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (m_bResultWnd)
    {
        if (FAILED(m_pTexture_ResultCheckButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iUpgradeButton_TextureIndex)))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultTextEffect()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResultTextEffect->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((m_bResultWnd)&&(m_bResultSuccess))
    {
        if (FAILED(m_pTexture_ResultTextEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Face()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Face->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorFaceItem, sizeof(Vec4))))
        return E_FAIL;
    
    if (nullptr != m_pTexture_ItemIcon[(_uint)CItem::PART::FACE])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)CItem::PART::FACE])))
            return E_FAIL;
    }
    else
    {
    if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Face()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Face->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::FACE)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::FACE)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Face()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Face->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::FACE)])&&(100.f <= m_pEquips[(_uint)(CItem::PART::FACE)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Face()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Face->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::FACE)])
    {
        if (FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Helemt()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Helemt->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorHelemtItem, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::HELMET)])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)(CItem::PART::HELMET)])))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Helemt()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Helemt->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::HELMET)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::HELMET)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Helemt()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Helemt->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::HELMET)])&& (100.f <= m_pEquips[(_uint)(CItem::PART::HELMET)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Helemt()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Helemt->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::HELMET)])
    {
        if (FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Shoulder()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Shoulder->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorShoulderItem, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::SHOULDER)])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)(CItem::PART::SHOULDER)])))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Shoulder()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Shoulder->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::SHOULDER)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::SHOULDER)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Shoulder()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Shoulder->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::SHOULDER)])&& (100.f <= m_pEquips[(_uint)(CItem::PART::SHOULDER)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Shoulder()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Shoulder->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::SHOULDER)])
    {
        if (FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Body()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Body->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorBodyItem, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::BODY)])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)(CItem::PART::BODY)])))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Body()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Body->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::BODY)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::BODY)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Body()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Body->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::BODY)])&& (100.f <= m_pEquips[ (_uint)(CItem::PART::BODY)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Body()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Body->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::BODY)])
    {
        if (FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Arm()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Arm->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::ARM)])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)(CItem::PART::ARM)])))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Arm()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Arm->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::ARM)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::ARM)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Arm()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Arm->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::ARM)])&& (100.f <= m_pEquips[(_uint)(CItem::PART::ARM)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Arm()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Arm->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::ARM)])
    {
        if (FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Leg()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Leg->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::LEG)])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)(CItem::PART::LEG)])))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Leg()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Leg->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::LEG)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::LEG)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Leg()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Leg->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::LEG)])&& (100.f <= m_pEquips[(_uint)(CItem::PART::LEG)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
        {
            if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
                return E_FAIL;
        }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Leg()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Leg->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::LEG)])
    {
        if(FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if(FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Weapon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Weapon->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::WEAPON)])
    {
        if (FAILED(m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex[(_uint)(CItem::PART::WEAPON)])))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Weapon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ItemIcon_Weapon->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::WEAPON)])
    {
        if (FAILED(m_pTexture_ItemIcon[(_uint)(CItem::PART::WEAPON)]->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Weapon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Weapon->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if ((nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::WEAPON)])&& (100.f <= m_pEquips[(_uint)(CItem::PART::WEAPON)]->Get_UpgradeGauge()))
    {
        if (FAILED(m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Weapon()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Weapon->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (nullptr != m_pTexture_ItemIcon[(_uint)(CItem::PART::WEAPON)])
    {
        if (FAILED(m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_NoneImg()
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
    if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_SidePannel_L()
{
    if (FAILED(Bind_ShaderResources_SidePannel()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Face
     if (FAILED(Bind_ShaderResources_EquipItemWnd_Face()))
       return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_ItemIcon_Face()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    if (FAILED(Bind_ShaderResources_UpgradeIcon_Face()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    if (FAILED(Bind_ShaderResources_EquipIcon_Face()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Helemt
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Helemt()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

     if (FAILED(Bind_ShaderResources_ItemIcon_Helemt()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();
    
     if (FAILED(Bind_ShaderResources_UpgradeIcon_Helemt()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_EquipIcon_Helemt()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Shoulder
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Shoulder()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_ItemIcon_Shoulder()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();
    
     if (FAILED(Bind_ShaderResources_UpgradeIcon_Shoulder()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_EquipIcon_Shoulder()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Body
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Body()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_ItemIcon_Body()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();
    
     if (FAILED(Bind_ShaderResources_UpgradeIcon_Body()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_EquipIcon_Body()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Arm
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Arm()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_ItemIcon_Arm()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();
    
     if (FAILED(Bind_ShaderResources_UpgradeIcon_Arm()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_EquipIcon_Arm()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Leg
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Leg()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_ItemIcon_Leg()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();
    
     if (FAILED(Bind_ShaderResources_UpgradeIcon_Leg()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    
    if (FAILED(Bind_ShaderResources_EquipIcon_Leg()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_SidePannel_R()
{
    if (FAILED(Bind_ShaderResources_SideWnd()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_First()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_Second()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_Third()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_Fourth()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_Fifth()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_Sixth()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GradeEffectWnd_Seventh()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    /*if (FAILED(Bind_ShaderResources_DragLine()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_DragBar()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();*/

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_First()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[0]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if(FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Second()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[1]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Third()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[2]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Fourth()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[3]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    
    if (FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
    return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Fifth()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[4]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Sixth()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[5]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Seventh()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect[6]->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_DragLine()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragLine->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_DragLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_DragBar()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragBar->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pTexture_DragBar->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_CurrItem()
{
    switch (m_iCurrItem)
    {
    case SELECTED_FACE:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Face->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_HELMET:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Helemt->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_SHOULDER:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Shoulder->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_BODY:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Body->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_ARM:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Arm->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_LEG:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Leg->Get_WorldMatrix())))
            return E_FAIL;
        break;
    default:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Face->Get_WorldMatrix())))
            return E_FAIL;
        break;
    }
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    if (SELECTED_END != m_iCurrItem)
    {
        if (FAILED(m_pTexture_CurrItemSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    else
    {
        if (FAILED(m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
            return E_FAIL;
    }
    return S_OK;
}

CUI_NPC_ItemUpgrade* CUI_NPC_ItemUpgrade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_NPC_ItemUpgrade* pInstance = new CUI_NPC_ItemUpgrade(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_NPC_ItemUpgrade");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_NPC_ItemUpgrade::Clone(void* pArg)
{
    CUI_NPC_ItemUpgrade* pInstance = new CUI_NPC_ItemUpgrade(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_NPC_ItemUpgrade");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_NPC_ItemUpgrade::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    if(nullptr != m_pItemNameWnd)
        m_pItemNameWnd->Set_Dead(true);
    if (nullptr != m_pCurrItemNameWnd)
        m_pCurrItemNameWnd->Set_Dead(true);
    if (nullptr != m_pCurrGaugeWnd)
        m_pCurrGaugeWnd->Set_Dead(true);
    if (nullptr != m_pResultWnd)
        m_pResultWnd->Set_Dead(true);
    if(nullptr != m_pNpcScriptWnd)
        m_pNpcScriptWnd->Set_Dead(true);

    Safe_Release(m_pTexture_None);
    Safe_Release(m_pTexture_HammerCap);
    Safe_Release(m_pTexture_HammerEffect);
    Safe_Release(m_pTexture_HammerAura);
    Safe_Release(m_pTexture_GaugeSmoke);
    Safe_Release(m_pTexture_GaugeEffect);
    Safe_Release(m_pTexture_GrowthButton);
    Safe_Release(m_pTexture_ItemName);
    Safe_Release(m_pTexture_NextLevelIcon);
    Safe_Release(m_pTexture_ItemGrade);
    Safe_Release(m_pTexture_Item_Ingredients);
    Safe_Release(m_pTexture_UpgradeButton);
    Safe_Release(m_pTexture_ItemSlot);
    Safe_Release(m_pTexture_IngredientSlot);
    Safe_Release(m_pTexture_SidePannel);
    Safe_Release(m_pTexture_SideWnd);
    Safe_Release(m_pTexture_NpcSpeech_BackGround);
    Safe_Release(m_pTexture_NpcSpeech_Wnd);
    Safe_Release(m_pTexture_QuitButton);
    Safe_Release(m_pTexture_ResultWaiting);
    Safe_Release(m_pTexture_ResultFailed);
    Safe_Release(m_pTexture_ResultSuccess);
    Safe_Release(m_pTexture_MaxGaugeEffect);
    Safe_Release(m_pTexture_GaugeSpin);
    Safe_Release(m_pTexture_ResultEffect_Success);
    Safe_Release(m_pTexture_ResultEffect_Failed);
    Safe_Release(m_pTexture_UpgradeButton_Effect);
    Safe_Release(m_pTexture_ResultWnd);
    Safe_Release(m_pTexture_ResultItemEffect);
    Safe_Release(m_pTexture_ResultItemSlot);
    Safe_Release(m_pTexture_ResultCheckButton);
    Safe_Release(m_pTexture_ResultItem);
    Safe_Release(m_pTexture_ResultTextEffect);
    Safe_Release(m_pTexture_Fragment);
    Safe_Release(m_pTexture_UpgradeStone);
    Safe_Release(m_pTexture_FusionStone);
    Safe_Release(m_pTexture_LeapStone);

    Safe_Release(m_pTransform_HammerCap);
    Safe_Release(m_pTransform_HammerEffect);
    Safe_Release(m_pTransform_HammerAura);
    Safe_Release(m_pTransform_Gauge);
    Safe_Release(m_pTransform_GaugeSmoke);
    Safe_Release(m_pTransform_GaugeEffect);
    Safe_Release(m_pTransform_GrowthButton);
    Safe_Release(m_pTransform_ItemName);
    Safe_Release(m_pTransform_NextLevelIcon);
    Safe_Release(m_pTransform_Ingredients);
    Safe_Release(m_pTransform_UpgradeButton);
    Safe_Release(m_pTransform_ItemSlot);
    Safe_Release(m_pTransform_IngredientSlotL);
    Safe_Release(m_pTransform_IngredientSlot);
    Safe_Release(m_pTransform_IngredientSlotR);
    Safe_Release(m_pTransform_SidePannel);
    Safe_Release(m_pTransform_SideWnd);
    Safe_Release(m_pTransform_NpcSpeech_BackGround);
    Safe_Release(m_pTransform_NpcSpeech_Wnd);
    Safe_Release(m_pTransform_QuitButton);
    Safe_Release(m_pTransform_Result);
    Safe_Release(m_pTransform_MaxGaugeEffect);
    Safe_Release(m_pTransform_ResultEffect);
    Safe_Release(m_pTransform_ResultItemEffect);
    Safe_Release(m_pTransform_ResultItemSlot);
    Safe_Release(m_pTransform_ResultItem);
    Safe_Release(m_pTransform_ResultCheckButton);
    Safe_Release(m_pTransform_ResultTextEffect);
    for (size_t i = 0; i < 3; i++)
    {
        Safe_Release(m_pTransform_Ingredient[i]);
        Safe_Release(m_pTransform_IngredientWnd[i]);
    }

    Free_Side_Pannel_L();
    Free_Side_Pannel_R();

}

void CUI_NPC_ItemUpgrade::Free_Side_Pannel_L()
{
    Safe_Release(m_pTexture_EquipItemWnd);
    Safe_Release(m_pTexture_UpgradeIcon);
    Safe_Release(m_pTexture_EquipIcon);
    for (size_t i = 0; i < (_uint)(CItem::PART::_END); i++)
    {
        if (nullptr != m_pTexture_ItemIcon[i])
            Safe_Release(m_pTexture_ItemIcon[i]);
    }
    Safe_Release(m_pTexture_CurrItemSlot);

    Safe_Release(m_pTransform_EquipItemWnd_Face);
    Safe_Release(m_pTransform_ItemIcon_Face);
    Safe_Release(m_pTransform_UpgradeIcon_Face);
    Safe_Release(m_pTransform_EquipIcon_Face);
    Safe_Release(m_pTransform_EquipItemWnd_Helemt);
    Safe_Release(m_pTransform_ItemIcon_Helemt);
    Safe_Release(m_pTransform_UpgradeIcon_Helemt);
    Safe_Release(m_pTransform_EquipIcon_Helemt);
    Safe_Release(m_pTransform_EquipItemWnd_Shoulder);
    Safe_Release(m_pTransform_ItemIcon_Shoulder);
    Safe_Release(m_pTransform_UpgradeIcon_Shoulder);
    Safe_Release(m_pTransform_EquipIcon_Shoulder);
    Safe_Release(m_pTransform_EquipItemWnd_Body);
    Safe_Release(m_pTransform_ItemIcon_Body);
    Safe_Release(m_pTransform_UpgradeIcon_Body);
    Safe_Release(m_pTransform_EquipIcon_Body);
    Safe_Release(m_pTransform_EquipItemWnd_Arm);
    Safe_Release(m_pTransform_UpgradeIcon_Arm);
    Safe_Release(m_pTransform_EquipIcon_Arm);
    Safe_Release(m_pTransform_ItemIcon_Arm);
    Safe_Release(m_pTransform_EquipItemWnd_Leg);
    Safe_Release(m_pTransform_EquipIcon_Leg);
    Safe_Release(m_pTransform_ItemIcon_Leg);
    Safe_Release(m_pTransform_UpgradeIcon_Leg);
    Safe_Release(m_pTransform_EquipItemWnd_Weapon);
    Safe_Release(m_pTransform_EquipIcon_Weapon);
    Safe_Release(m_pTransform_ItemIcon_Weapon);
    Safe_Release(m_pTransform_UpgradeIcon_Weapon);
}

void CUI_NPC_ItemUpgrade::Free_Side_Pannel_R()
{
    Safe_Release(m_pTexture_GradeEffectWnd);

    for (size_t i = 0; i < 7; i++)
    {
        Safe_Release(m_pTransform_GradeEffect[i]);
    }
    Safe_Release(m_pTexture_DragLine);
    Safe_Release(m_pTexture_DragBar);
    Safe_Release(m_pTransform_DragLine);
    Safe_Release(m_pTransform_DragBar);
}
