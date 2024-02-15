#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Lobby_NickName final:
    public CUI
{
private:
    CUI_Lobby_NickName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Lobby_NickName(const CUI& rhs);
    virtual ~CUI_Lobby_NickName() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_Textbox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_Emblem();

private: 
    void    InitialIze_EmblemPos();
    void    Update_NickNameFrame();
    void    Get_NickName_Changer();
    void    LateUpdate_NickNameFrame();
    void    Update_Equipment_Level();
    void    Initialize_Index(_uint iIndex);

public:
    void    Print_NickName();
    HRESULT Ready_TextBox();
    void    Set_NickName(const wstring& strStageName);
    wstring Get_NickName() { return m_strNickName; }
    void    Random_NickName();
    void	Start_NickNameText();
    void	End_NickNameText();

private:
    _uint m_iButtonState = { 0 };
    _uint   m_iClassIndex = { 0 };
    _bool   m_bInitializeEmblem = { false };

    CTransform* m_pTransformCom_Emblem = { nullptr };

    CTexture* m_pTextureCom_NickNameShine = { nullptr };
    CTexture* m_pTextureCom_Emblem = { nullptr };
    CTexture* m_pTextureCom_Equipment = { nullptr };

    CTextBox* m_pNickNameWnd = { nullptr };
    wstring m_strNickName = TEXT("MoCoCo");
    wstring m_strFont;
    CTextBox* m_pClassNameWnd = { nullptr };
    wstring m_strClassName;
    wstring m_strClassFont;

    CGameObject* m_pLobbyCharacter = { nullptr };
    _bool   m_bSound = { false };
public:
    static  CUI_Lobby_NickName* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END