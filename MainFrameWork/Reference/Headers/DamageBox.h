#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CVIBuffer_Point;


class ENGINE_DLL CDamageBox final: public CGameObject
{
public:
    typedef struct TextBoxDesc
    {
        Vec2 vSize;
        wstring szTextBoxTag;
        _bool m_bInstance;
        wstring szTextBoxName;
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
    CDamageBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CDamageBox(const CDamageBox& rhs);
    virtual ~CDamageBox() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual HRESULT Render_MakeSRV();
    virtual HRESULT	Render_Instance(_uint iSize);

    virtual void	Add_InstanceData(_uint iSize, _uint& iIndex) override;

    void    Set_Pos(_float fX, _float fY);
    void    Set_Text(const wstring& szTextTag, const wstring& szFont, const wstring& szText, Vec2 vTextPos, Vec2 vScale, Vec2 vOrigin, _float fRotation, Vec4 vColor);
    void    Set_ScaleUV(Vec2 vUV) { m_vUV = vUV; }
    void    Clear_Text() { m_TextList.clear(); }
    void    Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
    void    Decrease_Alpha(_float fAlpha) { m_fAlpha -= fAlpha; }


private:
    virtual HRESULT	Ready_Proto_InstanceBuffer() override;
    virtual HRESULT	Ready_Instance_For_Render(_uint iSize) override;


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
    _float m_fAlpha;


    Matrix m_ViewMatrix, m_ProjMatrix;


    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CVIBuffer_Point* m_pInstanceVIBufferCom = { nullptr };

    //Instancing
    unordered_map<wstring, _uint>* m_iTextMaxInstanceCount = nullptr;
    unordered_map<wstring, ID3D11Buffer*>* m_pTextInstanceBuffer = nullptr;
    unordered_map<wstring, tagTypeLess*>* m_pTextInstanceValue = nullptr;
    unordered_map<wstring, CShader*>* m_pTextInstanceShader = nullptr;


public:
    static  CDamageBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END