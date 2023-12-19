#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_BackGroundShine final :
    public CUI
{
private:
    CUI_IdentityGN_BackGroundShine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_BackGroundShine(const CUI& rhs);
    virtual ~CUI_IdentityGN_BackGroundShine() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Change_Weapon(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _bool   m_bHidden = { false };

public:
    static  CUI_IdentityGN_BackGroundShine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END