#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_TextBox final: public CUI
{
private:
    typedef struct TextDesc
    {
        wstring szText;
        wstring szFont;
        Vec2 vTextScale;
        _float fRotation;
        Vec2 vOrigin;
        Vec2 vTextPos;
        Vec4 vTextColor;
    }TEXTDESC;

private:
    CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_TextBox(const CUI& rhs);
    virtual ~CUI_TextBox() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual HRESULT Render_MakeSRV();


    void    Set_Text(const wstring& szTextTag, const wstring& szFont, const wstring& szText, Vec2 vTextPos, Vec2 vScale, Vec2 vOrigin, _float fRotation, Vec4 vColor);
    void    Set_ScaleUV(Vec2 vUV) { m_vUV = vUV; }
    void    Set_Pos(_float fX, _float fY);

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    virtual void Appear();
    virtual void Disappear();
private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();


    unordered_map<wstring, TEXTDESC> m_TextList;

    Vec2 m_vUV;
public:
    static  CUI_TextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END