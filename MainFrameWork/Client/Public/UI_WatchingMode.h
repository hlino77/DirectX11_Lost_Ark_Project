#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CPlayer;
class CUI_WatchingMode :
    public CUI
{
private:
    CUI_WatchingMode(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WatchingMode(const CUI& rhs);
    virtual ~CUI_WatchingMode() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_WatchingMode(_bool  bWatching);
    _bool   Get_WatchingMode() { return m_bWathingMode; }

    void    Update_CCTV(_float fTimeDelta, POINT pt);
    void    LateUpdate_CCTV(_float fTimeDelta);

    void    Set_TargetPlayer();
    void    Reset_TargetPlayer();
    void    Print_TextBox();

public:
    void    Create_NextButton();
    void    Is_Picking_NextButton(POINT pt);
    void    Create_BackButton();
    void    Is_Picking_BackButton(POINT pt);
    void    Create_ReturnButton();
    void    Is_Picking_ReturnButton(POINT pt);

private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_TextBox();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Button();
    HRESULT Bind_ShaderResources_NextButton();
    HRESULT Bind_ShaderResources_BackButton();
    
    void    Find_IndexPartyMember();

private:
    CTexture* m_pTexture_Button = { nullptr };
    CTexture* m_pTexture_ChangeButton = { nullptr };
  
    CTransform* m_pTransform_Button = { nullptr };
    CTransform* m_pTransform_NextButton = { nullptr };
    CTransform* m_pTransform_BackButton = { nullptr };

    CPlayer* m_pWatchingPlayer = nullptr;
    wstring strNowPlayer;

    CTextBox* m_pTextBox = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;

    CTextBox* m_pReturnTextBox = { nullptr };
    wstring m_strReturnWndTag;

    _int m_iPartyIndex = { 1 };
    _uint   m_iChangeButton[2] = { 0, 0 };

    RECT m_rcButton[3] = {};

    _bool   m_bWathingMode = { false };
    _bool   m_bSound[3] = { false, false, false };
public:
    static  CUI_WatchingMode* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END