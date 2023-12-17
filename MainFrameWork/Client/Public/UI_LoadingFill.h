#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingFill :
    public CUI
{
private:
    CUI_LoadingFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingFill(const CUI& rhs);
    virtual ~CUI_LoadingFill() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Change_SizeX(_float fSizeX);//MaxSize = g_WinSizeX
    _float    Get_SizeX() { return m_fSizeX; }
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

public:
    static  CUI_LoadingFill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END