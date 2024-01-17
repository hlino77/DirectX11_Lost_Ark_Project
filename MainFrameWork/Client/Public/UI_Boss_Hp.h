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

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_Hp_Rough();
    virtual HRESULT Bind_ShaderResources_Hp();
    virtual HRESULT Bind_ShaderResources_Frame();
    virtual HRESULT Bind_ShaderResources_GroggyFrame();
    virtual HRESULT Bind_ShaderResources_GroggyGauge();


private:
    void    Initialize_Position();
    void    Update_Hp(_float fTimeDelta);
    void    Update_PreHp();

private:
    void    Update_BossHp();
    HRESULT Ready_TextBox(const wstring& strName);

private:
    CGameObject* m_pOwner = { nullptr };
    CTexture* m_pTextureCom_Hp = { nullptr };
    CTexture* m_pTextureCom_Frame = { nullptr };
    CTexture* m_pTextureCom_NextHp = { nullptr };
    CTexture* m_pTextureCom_Groggy = { nullptr };
    CTexture* m_pTextureCom_GroggyGauge = { nullptr };
    CTransform* m_pTransformCom_Hp = { nullptr };
    CTransform* m_pTransformCom_Frame = { nullptr };
    CTransform* m_pTransformCom_Groggy = { nullptr };
    CTransform* m_pTransformCom_GroggyGauge = { nullptr };

    uint64      m_iMaxHp = { 0 };
    int64       m_iCurrHp = { 0 };
    int64       m_iPreHp = { 0 };
    _int        m_iHpCount = { 0 };
    _uint       m_iNextHpColor = { 0 };
    _uint       m_iCurrHpColor = { 0 };
    _float      m_fDivideCountHp = { 0.f };
    _float      m_fHpRatio = { 0.f };
    _float      m_fHpRoughRatio = { 0.f };
    _float      m_fHpRoughMax = { 0.f };
    _float      m_fHpRoughMin = { 0.f };
    _uint       m_iMaxGroggyGauge = { 0 };
    _int        m_iGroggyGauge = { 0 };
    _float      m_fGroggyRatio = { 0.f };

    _float      m_fSizeXHp, m_fSizeYHp, m_fXHp, m_fYHp;

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

public:
    static  CUI_Boss_Hp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END