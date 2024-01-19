#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_NPC_ItemUpgrade :
    public CUI
{
private:
    CUI_NPC_ItemUpgrade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_NPC_ItemUpgrade(const CUI& rhs);
    virtual ~CUI_NPC_ItemUpgrade() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    void    Update_Button();
    void    Create_Rect_GrowthButton();
    _bool   Is_Picking_GrowthButton(POINT pt);
    void    Create_Rect_UpgradeButton();
    _bool   Is_Picking_UpgradeButton(POINT pt);
    void    Create_Rect_QuitButton();
    _bool   Is_Picking_QuitButton(POINT pt);

private:
   


private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_HammerCap();
    HRESULT Bind_ShaderResources_HammerEffect();
    HRESULT Bind_ShaderResources_HammerAura();
    HRESULT Bind_ShaderResources_Gauge();
    HRESULT Bind_ShaderResources_GaugeEffect();
    HRESULT Bind_ShaderResources_GaugeSmoke();
    HRESULT Bind_ShaderResources_GrowthButton();
    HRESULT Bind_ShaderResources_ItemName();
    HRESULT Bind_ShaderResources_Ingredients();
    HRESULT Bind_ShaderResources_UpgradeButton();
    HRESULT Bind_ShaderResources_ItemFrame();
    HRESULT Bind_ShaderResources_IngredientSlotL();
    HRESULT Bind_ShaderResources_IngredientSlot();
    HRESULT Bind_ShaderResources_IngredientSlotR();
    HRESULT Bind_ShaderResources_SidePannel();
    HRESULT Bind_ShaderResources_SideWnd();
    HRESULT Bind_ShaderResources_NpcSpeech_BackGround();
    HRESULT Bind_ShaderResources_NpcSpeech_Wnd();
    HRESULT Bind_ShaderResources_QuitButton();

    
private:
    CTexture* m_pTexture_SideL = { nullptr };
    CTexture* m_pTexture_SideR = { nullptr };
    CTexture* m_pTexture_HammerEffect = { nullptr };
    CTexture* m_pTexture_HammerCap = { nullptr };
    CTexture* m_pTexture_HammerAura = { nullptr };
    CTexture* m_pTexture_Gauge = { nullptr };
    CTexture* m_pTexture_GaugeAura = { nullptr };
    CTexture* m_pTexture_GaugeSmoke = { nullptr };
    CTexture* m_pTexture_GaugeEffect = { nullptr };
    CTexture* m_pTexture_GrowthButton = { nullptr };
    CTexture* m_pTexture_ItemName = { nullptr };
    CTexture* m_pTexture_Item_Ingredients = { nullptr };
    CTexture* m_pTexture_UpgradeButton = { nullptr };
    CTexture* m_pTexture_ItemSlot = { nullptr };
    CTexture* m_pTexture_Item = { nullptr };//Clone해서 텍스처를 받아올것
    CTexture* m_pTexture_IngredientSlotL = { nullptr };
    CTexture* m_pTexture_IngredientSlot = { nullptr };
    CTexture* m_pTexture_IngredientSlotR = { nullptr };
    CTexture* m_pTexture_SidePannel = { nullptr };
    CTexture* m_pTexture_SideWnd = { nullptr };
    CTexture* m_pTexture_NpcSpeech_BackGround = { nullptr };
    CTexture* m_pTexture_NpcSpeech_Wnd = { nullptr };
    CTexture* m_pTexture_QuitButton = { nullptr };
    //CTexture* m_pTexture_ = { nullptr };

    CTransform* m_pTransform_SideL = { nullptr };
    CTransform* m_pTransform_SideR = { nullptr };
    CTransform* m_pTransform_HammerCap = { nullptr };
    CTransform* m_pTransform_HammerEffect = { nullptr };
    CTransform* m_pTransform_HammerAura = { nullptr };
    CTransform* m_pTransform_Gauge = { nullptr };
    CTransform* m_pTransform_GaugeAura = { nullptr };
    CTransform* m_pTransform_GaugeSmoke = { nullptr };
    CTransform* m_pTransform_GaugeEffect = { nullptr };
    CTransform* m_pTransform_GrowthButton = { nullptr };
    CTransform* m_pTransform_ItemName = { nullptr };
    CTransform* m_pTransform_Ingredients = { nullptr };
    CTransform* m_pTransform_UpgradeButton = { nullptr };
    CTransform* m_pTransform_ItemSlot = { nullptr };
    CTransform* m_pTransform_Item = { nullptr };
    CTransform* m_pTransform_IngredientSlotL = { nullptr };
    CTransform* m_pTransform_IngredientSlot = { nullptr };
    CTransform* m_pTransform_IngredientSlotR = { nullptr };
    CTransform* m_pTransform_SidePannel = { nullptr };
    CTransform* m_pTransform_SideWnd = { nullptr };
    CTransform* m_pTransform_NpcSpeech_BackGround = { nullptr };
    CTransform* m_pTransform_NpcSpeech_Wnd = { nullptr };
    CTransform* m_pTransform_QuitButton = { nullptr };
    //CTransform* m_pTransform_ = { nullptr };
    
    _float m_fFrame = { 0.f };
    _float  m_fFrame_GaugeEffect = { 0.f };
    _float  m_fFrame_Smoke = { 0.f };

    RECT    m_rcGrowthButton = {};
    RECT    m_rcUpgradeButton = {};
    RECT    m_rcQuitButton = {};
    _uint   m_iGrowthButton_TextureIndex = { 0 };
    _uint   m_iUpgradeButton_TextureIndex = { 0 };

    _uint   m_iItemGrowthGauge = { 0 };
    

public:
    static  CUI_NPC_ItemUpgrade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END