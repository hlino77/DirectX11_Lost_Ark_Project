#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Option_Sound :
    public CUI
{
private:
    CUI_Option_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Option_Sound(const CUI& rhs);
    virtual ~CUI_Option_Sound() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

public:
    void    Is_SoundOptionMode(_uint iMode);
    void    Update_OptionSound(_float fTimeDelta, class CUI* pUI, POINT pt);
    void    Set_Active_Option(_bool bOption);
    void    Apply_Option();
    void    Cancle_Option();
    void    Print_OptionText();

private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_TextBox();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_ChckBoxMaster();
    HRESULT Bind_ShaderResources_Line();
    HRESULT Bind_ShaderResources_ChckBoxBGM();
    HRESULT Bind_ShaderResources_ChckBoxUI();
    HRESULT Bind_ShaderResources_ChckBoxBACKGROUND();
    HRESULT Bind_ShaderResources_ChckBoxEFFECT();
    HRESULT Bind_ShaderResources_DragLineMaster();
    HRESULT Bind_ShaderResources_DragLineBGM();
    HRESULT Bind_ShaderResources_DragLineUI();
    HRESULT Bind_ShaderResources_DragLineBACKGROUND();
    HRESULT Bind_ShaderResources_DragLineEFFECT();
    HRESULT Bind_ShaderResources_DragBarMaster();
    HRESULT Bind_ShaderResources_DragBarBGM();
    HRESULT Bind_ShaderResources_DragBarUI();
    HRESULT Bind_ShaderResources_DragBarBACKGROUND();
    HRESULT Bind_ShaderResources_DragBarEFFECT();
 
private:
    void    Create_CheckButton();
    void    Is_Picking_CheckButton(POINT pt);
    void    Is_Picking_CheckButton_Master(POINT pt);
    void    Is_Picking_CheckButton_BGM(POINT pt);
    void    Is_Picking_CheckButton_UI(POINT pt);
    void    Is_Picking_CheckButton_BACKGROUND(POINT pt);
    void    Is_Picking_CheckButton_EFFECT(POINT pt);

    void    Update_DragBar();
    void    Create_DragBar();
    void   Is_Picking_DragBar(POINT pt);
    void   Is_Picking_DragBar_Master(POINT pt);
    void   Is_Picking_DragBar_BGM(POINT pt);
    void   Is_Picking_DragBar_UI(POINT pt);
    void   Is_Picking_DragBar_BACKGROUND(POINT pt);
    void   Is_Picking_DragBar_EFFECT(POINT pt);

    void    MovingWnd(CUI* pUI, _float fMoveX);

private:
    //Textures
    CTexture* m_pTexture_OptionCheckBox = { nullptr };
    CTexture* m_pTexture_OptionDetailButton = { nullptr };
    CTexture* m_pTexture_Line = { nullptr };
    CTexture* m_pTexture_OptionDragLine = { nullptr };
    CTexture* m_pTexture_ValueTextWnd = { nullptr };
    CTexture* m_pTexture_DragBar = { nullptr };

    _float  m_fDragLineSizeX = { 0.f };
    _float  m_fDragLineMinX = { 0.f };
    _float  m_fDragLineMaxX = { 0.f };
    _float  m_fX_MasterVolume = { 0.f };
    _float  m_fX_BgmVolume = { 0.f };
    _float  m_fX_UIVolume = { 0.f };
    _float  m_fX_BackGroundVolume = { 0.f };
    _float  m_fX_EffectVolume = {0.f};

    _float  m_fMasterVolume = { 0.2f };
    _float  m_fBgmVolume = { 0.2f };
    _float  m_fUIVolume = { 0.2f };
    _float  m_fBackGroundVolume = { 0.2f };
    _float  m_fEffectVolume = { 0.2f };

    _bool   m_bCheckVolume[5] = { true,true,true,true,true };
    _bool   m_bPreCheckVolume[5] = { false,false,false,false,false };
    _bool   m_bHolding[5] = { false,false,false,false,false };

    CTransform* m_pTransform_Line = { nullptr };
    CTransform* m_pTransform_CheckBox[5] = { nullptr, nullptr, nullptr ,nullptr, nullptr };
    CTransform* m_pTransform_DragLine[5] = { nullptr, nullptr, nullptr ,nullptr, nullptr };
    CTransform* m_pTransform_DragBar[5] = { nullptr, nullptr, nullptr ,nullptr, nullptr };

    RECT    m_rcCheckBox[5] = {};
    RECT    m_rcDragBar[5] = {};
    _float  m_fDragBarX[5] = {};
    _float  m_fPreDragBarX[5] = {};
    _float  m_fSoundRatio[5] = { 0.f,0.f,0.f,0.f,0.f };
    _float  m_fPreSoundRatio[5] = { 0.f,0.f,0.f,0.f,0.f };
    _float  m_fVolume[5] = { 0.2f,0.2f,0.2f,0.2f,0.2f };
    _uint   m_iTextureIndex[5] = { 0,0,0,0,0 };

    CTextBox* m_pTextBox = { nullptr };
    wstring m_strTextWnd;
    wstring m_strFont;

public:
    static  CUI_Option_Sound* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};
END