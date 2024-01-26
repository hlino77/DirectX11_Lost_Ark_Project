#pragma once
#include "Item.h"

BEGIN(Client)

class CItem_TestItem final:
    public CItem
{
private:
    CItem_TestItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CItem_TestItem(const CItem& rhs);
    virtual ~CItem_TestItem() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Tick(_float fTimeDelta);
	virtual void				LateTick(_float fTimeDelta);

public:
	virtual HRESULT				Use_Item(CPlayer* pOwner);
	virtual HRESULT				Disuse_Item(CPlayer* pOwner, _bool bUseDefault);

private:
	virtual HRESULT				Ready_Components();

public:
	static CItem_TestItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END