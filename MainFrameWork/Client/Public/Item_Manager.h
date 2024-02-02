#pragma once
#include "Base.h"
#include "Lock.h"
#include <filesystem>

BEGIN(Client)
class CItem;

class CItem_Manager final : public CBase
{
	DECLARE_SINGLETON(CItem_Manager);
public:
	CItem_Manager();
	virtual ~CItem_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	CItem* Get_Item(ITEMCODE eItem);
private:
	HRESULT	Loading_Inventory_UI();
	HRESULT Loading_Item();
	HRESULT	AutoLoad(const filesystem::path& strPath, LEVELID eLevel, Matrix Pivot = XMMatrixIdentity());
private:
	unordered_map<ITEMCODE, CItem*> m_Items;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	USE_LOCK
public:
	virtual void Free() override;
};

END

