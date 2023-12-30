#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WRIdentity_Spark :
    public CUI
{

private:
    CUI_WRIdentity_Spark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WRIdentity_Spark(const CUI& rhs);
    virtual ~CUI_WRIdentity_Spark() = default;

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
    CUI* m_pBodyUI = { nullptr };
    _float  m_fFrame = { 0.f };
    _bool   m_bSparkOn = { false };

public:
    static  CUI_WRIdentity_Spark* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END