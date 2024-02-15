#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ChaosDungeon final :
    public CUI
{
private:
    CUI_ChaosDungeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon(const CUI& rhs);
    virtual ~CUI_ChaosDungeon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Update_Gauge(_float fGauge);
    _float    Get_Gauge();

private:
    HRESULT	UI_Set();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    class CUI_ChaosDungeon_Gauge* m_pGaugeUI = { nullptr };

public:
    static  CUI_ChaosDungeon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END