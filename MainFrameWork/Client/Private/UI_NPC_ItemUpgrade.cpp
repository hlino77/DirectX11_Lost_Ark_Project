#include "stdafx.h"
#include "UI_NPC_ItemUpgrade.h"
#include "GameInstance.h"

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
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 37.f) * 1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_HammerEffect
    m_pTransform_HammerEffect->Set_Scale(Vec3(556.f * 0.8f, 374.f * 0.8f, 1.f));
    m_pTransform_HammerEffect->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 10.f) - g_iWinSizeX * 0.5f, -((m_fY - 108.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_HammerAura
    m_pTransform_HammerAura->Set_Scale(Vec3(669.f * 0.8f, 417.f * 0.8f, 1.f));
    m_pTransform_HammerAura->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 88.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_Gauge
    m_pTransform_Gauge->Set_Scale(Vec3(110.f * 0.8f, 110.f * 0.8f, 1.f));
    m_pTransform_Gauge->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 65.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GaugeEffect
    m_pTransform_GaugeEffect->Set_Scale(Vec3(200.f * 0.8f, 200.f * 0.8f, 1.f));
    m_pTransform_GaugeEffect->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 65.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GaugeSmoke
    m_pTransform_GaugeSmoke->Set_Scale(Vec3(152.f * 0.8f, 152.f * 0.8f, 1.f));
    m_pTransform_GaugeSmoke->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 65.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_GrowthButton
    m_pTransform_GrowthButton->Set_Scale(Vec3(108.f * 0.8f, 44.f * 0.8f, 1.f));
    m_pTransform_GrowthButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 36.f) * 1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ItemName
    m_pTransform_ItemName->Set_Scale(Vec3(566.f * 0.8f, 98.f * 0.8f, 1.f));
    m_pTransform_ItemName->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX - 4.f) - g_iWinSizeX * 0.5f, -((m_fY - 170.f) * 1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_Ingredients
    m_pTransform_Ingredients->Set_Scale(Vec3(478.f * 0.8f, 189.f * 0.8f, 1.f));
    m_pTransform_Ingredients->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 4.f) - g_iWinSizeX * 0.5f, -((m_fY + 212.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_UpgradeButton
    m_pTransform_UpgradeButton->Set_Scale(Vec3(221.f * 0.8f, 42.f * 0.8f, 1.f));
    m_pTransform_UpgradeButton->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 328.f)*1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_ItemSlot
    m_pTransform_ItemSlot->Set_Scale(Vec3(64.f * 0.8f, 64.f * 0.8f, 1.f));
    m_pTransform_ItemSlot->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY - 266.f) * 1.1f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_IngredientSlotL
    m_pTransform_IngredientSlotL->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlotL->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - 100.f)*1.1f) - g_iWinSizeX * 0.5f, -((m_fY + 190.f) * 0.8f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_IngredientSlot
    m_pTransform_IngredientSlot->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlot->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -((m_fY + 190.f) * 0.8f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_IngredientSlotR
    m_pTransform_IngredientSlotR->Set_Scale(Vec3(93.f * 0.8f, 104.f * 0.8f, 1.f));
    m_pTransform_IngredientSlotR->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 100.f)*0.8f) - g_iWinSizeX * 0.5f, -((m_fY + 190.f) * 0.8f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_SidePannel
    m_pTransform_SidePannel->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
    m_pTransform_SidePannel->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX - 466.f) * 1.1f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_SideWnd
    m_pTransform_SideWnd->Set_Scale(Vec3(450.f * 0.8f, 728.f * 0.8f, 1.f));
    m_pTransform_SideWnd->Set_State(CTransform::STATE_POSITION,
        Vec3(((m_fX + 466.f) * 0.8f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_NpcSpeech_BackGround
    m_pTransform_NpcSpeech_BackGround->Set_Scale(Vec3(1700.f, 100.f*0.8f, 1.f));
    m_pTransform_NpcSpeech_BackGround->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 350.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_NpcSpeech_Wnd
    m_pTransform_NpcSpeech_Wnd->Set_Scale(Vec3(g_iWinSizeX * 0.5f, 100.f*0.8f, 1.f));
    m_pTransform_NpcSpeech_Wnd->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX- g_iWinSizeX * 0.5f, -(m_fY + 350.f) + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransform_QuitButton
    m_pTransform_QuitButton->Set_Scale(Vec3(205.f * 0.8f, 53.f * 0.8f, 1.f));
    m_pTransform_QuitButton->Set_State(CTransform::STATE_POSITION,
        Vec3((m_fX + 700.f) - g_iWinSizeX * 0.5f, -(m_fY + 400.f) + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_NPC_ItemUpgrade::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_NPC_ItemUpgrade::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (34.f < m_fFrame)
        m_fFrame = 0.f;
    else if (34.f >= m_fFrame)
        m_fFrame += 34.f * fTimeDelta;
     
    if (43.f < m_fFrame_Smoke)
        m_fFrame_Smoke = 0.f;
    else  if (43.f >= m_fFrame_Smoke)
        m_fFrame_Smoke += 20.f * fTimeDelta;
 
    if (9.f < m_fFrame_GaugeEffect)
        m_fFrame_GaugeEffect = 0.f;
    else if (9.f >= m_fFrame_GaugeEffect)
        m_fFrame_GaugeEffect += 9.f * fTimeDelta;

    Update_Button();
}

HRESULT CUI_NPC_ItemUpgrade::Render()
{
    if (FAILED(Bind_ShaderResources_SidePannel()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_SideWnd()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerEffect()))
        return E_FAIL;
    m_pShaderCom->Begin(3);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerCap()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_HammerAura()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_Gauge()))
        return E_FAIL;
    m_pShaderCom->Begin(17);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_GaugeEffect()))
        return E_FAIL;
    m_pShaderCom->Begin(3);
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

    if (FAILED(Bind_ShaderResources_NpcSpeech_BackGround()))
        return E_FAIL;
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_NpcSpeech_Wnd()))
        return E_FAIL;
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources_QuitButton()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_NPC_ItemUpgrade::Update_Button()
{
    Create_Rect_GrowthButton();
    Create_Rect_UpgradeButton();

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);  // 클라이언트 내에 마우스 포인터 가져오기 
    _uint ViewPortIndex = 1;
    CD3D11_VIEWPORT ViewPort;
    m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // 뷰포트 가져오기 

    if (true == Is_Picking_GrowthButton(pt))
        m_iGrowthButton_TextureIndex = 1;
    else
        m_iGrowthButton_TextureIndex = 0;

    if(true == Is_Picking_UpgradeButton(pt))
        m_iUpgradeButton_TextureIndex = 1;
    else
        m_iUpgradeButton_TextureIndex = 0;

}

