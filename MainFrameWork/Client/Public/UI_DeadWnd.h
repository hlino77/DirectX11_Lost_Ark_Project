#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CPlayer;
class CUI_DeadWnd :
    public CUI
{
private:
    CUI_DeadWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_DeadWnd(const CUI& rhs);
    virtual ~CUI_DeadWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _bool   Is_WatchingMode() { return m_bWatchingMode; }
    void    Set_WatchingMode(_bool bWatchingMode) { m_bWatchingMode = bWatchingMode; }

public:
    void    Update_DeadWnd(_float fTimeDelta, POINT pt);
    void    LateUpdate_DeadWnd(_float fTimeDelta);

    void    Print_TextBox();
    void    Print_TextBoxWatching();
private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_TextBox();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_AnimFrame();
    HRESULT Bind_ShaderResources_ResurrectButton();
    HRESULT Bind_ShaderResources_WathchingButton();
    HRESULT Bind_ShaderResources_ButtonEffect();

private:
    void    MovingWnd(POINT pt);
    void    Create_MovingRect();
    void    Is_Picking_MovingRect(POINT pt);
    void    Create_ResurrectRect();
    void    Is_Picking_ResurrectRect(POINT pt);
    
    void    Create_WatchingRect();
    void    Is_Picking_WatchingRect(POINT pt);

private:
    CPlayer* m_pControlPlayer = { nullptr };

    CTexture* m_pTexture_AnimEffect = { nullptr };
    CTexture* m_pTexture_Button = { nullptr };
    CTexture* m_pTexture_Effect = { nullptr };

    CTransform* m_pTransform_AnimEffect = { nullptr };
    CTransform* m_pTransform_WatchingButton = { nullptr };
    CTransform* m_pTransform_ResurrectButton = { nullptr };

    CTextBox* m_pTextBox = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;

    CTextBox* m_pTextBox_Watching = { nullptr };
    wstring m_strWatchingTag;

    _float  m_fAnimFrame = { 0.f };
    _float  m_fEffectAlpha = { 0.f };

    _bool   m_bWatchingMode = { false };
    _bool   m_bMovingWnd = { false };
    _bool   m_bDecreaseAlpha = { false };

    RECT    m_rcMovingWnd = {};
    RECT    m_rcResurrect = {};
    RECT    m_rcWatching = {};

    _uint   m_iTextureIndex_Button[2] = { 0, 0 };
    _bool   m_bSound[2] = { false, false };
public:
    static  CUI_DeadWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END