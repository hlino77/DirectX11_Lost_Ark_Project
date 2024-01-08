#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Lobby_StageName final:
    public CUI
{
private:
    CUI_Lobby_StageName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Lobby_StageName(const CUI& rhs);
    virtual ~CUI_Lobby_StageName() = default;

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

private:
    void    Update_NickNameFrame();

private:
    _uint m_iButtonState = { 0 };

    CTexture* m_pTextureCom_NickNameShine = { nullptr };
    CTexture* m_pTextureCom_Emblem = { nullptr };
    CTexture* m_pTextureCom_Equipment = { nullptr };
public:
    static  CUI_Lobby_StageName* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END