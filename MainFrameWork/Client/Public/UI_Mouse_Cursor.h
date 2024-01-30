#pragma once
#include "UI.h"

BEGIN(Client)
class CUI_Mouse_PickedIcon;

class CUI_Mouse_Cursor :
    public CUI
{
public:
    enum MOUSE_STATE
    {
        MOUSE_NORMAL, MOUSE_TOOLTIP, MOUSE_PICKING, MOUSE_END
    };

    enum PICKED_TYPE
    {
        PICKED_SKILL, PICKED_ITEM, PICKED_END
    };

private:
    CUI_Mouse_Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Mouse_Cursor(const CUI& rhs);
    virtual ~CUI_Mouse_Cursor() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_Utility();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _bool   Get_IsPickedIcon() { return m_bPickedIcon; }
    void    Set_Is_PickedIcon(_bool bPickedIcon) { m_bPickedIcon = bPickedIcon; }
    _uint   Get_PickedType() { return m_iPickedType; }
    CGameObject* Get_PickedObject() { return m_pPickedObject; }

public:
    void    Picked_Icon(CTexture* pTexture_Icon, _uint iItemGrade);
    void    Reset_Icon();
    _bool   Is_PickedIcon() { return m_bPickedIcon; }
    void    Set_PickedIcon(_bool bPickedIcon) { m_bPickedIcon = bPickedIcon; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

    void    Moving_MouseCursor();

private:
    CGameObject* m_pPickedObject = { nullptr };
    CUI_Mouse_PickedIcon* m_pUitility_PickedIcon = { nullptr };

    _bool   m_bPickedIcon = { false };
    _uint   m_iMouseState = { 0 };
    _uint   m_iPickedType = { 0 };

public:
    static  CUI_Mouse_Cursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END