#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity_BackGround final:
    public CUI
{
private:
    CUI_WDRIdentity_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_BackGround(const CUI& rhs);
    virtual ~CUI_WDRIdentity_BackGround() = default;

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
    void    Update_IdentityBody();

private:
    CGameObject* m_pPlayer = { nullptr };
    _bool   m_bIdentity_On = { false };

public:
    static  CUI_WDRIdentity_BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END