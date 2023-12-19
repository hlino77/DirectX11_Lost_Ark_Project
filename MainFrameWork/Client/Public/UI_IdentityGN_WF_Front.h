#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_WF_Front :
    public CUI
{
private:
    CUI_IdentityGN_WF_Front(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_WF_Front(const CUI& rhs);
    virtual ~CUI_IdentityGN_WF_Front() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Change_Icon(_float fTimeDetla);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _float m_fAngle = { 0.f };
    _bool   m_bChangeIcon = { true };
    _bool   m_bTapKey = { false };
    _bool   m_bReturn = { false };

public:
    static  CUI_IdentityGN_WF_Front* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;

};

END