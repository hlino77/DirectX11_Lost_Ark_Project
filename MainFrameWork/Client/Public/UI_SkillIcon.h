#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_SkillIcon :
    public CUI
{
private:
    CUI_SkillIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_SkillIcon(const CUI& rhs);
    virtual ~CUI_SkillIcon() = default;

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
    wstring m_strSkillName = TEXT("Death_Fire");
    _bool   m_bSkillOn = { false };
    _float  m_fCoolMaxTime = { 24.f };
    _float  m_fCurrCool = { 24.f };
    _float  m_fCoolRatio = { 0.f };
    _float  m_fCoolAngle = { 0.f };

public:
    static  CUI_SkillIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END