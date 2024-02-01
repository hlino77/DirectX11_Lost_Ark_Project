#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
class CPlayer_Skill;
END


BEGIN(Client)

class CUI_SkillIcon_Frame :
    public CUI
{
private:
    CUI_SkillIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_SkillIcon_Frame(const CUI& rhs);
    virtual ~CUI_SkillIcon_Frame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_Percent();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_SkillIcon(const wstring & strSkillName);
    void    Set_IsHaveSkill(_bool IsHave) { m_bHaveSkill = IsHave; }
    _bool   Get_IsHaveSkill() { return m_bHaveSkill; }
    class CTexture* Get_SkillTexture() { return m_pTextureCom_Skill; }
    void    Set_PickedFrame(_bool IsPicked) { m_bPicked = IsPicked; }

private:
    void    Get_Player_BindingSkill();
    void Get_Player_GN(class CPlayer* _pPlayer, CTexture* _pTexture);
    void Get_Player_WR(class CPlayer* _pPlayer, CTexture* _pTexture);
    void Get_Player_WDR(class CPlayer* _pPlayer, CTexture* _pTexture);
    void Get_Player_MG(class CPlayer* _pPlayer, CTexture* _pTexture);
    void Get_Player_SP(class CPlayer* _pPlayer, CTexture* _pTexture);
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Picked();
    HRESULT Bind_ShaderResources_Shine();
    HRESULT Bind_ShaderResources_Skill();
    HRESULT Bind_ShaderResources_ChangeFrame();
    void    Print_CoolTime();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox();
    void	Start_CoolTimeText();
    void	End_CoolTimeText();
    void    Set_StringCoolTime();

private:
    _uint  m_eSkillKey = { 0 };

    _bool   m_bHaveSkill = { false };
    _bool   m_bPicked = { false };
    _bool   m_bSilence = { false };
    _float  m_fAlphaShine = (0.f);
    Vec4    m_vecPickedColor = { 0.5f, 0.5f, 0.5f, 1.f };
    Vec4    m_vecSilenceColor = { 1.f, 1.f, 1.f, 1.f };

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

    CTextBox* m_pCoolTimetWnd = nullptr;
    wstring m_szFont;
    _bool   m_bTextOn = false;
    wstring m_strCoolTime = TEXT("");
    wstring m_strWndName;

public:
    static  CUI_SkillIcon_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END