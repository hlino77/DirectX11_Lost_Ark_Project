#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity_Gauge final:
    public CUI
{
private:
    CUI_WDRIdentity_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_Gauge(const CUI& rhs);
    virtual ~CUI_WDRIdentity_Gauge() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _float   Get_Identity_Ratio() { return m_fIdentity_Ratio; }
    _bool   Get_IdentityOn() { return m_bIdentityOn; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void    Update_Identity_Gauge();

private:
    CGameObject* m_pPlayer = { nullptr };
    _float m_fIdentity_MaxGauge = { 0.f };
    _float  m_fIdentity_CurrGauge = { 0.f };
    _float   m_iIdentity_PreGauge = { 0.f };
    _float  m_fIdentity_Ratio = { 0.f };
    _bool   m_bIdentityOn = { false };

public:
    static  CUI_WDRIdentity_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END