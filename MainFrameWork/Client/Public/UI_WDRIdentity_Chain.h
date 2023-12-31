#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity_Chain final:
    public CUI
{
public:
    struct CHAIN_DESC
    {
        CUI* pGaugeUI;
        CUI* pHammerUI;
    };

    enum CHAIN_STATE {
        CHAIN_THREE, CHAIN_CUT1, CHAIN_TWO, CHAIN_CUT2, CHAIN_ONE, CHAIN_CUT3, CHAIN_NONE, CHAIN_END
    };

private:
    CUI_WDRIdentity_Chain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_Chain(const CUI& rhs);
    virtual ~CUI_WDRIdentity_Chain() = default;

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
    void    Update_Chain_State(_float fTimeDelta);
    void    Update_Chain_Idle(_float fTimeDelta);
    void    Update_Chain_Cutting(_float fTimeDelta);

private:
    CUI* m_pGaugeUI = { nullptr };
    CUI* m_pHammerUI = { nullptr };

    CTexture* m_pTexture_ChainCut = { nullptr };
    _float   m_fCurrRatio = { 0 };
    _uint   m_iCurrChaniState = { CHAIN_THREE };
    _bool   m_bCutting = { false };
    _bool   m_bIdentityOn = { false };
    _float  m_fFrame = { 0.f };
    _float  m_fDelay = { 0.f };

public:
    static  CUI_WDRIdentity_Chain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END