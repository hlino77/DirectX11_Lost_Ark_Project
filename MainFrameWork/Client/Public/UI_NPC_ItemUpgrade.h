#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END


BEGIN(Client)

class CUI_NPC_ItemUpgrade :
    public CUI
{
public:
    enum CURR_SELECTED_ITEM {
        SELECTED_HEAD, SELECTED_BODY, SELECTED_LEG, SELECTED_WEAPON, SELECTED_END
    };

private:
    CUI_NPC_ItemUpgrade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_NPC_ItemUpgrade(const CUI& rhs);
    virtual ~CUI_NPC_ItemUpgrade() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_Transform_BaseWnd();
    HRESULT Initialize_Transform_SidePannel_L();
    HRESULT Initialize_Transform_SidePannel_R();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Get_PlayerInformation();

private:
    void    Update_Button(_float fTimeDelta);
    void    Upadate_GrowthButton(POINT pt);
    void    Update_UpgradeButton(POINT pt, _float fTimeDelta);
    void    Update_QuitButton(POINT pt);
    void    Update_ResultCheckButton(POINT pt);
    void    Create_Rect_GrowthButton();
    void   Is_Picking_GrowthButton(POINT pt);
    void    Create_Rect_UpgradeButton();
    void   Is_Picking_UpgradeButton(POINT pt);
    void    Create_Rect_QuitButton();
    void   Is_Picking_QuitButton(POINT pt);

private:
    void    Update_Items(POINT pt);
    void    Create_Rect_HeadItem();
    void   Is_Picking_HeadItem(POINT pt);
    void    Create_Rect_BodyItem();
    void   Is_Picking_BodyItem(POINT pt);
    void    Create_Rect_LegItem();
    void   Is_Picking_LegItem(POINT pt);
    void    Create_Rect_WeaponItem();
    void   Is_Picking_WeaponItem(POINT pt);

private:
    void    Update_GrowthGauge(_float fTimeDelta);
    void    Update_Hammer_Effects(_float fTimeDelta);
    void    Update_Hammer_Effect(_float fTimeDelta);
    void    Update_Hammer_Aura(_float fTimeDelta);

    void    Update_Gague_Smoke(_float fTimeDelta);
    void    Update_Gauge_Effect(_float fTimeDelta);
    void    Update_Gauge_Spin(_float fTimeDelta);
    void    Update_Max_Gauge_Effect(_float fTimeDelta);
    void    Update_ItemIcon();//수정해야함

    void    Update_ResultWaiting(_float fTimeDelta);
    void    Update_ResultEffect(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_Components_SidePannel_L();
    HRESULT Ready_Components_SidePannel_R();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_HammerAura();
    HRESULT Bind_ShaderResources_HammerCap();
    HRESULT Bind_ShaderResources_HammerEffect();
    HRESULT Bind_ShaderResources_Gauge();
    HRESULT Bind_ShaderResources_GaugeEffect();
    HRESULT Bind_ShaderResources_GaugeSmoke();
    HRESULT Bind_ShaderResources_GaugeSpin();
    HRESULT Bind_ShaderResources_GrowthButton();
    HRESULT Bind_ShaderResources_ItemName();
    HRESULT Bind_ShaderResources_Ingredients();
    HRESULT Bind_ShaderResources_UpgradeButton();
    HRESULT Bind_ShaderResources_UpgradeButtonEffect();
    HRESULT Bind_ShaderResources_ItemFrame();
    HRESULT Bind_ShaderResources_MaxGaugeEffect();
    HRESULT Bind_ShaderResources_IngredientSlotL();
    HRESULT Bind_ShaderResources_IngredientSlot();
    HRESULT Bind_ShaderResources_IngredientSlotR();
    HRESULT Bind_ShaderResources_SidePannel();
    HRESULT Bind_ShaderResources_SideWnd();
    HRESULT Bind_ShaderResources_NpcSpeech_BackGround();
    HRESULT Bind_ShaderResources_NpcSpeech_Wnd();
    HRESULT Bind_ShaderResources_QuitButton();
    HRESULT Bind_ShaderResources_ResultWaiting();
    HRESULT Bind_ShaderResources_ResultEffect();
    HRESULT Bind_ShaderResources_ResultWnd();
    HRESULT Bind_ShaderResources_ResultItemEffect();
    HRESULT Bind_ShaderResources_ResultItemSlot();
    HRESULT Bind_ShaderResources_ResultCheckButton();

private:
    HRESULT Bind_ShaderResources_SidePannel_L();
    HRESULT Bind_ShaderResources_CurrItem();
    HRESULT Bind_ShaderResources_EquipItemWnd_Head();
    HRESULT Bind_ShaderResources_ItemIcon_Head();
    HRESULT Bind_ShaderResources_UpgradeIcon_Head();
    HRESULT Bind_ShaderResources_EquipIcon_Head();
    HRESULT Bind_ShaderResources_EquipItemWnd_Body();
    HRESULT Bind_ShaderResources_ItemIcon_Body();
    HRESULT Bind_ShaderResources_UpgradeIcon_Body();
    HRESULT Bind_ShaderResources_EquipIcon_Body();
    HRESULT Bind_ShaderResources_EquipItemWnd_Leg();
    HRESULT Bind_ShaderResources_ItemIcon_Leg();
    HRESULT Bind_ShaderResources_UpgradeIcon_Leg();
    HRESULT Bind_ShaderResources_EquipIcon_Leg();
    HRESULT Bind_ShaderResources_EquipItemWnd_Weapon();
    HRESULT Bind_ShaderResources_ItemIcon_Weapon();
    HRESULT Bind_ShaderResources_UpgradeIcon_Weapon();
    HRESULT Bind_ShaderResources_EquipIcon_Weapon();

private:
    HRESULT Bind_ShaderResources_SidePannel_R();
    HRESULT Bind_ShaderResources_GradeEffectWnd_First();
    HRESULT Bind_ShaderResources_GradeEffectWnd_Second();
    HRESULT Bind_ShaderResources_GradeEffectWnd_Third();
    HRESULT Bind_ShaderResources_GradeEffectWnd_Fourth();

private:
    //BaseWnd_Texture
    CTexture* m_pTexture_SideL = { nullptr };
    CTexture* m_pTexture_SideR = { nullptr };
    CTexture* m_pTexture_HammerAura = { nullptr };
    CTexture* m_pTexture_HammerCap = { nullptr };
    CTexture* m_pTexture_HammerEffect = { nullptr };
    CTexture* m_pTexture_Gauge = { nullptr };
    CTexture* m_pTexture_GaugeAura = { nullptr };
    CTexture* m_pTexture_GaugeSmoke = { nullptr };
    CTexture* m_pTexture_GaugeEffect = { nullptr };
    CTexture* m_pTexture_GaugeSpin = { nullptr };
    CTexture* m_pTexture_GrowthButton = { nullptr };
    CTexture* m_pTexture_ItemName = { nullptr };
    CTexture* m_pTexture_Item_Ingredients = { nullptr };
    CTexture* m_pTexture_UpgradeButton = { nullptr };
    CTexture* m_pTexture_UpgradeButton_Effect = { nullptr };
    CTexture* m_pTexture_ItemSlot = { nullptr };
    CTexture* m_pTexture_Item = { nullptr };//클론해서 받아옴
    CTexture* m_pTexture_IngredientSlot = { nullptr };
    CTexture* m_pTexture_NpcSpeech_BackGround = { nullptr };
    CTexture* m_pTexture_NpcSpeech_Wnd = { nullptr };
    CTexture* m_pTexture_QuitButton = { nullptr };
    CTexture* m_pTexture_ResultWaiting = { nullptr };
    CTexture* m_pTexture_ResultFailed = { nullptr };
    CTexture* m_pTexture_ResultSuccess = { nullptr };
    CTexture* m_pTexture_None = { nullptr };
    CTexture* m_pTexture_MaxGaugeEffect = { nullptr };

    //Side_L_Texture
    CTexture* m_pTexture_SidePannel = { nullptr };//Wnd
    CTexture* m_pTexture_EquipItemWnd = { nullptr };
    CTexture* m_pTexture_ItemIcon_Head = { nullptr };
    CTexture* m_pTexture_ItemIcon_Body = { nullptr };
    CTexture* m_pTexture_ItemIcon_Leg = { nullptr };
    CTexture* m_pTexture_ItemIcon_Weapon = { nullptr };
    CTexture* m_pTexture_UpgradeIcon = { nullptr };
    CTexture* m_pTexture_EquipIcon = { nullptr };
    CTexture* m_pTexture_CurrItemSlot = { nullptr };

    //Side_R_Texture
    CTexture* m_pTexture_SideWnd = { nullptr };//Wnd
    CTexture* m_pTexture_GradeEffectWnd = { nullptr };

    //ResultWnd_Texture
    CTexture* m_pTexture_ResultEffect_Success = { nullptr };
    CTexture* m_pTexture_ResultEffect_Failed = { nullptr };
    CTexture* m_pTexture_ResultWnd = { nullptr };
    CTexture* m_pTexture_ResultItemEffect = { nullptr };
    CTexture* m_pTexture_ResultItemSlot = { nullptr };
    CTexture* m_pTexture_ResultItem = { nullptr };
    CTexture* m_pTexture_ResultCheckButton = { nullptr };
    //CTexture* m_pTexture_ = { nullptr };

    //BaseWnd_Transform
    CTransform* m_pTransform_SideL = { nullptr };
    CTransform* m_pTransform_SideR = { nullptr };
    CTransform* m_pTransform_HammerAura = { nullptr };
    CTransform* m_pTransform_HammerCap = { nullptr };
    CTransform* m_pTransform_HammerEffect = { nullptr };
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
    CTransform* m_pTransform_NpcSpeech_BackGround = { nullptr };
    CTransform* m_pTransform_NpcSpeech_Wnd = { nullptr };
    CTransform* m_pTransform_QuitButton = { nullptr };
    CTransform* m_pTransform_Result = { nullptr };
    CTransform* m_pTransform_MaxGaugeEffect = { nullptr };

    //Side_L_Pannel
    CTransform* m_pTransform_SidePannel = { nullptr };
    CTransform* m_pTransform_EquipItemWnd_Head = { nullptr };
    CTransform* m_pTransform_ItemIcon_Head = { nullptr };
    CTransform* m_pTransform_UpgradeIcon_Head = { nullptr };
    CTransform* m_pTransform_EquipIcon_Head = { nullptr };
    CTransform* m_pTransform_EquipItemWnd_Body = { nullptr };
    CTransform* m_pTransform_ItemIcon_Body = { nullptr };
    CTransform* m_pTransform_UpgradeIcon_Body = { nullptr };
    CTransform* m_pTransform_EquipIcon_Body = { nullptr };
    CTransform* m_pTransform_EquipItemWnd_Leg = { nullptr };
    CTransform* m_pTransform_ItemIcon_Leg = { nullptr };
    CTransform* m_pTransform_UpgradeIcon_Leg = { nullptr };
    CTransform* m_pTransform_EquipIcon_Leg = { nullptr };
    CTransform* m_pTransform_EquipItemWnd_Weapon = { nullptr };
    CTransform* m_pTransform_ItemIcon_Weapon = { nullptr };
    CTransform* m_pTransform_UpgradeIcon_Weapon = { nullptr };
    CTransform* m_pTransform_EquipIcon_Weapon = { nullptr };

    //Side_R_Pannel
    CTransform* m_pTransform_SideWnd = { nullptr };
    CTransform* m_pTransform_GradeEffect_First = { nullptr };
    CTransform* m_pTransform_GradeEffect_Second = { nullptr };
    CTransform* m_pTransform_GradeEffect_Third = { nullptr };
    CTransform* m_pTransform_GradeEffect_Fourth = { nullptr };

    //ResultWnd_Transform
    CTransform* m_pTransform_ResultEffect = { nullptr };
    CTransform* m_pTransform_ResultWnd = { nullptr };
    CTransform* m_pTransform_ResultItemEffect = { nullptr };
    CTransform* m_pTransform_ResultItemSlot = { nullptr };

    //CTransform* m_pTransform_ = { nullptr };

    _float  m_fFrame = { 0.f };
    _float  m_fFrame_GaugeEffect = { 0.f };
    _float  m_fFrame_Smoke = { 0.f };
    _float  m_fFrame_MaxGaugeEffect = { 0.f };
    _float  m_fFrame_GaugeSpin = { 0.f };
    _float  m_fFrame_ResultWaiting = { 0.f };
    _float  m_fFrame_ResultEffect = { 0.f };

    RECT    m_rcGrowthButton = {};
    RECT    m_rcUpgradeButton = {};
    RECT    m_rcQuitButton = {};
    RECT    m_rcHeadItem = {};
    RECT    m_rcBodyItem = {};
    RECT    m_rcLegItem = {};
    RECT    m_rcWeaponItem = {};

    _uint   m_iGrowthButton_TextureIndex = { 0 };
    _uint   m_iUpgradeButton_TextureIndex = { 0 };
    _uint   m_iQuitButton_TextureIndex = { 0 };
    _uint   m_iResultButton_TextureIndex = { 0 };
    _uint   m_iSidePannel_L_Wnd_TextureIndex = { 0 };

    _uint   m_iCurrItem = { SELECTED_END };

    _float   m_fItemGrowthCurrGauge = { 0.f };
    _float   m_fItemGrowthMaxGauge = { 100.f };
    _float  m_fItemGrowthRatio = { 0.f };
    _float  m_fHammer_Effect_Alpha = { 0.f };
    _float  m_fAlpha_ResultWaiting = { 1.f };
    _float  m_fAlpha_UpgradeButton_Effect = { 0.f };

    _bool   m_bMaxGauge = { false };
    _bool   m_bDecreaseEffect = { true };
    _bool   m_bIncreaseEffect = { false };
    _bool   m_bDecreaseUpgradeEffect = { false };

    _bool   m_bGrowthButton = { false };
    _bool   m_bUpgradeButton = { false };
    _bool   m_bQuitButton = { false };

    _bool   m_bResultWaiting = { false };
    _bool   m_bResult = { false };
    _bool   m_bResultWnd = { false };

    _bool   m_bTestGaugeOn = { false };//Test
    _bool   m_bMaxGaugeEffect = { false };

    Vec4    m_vColorHeadItem = Vec4(1.f, 1.f, 1.f, 1.f);
    Vec4    m_vColorBodyItem = Vec4(1.f, 1.f, 1.f, 1.f);
    Vec4    m_vColorLegItem = Vec4(1.f, 1.f, 1.f, 1.f);
    Vec4    m_vColorWeaponItem = Vec4(1.f, 1.f, 1.f, 1.f);

    CTextBox* m_pItemNameWnd = { nullptr };
    CTextBox* m_pCurrGradeWnd = { nullptr };
    CTextBox* m_pNextGradeWnd = { nullptr };

    CGameObject* m_pUsingPlayer = { nullptr };
    CGameObject* m_pCurrEquipItem = { nullptr };

public:
    static  CUI_NPC_ItemUpgrade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
    void    Free_Side_Pannel_L();
    void    Free_Side_Pannel_R();
};

END