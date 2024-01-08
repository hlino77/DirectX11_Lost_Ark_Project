#pragma once
#include "UI.h"

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
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void    Update_Buttton();
    void    Update_Button_Texture();
    

   
public:
    static  CUILobby_Entrance_to_ServrerButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END