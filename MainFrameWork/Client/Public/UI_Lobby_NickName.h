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
    void    Update_NickNameFrame();
    void    InitialIze_EmblemPos();
    void    Update_Equipment_Level();

public:
    void    Print_NickName();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox();
    void    Set_NickName(const wstring& strStageName);

private:
    _uint m_iButtonState = { 0 };
    _uint   m_iClassIndex = { 0 };
    _bool   m_bInitializeEmblem = { false };

    CTransform* m_pTransformCom_Emblem = { nullptr };

    CTexture* m_pTextureCom_NickNameShine = { nullptr };
    CTexture* m_pTextureCom_Emblem = { nullptr };
    CTexture* m_pTextureCom_Equipment = { nullptr };

    CTextBox* m_pNickNameWnd = { nullptr };
    wstring m_strNickName[4];
    wstring m_strFont;
public:
    static  CUI_Lobby_NickName* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END