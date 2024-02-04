#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_ValtanClearWnd :
    public CUI
{
private:
    CUI_ValtanClearWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ValtanClearWnd(const CUI& rhs);
    virtual ~CUI_ValtanClearWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_Is_ValtanClear(_bool    bValtanClear) { m_bValtanClear = bValtanClear; }


private:
    void    Update_BackGroundAlpha(_float fTimeDelta);
    void    Update_ClearBackGround(_float fTimeDelta);
    void    Update_MaskedAnimFrame(_float fTimeDleta);
    void    Update_MaskedShineAnimFrame(_float fTimeDelta);
    void    Update_MaskedCircle(_float fTimeDelta);
    void    Update_MaskedShine(_float fTimeDelta);
    void    Update_Emblem(_float fTimeDelta);
    void    Update_Text(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_BackGround();
    virtual HRESULT Bind_ShaderResources_ClearBackGround();
    virtual HRESULT Bind_ShaderResources_MaskedAnimEffect();
    virtual HRESULT Bind_ShaderResources_MaskedShineAnim();
    virtual HRESULT Bind_ShaderResources_ClearDots();
    virtual HRESULT Bind_ShaderResources_MaskedCircle();
    virtual HRESULT Bind_ShaderResources_MaskedShine();
    virtual HRESULT Bind_ShaderResourcese_ClearCircle();
    virtual HRESULT Bind_ShaderResourcese_ClearText();

private:
    //기본 TextureCom, 기본 Transform은 엠블럼으로 사용.
    CTexture* m_pTexture_BackGround = { nullptr };
    CTexture* m_pTexture_ClearBackGround = { nullptr };
    CTexture* m_pTexture_MaskedAnimEffect = { nullptr };
    CTexture* m_pTexture_MaskedAnimShine= { nullptr };
    CTexture* m_pTexture_ClearDots = { nullptr };
    CTexture* m_pTexture_MaskedCircle = { nullptr };
    CTexture* m_pTexture_MaskedShine = { nullptr };
    CTexture* m_pTexture_ClearCircle = { nullptr };
    // CTexture*    m_pTextureCom = 엠블럼
    CTexture* m_pTexture_ClearText = { nullptr };

    CTransform* m_pTransform_BackGround = { nullptr };
    CTransform* m_pTransform_ClearBackGround = { nullptr };
    CTransform* m_pTransform_MaskedAnimEffect = { nullptr };
    CTransform* m_pTransform_MaskedAnimShine = { nullptr };
    CTransform* m_pTransform_ClearDots = { nullptr };
    CTransform* m_pTransform_MaskedCircle = { nullptr };
    CTransform* m_pTransform_MaskedShine = { nullptr };
    CTransform* m_pTransform_ClearCircle = { nullptr };
    // CTransform*    m_pTransformCom = 엠블럼
    CTransform* m_pTransform_ClearText = { nullptr };

    _float  m_fBackGroundAlpha = { 0.f };
    _float  m_fClearAnimAlpha = { 0.f };
    _float  m_fMaskedShineAlpha = { 0.f };
    _float  m_fEmblemAlpha = { 0.f };
    _float  m_fMaskedAnimAlpha = { 0.f };

    _float  m_fSizeX_Text = { 0.f };
    _float  m_fSizeY_Text = { 0.f };
    _float  m_fAlpha_Text = { 0.f };
    _float  m_fMaskedAnimFrame = { 0.f };
    _float  m_fMaskedShineAnimFrame = { 0.f };

    _bool   m_bValtanClear = { false };
    _bool   m_bClearAnim = { false };
    _bool   m_bMaskedShine = { false };
    _bool   m_bTextOn = { false };

    Vec4    m_vMaskedColor = { 1.f, 1.f, 1.f,1.f };

public:
    static  CUI_ValtanClearWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END