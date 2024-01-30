#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentitySP_MainWnd :
    public CUI
{
private:
    CUI_IdentitySP_MainWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentitySP_MainWnd(const CUI& rhs);
    virtual ~CUI_IdentitySP_MainWnd() = default;

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
    virtual HRESULT Bind_ShaderResources_MainWnd();
    virtual HRESULT Bind_ShaderResources_BlueLight();
    virtual HRESULT Bind_ShaderResources_Ink();
    virtual HRESULT Bind_ShaderResources_Brush();
    virtual HRESULT Bind_ShaderResources_BubbleFrames();

private:
    CTexture* m_pTexture_MainWnd = { nullptr };
    CTexture* m_pTexture_BlueLight = { nullptr };
    CTexture* m_pTexture_Ink = { nullptr };
    CTexture* m_pTexture_Brush = { nullptr };
    CTexture* m_pTexture_BubbleFrmaes = { nullptr };

    CTransform* m_PTransform_MainWnd = { nullptr };
    CTransform* m_PTransform_BlueLight = { nullptr };
    CTransform* m_PTransform_Ink = { nullptr };
    CTransform* m_PTransform_Brush = { nullptr };
    CTransform* m_PTransform_BubbleFrmaes = { nullptr };

    _float  m_fFrame = { 0.f };

public:
    static  CUI_IdentitySP_MainWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END