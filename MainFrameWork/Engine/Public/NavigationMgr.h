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
	void	Render();


	void	Add_Navigation(const wstring& szNavigationName, const wstring& szFileName);
	void	Set_CurrNavigation(const wstring& szNavigationName);
	void	SetUp_OnCell(CGameObject * pObject);
	void	Find_FirstCell(CGameObject * pObject);
	_bool	Picking_Cell(Vec3 vRayPos, Vec3 vRayDir, Vec3& vResultPos);
	_int	Check_Pos_InCell(Vec3 vPos);


	void	Reset_Navigation();

private:
	unordered_map<wstring, CNavigation*> m_Navigations;
	CNavigation* m_pCurrNavigation;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	
	USE_LOCK
public:
	virtual void Free() override;
};

END
