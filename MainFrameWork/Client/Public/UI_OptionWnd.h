#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_OptionWnd :
    public CUI
{
public:
    enum OPTION_MODE {
        OPTION_VIDEO, OPTION_SOUND
    };

private:
    CUI_OptionWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_OptionWnd(const CUI& rhs);
    virtual ~CUI_OptionWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    _uint   Get_OptionMode() { return m_iOptionMode; }
    _bool   Get_QuitButton() { return m_bQuitButton; }
    _bool   Get_ApplyButton() { return m_bApplyButton; }
    _bool   Get_CancleButton() { return m_bCancleButton; }

    void    Set_QuitButton(_bool bQuit) { m_bQuitButton = bQuit; }
    void    Set_ApplyButton(_bool bApply) { m_bApplyButton = bApply; }
    void    Set_CancleButton(_bool bCancle) { m_bCancleButton = bCancle; }

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Update_Buttons(_float fTimeDelta, POINT pt);
    void    Moving_Wnd(POINT pt);

private:
    void    Print_OptionText();

private:
    void    Is_Picking_MovingWnd(POINT pt);
    void    Is_Picking_ApplyButton(POINT pt);
    void    Is_Picking_CancleButton(POINT pt);
    void    Is_Picking_QuitButton(POINT pt);
    void    Is_Picking_ObjectTag(POINT pt);
    void    Create_ButtonRect();

private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_TextBox();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_ApplyBox();
    HRESULT Bind_ShaderResources_CancleBox();
    HRESULT Bind_ShaderResources_QuitBox();
    HRESULT Bind_ShaderResources_OptionTagVedio();
    HRESULT Bind_ShaderResources_OptionTagSound();

private:
    //Textures
    CTexture* m_pTexture_Button = { nullptr };
    CTexture* m_pTexture_QuitBox = { nullptr };
    CTexture* m_pTexture_OptionTag = { nullptr };

    //QuitButton
    CTransform* m_pTransform_QuitBox = { nullptr };
    //ApplyButton
    CTransform* m_pTransform_ApplyBox = { nullptr };
    _uint   m_iTextureIndex_Button[3] = { 0,0,0 };
    //CancleButton
    CTransform* m_pTransform_CancleBox = { nullptr };
    //OptionTag
    CTransform* m_pTransform_OptionTag[2] = { nullptr };
    _uint   m_iTextureIndex_OptionTag[2] = { 0, 0 };

    RECT    m_rcMovingRect = {};
    RECT    m_rcQuitButton = {};
    RECT    m_rcObjectTag[2] = {};
    RECT    m_rcApplyButton = {};
    RECT    m_rcCancleButton = {};

    CTextBox* m_pOptionTextWnd = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;
    wstring m_strOptionTag[2];
    wstring m_strNowOption;

    _bool   m_bMovingWnd = { false };
    _bool   m_bApplyButton = { false };
    _bool   m_bCancleButton = { false };
    _bool   m_bQuitButton = { false };

    _uint   m_iOptionMode = { 0 };

    _bool   m_bSound[5] = { false, false, false, false, false };
public:
    static  CUI_OptionWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};
END