#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityMG final:
    public CUI
{
private:
    CUI_IdentityMG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityMG(const CUI& rhs);
    virtual ~CUI_IdentityMG() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT UI_Set();

private:
    CGameObject* m_pPlayer = { nullptr };
    _uint   m_iIdentity_MaxGauge = { 0 };
    _uint   m_iIdentity_CurrGauge = { 0 };
    _float  m_fIdentity_Ratio = { 0.f };
    _uint   m_iKey = { 0 };

    CTexture* m_pTextureCom_Frame = { nullptr };

public:
    static  CUI_IdentityMG* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END