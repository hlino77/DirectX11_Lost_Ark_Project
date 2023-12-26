#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer_Rect;

class ENGINE_DLL CTextBox final: public CGameObject
{
public:
    typedef struct TextBoxDesc
    {
        Vec2 vSize;
        wstring szTextBoxTag;
    }TEXTBOXDESC;

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
    CTextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTextBox(const CTextBox& rhs);
    virtual ~CTextBox() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(const _float& fTimeDelta);
    virtual void LateTick(const _float& fTimeDelta);
    virtual HRESULT Render();
    virtual HRESULT Render_MakeSRV();


    void    Set_Pos(_float fX, _float fY);
    void    Set_Text(const wstring& szTextTag, const wstring& szFont, const wstring& szText, Vec2 vTextPos, Vec2 vScale, Vec2 vOrigin, _float fRotation, Vec4 vColor);
    void    Set_ScaleUV(Vec2 vUV) { m_vUV = vUV; }


private:
    virtual HRESULT Ready_Components();

    HRESULT Ready_RenderTarget();

    unordered_map<wstring, TEXTDESC> m_TextList;


    Vec2 m_vUV;
    Vec2 m_vSize;

    _bool m_bUpdate = false;

    wstring m_szTargetTag;
    wstring m_szMRTTag;

    _float m_fWinSizeX;
    _float m_fWinSizeY;


    Matrix m_ViewMatrix, m_ProjMatrix;


    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
public:
    static  CTextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END