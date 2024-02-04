#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity_Hammer final:
    public CUI
{
public:
    enum HAMMER_STATE
    {
        HAMMER_NORMAL, HAMMER_TRANSFORM_ON, HAMMER_ON, HAMMER_TRANSFORM_OFF, HAMMER_END
    };

private:
    CUI_WDRIdentity_Hammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_Hammer(const CUI& rhs);
    virtual ~CUI_WDRIdentity_Hammer() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _uint   Get_CurrHammerState() { return m_iCurrHammerState; }
    void    Update_EffectPos(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_OnEffect();
    virtual HRESULT Bind_ShaderResources_MaskedEffect();
    virtual HRESULT Bind_ShaderResources_HorizonEffect();

private:
    void    Update_Identity_Hammer(_float fTimeDelta);
    void    Identity_Hammer_Normal(_float fTimeDelta);
    void    Identity_Hammer_Transform_On(_float fTimeDelta);
    void    Identity_Hammer_On(_float fTimeDelta);
    void    Identity_Hammer_Transform_Off(_float fTimeDelta);

private:
    CGameObject* m_pPlayer = { nullptr };
    _uint   m_iCurrHammerState = { (_uint)HAMMER_NORMAL };
    Vec4    m_vMaxColor = { 5.f, 5.f, 5.f, 1.f };
    Vec4    m_vNormalColor = { 1.f, 1.f, 1.f, 1.f };
    _bool   m_bIdentity_On = { false };
    _bool   m_bColorDecrease = { false };

    CTexture* m_pTexture_HammerOnEffect = { nullptr };
    CTransform* m_pTransform_HammerOnEffect = { nullptr };

    CTexture* m_pTexture_MaskedOnEffect = { nullptr };
    CTransform* m_pTransform_MaskedOnEffect = { nullptr };

    _float m_fSizeX_OnEffect = { 0.f };
    _float m_fSizeY_OnEffect = { 0.f };
    _float m_fOnEffectAlpha = { 0.f };

    _float m_fSizeX_MaskedEffect = { 0.f };
    _float m_fSizeY_MaskedEffect = { 0.f };
    _float  m_fMaskedEffectAlpha = { 0.f };
    _float  m_fMaskedEffectDelay = { 0.f };
    _bool   m_bMaskedEffectAlphaDecrease = { false };

    CTexture* m_pTexture_HorizonEffect = { nullptr };
    CTransform* m_pTransform_HorizonEffect = { nullptr };
    

public:
    static  CUI_WDRIdentity_Hammer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END