#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Player_MPFrame :
    public CUI
{
private:
    CUI_Player_MPFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Player_MPFrame(const CUI& rhs);
    virtual ~CUI_Player_MPFrame() = default;

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
    static  CUI_Player_MPFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END