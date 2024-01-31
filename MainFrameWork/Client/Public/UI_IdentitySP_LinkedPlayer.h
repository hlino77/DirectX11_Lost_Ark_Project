#pragma once
#include "UI.h"

BEGIN(Client)
class CPlayer;
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

private:
    CPlayer* m_pPlayer = { nullptr };

    CTexture* m_pTexture_BubbleGauge = { nullptr };
    CTexture* m_pTexture_BubbleAnim = { nullptr };
    CTexture* m_pTexture_GaugeCut = { nullptr };
    CTexture* m_pTexture_MoonFall = { nullptr };
    CTexture* m_pTexture_SunRise = { nullptr };
    CTexture* m_pTexture_SkillFrame = { nullptr };
    CTexture* m_pTexture_None = { nullptr };

    CTransform* m_pTransform_BubbleGaugeL = { nullptr };
    CTransform* m_pTransform_BubbleGaugeC = { nullptr };
    CTransform* m_pTransform_BubbleGaugeR = { nullptr };
    CTransform* m_pTransform_SkillL = { nullptr };
    CTransform* m_pTransform_SkillR = { nullptr };
    CTransform* m_pTransfrom_SkillFrameL = { nullptr };
    CTransform* m_pTransfrom_SkillFrameR = { nullptr };

    _float  m_pBubbleFrame = { 0.f };

public:
    static  CUI_IdentitySP_LinkedPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END