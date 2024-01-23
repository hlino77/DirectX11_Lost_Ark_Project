#pragma once
#include "UI.h"

BEGIN(Client)
class CItem;
class CPlayer;

class CUI_Inventory :
    public CUI
{
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
    void    Set_InventoryOwner(CPlayer* pPlayer) { if(pPlayer != nullptr)m_pOwner = pPlayer; }
    void    Set_PlayerItems(unordered_map<wstring, vector<class CItem*>> mapPlayerItems);
    void    Update_PlayerItems(unordered_map<wstring, vector<class CItem*>> mapPlayerItems);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    CPlayer* m_pOwner = { nullptr };
    unordered_map<wstring, vector<class CItem*>>    m_mapPlayerItems;

public:
    static  CUI_Inventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END