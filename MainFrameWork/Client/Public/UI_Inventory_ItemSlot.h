#pragma once
#include "UI.h"

BEGIN(Client)
class CItem;
class CPlayer;

class CUI_Inventory_ItemSlot :
    public CUI
{
private:
    CUI_Inventory_ItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Inventory_ItemSlot(const CUI& rhs);
    virtual ~CUI_Inventory_ItemSlot() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Clear_ItemSlot();
    void    Set_ItemInfo(vector<class CItem*> vecItems);
    void    Following_InventortWnd(_float fX, _float fY);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_ItemGrade();
    virtual HRESULT Bind_ShaderResources_ItemIcon();
    virtual HRESULT Bind_ShaderResources_Picked_Effect();
    
private:
    void    Set_Item_StringInfo(const _uint& iItemType, const _uint& iEquipType, const _uint& iItemGrade);
    void    Picking_Effect(_float fTimeDelta);

private:
    CPlayer* m_pOwner = { nullptr };
    CTexture* m_pTexture_ItemGrade = { nullptr };
    CTexture* m_pTexture_ItemIcon = { nullptr };
    CTexture* m_pTexture_None = { nullptr };
    CTexture* m_pTexture_Picked_Effect = { nullptr };
    CTransform* m_pTransform_ItemIcon = { nullptr };
    wstring m_strItemName;
    wstring m_strItemDescript;
    wstring m_strItemType;
    wstring m_strItemGrade;
    vector<CItem*> m_vecItems;
    _uint   m_iItemCount = { 0 };
    _uint   m_iItemGrade = { 0 };
    _uint   m_iItemType = { 0 };
    _uint   m_iEquipType = { 0 };
    _int   m_iSlotIndexX = { -1 };
    _int    m_iSlotIndexY = { -1 };
    _uint   m_iSlotIndex = { 0 };
    _float  m_fPickedAlpha = { 0.f };
public:
    static  CUI_Inventory_ItemSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END