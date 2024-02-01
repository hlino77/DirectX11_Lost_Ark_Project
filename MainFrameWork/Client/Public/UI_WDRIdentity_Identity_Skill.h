#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_WDRIdentity_Identity_Skill final:
    public CUI
{
private:
    CUI_WDRIdentity_Identity_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_Identity_Skill(const CUI& rhs);
    virtual ~CUI_WDRIdentity_Identity_Skill() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    void    Print_CoolTime();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Ready_TextBox();
private:
    _bool   m_bIdentity_On = { false };

    //ÄðÅ¸ÀÓ
    CPlayer_Skill* m_pSkill = { nullptr };
    CTexture* m_pTextureCom_Skill = { nullptr };
    CTexture* m_pTextureCom_Shine = { nullptr };
    CTexture* m_pTextureCom_Frame = { nullptr };

    _float  m_fCoolMaxTime = { 0.f };
    _float  m_fCurrCool = { 0.f };
    _float  m_fResultCool = { 0.f };
    _float  m_fCoolRatio = { 0.f };
    _float  m_fCoolAngle = { 0.f };

    CTextBox* m_pCoolTimeWnd = { nullptr };
    wstring m_strTag;
    wstring m_strFont;
public:
    static  CUI_WDRIdentity_Identity_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END