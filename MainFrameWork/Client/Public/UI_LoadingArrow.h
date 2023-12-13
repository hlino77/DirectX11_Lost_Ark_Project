#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingArrow :
    public CUI
{
private:
    CUI_LoadingArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingArrow(const CUI& rhs);
    virtual ~CUI_LoadingArrow() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Move_Arrow(_float fSizeX);
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    wstring m_strStage = TEXT("º£¸¥ ºÏºÎ");

public:
    static  CUI_LoadingArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END