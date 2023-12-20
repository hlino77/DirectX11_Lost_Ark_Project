#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Player_MPFill :
    public CUI
{
private:
    CUI_Player_MPFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Player_MPFill(const CUI& rhs);
    virtual ~CUI_Player_MPFill() = default;

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
    _float  m_fCurrentRatio = { 1.f };
    _float  m_fPreRatio = { 1.f };
    class CGameObject* m_pPlayer = { nullptr };

public:
    static  CUI_Player_MPFill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END