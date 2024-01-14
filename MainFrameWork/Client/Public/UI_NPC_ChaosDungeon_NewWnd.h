#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_NPC_ChaosDungeon_NewWnd :
    public CUI
{
private:
    CUI_NPC_ChaosDungeon_NewWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_NPC_ChaosDungeon_NewWnd(const CUI& rhs);
    virtual ~CUI_NPC_ChaosDungeon_NewWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    void    Start_Text();
    void    End_Text();
    void    Print_Text();
    void    Set_Active(_bool bActive);
    HRESULT Ready_TextBox();

private:
    void    Create_Rect_AcceptButton();
    void    Create_Rect_RefuseButton();
    void    Update_Button();
    void    Update_AcceptButton(POINT pt);
    void    Update_RefuseButton(POINT pt);
    _bool    Is_Picking_AcceptButton(POINT pt);
    _bool    Is_Picking_RefuseButton(POINT pt);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_AcceptButton();
    HRESULT Bind_ShaderResources_RefuseButton();
    HRESULT Bind_ShaderResources_Timer();

private:
    CTexture* m_pTexture_AcceptButton = { nullptr };
    CTexture* m_pTexture_RefuseButton = { nullptr };
    CTexture* m_pTexture_TimerEmpty = { nullptr };
    CTexture* m_pTexture_TimerGauge = { nullptr };

    CTransform* m_pTransform_AcceptButton = { nullptr };
    CTransform* m_pTransform_RefuseButton = { nullptr };
    CTransform* m_pTransform_Timer = { nullptr };

    _float      m_fMaxTimer = { 10.f };
    _float      m_fCurrTimer = { 10.f };
    _float      m_fTimerRatio = { 0.f };

    RECT        m_rcAcceptButton = {};
    RECT        m_rcRefuseButton = {};

    _bool       m_bPicking_AcceptButton = { false };
    _bool       m_bPicking_RefuseButton = { false };

    _uint       m_iTextureIndex_AcceptButton = { 0 };
    _uint       m_iTextureIndex_RefuseButton = { 0 };

    CTextBox*    m_pTextBoxWnd = { nullptr };
    CTextBox*    m_pTimeCountWnd = { nullptr };
    wstring      m_strText;
    wstring      m_strTimeCount;
    wstring      m_strTag;
    wstring      m_strTag_Timer;

public:
    static  CUI_NPC_ChaosDungeon_NewWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END