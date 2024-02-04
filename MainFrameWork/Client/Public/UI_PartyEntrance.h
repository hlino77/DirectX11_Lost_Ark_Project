#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CPlayer;
class CUI_PartyEntrance :
    public CUI
{
private:
    CUI_PartyEntrance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_PartyEntrance(const CUI& rhs);
    virtual ~CUI_PartyEntrance() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    const _bool Get_IsClicked();

    void    Set_Player_Control();
    void    Reset_Player_Control();
    _bool   Is_Clicked() { return m_IsClicked; }
    void    Set_PartyLeaderName(CPlayer* pPlayer);
    void    Set_ControlPlayer(CPlayer* pPlayer) { m_pControlPlayer = pPlayer; }

    void    Set_Active_EntranceParty(CPlayer* pPartyLeader, CPlayer* pPlayer);

private:
    void    Print_Text();
    HRESULT Ready_TextBox();
    void    Update_OnTransform(_float fTimeDelta);

private:
    void    Create_RectButton();
    void    Update_Button();
    void    Update_Buttons(POINT pt);
    _bool    Is_Picking_AcceptButton(POINT pt);
    _bool    Is_Picking_RefuseButton(POINT pt);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_AcceptButton();
    HRESULT Bind_ShaderResources_RefuseButton();
    HRESULT Bind_ShaderResources_TimerFrame();
    HRESULT Bind_ShaderResources_Timer();

private:
    CPlayer* m_pPartyLeader = { nullptr };
    CPlayer* m_pControlPlayer = { nullptr };

    CTexture* m_pTexture_AcceptButton = { nullptr };
    CTexture* m_pTexture_RefuseButton = { nullptr };
    CTexture* m_pTexture_TimerEmpty = { nullptr };
    CTexture* m_pTexture_TimerGauge = { nullptr };

    CTransform* m_pTransform_AcceptButton = { nullptr };
    CTransform* m_pTransform_RefuseButton = { nullptr };
    CTransform* m_pTransform_TimerFrame = { nullptr };
    CTransform* m_pTransform_Timer = { nullptr };

    _float      m_fMaxTimer = { 20.f };
    _float      m_fCurrTimer = { 20.f };
    _float      m_fTimerRatio = { 0.f };

    _float      m_fSizeX_Button = { 0.f };
    _float      m_fSizeY_Button = { 0.f };
    _float      m_fX_Button = { 0.f };
    _float      m_fY_Button = { 0.f };

    RECT    m_rcButton[2] = {};//0:Accept  1:Refuse
    
    _bool       m_bPicking_AcceptButton = { false };
    _bool       m_bPicking_RefuseButton = { false };
    _uint       m_iTextureIndex_AcceptButton = { 0 };
    _uint       m_iTextureIndex_RefuseButton = { 0 };

    CTextBox* m_pTextBoxWnd = { nullptr };

    wstring      m_strText;
    wstring      m_strTimeCount;
    wstring      m_strTag;
    wstring      m_strTag_Timer;

    _bool       m_bClicked_Entrance = { false };
    _bool       m_IsClicked = { false };

    LERP_FLOAT  m_tLerp = {};
    _bool   m_bTestBool = { false };

public:
    static  CUI_PartyEntrance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END