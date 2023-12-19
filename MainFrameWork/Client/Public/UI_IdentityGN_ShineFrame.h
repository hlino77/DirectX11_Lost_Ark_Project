#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_ShineFrame final :
    public CUI
{
private:
    CUI_IdentityGN_ShineFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_ShineFrame(const CUI& rhs);
    virtual ~CUI_IdentityGN_ShineFrame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

private:
    _bool   m_bHidden = { false };
    class CLockFree_Transform* m_pOffSetTrasformCom = { nullptr };
    CUI* m_pMainFrame = { nullptr };

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    Matrix CombineRotations(const Matrix& rotationSelf, const Matrix& rotationOrbit);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

public:
    static  CUI_IdentityGN_ShineFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END