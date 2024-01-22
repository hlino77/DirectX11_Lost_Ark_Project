#include "stdafx.h"
#include "UI_NPC_ItemUpgrade.h"
#include "GameInstance.h"
#include "TextBox.h"

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
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_HammerCap
    m_pTransform_HammerCap->Set_Scale(Vec3(423.f * 0.8f, 207.f * 0.8f, 1.f));
    m_pTransform_HammerCap->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 30.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_HammerEffect
    m_pTransform_HammerEffect->Set_Scale(Vec3(556.f * 0.8f, 374.f * 0.8f, 1.f));
    m_pTransform_HammerEffect->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 10.f) - g_iWinSizeX * 0.5f, -((m_fY - 88.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_HammerAura
    m_pTransform_HammerAura->Set_Scale(Vec3(669.f * 0.8f, 417.f * 0.8f, 1.f));
    m_pTransform_HammerAura->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 72.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_Gauge
    m_pTransform_Gauge->Set_Scale(Vec3(110.f * 0.8f, 110.f * 0.8f, 1.f));
    m_pTransform_Gauge->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GaugeEffect
    m_pTransform_GaugeEffect->Set_Scale(Vec3(200.f * 0.8f, 200.f * 0.8f, 1.f));
    m_pTransform_GaugeEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 45.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GaugeSmoke
    m_pTransform_GaugeSmoke->Set_Scale(Vec3(152.f * 0.8f, 152.f * 0.8f, 1.f));
    m_pTransform_GaugeSmoke->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GrowthButton
    m_pTransform_GrowthButton->Set_Scale(Vec3(108.f * 0.8f, 44.f * 0.8f, 1.f));
    m_pTransform_GrowthButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 30.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ItemName
    m_pTransform_ItemName->Set_Scale(Vec3(566.f * 0.8f, 98.f * 0.8f, 1.f));
    m_pTransform_ItemName->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 4.f) - g_iWinSizeX * 0.5f, -(m_fY - 140.f) + g_iWinSizeY * 0.5f, 0.f));
    ////m_pTransform_Ingredients
    m_pTransform_Ingredients->Set_Scale(Vec3(478.f * 0.8f, 189.f * 0.8f, 1.f));
    m_pTransform_Ingredients->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 4.f) - g_iWinSizeX * 0.5f, -((m_fY + 160.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_UpgradeButton
    m_pTransform_UpgradeButton->Set_Scale(Vec3(221.f * 0.8f, 42.f * 0.8f, 1.f));
    m_pTransform_UpgradeButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 264.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ItemSlot
    m_pTransform_ItemSlot->Set_Scale(Vec3(64.f * 0.8f, 64.f * 0.8f, 1.f));
    m_pTransform_ItemSlot->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 220.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_IngredientSlotL
    m_pTransform_IngredientSlotL->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlotL->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - 100.f)) - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_IngredientSlot
    m_pTransform_IngredientSlot->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlot->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_IngredientSlotR
    m_pTransform_IngredientSlotR->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlotR->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 100.f)) - g_iWinSizeX * 0.5f, -(m_fY + 155.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_SidePannel
    m_pTransform_SidePannel->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
    m_pTransform_SidePannel->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - 366.f)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_SideWnd
    m_pTransform_SideWnd->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
    m_pTransform_SideWnd->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 366.f)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_NpcSpeech_BackGround
    m_pTransform_NpcSpeech_BackGround->Set_Scale(Vec3(1620.f, 200.f, 1.f));
    m_pTransform_NpcSpeech_BackGround->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY - 80.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_NpcSpeech_Wnd
    m_pTransform_NpcSpeech_Wnd->Set_Scale(Vec3(g_iWinSizeX * 0.5f, 80.f * 0.8f, 1.f));
    m_pTransform_NpcSpeech_Wnd->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 10.f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY - 80.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_QuitButton
    m_pTransform_QuitButton->Set_Scale(Vec3(205.f * 0.8f, 48.f * 0.8f, 1.f));
    m_pTransform_QuitButton->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 700.f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY - 24.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_Result
    m_pTransform_Result->Set_Scale(Vec3(1092.f, 581.f, 1.f));
    m_pTransform_Result->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_MaxGaugeEffect
    m_pTransform_MaxGaugeEffect->Set_Scale(Vec3(200.f * 0.8f, 200.f * 0.8f, 1.f));
    m_pTransform_MaxGaugeEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 55.f)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ResultEffect
    m_pTransform_ResultEffect->Set_Scale(Vec3(500.f, 500.f, 1.f));
    m_pTransform_ResultEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY)) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ResultWnd
    m_pTransform_ResultWnd->Set_Scale(Vec3(1092.f, 581.f, 1.f));
    m_pTransform_ResultWnd->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ResultItemEffect
    m_pTransform_ResultItemEffect->Set_Scale(Vec3(128.f * 5.f, 60.f * 5.f, 1.f));
    m_pTransform_ResultItemEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 98.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ResultItemSlot
    m_pTransform_ResultItemSlot->Set_Scale(Vec3(64.f * 0.8f, 64.f * 0.8f, 1.f));
    m_pTransform_ResultItemSlot->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 2.f) - g_iWinSizeX * 0.5f, -(m_fY - 98.f) + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_Transform_SidePannel_L()
{
    //m_pTransform_EquipItemWnd_Head
    m_pTransform_EquipItemWnd_Head->Set_Scale(Vec3(323.f*0.9f , 60.f, 1.f));
    m_pTransform_EquipItemWnd_Head->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 200.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_ItemIcon_Head
    m_pTransform_ItemIcon_Head->Set_Scale(Vec3(m_fSizeX * 0.8f, m_fSizeY * 0.8f, 1.f));
    m_pTransform_ItemIcon_Head->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 400.f) - g_iWinSizeX * 0.5f, -(m_fY - 200.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_EquipIcon_Head
    m_pTransform_EquipIcon_Head->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Head->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 215.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_UpgradeIcon_Head
    m_pTransform_UpgradeIcon_Head->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.8f, 1.f));
    m_pTransform_UpgradeIcon_Head->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 185.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_EquipItemWnd_Body
    m_pTransform_EquipItemWnd_Body->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 135.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_ItemIcon_Body
    m_pTransform_ItemIcon_Body->Set_Scale(Vec3(m_fSizeX * 0.8f, m_fSizeY * 0.8f, 1.f));
    m_pTransform_ItemIcon_Body->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 135.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_UpgradeIcon_Body
    m_pTransform_UpgradeIcon_Body->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.8f, 1.f));
    m_pTransform_UpgradeIcon_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 120.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_EquipIcon_Body
    m_pTransform_EquipIcon_Body->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Body->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 150.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_EquipItemWnd_Leg
    m_pTransform_EquipItemWnd_Leg->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 75.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_ItemIcon_Leg
    m_pTransform_ItemIcon_Leg->Set_Scale(Vec3(m_fSizeX * 0.8f, m_fSizeY * 0.8f, 1.f));
    m_pTransform_ItemIcon_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 75.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_UpgradeIcon_Leg
    m_pTransform_UpgradeIcon_Leg->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.8f, 1.f));
    m_pTransform_UpgradeIcon_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 60.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_EquipIcon_Leg
    m_pTransform_EquipIcon_Leg->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Leg->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 90.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_EquipItemWnd_Weapon
    m_pTransform_EquipItemWnd_Weapon->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_EquipItemWnd_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 380.f) - g_iWinSizeX * 0.5f, -(m_fY - 15.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_ItemIcon_Weapon
    m_pTransform_ItemIcon_Weapon->Set_Scale(Vec3(m_fSizeX * 0.8f, m_fSizeY * 0.8f, 1.f));
    m_pTransform_ItemIcon_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 15.f) + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_UpgradeIcon_Weapon
    m_pTransform_UpgradeIcon_Weapon->Set_Scale(Vec3(26.f * 0.8f, 22.f * 0.8f, 1.f));
    m_pTransform_UpgradeIcon_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
   //m_pTransform_EquipIcon_Weapon
    m_pTransform_EquipIcon_Weapon->Set_Scale(Vec3(24.f, 24.f, 1.f));
    m_pTransform_EquipIcon_Weapon->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 250.f) - g_iWinSizeX * 0.5f, -(m_fY - 30.f) + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Initialize_Transform_SidePannel_R()
{
    //m_pTransform_GradeEffect_First
    m_pTransform_GradeEffect_First->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_GradeEffect_First->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 370.f)) - g_iWinSizeX * 0.5f, -(m_fY - 190.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GradeEffect_Second
    m_pTransform_GradeEffect_Second->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_GradeEffect_Second->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 370.f)) - g_iWinSizeX * 0.5f, -(m_fY - 125.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GradeEffect_Third
    m_pTransform_GradeEffect_Third->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_GradeEffect_Third->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 370.f)) - g_iWinSizeX * 0.5f, -(m_fY - 60.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GradeEffect_Fourth
    m_pTransform_GradeEffect_Fourth->Set_Scale(Vec3(323.f * 0.9f, 60.f, 1.f));
    m_pTransform_GradeEffect_Fourth->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 370.f)) - g_iWinSizeX * 0.5f, -(m_fY + 5.f) + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CUI_NPC_ItemUpgrade::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

      if (KEY_TAP(KEY::B))//Test
    {
          m_bTestGaugeOn = !m_bTestGaugeOn;
          m_bMaxGaugeEffect = true;
          m_bDecreaseEffect = true;
    }

    if (m_bTestGaugeOn)
    {
        if (m_fItemGrowthMaxGauge > m_fItemGrowthCurrGauge)
            m_fItemGrowthCurrGauge += 100.f * fTimeDelta;
        else if (m_fItemGrowthMaxGauge < m_fItemGrowthCurrGauge)
        {
            m_bTestGaugeOn = false;
            m_fItemGrowthCurrGauge = m_fItemGrowthMaxGauge;
        }

        if (1.f > m_fHammer_Effect_Alpha)
            m_fHammer_Effect_Alpha += fTimeDelta;
        else if (1.f < m_fHammer_Effect_Alpha)
            m_fHammer_Effect_Alpha = 1.f;
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

    if (FAILED(Bind_ShaderResources_SidePannel_R()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerAura()))
        return E_FAIL;
    if(!m_bDecreaseEffect)
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
    if (!m_bDecreaseEffect)
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

    if (FAILED(Bind_ShaderResources_ResultItemSlot()))
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

void CUI_NPC_ItemUpgrade::Update_Button(_float fTimeDelta)
{
    Create_Rect_GrowthButton();
    Create_Rect_UpgradeButton();
    Create_Rect_QuitButton();

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);  // 클라이언트 내에 마우스 포인터 가져오기 
    _uint ViewPortIndex = 1;
    CD3D11_VIEWPORT ViewPort;
    m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // 뷰포트 가져오기 

    Upadate_GrowthButton(pt);
    Update_UpgradeButton(pt, fTimeDelta);
    Update_QuitButton(pt);
    Update_ResultCheckButton(pt);
    Update_Items(pt);
}

