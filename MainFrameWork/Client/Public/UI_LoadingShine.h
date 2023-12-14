#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingShine :
    public CUI
{
private:
    CUI_LoadingShine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingShine(const CUI& rhs);
    virtual ~CUI_LoadingShine() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Change_SizeX();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _float  m_fMaxLengthX = { 0.f };

public:
    static  CUI_LoadingShine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END