#pragma once
#include "UI.h"

BEGIN(Client)
class CPlayer;
class CItem;
class CUI_InventoryWnd;
class CUI_Inventory_ItemSlot;

class CUI_Inventory :
    public CUI
{
public:
    typedef struct Inventory_Item_Desc:public UIDESC
    {
        _uint   iSlotIndexX;
        _uint   iSlotIndexY;
        CUI* pUIWnd;
        class CPlayer* pPlayer;
        unordered_map<wstring, vector<class CItem*>> mapItems;
        vector<class CItem*>    vecItems;
    }INVEN_ITEMDESC;

private:
    CUI_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Inventory(const CUI& rhs);
    virtual ~CUI_Inventory() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Update_Used_Item();
    void    Move_InventoryWNd();

private:
    virtual HRESULT Ready_Components();
    HRESULT UI_SET();

private:
    CPlayer* m_pOwner = { nullptr };
    unordered_map<wstring, vector<class CItem*>>    m_mapPlayerItems;
    CUI* m_pInventoryWnd = { nullptr };
    _bool   m_bTestActiveKey = { false };
public:
    static  CUI_Inventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END