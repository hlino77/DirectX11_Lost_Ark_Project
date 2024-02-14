#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ServerEntranceButton :
    public CUI
{
private:
    CUI_ServerEntranceButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ServerEntranceButton(const CUI& rhs);
    virtual ~CUI_ServerEntranceButton() = default;

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

private:
    _float  m_fAlpha = { 1.f };
    Vec4    m_vColor = { 3.f, 3.f, 3.f, 1.f };
    wstring m_strWord = TEXT("¿‘¿Â");
    _bool   m_bSound = { false };

public:
    static  CUI_ServerEntranceButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END