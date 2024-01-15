#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_ChaosDungeon_Clear final :
    public CUI
{
private:
    CUI_ChaosDungeon_Clear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_ChaosDungeon_Clear(const CUI& rhs);
    virtual ~CUI_ChaosDungeon_Clear() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_ClearUIStart(_bool IsStart) { m_bStart = IsStart; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT Bind_ShaderResources_ClearLine();
    HRESULT Bind_ShaderResources_ClearShine();
    HRESULT Bind_ShaderResources_Line_L();
    HRESULT Bind_ShaderResources_Line_R();
    HRESULT Bind_ShaderResources_ClearFont();
    HRESULT Bind_ShaderResources_ClearBackGround();
    HRESULT Bind_ShaderResources_ClearCheckButton();

private:
    void    Update_Clear(_float fTimeDelta);
    void    Update_Line(_float fTimeDelta);
    void    Update_ClearLine(_float fTimeDelta);
    void    Update_ClearFont(_float fTimeDelta);
    void    Update_ClearCheckButton();
    void    Is_Picking_ClearButton(POINT pt);
    void    Create_ClearCheckButton();

private:
    CTexture* m_pTexture_ClearLine = { nullptr };
    CTexture* m_pTexture_ClearShine = { nullptr };
    CTexture* m_pTexture_Line_L = { nullptr };
    CTexture* m_pTexture_Line_R = { nullptr };
    CTexture* m_pTexture_ClearMessage = { nullptr };
    CTexture* m_pTexture_ClearBackGround = { nullptr };
    CTexture* m_pTexture_ClearCheckButton = { nullptr };

    CTransform* m_pTransform_ClearLine = { nullptr };
    CTransform* m_pTransform_ClearShine = { nullptr };
    CTransform* m_pTransform_Line_L = { nullptr };
    CTransform* m_pTransform_Line_R = { nullptr };
    CTransform* m_pTransform_ClearMessage = { nullptr };
    CTransform* m_pTransform_ClearBackGround = { nullptr };
    CTransform* m_pTransform_ClearCheckButton = { nullptr };

    _float m_fFrame = { 0.f };
    _bool   m_bClearLine = { false };
    _bool   m_bClearShine = { false };
    _bool   m_bLine = { false };
    _bool   m_bDecreaseLine = { false };
    _bool   m_bShowFont = { false };
    _bool   m_bPickedCheckButton = { false };
       
    _float  m_fSizeX_Line_L = { 0.f };
    _float  m_fX_Line_L = { 0.f };
    _float  m_fX_Line_L_Decrease = { 0.f };
    _float  m_fSizeX_Line_R = { 0.f };
    _float  m_fX_Line_R = { 0.f };
    _float  m_fX_Line_R_Decrease = { 0.f };
    _float  m_fClearLineAlpha = { 0.f };
    _float  m_fSizeX_Font = { 0.f };
    _float  m_fSizeY_Font = { 0.f };
    _float  m_fClearFontAlpha = { 0.f };

    RECT    m_rcClearCheckButton = {};

    _bool  m_bStart = { false };
    
public:
    static  CUI_ChaosDungeon_Clear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END