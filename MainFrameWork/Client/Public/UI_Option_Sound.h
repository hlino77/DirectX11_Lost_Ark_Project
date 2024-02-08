#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Option_Sound :
    public CUI
{
private:
    CUI_Option_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Option_Sound(const CUI& rhs);
    virtual ~CUI_Option_Sound() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    //Textures
    CTexture* m_pTexture_OptionCheckBox = { nullptr };
    CTexture* m_pTexture_OptionDetailButton = { nullptr };
    CTexture* m_pTexture_Line = { nullptr };
    CTexture* m_pTexture_OptionDragLine = { nullptr };
    CTexture* m_pTexture_ValueTextWnd = { nullptr };
    CTexture* m_pTexture_DragBar = { nullptr };   

    _float  m_fMasterVolume = { 0.f };
    _float  m_fEffectVolume = { 0.f };
    _float  m_fMusicVolume = { 0.f };

    _bool   m_bMasterVolume = { true };
    _bool   m_bEffectVolume = { true };
    _bool   m_bMusicVolume = { true };
    
    CTransform* m_pTransform_CheckBox[5] = { nullptr };

    CTextBox* m_pTextBox = { nullptr };
    wstring m_strTextWnd;
    wstring m_strFont;

public:
    static  CUI_Option_Sound* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};
END