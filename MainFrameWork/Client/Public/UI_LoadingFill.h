#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_LoadingFill :
    public CUI
{
private:
    CUI_LoadingFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_LoadingFill(const CUI& rhs);
    virtual ~CUI_LoadingFill() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Change_SizeX(_float fSizeX);
    _float    Get_SizeX() { return m_fSizeX; }
    void    Set_MaxFiles(_uint iFilesNum) { m_iMaxFiles = iFilesNum; }
    void    Add_CurrFile() { m_iCurrFiles++; }
    void    Add_CurrFiles(_uint iFilesNum) { m_iCurrFiles += iFilesNum; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_Arrow();
    void    Update_Loading();

private:
    CTexture* m_pTexture_Arrow = { nullptr };
    CTransform* m_pTransform_Arrow = { nullptr };

    _float m_fX_Arrow = { 0.f };
    _float m_fY_Arrow = { 0.f };
    _float m_fSizeX_Arrow = { 0.f };
    _float m_fSizeY_Arrow = { 0.f };

    _uint   m_iCurrFiles = { 0 };
    _uint   m_iMaxFiles = { 0 };
    _float  m_fFilesRatio = { 0.f };

public:
    static  CUI_LoadingFill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END