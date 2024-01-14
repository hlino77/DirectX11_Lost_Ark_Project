#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_ChaosDungeon_Gauge final :
    public CUI
{
private:
    CUI_ChaosDungeon_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon_Gauge(const CUI& rhs);
    virtual ~CUI_ChaosDungeon_Gauge() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Initialize_Percent();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    _float  Get_MaxGauge() { return m_fMaxGauge; }
    _float  Get_CurrGauge() { return m_fCurrGauge; }
    _float  Get_GaugeRatio() { return m_fRatio; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    void	Start_Stage_Name();
    void	End_Stage_Name();

public:
    void    Print_Stage_Percent();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox();
    void    Set_Percent(const _uint& iGague);

private:
    _float  m_fMaxGauge = { 100.f };
    _float  m_fCurrGauge = { 0.f };
    _float  m_fRatio = { 0.f };

    wstring m_szFont;
    wstring m_strPercent = TEXT("0%");
    CTextBox* m_pPercentWnd = nullptr;
    _bool   m_bTextOn = false;

public:
    static  CUI_ChaosDungeon_Gauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END