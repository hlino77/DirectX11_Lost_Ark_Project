#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
class CState_Skill;
END

BEGIN(Client)

class CUI_HoldingFrame :
    public CUI
{
public:
    struct HOLDING_SKILL_DESC
    {
        class CState_Skill* pSkill;
        wstring strSkillName;

        _float fSkillTimeAcc = 0.f;
        _float fSkillEndTime = 0.f;
        _float fSkillSuccessTime_Min = 0.f;
        _float fSkillSuccessTime_Max = 0.f;
    };

private:
    CUI_HoldingFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_HoldingFrame(const CUI& rhs);
    virtual ~CUI_HoldingFrame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

public:
    void    Set_SkillName(const wstring& strSkillName) { m_strSkillName = strSkillName; }
    void    Set_SkillEnd(_bool bIsSkillEnd) { m_bIsSkillEnd = bIsSkillEnd; }
    void    Set_SkillOn(_bool bSkillOn);
    void    Set_SkillTimeAcc(_float fAccTime) { m_fSkillTimeAcc = fAccTime; }

private:
    void    Update_CurrGauge();
    void    Update_GaugeCut();
    void    Update_HoldingEnd(_float fTimeDelta);

private:
    void    Print_SkillName();

private:
    virtual HRESULT Ready_Components();
    HRESULT Ready_TextBox();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Gauge();
    HRESULT Bind_ShaderResources_Empty();
    HRESULT Bind_ShaderResources_Frame();
    HRESULT Bind_ShaderResources_GaugeCut();
    HRESULT Bind_ShaderResources_PerfectZone();
    HRESULT Bind_ShaderResources_HoldingEnd();

private:
    CTexture* m_pTextureCom_Empty = { nullptr };
    CTexture* m_pTexture_Frame = { nullptr };
    CTexture* m_pTexture_GaugeCut = { nullptr };
    CTexture* m_pTexture_PerfectZone = { nullptr };
    CTexture* m_pTexture_HoldingEnd = { nullptr };
    CTexture* m_pTexture_None = { nullptr };

    CTransform* m_pTransform_Empty = { nullptr };
    CTransform* m_pTransform_Frame = { nullptr };
    CTransform* m_pTransform_GaugeCut = { nullptr };
    CTransform* m_pTransform_PerfectZone = { nullptr };
    CTransform* m_pTransform_HoldingEnd = { nullptr };

    _float  m_fHoldRatio = { 0.f };

    _float  m_fSizeX_PerfectZone = { 0.f };
    _float  m_fX_PerfectZone = { 0.f };

    _float  m_fSizeX_HoldEnd = { 0.f };
    _float  m_fSizeY_HoldEnd = { 0.f };

    _float	m_fSkillTimeAcc = { 0.f };
    _float	m_fSkillEndTime = { 0.f };

    _float	m_fSkillSuccessTime_Min = { 0.f };
    _float	m_fSkillSuccessTime_Max = { 0.f };

    _bool   m_bIsSkillEnd = { false };

    CState_Skill* m_pSkill = { nullptr };

    class CTextBox* m_pSkillNameWnd = { nullptr };
    wstring m_strFont;
    wstring m_strWndTag;
    wstring m_strSkillName;

public:
    static  CUI_HoldingFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END