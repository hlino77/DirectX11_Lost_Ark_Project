#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CPlayer;
class CUI_DeadWnd :
    public CUI
{
private:
    CUI_DeadWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_DeadWnd(const CUI& rhs);
    virtual ~CUI_DeadWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

public:
    void    Update_DeadWnd(_float fTimeDelta);
    void    LateUpdate_DeadWnd(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_AnimFrame();
    virtual HRESULT Bind_ShaderResources_ResurrectButton();
    virtual HRESULT Bind_ShaderResources_WathchingButton();

private:
    CPlayer* m_pControlPlayer = { nullptr };

    CTexture* m_pTexture_AnimEffect = { nullptr };
    CTexture* m_pTexture_Button = { nullptr };

    CTransform* m_pTransform_AnimEffect = { nullptr };
    CTransform* m_pTransform_WatchingButton = { nullptr };
    CTransform* m_pTransform_ResurrectButton = { nullptr };

    CTextBox* m_pTextBox = { nullptr };
    wstring m_strTextWnd;
    wstring m_strFont;

    _float  m_fAnimFrame = { 0.f };

    _bool   m_bWatchingMode = { false };

public:
    static  CUI_DeadWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END