#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ServerGrid final:
    public CUI
{
public:
    enum GRIDSTATE
    {
        GRID_NORMAL, GRID_PICKING, GRID_SELECTED, GRID_UNPICKING, GRID_END
    };

private:
    CUI_ServerGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ServerGrid(const CUI& rhs);
    virtual ~CUI_ServerGrid() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override;
    
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    Vec4  Linear_Color(_float fTimeDelta);
    void    Move_UI(_float fTimeDelta);
private:
    GRIDSTATE   m_eState = { GRID_NORMAL };
    _float      m_fDelay = {0.f};
    Vec4        m_vEndColor = { 5.f, 5.f, 5.f, 1.f };
    Vec4        m_vLinearColor = {1.f, 1.f, 1.f, 1.f};
public:
    static  CUI_ServerGrid* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END