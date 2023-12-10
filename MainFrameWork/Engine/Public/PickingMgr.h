#pragma once
#include "Base.h"
#include "AsTypes.h"
#include "Lock.h"

BEGIN(Engine)

class CTransform;
class CGameObject;

class ENGINE_DLL CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr);

public:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	HRESULT Ready_PickingMgr();
	void	Update_PickingMgr();


	void	Add_ColMesh(CGameObject* pObj);
	void	Delete_ColMesh(CGameObject* pObj);

	void	Set_Ray(Vec3 vRayPos, Vec3 vRayDir);
	void	Get_Ray(Vec3& vRayPos, Vec3& vRayDir) { vRayPos = m_vRayPos; vRayDir = m_vRayDir; }


	void	Reset();

	void	Set_Stop(_bool bStop) { m_bStop = bStop; }
	_bool	Is_Stop() { return m_bStop; }

	void	Set_Player(CGameObject* pPlayer) { m_pPlayer = pPlayer; }

private:
	void	Compute_LocalLayInfo(Vec3* pDir, Vec3* pRayPos, CTransform* pTransform);
	void	Compute_LocalLayInfo(Vec3* pOutDir, Vec3* pOutRayPos, Vec3 vRayPos, Vec3 vRayDir, CTransform* pTransform);

	CGameObject* Find_ColMesh(CGameObject* pObj);
public:
	BOOL	IsPicking(CGameObject* _pObj, TRIAGLEDESC* tTriangle);
	
	BOOL	IsPicking(Vec3 vRayPos, Vec3 vRayDir, CGameObject* _pObj, TRIAGLEDESC* tTriangle, _float fMinDistance);

private:
	USE_LOCK


	Vec3 m_vRayDir;
	Vec3 m_vRayPos;



	list<CGameObject*> m_ColMeshList;

	_bool				m_bStop = false;

	CGameObject*		m_pPlayer = nullptr;
public:
	virtual void Free() override;
};

END