#pragma once
#include "Engine_Defines.h"
#include "Base.h"
#include "Lock.h"

BEGIN(Engine)

class CNavigation;
class CGameObject;

class ENGINE_DLL CNavigationMgr final : public CBase
{
	DECLARE_SINGLETON(CNavigationMgr);

public:
	CNavigationMgr();
	virtual ~CNavigationMgr() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Render(_uint iLevel);


	void	Add_Navigation(_uint iLevel, const wstring& szFileName);
	void	SetUp_OnCell(_uint iLevel, CGameObject * pObject);
	void	Find_FirstCell(_uint iLevel, CGameObject * pObject);
	_bool	Picking_Cell(_uint iLevel, Vec3 vRayPos, Vec3 vRayDir, Vec3& vResultPos);


	void	OnOff_Render() { m_bRender = !m_bRender; }
private:
	unordered_map<_uint, CNavigation*> m_Navigations;

private:
	_bool m_bRender = true;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	
	USE_LOCK
public:
	virtual void Free() override;
};

END
