#pragma once
#include "UI.h"

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

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    void    Setting_HostPos();

private:
    Vec4    m_vHostProjPos = { 0.f, 0.f, 0.f, 0.f };
    CGameObject* m_pHost = { nullptr };

public:
    static  CUI_SpeechBubble* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END