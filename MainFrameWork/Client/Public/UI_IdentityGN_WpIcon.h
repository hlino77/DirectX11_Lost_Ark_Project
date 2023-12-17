#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_WpIcon :
    public CUI
{
private:
    CUI_IdentityGN_WpIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_WpIcon(const CUI& rhs);
    virtual ~CUI_IdentityGN_WpIcon() = default;

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
 _uint  m_iTextureIndex = { 0 };

public:
    static  CUI_IdentityGN_WpIcon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END