#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_BackGroundFWing final:
    public CUI
{
private:
    CUI_IdentityGN_BackGroundFWing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_BackGroundFWing(const CUI& rhs);
    virtual ~CUI_IdentityGN_BackGroundFWing() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Change_Weapon();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _bool   m_bUpState = { false };
    _float  m_fDistance = { 0.f };

public:
    static  CUI_IdentityGN_BackGroundFWing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END