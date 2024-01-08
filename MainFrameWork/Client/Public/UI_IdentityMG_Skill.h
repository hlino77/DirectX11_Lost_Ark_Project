#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityMG_Skill final:
    public CUI
{
private:
    CUI_IdentityMG_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityMG_Skill(const CUI& rhs);
    virtual ~CUI_IdentityMG_Skill() = default;

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
    CGameObject* m_pPlayer = { nullptr };
    _float  m_fCoolMaxTime = { 0.f };
    _float  m_fCurrCool = { 0.f };
    _float  m_fResultCool = { 0.f };
    _float  m_fCoolRatio = { 0.f };
    _float  m_fCoolAngle = { 0.f };
    _uint   m_iKey = { 0 };

public:
    static  CUI_IdentityMG_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END