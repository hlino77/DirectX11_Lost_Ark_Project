#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Boss_Hp final :
    public CUI
{
public:
    enum BOSS_TYPE
    {
        BOSS_GOLEM, BOSS_KING, BOSS_VALTAN, BOSS_END
    };

    enum HP_COLOR  //파란 -> 시안 -> 연두 -> 노란 -> 주황 -> 빨간 -> 보라 -> 파란
    {
        HP_BLUE, HP_SIAN, HP_GREEN, HP_YELLOW, HP_ORANGE, HP_RED, HP_PURPLE, HP_END
    };

private:
    CUI_Boss_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Boss_Hp(const CUI& rhs);
    virtual ~CUI_Boss_Hp() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_BossHP();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
    void	Set_Active(_bool bActive);
    void    Set_Dead_BossHpUI(); 
    void    Set_HpCount(_int iHpCount) { m_iHpCount = iHpCount; }
    void    Set_MaxHp(_int  iMaxHp);
    void    Set_CurrHpColor(_uint iHpColor) { m_iCurrHpColor = iHpColor; }//절대 6을 넘기지마!!!
    void    Set_BossName(wstring strBossName) { m_strOutputName = strBossName; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Hp_Next();
    HRESULT Bind_ShaderResources_Hp_Lerp();
    HRESULT Bind_ShaderResources_Hp();
    HRESULT Bind_ShaderResources_HPGaugeCut();
    HRESULT Bind_ShaderResources_Frame();
    HRESULT Bind_ShaderResources_GroggyFrame();
    HRESULT Bind_ShaderResources_GroggyGauge();

private:
    void    Initialize_Position();
    void    Update_Hp(_float fTimeDelta);
    void    Update_PreHp();
    void    Update_BossName();

private:
    void    Update_BossHp();
    void    Update_HpCut();
    void    Update_LerpHp(_float fTimeDelta);
    HRESULT Ready_TextBox(const wstring& strName);

private:
    CGameObject* m_pOwner = { nullptr };
    CTexture* m_pTextureCom_Hp = { nullptr };
    CTexture* m_pTextureCom_Frame = { nullptr };
    CTexture* m_pTextureCom_NextHp = { nullptr };
    CTexture* m_pTextureCom_Lerp = { nullptr };
    CTexture* m_pTextureCom_Groggy = { nullptr };
    CTexture* m_pTextureCom_GroggyGauge = { nullptr };
    CTexture* m_pTextureCom_HpCut = { nullptr };
    CTransform* m_pTransformCom_Hp = { nullptr };
    CTransform* m_pTransformCom_Frame = { nullptr };
    CTransform* m_pTransformCom_Groggy = { nullptr };
    CTransform* m_pTransformCom_GroggyGauge = { nullptr };
    CTransform* m_pTansformCom_HpCut = { nullptr };

    uint64      m_iMaxHp = { 0 };
    int64       m_iCurrHp = { 0 };
    int64       m_iPreHp = { 0 };
    _int        m_iHpCount = { 0 };
    _int        m_iPreCount = { 0 };
    _uint       m_iNextHpColor = { 0 };
    _uint       m_iCurrHpColor = { 0 };
    _float      m_fDivideCountHp = { 0.f };
    _float      m_fHpRatio = { 0.f };
    _float      m_fHpLerpRatio = { 0.f };
    _float      m_fLerpAlpha = { 0.6f };
    _uint       m_iMaxGroggyGauge = { 0 };
    _int        m_iGroggyGauge = { 0 };
    _float      m_fGroggyRatio = { 0.f };
    _float      m_fHPCutAlpha = { 1.f };

    _float      m_fSizeXHp = { 0.f };
    _float      m_fSizeYHp = { 0.f };
    _float      m_fXHp = { 0.f };
    _float      m_fYHp = { 0.f };

    _uint   m_iBossType = { (_uint)BOSS_END };

    //TextBox
    wstring m_strBossName;
    _uint m_iTargetName;
    wstring m_strOutputName;
    wstring m_strMonsterRank;
    wstring m_szFont;
    wstring m_strTag;
    wstring m_strTagHP;
    wstring m_strTagHpCount;
    CTextBox* m_pInGameNameWnd = { nullptr };
    CTextBox* m_pInGameHpWnd = { nullptr };
    CTextBox* m_pInGameHpCountWnd = { nullptr };
    _bool   m_bTextOn = false;

    LERP_FLOAT  m_tLerp = {};

public:
    static  CUI_Boss_Hp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END