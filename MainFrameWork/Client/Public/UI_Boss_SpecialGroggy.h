#pragma once
#include "UI.h"

BEGIN(Client)
class CBoss;
class CUI_Boss_SpecialGroggy final:
    public CUI
{
private:
    CUI_Boss_SpecialGroggy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Boss_SpecialGroggy(const CUI& rhs);
    virtual ~CUI_Boss_SpecialGroggy() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_Fill();
    void    Update_GroggyCount();
    void    Update_Position();

private:
    CTexture* m_pTexture_Fill = { nullptr };
    CGameObject* m_pOwner = { nullptr };

    _float  m_fMaxGroggyCount = { 0.f };
    _float  m_fCurrGroggyCount = { 0.f };
    _float  m_fRatio = { 0.f };

public:
    static  CUI_Boss_SpecialGroggy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END