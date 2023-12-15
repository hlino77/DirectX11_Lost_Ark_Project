#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ChatLanguageIcon final:
    public CUI
{
    enum LANGUAGE_STATE 
    {
        LANGUAGE_EN, LANGUAGE_KR
    };
private:
    CUI_ChatLanguageIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChatLanguageIcon(const CUI& rhs);
    virtual ~CUI_ChatLanguageIcon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _uint   Get_LanguageState() { return m_iLanguageState; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    
private:
    _uint   m_iLanguageState = { LANGUAGE_EN };//false가 한글키 상태.

public:
    static  CUI_ChatLanguageIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END