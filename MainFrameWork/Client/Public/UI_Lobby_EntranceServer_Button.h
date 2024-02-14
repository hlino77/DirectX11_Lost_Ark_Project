#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END


BEGIN(Client)

class CUILobby_Entrance_to_ServrerButton final:
    public CUI
{
private:
    CUILobby_Entrance_to_ServrerButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUILobby_Entrance_to_ServrerButton(const CUI& rhs);
    virtual ~CUILobby_Entrance_to_ServrerButton() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _bool   Get_Entrance() { return m_bEntrance; }
    void    Set_Entrance(_bool bEntrance) { m_bEntrance = bEntrance; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void    Update_Buttton();
    void    Update_Button_Texture();

private:
    void    Print_TextBox();
    HRESULT Ready_TextBox();
    void	Start_TextBox();
    void	End_TextBox();

private:
    _bool   m_bEntrance = { false };
    CTextBox* m_pTextWnd = { nullptr };
    wstring m_strFont;
    wstring m_strText = TEXT("게임시작");

    _bool   m_bSound = { false };

public:
    static  CUILobby_Entrance_to_ServrerButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END