#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Identity_WF_Front :
    public CUI
{
private:
    CUI_Identity_WF_Front(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Identity_WF_Front(const CUI& rhs);
    virtual ~CUI_Identity_WF_Front() = default;

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
 _uint  m_iTextureIndex = { 0 };

public:
    static  CUI_Identity_WF_Front* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;

};

END