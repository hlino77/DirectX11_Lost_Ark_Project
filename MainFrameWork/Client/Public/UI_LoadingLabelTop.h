#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingLabelTop :
    public CUI
{
private:
    CUI_LoadingLabelTop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingLabelTop(const CUI& rhs);
    virtual ~CUI_LoadingLabelTop() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_StageName(const wstring& strStageName);


private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    wstring m_strStage = TEXT("º£¸¥ ¼º");

public:
    static  CUI_LoadingLabelTop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END