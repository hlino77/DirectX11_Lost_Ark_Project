#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingLabelBottom :
    public CUI
{
private:
    CUI_LoadingLabelBottom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingLabelBottom(const CUI& rhs);
    virtual ~CUI_LoadingLabelBottom() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_ToolTip();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    wstring m_strTip = TEXT("фа");
    wstring m_strToolTips;

public:
    static  CUI_LoadingLabelBottom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END