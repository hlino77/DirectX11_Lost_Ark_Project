#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_DamageFont :
    public CUI
{
public:
    struct DAMAGEFONT_DESC
    {
        _uint   RenderTargetIndex;
    };

private:
    CUI_DamageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_DamageFont(const CUI& rhs);
    virtual ~CUI_DamageFont() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_DamageFont(_uint iIndex);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override;
    void	Set_Active(_bool bActive);
    void	Print_DamageFont(CGameObject* pMonster, _float TextBoxScale, _float fOffsetY, _float fLength, _bool IsCritical, _uint iDamage);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Ready_TextBox();
    void    Release_ChatWindows();

private:
    void    Update_DamageFont();
    void	Start_DamageFont();
    void	End_DamageFont();

private:
    CGameObject* m_pOwner;

    wstring m_szInputText;
    wstring m_szFont;
    wstring m_szRenderTargetIndex = TEXT("");
    Vec2 m_vTextBoxScale ;
    Vec2 m_vTextBoxMaxScale;

    _uint   m_iDamage = { 0 };
    _bool   m_bCriticalHit = { false };

    _float  m_fDuration = { 0.f };
    _float m_fRandomX = { 0.f };
    _float m_fOffSetY = { 0.f };
    _float m_fRandomZ = { 0.f };

    Vec4    m_vColorCrit = { 1.0f, 0.87f, 0.21f, 1.f };
    Vec4    m_vColorNormal = { 0.97f, 0.96f, 0.98f, 1.f };
    Vec2    m_vHostPos = {};
    Vec3    m_vHostSize = {};

    CTextBox* m_pDamageFontWnd = nullptr;
public:
    static  CUI_DamageFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END