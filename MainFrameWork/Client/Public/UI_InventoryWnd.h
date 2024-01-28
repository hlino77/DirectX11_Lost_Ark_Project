#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CItem;
class CPlayer;

class CUI_InventoryWnd :
    public CUI
{
private:
    CUI_InventoryWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_InventoryWnd(const CUI& rhs);
    virtual ~CUI_InventoryWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox(class CPlayer* pOwner);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Move_InventoryWnd(POINT pt);
    void    Create_Rect_MoveWnd();
    void    Is_Picking_MoveWnd(POINT pt);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Ready_MoneyTextBox(class CPlayer* pOwner);
    void    Print_Player_Money();

private:
    RECT    m_rcMovingWnd = {};
    _bool   m_bHolding = { false };

    CTextBox* m_pMoneyWnd = { false };
    wstring   m_strWndTag;
    wstring   m_strSilver;
    wstring   m_strGold;
    wstring   m_strFont;

public:
    static  CUI_InventoryWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END