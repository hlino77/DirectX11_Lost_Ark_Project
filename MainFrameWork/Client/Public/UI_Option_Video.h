#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Option_Video :
    public CUI
{
private:
    CUI_Option_Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Option_Video(const CUI& rhs);
    virtual ~CUI_Option_Video() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

private:
    void    Print_OptionText();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Is_VideoOptionMode(_uint iMode);
    void    Update_OptionVideo(_float fTimeDelta, class CUI* pUI, POINT pt);
    void    Set_Active_Option(_bool bOption);
    void    Apply_Option();
    void    Cancle_Option();

private:
    void    Create_CheckButton();
    void    Is_Picking_CheckButton(POINT pt);
    void    Is_Picking_CheckButton_PBR(POINT pt);
    void    Is_Picking_CheckButton_SSAO(POINT pt);
    void    Is_Picking_CheckButton_Fxaa3_11(POINT pt);

    void    Update_DragBar();
    void    Create_DragBar();
    void   Is_Picking_DragBar(POINT pt);
    void   Is_Picking_DragBar_IBL(POINT pt);
    void   Is_Picking_DragBar_SSR(POINT pt);
    void   Is_Picking_DragBar_ScreenTone_GBar(POINT pt);
    void   Is_Picking_DragBar_ScreenTone_CBar(POINT pt);
    void   Is_Picking_DragBar_ScreenTone_SBar(POINT pt);
    void    MovingWnd(CUI* pUI, _float fMoveX);

private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_TextBox();
    virtual HRESULT Bind_ShaderResources();
   HRESULT Bind_ShaderResources_PBR();
   HRESULT Bind_ShaderResources_SSAO();
   HRESULT Bind_ShaderResources_Fxaa3_11();
   HRESULT Bind_ShaderResourcess_LineT();
   HRESULT Bind_ShaderResources_IBL();
   HRESULT Bind_ShaderResources_IBLDrag();
   HRESULT Bind_ShaderResources_SSR();
   HRESULT Bind_ShaderResources_SSRDrag();
   HRESULT Bind_ShaderResourcess_LineB();

   HRESULT Bind_ShaderResources_ScreenTone_G();
   HRESULT Bind_ShaderResources_ScreenTone_GBar();
   HRESULT Bind_ShaderResources_ScreenTone_C();
   HRESULT Bind_ShaderResources_ScreenTone_CBar();
   HRESULT Bind_ShaderResources_ScreenTone_S();
   HRESULT Bind_ShaderResources_ScreenTone_SBar();

private:
    CTexture* m_pTexture_OptionCheckBox = { nullptr };
    CTexture* m_pTexture_OptionDetailButton = { nullptr };
    CTexture* m_pTexture_Line = { nullptr };
    CTexture* m_pTexture_OptionDragLine = { nullptr };
    CTexture* m_pTexture_ValueTextWnd = { nullptr };
    CTexture* m_pTexture_DragBar = { nullptr };

    CTransform* m_pTransform_OptionLine[2] = { nullptr, nullptr };
    CTransform* m_pTransform_OptionCheckBox[3] = { nullptr, nullptr ,nullptr};
    CTransform* m_pTransform_OptionDragLine[5] = { nullptr, nullptr, nullptr, nullptr ,nullptr };
    CTransform* m_pTransform_OptionDragBar[5] = { nullptr, nullptr, nullptr, nullptr ,nullptr };

    RECT    m_rcCheckButton[3] = {};
    RECT    m_rcDragBar[5] = { };

    CTextBox* m_pOptionTextWnd = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;

    _bool   m_bOption_PBR = { true };
    _bool   m_bOption_SSAO = { true };
    _bool   m_bOption_Fxaa3_11 = { true };

    _uint   m_iTextureIndex_PBR = { 0 };
    _uint   m_iTextureIndex_SSAO = { 0 };
    _uint   m_iTextureIndex_Fxaa3_11 = { 0 };

    //DragOption
    _float  m_fDragLineSizeX = { 0.f };
    _float  m_fDragLineMinX = { 0.f };
    _float  m_fDragLineMaxX = { 0.f };
    _float  m_fDragLine_ScreenTone = { 0.f };
    _float  m_fDragBarX_IBL = { 0.f };
    _float  m_fDragBarX_SSR = { 0.f };
    _float  m_fDragBarX_ScreenTone_Grayscale = { 0.f };
    _float  m_fDragBarX_ScreenTone_Contrast = { 0.f };
    _float  m_fDragBarX_ScreenTone_Saturation = { 0.f };
    _float  m_fRatioX[5] = {0.f, 0.f, 0.f, 0.f, 0.f};

    _uint   m_iIndex_IBL = { 0 };
    _uint   m_iIndex_SSR = { 0 };
    _float  m_fScreenTone_Max = { 255.f };
    _float  m_fScreenTone_Grayscale = { 0.f };
    _float  m_fScreenTone_Contrast = { 0.f };
    _float  m_fScreenTone_Saturation = { 0.f };

    //취소 및 닫기 롤백함수
    _bool   m_bPre_PBR = { true };
    _bool   m_bPre_SSAO = { true };
    _bool   m_bPre_Fxaa3_11 = { true };
    _float  m_fPreX_IBL = { 0.f };
    _float  m_fPreX_SSR = { 0.f };
    _float  m_fPreX_ScreenTone_Grayscale = { 0.f };
    _float  m_fPreX_ScreenTone_Contrast = { 0.f };
    _float  m_fPreX_ScreenTone_Saturation = { 0.f };
    _float  m_fPreRatioX[5] = { 0.f, 0.f, 0.f, 0.f, 0.f };

    _bool   m_bHolding[5] = { false, false, false, false, false };
public:
    static  CUI_Option_Video* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};
END