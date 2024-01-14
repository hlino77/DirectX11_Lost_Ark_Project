#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WRIdentity_Gauge :
    public CUI
{
private:
    CUI_WRIdentity_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WRIdentity_Gauge(const CUI& rhs);
    virtual ~CUI_WRIdentity_Gauge() = default;

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
    void    Identity_Gauge();

private:
    CUI* m_pBodyUI = { nullptr };
    _float  m_fResultGauge = { 0.f };
    _float  m_fGaugeAngle = { 0.f };
    _float  m_fGaugeRatio = { 0.f };

    _float  m_fIdentity_Gauge = { 0.f };
    _float  m_fIdentity_MaxGauge = { 0.f };

public:
    static  CUI_WRIdentity_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END