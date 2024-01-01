#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END


BEGIN(Client)

class CUI_SpeechBubble :
    public CUI
{
private:
    CUI_SpeechBubble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_SpeechBubble(const CUI& rhs);
    virtual ~CUI_SpeechBubble() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_Host(CGameObject* pObject) { m_pHost = pObject; }
    HRESULT Active_SpeechBuble(wstring szChat);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    void    Setting_HostPos();

    HRESULT Ready_TextBox(_uint iIndex);

private:
    Vec4    m_vHostProjPos = { 0.f, 0.f, 0.f, 0.f };
    CGameObject* m_pHost = { nullptr };

    _float	m_fDuration = { 3.f };

    CTextBox* m_pTextBox = nullptr;

    wstring m_szFont;
    Vec2 m_vTextScale;

    //SpeechBuble
    CTransform* m_pTransformUp = nullptr;
    CTransform* m_pTransformDown = nullptr;

    CTexture* m_pTextureUp = nullptr;
    CTexture* m_pTextureDown = nullptr;

public:
    static  CUI_SpeechBubble* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END