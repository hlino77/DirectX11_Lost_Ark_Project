#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_InGame_NamePlate final :
    public CUI
{
private:
    CUI_InGame_NamePlate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_InGame_NamePlate(const CUI& rhs);
    virtual ~CUI_InGame_NamePlate() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_StageName(const wstring& strName);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
    void    Set_TextOn(_bool bOn) { m_bTextOn = bOn; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void	Start_InGame_Name();
    void	End_InGame_Name();
    void    Update_NamePlatePos();

public:
    void    Print_InGame_Name();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox(const wstring& strName);

private:
    CGameObject* m_pOwner = { nullptr };

    wstring m_szFont;
    wstring m_strName = TEXT("");
    wstring m_strTag;
    CTextBox* m_pInGameNameWnd = nullptr;
    _bool   m_bTextOn = true;
    
public:
    static  CUI_InGame_NamePlate* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END