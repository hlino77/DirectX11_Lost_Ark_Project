#pragma once
#include "UI.h"

BEGIN(Engine)
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_ItemIcon_Frame :
    public CUI
{
private:
    CUI_ItemIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ItemIcon_Frame(const CUI& rhs);
    virtual ~CUI_ItemIcon_Frame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_SkillIcon(const wstring & strSkillName);
    void    Set_IsHaveSkill(_bool IsHave) { m_bHaveSkill = IsHave; }
    _bool   Get_IsHaveSkill() { return m_bHaveSkill; }
    class CTexture* Get_SkillTexture() { return m_pTextureCom_Test; }
    void    Set_PickedFrame(_bool IsPicked) { m_bPicked = IsPicked; }
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Picked();
    HRESULT Bind_ShaderResources_Shine();
    HRESULT Bind_ShaderResources_Test();

private:
    _bool   m_bHaveSkill = { false };
    _bool   m_bPicked = { false };
    wstring m_strCurrSkillName = TEXT("");
    wstring m_strPreSkillName = TEXT("");
    _float  m_fAlphShine = (0.f);
    Vec4    m_vecPickedColor = { 0.5f, 0.5f, 0.5f, 1.f };

    class CPlayer_Skill* m_pPlayer_Skill = { nullptr };
    CTexture* m_pTextureCom_Test = { nullptr };
    CTexture* m_pTextureCom_Shine = { nullptr };
    CTexture* m_pTextureCom_Frame = { nullptr };

    //Test
    _bool   m_bSkillOn = { false };
    _float  m_fCoolMaxTime = { 24.f };
    _float  m_fCurrCool = { 24.f };
    _float  m_fCoolRatio = { 0.f };
    _float  m_fCoolAngle = { 0.f };

public:
    static  CUI_ItemIcon_Frame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END