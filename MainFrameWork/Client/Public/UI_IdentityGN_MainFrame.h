#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_MainFrame :
    public CUI
{
private:
    CUI_IdentityGN_MainFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_MainFrame(const CUI& rhs);
    virtual ~CUI_IdentityGN_MainFrame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override;
    Matrix  Get_WorldMatrix();
    void    Set_Angle(_float fAngle);
    _float  Get_Angle() { return m_fAngle; }
    void    Change_Icon(_float fTimeDelta);
    _bool   Get_IsEventtOn() { return m_bEventOn; }
    _bool   Get_IsTapKey() { return m_bTapKey; }
    
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _uint   m_iCurrStance = { 0 };
    _uint   m_iPreStance = { 0 };
    _float m_fAngle = { 0.f };
    _bool   m_bChangeIcon = { true };
    _bool   m_bTapKey = { false };
    _bool   m_bReturn = { false };
    _bool   m_bEventOn = { false };
public:
    static  CUI_IdentityGN_MainFrame* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;

};

END