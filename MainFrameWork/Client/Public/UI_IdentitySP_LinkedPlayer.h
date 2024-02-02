#pragma once
#include "UI.h"

BEGIN(Client)
class CPlayer;
class CPlayer_Doaga;

class CUI_IdentitySP_LinkedPlayer :
    public CUI
{
private:
    CUI_IdentitySP_LinkedPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentitySP_LinkedPlayer(const CUI& rhs);
    virtual ~CUI_IdentitySP_LinkedPlayer() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Update_BubbleGauge(_float fTimeDelta);
    void    Update_BubbleAnim(_float fTimeDelta);
    void    Update_BubbleCount(_float fTimeDelta);
    void    Update_BubblePopAnim(_float fTimeDelta);
    void    Update_BubbleMaxEffext(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_BubbleGaugeL();
    HRESULT Bind_ShaderResources_BubbleGaugeC();
    HRESULT Bind_ShaderResources_BubbleGaugeR();
    HRESULT Bind_ShaderResources_SkillL();
    HRESULT Bind_ShaderResources_SkillR();
    HRESULT Bind_ShaderResources_SkillFrameL();
    HRESULT Bind_ShaderResources_SkillFrameR();
    HRESULT Bind_ShaderResources_MaskedShineL();
    HRESULT Bind_ShaderResources_MaskedShineC();
    HRESULT Bind_ShaderResources_MaskedShineR();
    HRESULT Bind_ShaderResources_BubbleMaxEffectR();
    HRESULT Bind_ShaderResources_BubbleMaxEffectC();
    HRESULT Bind_ShaderResources_BubbleMaxEffectL();

private:
    CPlayer* m_pPlayer = { nullptr };

    CTexture* m_pTexture_BubbleGaugeL = { nullptr };
    CTexture* m_pTexture_BubbleGaugeC = { nullptr };
    CTexture* m_pTexture_BubbleGaugeR = { nullptr };
    CTexture* m_pTexture_BubbleAnim = { nullptr };
    CTexture* m_pTexture_Masked_Effect = { nullptr };
    CTexture* m_pTexture_MoonFall = { nullptr };
    CTexture* m_pTexture_SunRise = { nullptr };
    CTexture* m_pTexture_SkillFrame = { nullptr };
    CTexture* m_pTexture_None = { nullptr };
    CTexture* m_pTexture_BubblePop = { nullptr };
    CTexture* m_pTexture_BubbleMaxEffect= { nullptr };

    CTransform* m_pTransform_BubbleGaugeL = { nullptr };
    CTransform* m_pTransform_BubbleGaugeC = { nullptr };
    CTransform* m_pTransform_BubbleGaugeR = { nullptr };
    CTransform* m_pTransform_Masked_EffectL = { nullptr };
    CTransform* m_pTransform_Masked_EffectC = { nullptr };
    CTransform* m_pTransform_Masked_EffectR = { nullptr };
    CTransform* m_pTransform_SkillL = { nullptr };
    CTransform* m_pTransform_SkillR = { nullptr };
    CTransform* m_pTransform_SkillFrameL = { nullptr };
    CTransform* m_pTransform_SkillFrameR = { nullptr };
    CTransform* m_pTransform_BubbleMaxEffectL = { nullptr };
    CTransform* m_pTransform_BubbleMaxEffectC = { nullptr };
    CTransform* m_pTransform_BubbleMaxEffectR = { nullptr };

    _uint   m_iMaxBubble = { 3 };
    _uint   m_iBubbleTextureIndex[3] = { 0, 0 };
    _uint   m_iCurrBubbleCount = { 0 };
    _uint   m_iPreBubbleCount = { 0 };

    _float  m_fCurrBubbleGauge[3] = { 0 };
    _float  m_fMaxBubbleGauge = { 0 };
    _float  m_fMaxCool = { 1.f };
    _float  m_fBubbleAnimFrame[3] = { 0.f, 0.f, 0.f };
    _float  m_fBubbleRatio = { 0.f };
    _float  m_fRatioAngle = { 0.f };
    _float  m_fSkillAlpha[2] = { 1.f, 1.f };
    _bool   m_bBubbleMax[3] = { false, false, false };

    _float  m_fSizeX_Masked[3] = { 0.f, 0.f, 0.f };
    _float  m_fSizeY_Masked[3] = { 0.f, 0.f, 0.f };
    _float  m_fSizeX_MaxEffect[3] = { 0.f, 0.f, 0.f };
    _float  m_fSizeY_MaxEffect[3] = { 0.f, 0.f, 0.f };
    _float  m_fMaskedEffectAlpha[3] = { 0.f, 0.f, 0.f };
    _float  m_fBubbleMaxAlpha[3] = { 0.f, 0.f, 0.f };
    _float  m_fBubblePopFrame[3] = { 0.f, 0.f, 0.f };
    
    _bool  m_fBubbleMaxEffect[3] = { false, false, false };//나타나는 이펙트
    _bool   m_bMaskedEffect[3] = { false, false, false };//사라지는 이펙트 On
    
public:
    static  CUI_IdentitySP_LinkedPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END