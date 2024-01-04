#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_SpaceBar_Icon :
    public CUI
{
private:
    CUI_SpaceBar_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_SpaceBar_Icon(const CUI& rhs);
    virtual ~CUI_SpaceBar_Icon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_Percent();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    void    Print_CoolTime();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox();
    void	Start_CoolTimeText();
    void	End_CoolTimeText();
    void    Set_StringCoolTime();

private:
    wstring m_strCurrSkillName = TEXT("");
    wstring m_strPreSkillName = TEXT("");

    CPlayer_Skill* m_pSkill = { nullptr };
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
    static  CUI_SpaceBar_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END