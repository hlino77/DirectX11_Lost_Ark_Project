#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ChaosDungeon_GaugeSpinShine final :
    public CUI
{
private:
    CUI_ChaosDungeon_GaugeSpinShine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon_GaugeSpinShine(const CUI& rhs);
    virtual ~CUI_ChaosDungeon_GaugeSpinShine() = default;

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
    HRESULT Bind_ShaderResources_Orange();

private:
    CTexture* m_pTexture_OrangeShine = { nullptr };
    CTransform* m_pTransform_OrangeShine = { nullptr };

    _float  m_fFrame = { 0.f };
    _bool   m_bInitPos = { false };

public:
    static  CUI_ChaosDungeon_GaugeSpinShine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END