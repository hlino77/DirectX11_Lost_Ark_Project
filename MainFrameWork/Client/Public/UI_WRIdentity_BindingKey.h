#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WRIdentity_BindingKey :
    public CUI
{

private:
    CUI_WRIdentity_BindingKey(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WRIdentity_BindingKey(const CUI& rhs);
    virtual ~CUI_WRIdentity_BindingKey() = default;

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
    CUI*    m_pBodyUI = { nullptr };
    _bool   m_bKeyOff = { false };

public:
    static  CUI_WRIdentity_BindingKey* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END