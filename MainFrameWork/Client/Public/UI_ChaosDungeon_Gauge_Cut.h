#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ChaosDungeon_GaugeCut final :
    public CUI
{
private:
    CUI_ChaosDungeon_GaugeCut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon_GaugeCut(const CUI& rhs);
    virtual ~CUI_ChaosDungeon_GaugeCut() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    void    Update_GaugeCut();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    CUI* m_pGaugeUI = { nullptr };
    _float  m_fMaxGauge = { 0.f };
    _float  m_fCurrGauge = { 0.f };
    _float  m_fGaugeSizeY = { 0.f };
    _float  m_fGaugeY = { 0.f };
    _float  m_fGaugeRatio = { 0.f };

public:
    static  CUI_ChaosDungeon_GaugeCut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END