void CUI_NPC_ItemUpgrade::Upadate_GrowthButton(POINT pt)
{
    if ((m_bMaxGauge) || (m_fItemGrowthCurrGauge == m_fItemGrowthMaxGauge))
    {
        m_iGrowthButton_TextureIndex = 2;
        return;
    }

    Is_Picking_GrowthButton(pt);
    if ((KEY_TAP(KEY::LBTN)&&(m_bGrowthButton)))
    {
        m_bTestGaugeOn = true;
        m_bMaxGaugeEffect = true;
    }
}

void CUI_NPC_ItemUpgrade::Update_UpgradeButton(POINT pt, _float fTimeDelta)
{
    if ((!m_bMaxGauge) || (m_fItemGrowthMaxGauge > m_fItemGrowthCurrGauge ))
    {
        m_iUpgradeButton_TextureIndex = 2;
        return;
    }
    Is_Picking_UpgradeButton(pt);
    if ((m_bUpgradeButton) && (KEY_TAP(KEY::LBTN)))
    {
        m_bResultWaiting = true;
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
        Set_Active(false);
    }
}

void CUI_NPC_ItemUpgrade::Update_ResultCheckButton(POINT pt)
{
    if (!m_bResultWnd)
        return;
    Is_Picking_UpgradeButton(pt);
    if ((m_bUpgradeButton) && (KEY_TAP(KEY::LBTN)))
    {
        m_bResultWnd = false;
    }
}

