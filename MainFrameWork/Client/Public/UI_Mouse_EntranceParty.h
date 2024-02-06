#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CPlayer;
class CUI_Mouse_EntranceParty :
    public CUI
{
private:
    CUI_Mouse_EntranceParty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Mouse_EntranceParty(const CUI& rhs);
    virtual ~CUI_Mouse_EntranceParty() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

private:
    void    Print_NickNameText();
    

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_UIPosition(_bool & bPick);
    void    Set_NickName(const wstring& strNickName);
    void    Set_Active_EntrancePartyWnd(_bool  bActive, CPlayer* pPlayer);
    _bool   Get_InviteParty() { return m_bInviteParty; }
    void    Set_InviteParty(_bool bInviteParty) { m_bInviteParty = bInviteParty; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_EntranceWnd();
    HRESULT Ready_TextBox();

private:
    void    Update_EntranceButton();
    void    Create_EntranceWnd();
    void    Send_InvitationPlayer();

private:
    CPlayer* m_pInvitedPlayer = { nullptr };

    CTexture* m_pTexture_EntranceWnd = { nullptr };
    CTransform* m_pTransform_EntranceWnd = { nullptr };
    
    CTextBox* m_pTextBox = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;
    wstring m_strNickName;

    _bool   m_bDeActive = { false };
    _float  m_fDeActiveDelay = { 0.f };
    _bool   m_bInviteParty = {false};
    
public:
    static  CUI_Mouse_EntranceParty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END