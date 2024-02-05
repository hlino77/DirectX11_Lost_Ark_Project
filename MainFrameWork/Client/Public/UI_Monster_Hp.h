#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Monster_Hp final :
    public CUI
{
private:
    CUI_Monster_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Monster_Hp(const CUI& rhs);
    virtual ~CUI_Monster_Hp() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_Hp();
    void    Update_Postion();

private:
    CGameObject* m_pOwner = { nullptr };
    CTexture* m_pTextureCom_Hp = { nullptr };
    CTransform* m_pTransform_Hp = { nullptr };

    CTexture* m_pTextureCom_PrisonWnd= { nullptr };
    CTexture* m_pTextureCom_PrisonGauge = { nullptr };

    _uint   m_iMaxHp = { 0 };
    _int    m_iCurrHp = { 0 };
    _float  m_fHpRatio = { 0.f };
    wstring m_szFont;
    wstring m_strName = TEXT("");

    Vec3    m_vEffectPos = {};

public:
    static  CUI_Monster_Hp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END