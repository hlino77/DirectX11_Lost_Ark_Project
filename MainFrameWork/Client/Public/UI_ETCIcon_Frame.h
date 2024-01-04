#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_ETCIcon_Frame :
    public CUI
{
private:
    CUI_ETCIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ETCIcon_Frame(const CUI& rhs);
    virtual ~CUI_ETCIcon_Frame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_SkillIcon(const wstring & strSkillName);
    void    Set_IsHaveSkill(_bool IsHave) { m_bHaveItem = IsHave; }
    _bool   Get_IsHaveSkill() { return m_bHaveItem; }
    class CTexture* Get_SkillTexture() { return m_pTextureCom_Item; }
    void    Set_PickedFrame(_bool IsPicked) { m_bPicked = IsPicked; }
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Picked();
    HRESULT Bind_ShaderResources_Shine();
    HRESULT Bind_ShaderResources_Test();

private:
    _bool   m_bHaveItem = { false };
    _bool   m_bPicked = { false };
    _float  m_fAlphaShine = (0.f);
    Vec4    m_vecPickedColor = { 0.5f, 0.5f, 0.5f, 1.f };

    class CPlayer_Skill* m_pPlayer_Item = { nullptr };
    CTexture* m_pTextureCom_Item = { nullptr };
    CTexture* m_pTextureCom_Shine = { nullptr };
    CTexture* m_pTextureCom_Frame = { nullptr };

    //Test
    _bool   m_bSkillOn = { false };
    _float  m_fCoolMaxTime = { 24.f };
    _float  m_fCurrCool = { 24.f };
    _float  m_fCoolRatio = { 0.f };
    _float  m_fCoolAngle = { 0.f };

    CTextBox* m_pCoolTimetWnd = nullptr;
    wstring m_szFont;
    _bool   m_bTextOn = false;
    wstring m_strCoolTime = TEXT("");
    wstring m_strWndName;

public:
    static  CUI_ETCIcon_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END