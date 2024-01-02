#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity_Shine_IdenOff final:
    public CUI
{
private:
    CUI_WDRIdentity_Shine_IdenOff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_Shine_IdenOff(const CUI& rhs);
    virtual ~CUI_WDRIdentity_Shine_IdenOff() = default;

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
    //_uint  m_iCurrGauge = { 0 };
    //_uint  m_iPreGauge = {0};
    //_float m_fFrame = { 0.f };

public:
    static  CUI_WDRIdentity_Shine_IdenOff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END