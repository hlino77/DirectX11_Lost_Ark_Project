#pragma once
#include "UI.h"

BEGIN(Client)
class CPlayer;

class CUI_EstherSkill :
    public CUI
{
private:
    CUI_EstherSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_EstherSkill(const CUI& rhs);
    virtual ~CUI_EstherSkill() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_CurrEstherGauge(_float fCurrGauge) { m_fCurrGauge = fCurrGauge; }
    void    Set_MaxEstherGauge(_float   fMaxGauge) { m_fMaxGauge = fMaxGauge; }
    void    Set_PartyLeader(class CPlayer* pPlayer) { m_pPartyLeader = pPlayer; }
    class CPlayer* Get_PartyLeader() { return m_pPartyLeader; }

private:
    void    Update_UseEshterSkill();
    void    Update_EstherGauge(_float  fTimeDelta);
    void    Update_ShineEffect(_float  fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();//Wnd
    HRESULT Bind_ShaderResources_IconFrameEffectL();
    HRESULT Bind_ShaderResources_IconFrameEffectC();
    HRESULT Bind_ShaderResources_IconFrameEffectR();
    HRESULT Bind_ShaderResources_IconFrameL();
    HRESULT Bind_ShaderResources_IconFrameC();
    HRESULT Bind_ShaderResources_IconFrameR();
    HRESULT Bind_ShaderResources_GaugeFrame();
    HRESULT Bind_ShaderResources_GaugeFrameEffect();
    HRESULT Bind_ShaderResources_Gauge();

private:
    CPlayer* m_pPartyLeader = { nullptr };

    CTexture* m_pTexture_IconFrameEffect = { nullptr };
    CTexture* m_pTexture_IconFrame = { nullptr };
    CTexture* m_pTexture_GaugeFrame = { nullptr };
    CTexture* m_pTexture_Gauge = { nullptr };
    CTexture* m_pTexture_GaugeFrameEffect = { nullptr };

    CTransform* m_pTransform_IconFrameEffect[3] = { nullptr, nullptr, nullptr };
    CTransform* m_pTransform_IconFrame[3] = { nullptr, nullptr, nullptr };
    CTransform* m_pTransform_GaugeFrame = { nullptr };
    CTransform* m_pTransform_GaugeFrameEffect = { nullptr };
    CTransform* m_pTransform_Gauge = { nullptr };

    _float  m_fGaugeRatio = { 0.f };
    _float  m_fCurrGauge = { 0.f };
    _float  m_fMaxGauge = { 0.f };
    _float  m_fMinGauge = { 0.f };

    _float  m_fFrameEffectAlpha = { 0.f };
    _float  m_fGaugeEffectAlpha = { 0.f };

    _bool   m_bMaxGauge = { false };
    _bool   m_bSkillUse = { false };
    _bool   m_bDecreaseEffect = { false };

public:
    static  CUI_EstherSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END