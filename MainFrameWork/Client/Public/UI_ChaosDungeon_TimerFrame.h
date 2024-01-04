#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_ChaosDungeon_TimerFrame final :
    public CUI
{
private:
    CUI_ChaosDungeon_TimerFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon_TimerFrame(const CUI& rhs);
    virtual ~CUI_ChaosDungeon_TimerFrame() = default;

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
    void	Start_Stage_Timer();
    void	End_Stage_Timer();

public:
    void    Print_Stage_Timer();
    void	Set_Active(_bool bActive);
    HRESULT Ready_TextBox();
    void    Set_Stage_Timer(const wstring& strStageName) { m_strStageTimer.clear(); m_strStageTimer = strStageName; }

private:
    wstring m_szFont;
    wstring m_strTimerAnnounce = TEXT("다음 지역 진입까지 남은 시간");
    wstring m_strStageTimer = TEXT("");
    CTextBox* m_pStageNameWnd = nullptr;
    _bool   m_bTextOn = false;

public:
    static  CUI_ChaosDungeon_TimerFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END