#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityMG_Gauge final:
    public CUI
{
private:
    CUI_IdentityMG_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityMG_Gauge(const CUI& rhs);
    virtual ~CUI_IdentityMG_Gauge() = default;

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
    virtual HRESULT Bind_ShaderResources_Gauge();
    virtual HRESULT Bind_ShaderResources_Cut();

private:
    void    Update_Identity_Gauge();
    void    Setting_GaugeScale();
    void    Update_Identity_Cut();
private:
    CGameObject* m_pPlayer = { nullptr };
    _uint   m_iGaugeIndex = { 0 };
    _uint   m_iIdentity_MaxGauge = { 0 };
    _uint   m_iIdentity_CurrGauge = { 0 };
    _float  m_fIdentity_Ratio = { 0.f };

    CTexture* m_pTextureCom_Empty = { nullptr };
    CTexture* m_pTextureCom_Frame = { nullptr };
    CTexture* m_pTextureCom_Cut = { nullptr };

    CTransform* m_pTransform_Gauge = { nullptr };
    _bool   m_bSetScale_Gauge = { false };

    CTransform* m_pTransform_Cut = { nullptr };
    _float    m_fGaugeCutY = { 0.f };
    _float    m_fGaugeCutOffSet = { 0.f };
    _uint   m_iGaugeCutTextureIndex = { 0 };

public:
    static  CUI_IdentityMG_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END