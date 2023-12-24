#pragma once
#include "UI.h"

BEGIN(Engine)
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_HoldingFill :
    public CUI
{
private:
    CUI_HoldingFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_HoldingFill(const CUI& rhs);
    virtual ~CUI_HoldingFill() = default;

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
    class CPlayer_Skill* m_pPlayerSkill = { nullptr };
    _bool   m_bHoldOn = { 0.f };
    _float m_fMaxDuration = { 0.f };
    _float m_fDuration = { 0.f };

public:
    static  CUI_HoldingFill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END