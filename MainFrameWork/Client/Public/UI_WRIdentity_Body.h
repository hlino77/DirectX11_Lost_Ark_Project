#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WRIdentity_Body :
    public CUI
{
public:
    enum WRIDENTITY_STATE
    {
        WRIDENTITY_NORMAL, WRIDENTITY_MAX, WRIDENTITY_TRANSFORM_ON, WRIDENTITY_ON,
        WRIDENTITY_TRANSFORM_OFF, WRIDENTITY_COOL, WRIDENTITY_END
    };

private:
    CUI_WRIdentity_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WRIdentity_Body(const CUI& rhs);
    virtual ~CUI_WRIdentity_Body() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _uint   Get_Current_IdentityState() { return m_iIdentity_CurrState; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void    Get_Identity_State(class CPlayer* pPlayer);
    void    Identity_NormalState(_float fTimeDelta);
    void    Identity_MaxGauage();
    void    Identity_On();
    void    Identity_CoolState(_float fTimeDelta);
    void    Transform_Identity(_float fTimeDelta);

private:
    _bool   m_bIdentity = { false };
    _bool   m_bTransfrom = { false };

    _uint   m_iIdentity_CurrState = { 0 };

    _uint  m_iIdentity_Gauge = { 0 };
    _uint  m_iIdentity_MaxGauge = { 0 };

    _float  m_fIdentity_CurrCool = {0.f};
    _float  m_fIdentity_MaxCool = { 0.f };

    _float  m_fFrame = { 0.f };
    _float  m_fDelay = { 2.f };
public:
    static  CUI_WRIdentity_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END