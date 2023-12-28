#pragma once
#include "UI.h"

BEGIN(Engine)
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_Mouse_ToolTip_Icon :
    public CUI
{
private:
    CUI_Mouse_ToolTip_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Mouse_ToolTip_Icon(const CUI& rhs);
    virtual ~CUI_Mouse_ToolTip_Icon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    
    void    Set_IconTexture(CTexture* pTexture);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

public:
    static  CUI_Mouse_ToolTip_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END