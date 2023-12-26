#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Player_MPEmpty :
    public CUI
{
private:
    CUI_Player_MPEmpty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Player_MPEmpty(const CUI& rhs);
    virtual ~CUI_Player_MPEmpty() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(const _float& fTimeDelta);
    virtual void LateTick(const _float& fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

public:
    static  CUI_Player_MPEmpty* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END