void CUI_NPC_ItemUpgrade::Create_Rect_GrowthButton()
{
    m_rcGrowthButton.left = LONG(m_fX - ((108.f * 0.8f) / 2));
    m_rcGrowthButton.top = LONG((m_fY + 30.f) - ((44.f * 0.8f) / 2));
    m_rcGrowthButton.right = LONG(m_fX + ((108.f * 0.8f) / 2));
    m_rcGrowthButton.bottom = LONG((m_fY + 30.f) + ((44.f * 0.8f) / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_GrowthButton(POINT pt)
{
    if (PtInRect(&m_rcGrowthButton, pt))
    {
        m_iGrowthButton_TextureIndex = 1;
        m_bGrowthButton = true;
    }
    else
    {
        m_iGrowthButton_TextureIndex = 0;
        m_bGrowthButton = false;
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
        m_iUpgradeButton_TextureIndex = 1;
        m_bUpgradeButton = true;
    }
    else
    {
        m_iUpgradeButton_TextureIndex = 0;
        m_bUpgradeButton = false;
    }
}

void CUI_NPC_ItemUpgrade::Create_Rect_QuitButton()
{
    m_rcQuitButton.left = LONG((m_fX + 700.f) - ((205.f * 0.8f) / 2));
    m_rcQuitButton.top = LONG((g_iWinSizeY - 24.f) - ((48.f * 0.8f) / 2));
    m_rcQuitButton.right = LONG((m_fX + 700.f) + ((205.f * 0.8f) / 2));
    m_rcQuitButton.bottom = LONG((g_iWinSizeY - 24.f) + ((48.f * 0.8f) / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_QuitButton(POINT pt)
{
    if (PtInRect(&m_rcQuitButton, pt))
    {
        m_iQuitButton_TextureIndex = 1;
        m_bQuitButton = true;
    }
    else
    {
        m_iQuitButton_TextureIndex = 0;
        m_bQuitButton = false;
    }
}

void CUI_NPC_ItemUpgrade::Update_Items(POINT pt)
{
    Create_Rect_HeadItem();
    Create_Rect_BodyItem();
    Create_Rect_LegItem();
    Create_Rect_WeaponItem();

    Is_Picking_HeadItem(pt);
    Is_Picking_BodyItem(pt);
    Is_Picking_LegItem(pt);
    Is_Picking_WeaponItem(pt);

}

void CUI_NPC_ItemUpgrade::Create_Rect_HeadItem()
{
    m_rcHeadItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcHeadItem.top = LONG((m_fY - 200.f) - (60.f / 2));
    m_rcHeadItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcHeadItem.bottom = LONG((m_fY - 200.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_HeadItem(POINT pt)
{
    if (PtInRect(&m_rcHeadItem, pt))
    {
        m_vColorHeadItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            m_iCurrItem = SELECTED_HEAD;
            //Update_ItemIcon();
        }
    }
    else
        m_vColorHeadItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_BodyItem()
{
    m_rcBodyItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcBodyItem.top = LONG((m_fY - 135.f) - (60.f / 2));
    m_rcBodyItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcBodyItem.bottom = LONG((m_fY - 135.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_BodyItem(POINT pt)
{
    if (PtInRect(&m_rcBodyItem, pt))
    {
        m_vColorBodyItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            m_iCurrItem = SELECTED_BODY;
            //Update_ItemIcon();
        }
    }
    else
        m_vColorBodyItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_LegItem()
{
    m_rcLegItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcLegItem.top = LONG((m_fY - 75.f) - (60.f / 2));
    m_rcLegItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcLegItem.bottom = LONG((m_fY - 75.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_LegItem(POINT pt)
{
    if (PtInRect(&m_rcLegItem, pt))
    {
        m_vColorLegItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            m_iCurrItem = SELECTED_LEG;
            //Update_ItemIcon();
        }
    }
    else
        m_vColorLegItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Create_Rect_WeaponItem()
{
    m_rcWeaponItem.left = LONG((m_fX - 380.f) - ((323.f * 0.9f) / 2));
    m_rcWeaponItem.top = LONG((m_fY - 15.f) - (60.f / 2));
    m_rcWeaponItem.right = LONG((m_fX - 380.f) + ((323.f * 0.9f) / 2));
    m_rcWeaponItem.bottom = LONG((m_fY - 15.f) + (60.f / 2));
}

void CUI_NPC_ItemUpgrade::Is_Picking_WeaponItem(POINT pt)
{
    if (PtInRect(&m_rcWeaponItem, pt))
    {
        m_vColorWeaponItem = Vec4(2.f, 2.f, 2.f, 1.f);
        if (KEY_TAP(KEY::LBTN))
        {
            m_iCurrItem = SELECTED_WEAPON;
            //Update_ItemIcon();
        }
    }
    else
        m_vColorWeaponItem = Vec4(1.f, 1.f, 1.f, 1.f);
}

void CUI_NPC_ItemUpgrade::Update_GrowthGauge(_float fTimeDelta)
{


}

void CUI_NPC_ItemUpgrade::Update_Hammer_Effects(_float fTimeDelta)
{
    if (!m_bMaxGauge)
        return;

    if (34.f < m_fFrame)
        m_fFrame = 0.f;
    else if (34.f >= m_fFrame)
        m_fFrame += 34.f * fTimeDelta;
}

void CUI_NPC_ItemUpgrade::Update_Hammer_Effect(_float fTimeDelta)
{
    if (!m_bMaxGauge)
        return;

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
}

void CUI_NPC_ItemUpgrade::Update_Gague_Smoke(_float fTimeDelta)
{
    if (43.f < m_fFrame_Smoke)
        m_fFrame_Smoke = 0.f;
    else  if (43.f >= m_fFrame_Smoke)
        m_fFrame_Smoke += 20.f * fTimeDelta;
}

void CUI_NPC_ItemUpgrade::Update_Gauge_Effect(_float fTimeDelta)
{
    if (!m_bMaxGauge)
        return;

    if (9.f < m_fFrame_GaugeEffect)
        m_fFrame_GaugeEffect = 0.f;
    else if (9.f >= m_fFrame_GaugeEffect)
        m_fFrame_GaugeEffect += 9.f * fTimeDelta;
}

void CUI_NPC_ItemUpgrade::Update_Gauge_Spin(_float fTimeDelta)
{
    if (m_bDecreaseEffect)
        return;

    if (78.f <= m_fFrame_GaugeSpin)
        m_fFrame_GaugeSpin = 0.f;
    else if (78.f > m_fFrame_GaugeSpin)
        m_fFrame_GaugeSpin += 40.f * fTimeDelta;

}

void CUI_NPC_ItemUpgrade::Update_Max_Gauge_Effect(_float fTimeDelta)
{
    if (!m_bMaxGaugeEffect)
    {
        m_fFrame_MaxGaugeEffect = 0.f;
        return;
    }

    if (24.f < m_fFrame_MaxGaugeEffect)
    {
        m_bMaxGaugeEffect = false;
        m_fFrame_MaxGaugeEffect = 0.f;
    }
    else if (24.f >= m_fFrame_MaxGaugeEffect)
    {
        m_fFrame_MaxGaugeEffect += 20.f * fTimeDelta;
    }
}

void CUI_NPC_ItemUpgrade::Update_ItemIcon()
{
    //if ()
    {
        m_pTexture_Item;    //Clone
        m_pTexture_ResultItem;//Clone
    }

}

void CUI_NPC_ItemUpgrade::Update_ResultWaiting(_float fTimeDelta)
{
    if (!m_bResultWaiting)
        return; 
    if (77.f <= m_fFrame_ResultWaiting)
    {
        m_bResult = true;
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
    }
    else if (77.f > m_fFrame_ResultWaiting)
        m_fFrame_ResultWaiting += 30.f * fTimeDelta;
}

void CUI_NPC_ItemUpgrade::Update_ResultEffect(_float fTimeDelta)
{
    if (!m_bResult)
        return;

    if (22.f <= m_fFrame_ResultEffect)
    {
        m_bResult = false;
        m_bResultWaiting = false;
        m_fFrame_ResultEffect = 0.f;
        m_fFrame_ResultWaiting = 0.f;
        m_fAlpha_ResultWaiting = 1.f;
    }
    else
    {
        m_fFrame_ResultEffect += 30.f * fTimeDelta;
        m_fAlpha_ResultWaiting -= 1.5f * fTimeDelta;
    }
}

HRESULT CUI_NPC_ItemUpgrade::Ready_Components()
{
    __super::Ready_Components();

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
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Ingredients_Frame"),
        TEXT("Com_Texture_Ingredients"), (CComponent**)&m_pTexture_Item_Ingredients)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Upgrade_Button"),
        TEXT("Com_Texture_UpgradeButton"), (CComponent**)&m_pTexture_UpgradeButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_Slot"),
        TEXT("Com_Texture_ItemFrame"), (CComponent**)&m_pTexture_ItemSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Ingredient"),
        TEXT("Com_Texture_IngredientSlot"), (CComponent**)&m_pTexture_IngredientSlot)))
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
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_Success_Effect"),
        TEXT("Com_Texturem_ResultItemEffect"), (CComponent**)&m_pTexture_ResultItemEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Item_Slot"),
        TEXT("Com_Texturem_ResultItemSlot"), (CComponent**)&m_pTexture_ResultItemSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_ResultCheck_Button"),
        TEXT("Com_Texturem_ResultCheckButton"), (CComponent**)&m_pTexture_ResultCheckButton)))
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
        TEXT("Com_Transform_Ingredients"), (CComponent**)&m_pTransform_Ingredients)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeButton"), (CComponent**)&m_pTransform_UpgradeButton)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemSlot"), (CComponent**)&m_pTransform_ItemSlot)))
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
        TEXT("Com_Transform_EquipItemWnd_Head"), (CComponent**)&m_pTransform_EquipItemWnd_Head)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_ItemIcon_Head"), (CComponent**)&m_pTransform_ItemIcon_Head)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_UpgradeIcon_Head"), (CComponent**)&m_pTransform_UpgradeIcon_Head)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_EquipIcon_Head"), (CComponent**)&m_pTransform_EquipIcon_Head)))
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
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_ListFrame"),
        TEXT("Com_Texturem_GradeEffectWnd"), (CComponent**)&m_pTexture_GradeEffectWnd)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GradeEffect_First"), (CComponent**)&m_pTransform_GradeEffect_First)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GradeEffect_Second"), (CComponent**)&m_pTransform_GradeEffect_Second)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GradeEffect_Third"), (CComponent**)&m_pTransform_GradeEffect_Third)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
        TEXT("Com_Transform_GradeEffect_Fourth"), (CComponent**)&m_pTransform_GradeEffect_Fourth)))
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

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

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

    m_pTexture_HammerCap->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

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
    if(!m_bDecreaseEffect)
        m_pTexture_HammerAura->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame);
    else 
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

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
    m_pTexture_HammerEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_Gauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
        m_pTexture_GaugeEffect->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_GaugeEffect);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

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
    m_pTexture_GaugeSmoke->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_Smoke);
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
    if (!m_bDecreaseEffect)
        m_pTexture_GaugeSpin->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_GaugeSpin);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

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
    m_pTexture_GrowthButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iGrowthButton_TextureIndex);
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
    m_pTexture_ItemName->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_Item_Ingredients->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_UpgradeButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iUpgradeButton_TextureIndex);
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
    m_pTexture_UpgradeButton_Effect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_ItemSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
        m_pTexture_MaxGaugeEffect->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_MaxGaugeEffect);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

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
    m_pTexture_IngredientSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_IngredientSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_IngredientSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_SidePannel->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_SideWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_NpcSpeech_BackGround->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_NpcSpeech_Wnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_QuitButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iQuitButton_TextureIndex);
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
    if(m_bResultWaiting)
        m_pTexture_ResultWaiting->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame_ResultWaiting);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    if (m_bResult)
        m_pTexture_ResultEffect_Success->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_ResultEffect);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
        m_pTexture_ResultWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
        m_pTexture_ResultItemEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
        m_pTexture_ResultItemSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ResultCheckButton()
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
    if (m_bResultWnd)
        m_pTexture_ResultCheckButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iUpgradeButton_TextureIndex);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipItemWnd_Head()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Head->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorHeadItem, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
        m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Head()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Head->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
        m_pTexture_ItemIcon_Head->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_UpgradeIcon_Head()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_UpgradeIcon_Head->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
        m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_EquipIcon_Head()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipIcon_Head->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
        m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Body()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Body->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_ItemIcon_Body->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorLegItem, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Leg()
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
    //if (m_bResultWnd)
    m_pTexture_ItemIcon_Leg->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColorWeaponItem, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_EquipItemWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_ItemIcon_Weapon()
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
    //if (m_bResultWnd)
    m_pTexture_ItemIcon_Weapon->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_UpgradeIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    //if (m_bResultWnd)
    m_pTexture_EquipIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_SidePannel_L()
{
    if (FAILED(Bind_ShaderResources_SidePannel()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Head
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Head()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
   /* if (FAILED(Bind_ShaderResources_ItemIcon_Head()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();*/
    if (FAILED(Bind_ShaderResources_UpgradeIcon_Head()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    if (FAILED(Bind_ShaderResources_EquipIcon_Head()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Body
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Body()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    /* if (FAILED(Bind_ShaderResources_ItemIcon_Body()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();*/
    if (FAILED(Bind_ShaderResources_UpgradeIcon_Body()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    if (FAILED(Bind_ShaderResources_EquipIcon_Body()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Leg
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Leg()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    /* if (FAILED(Bind_ShaderResources_ItemIcon_Leg()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();*/
    if (FAILED(Bind_ShaderResources_UpgradeIcon_Leg()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    if (FAILED(Bind_ShaderResources_EquipIcon_Leg()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    //Weapon
    if (FAILED(Bind_ShaderResources_EquipItemWnd_Weapon()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    /* if (FAILED(Bind_ShaderResources_ItemIcon_Leg()))
         return E_FAIL;
     m_pShaderCom->Begin(0);
     m_pVIBufferCom->Render();*/
    if (FAILED(Bind_ShaderResources_UpgradeIcon_Weapon()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();
    if (FAILED(Bind_ShaderResources_EquipIcon_Weapon()))
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

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_First()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect_First->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Second()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect_Second->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Third()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect_Third->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_GradeEffectWnd_Fourth()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GradeEffect_Fourth->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    //if (m_bResultWnd)
    m_pTexture_GradeEffectWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    //else
        //m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_CurrItem()
{
    switch (m_iCurrItem)
    {
    case SELECTED_HEAD:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Head->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_BODY:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Body->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_LEG:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Leg->Get_WorldMatrix())))
            return E_FAIL;
        break;
    case SELECTED_WEAPON:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Weapon->Get_WorldMatrix())))
            return E_FAIL;
        break;
    default:
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EquipItemWnd_Head->Get_WorldMatrix())))
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
        m_pTexture_CurrItemSlot->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iSidePannel_L_Wnd_TextureIndex);
    else
        m_pTexture_None->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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

    //m_pItemNameWnd->Set_Dead(true);
    //m_pCurrGradeWnd->Set_Dead(true);
    //m_pNextGradeWnd->Set_Dead(true);

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTexture_None);
    Safe_Release(m_pTexture_HammerCap);
    Safe_Release(m_pTexture_HammerEffect);
    Safe_Release(m_pTexture_HammerAura);
    Safe_Release(m_pTexture_GaugeSmoke);
    Safe_Release(m_pTexture_GaugeEffect);
    Safe_Release(m_pTexture_GrowthButton);
    Safe_Release(m_pTexture_ItemName);
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

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pTransform_HammerCap);
    Safe_Release(m_pTransform_HammerEffect);
    Safe_Release(m_pTransform_HammerAura);
    Safe_Release(m_pTransform_GaugeSmoke);
    Safe_Release(m_pTransform_GaugeEffect);
    Safe_Release(m_pTransform_GrowthButton);
    Safe_Release(m_pTransform_ItemName);
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

    Free_Side_Pannel_L();
    Free_Side_Pannel_R();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);

}

void CUI_NPC_ItemUpgrade::Free_Side_Pannel_L()
{
    Safe_Release(m_pTexture_EquipItemWnd);
    Safe_Release(m_pTexture_UpgradeIcon);
    Safe_Release(m_pTexture_EquipIcon);
    Safe_Release(m_pTexture_ItemIcon_Head);
    Safe_Release(m_pTexture_ItemIcon_Body);
    Safe_Release(m_pTexture_ItemIcon_Leg);
    Safe_Release(m_pTexture_ItemIcon_Weapon);
    Safe_Release(m_pTexture_CurrItemSlot);

    Safe_Release(m_pTransform_EquipItemWnd_Head);
    Safe_Release(m_pTransform_ItemIcon_Head);
    Safe_Release(m_pTransform_UpgradeIcon_Head);
    Safe_Release(m_pTransform_EquipIcon_Head);
    Safe_Release(m_pTransform_EquipItemWnd_Body);
    Safe_Release(m_pTransform_ItemIcon_Body);
    Safe_Release(m_pTransform_UpgradeIcon_Body);
    Safe_Release(m_pTransform_EquipIcon_Body);
    Safe_Release(m_pTransform_EquipItemWnd_Leg);
    Safe_Release(m_pTransform_ItemIcon_Leg);
    Safe_Release(m_pTransform_UpgradeIcon_Leg);
    Safe_Release(m_pTransform_EquipIcon_Leg);
    Safe_Release(m_pTransform_EquipItemWnd_Weapon);
    Safe_Release(m_pTransform_ItemIcon_Weapon);
    Safe_Release(m_pTransform_UpgradeIcon_Weapon);
    Safe_Release(m_pTransform_EquipIcon_Weapon);
}

void CUI_NPC_ItemUpgrade::Free_Side_Pannel_R()
{
    Safe_Release(m_pTexture_GradeEffectWnd);

    Safe_Release(m_pTransform_GradeEffect_First);
    Safe_Release(m_pTransform_GradeEffect_Second);
    Safe_Release(m_pTransform_GradeEffect_Third);
    Safe_Release(m_pTransform_GradeEffect_Fourth);

}