void CUI_NPC_ItemUpgrade::Create_Rect_GrowthButton()
{
    m_rcGrowthButton.left = LONG(m_fX - (108.f / 2));
    m_rcGrowthButton.top = LONG((m_fY + 36.f) - (44.f / 2));
    m_rcGrowthButton.right = LONG(m_fX + (108.f / 2));
    m_rcGrowthButton.bottom = LONG((m_fY + 36.f) + (44.f / 2));
}

_bool CUI_NPC_ItemUpgrade::Is_Picking_GrowthButton(POINT pt)
{
    if (PtInRect(&m_rcGrowthButton, pt))
        return true;
    else
        return false;

}

void CUI_NPC_ItemUpgrade::Create_Rect_UpgradeButton()
{
    m_rcUpgradeButton.left = LONG(m_fX - (221.f / 2));
    m_rcUpgradeButton.top = LONG((m_fY + 328.f) - (42.f / 2));
    m_rcUpgradeButton.right = LONG(m_fX + (221.f / 2));
    m_rcUpgradeButton.bottom = LONG((m_fY + 328.f) + (42.f / 2));
}

_bool CUI_NPC_ItemUpgrade::Is_Picking_UpgradeButton(POINT pt)
{
    if (PtInRect(&m_rcUpgradeButton, pt))
        return true;
    else
        return false;

}

void CUI_NPC_ItemUpgrade::Create_Rect_QuitButton()
{
}

_bool CUI_NPC_ItemUpgrade::Is_Picking_QuitButton(POINT pt)
{
    return _bool();
}

HRESULT CUI_NPC_ItemUpgrade::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer_Cap"),
        TEXT("Com_Texture_HammerCap"), (CComponent**)&m_pTexture_HammerCap)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer_Effect"),
        TEXT("Com_Texture_HammerEffect"), (CComponent**)&m_pTexture_HammerEffect)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Hammer_Shine"),
        TEXT("Com_Texture_HammerAura"), (CComponent**)&m_pTexture_HammerAura)))
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
        TEXT("Com_Texture_IngredientSlotL"), (CComponent**)&m_pTexture_IngredientSlotL)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Ingredient"),
        TEXT("Com_Texture_IngredientSlot"), (CComponent**)&m_pTexture_IngredientSlot)))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Ingredient"),
        TEXT("Com_Texture_IngredientSlotR"), (CComponent**)&m_pTexture_IngredientSlotR)))
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

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_HammerEffect()
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
    m_pTexture_HammerEffect->Set_SRV(m_pShaderCom, "g_MaskTexture",(_uint)m_fFrame);
    return S_OK;
}

HRESULT CUI_NPC_ItemUpgrade::Bind_ShaderResources_HammerAura()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_HammerAura->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;
    m_pTexture_HammerAura->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_GaugeEffect->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fFrame_GaugeEffect);

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
    m_pTexture_IngredientSlotL->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_IngredientSlotR->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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
    m_pTexture_QuitButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
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

    Safe_Release(m_pTextureCom);
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
    Safe_Release(m_pTexture_IngredientSlotL);
    Safe_Release(m_pTexture_IngredientSlot);
    Safe_Release(m_pTexture_IngredientSlotR);
    Safe_Release(m_pTexture_SidePannel);
    Safe_Release(m_pTexture_SideWnd);
    Safe_Release(m_pTexture_NpcSpeech_BackGround);
    Safe_Release(m_pTexture_NpcSpeech_Wnd);
    Safe_Release(m_pTexture_QuitButton);

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

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);

}
