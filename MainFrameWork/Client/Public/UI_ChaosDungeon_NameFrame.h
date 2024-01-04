#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_ChaosDungeon_NameFrame final :
    public CUI
{
private:
    CUI_ChaosDungeon_NameFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon_NameFrame(const CUI& rhs);
    virtual ~CUI_ChaosDungeon_NameFrame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_StageName();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void	Start_Stage_Name();
    void	End_Stage_Name();

public:
    void    Print_Stage_Name();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox();
    void    Set_Stage_Name(const wstring& strStageName) { m_strStageName.clear(); m_strStageName = strStageName; }

private:
    wstring m_szFont;
    wstring m_strStageName = TEXT("타락 2단계");
    CTextBox* m_pStageNameWnd = nullptr;
    _bool   m_bTextOn = false;

public:
    static  CUI_ChaosDungeon_NameFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END