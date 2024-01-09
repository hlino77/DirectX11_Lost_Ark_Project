#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingWnd :
    public CUI
{
private:
    CUI_LoadingWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingWnd(const CUI& rhs);
    virtual ~CUI_LoadingWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    void    Set_TextureIndex(_uint iLevelIndex);

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResourcesImg();
    virtual HRESULT Bind_ShaderResourcesFrame();

private:
    wstring m_strBlahBlah   =   TEXT("");

    CTexture* m_pTextureComImg = {nullptr};
    CTexture* m_pTextureCom_Frame;

public:
    static  CUI_LoadingWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END