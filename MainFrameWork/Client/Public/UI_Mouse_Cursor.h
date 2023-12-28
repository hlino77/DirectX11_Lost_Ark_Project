#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Mouse_Cursor :
    public CUI
{
public:
    enum MOUSE_STATE
    {
        MOUSE_NORMAL, MOUSE_TOOLTIP, MOUSE_PICKING, MOUSE_END
    };

    enum PICKED_INFO
    {
        PICKED_SKILL, PICKED_ITEM, PICKED_HELMET, PICKED_SHOLDER, PICKED_ARMOUR, PICKED_GLOVE, PICKED_BOOTS,
        PICKED_STONE, PICKED_END
    };

private:
    CUI_Mouse_Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Mouse_Cursor(const CUI& rhs);
    virtual ~CUI_Mouse_Cursor() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _bool   Get_IsPickedIcon() { return m_bPickedIcon; }
    _uint   Get_MouseState() { return m_bPickedIcon; }
    _uint   Get_PickedInfo() { return m_iPickedInfo; }
    CGameObject* Get_PickedObject() { return m_pPickedObject; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    CGameObject* m_pPickedObject = { nullptr };
    _bool   m_bPickedIcon = { false };
    _uint   m_iMouseState = { 0 };
    _uint   m_iPickedInfo = { 0 };

public:
    static  CUI_Mouse_Cursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END