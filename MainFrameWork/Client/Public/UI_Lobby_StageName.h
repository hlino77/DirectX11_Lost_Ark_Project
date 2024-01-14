#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Lobby_StageName final:
    public CUI
{
public:
    struct LOBBYSTAGE_DESC {
        CUI* pUI;
        _uint iIndex;
    };

private:
    CUI_Lobby_StageName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Lobby_StageName(const CUI& rhs);
    virtual ~CUI_Lobby_StageName() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_CharacterIndex(_uint iIndex) { m_iCharacterIndex = iIndex; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void    Update_NickNameFrame();

private:
    void    Print_TextBox();
    HRESULT Ready_TextBox();
    void	Start_TextBox();
    void	End_TextBox();

private:
    _uint   m_iCharacterIndex = { 0 };
    _bool   m_bTextOn = { false };
    CUI*     m_pNickName = { nullptr };
    CTextBox* m_pTextWnd = { nullptr };
    wstring m_strFont;
    wstring m_strText = TEXT("º£¸¥ ¼º");

public:
    static  CUI_Lobby_StageName* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END