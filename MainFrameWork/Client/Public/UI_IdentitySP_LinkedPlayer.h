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

private:
    CPlayer* m_pPlayer = { nullptr };

public:
    static  CUI_IdentitySP_LinkedPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END