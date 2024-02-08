#pragma once
#include "UI.h"

BEGIN(Client)
class CUI_OptionWnd;
class CUI_Option_Video;
class CUI_Option_Sound;
class CUI_Option :
    public CUI
{
private:
    CUI_Option(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Option(const CUI& rhs);
    virtual ~CUI_Option() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

private:
    HRESULT UI_Set();
    void    Update_OptionWnd(_float fTimeDelta);
    void    LateUpdate_OptionWnd(_float fTimeDelta);

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void    Option_OnOff();

private:
    CUI_OptionWnd* m_pOptionWnd = { nullptr };
    CUI_Option_Video* m_pOptionVideo = { nullptr };
    CUI_Option_Sound* m_pOptionSound = { nullptr };
    _uint   m_iOptionMode = { 0 };

    _bool   m_bOption = { false };
    _float  m_fCloseAcc = { 0.f };
public:
    static  CUI_Option* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};
END