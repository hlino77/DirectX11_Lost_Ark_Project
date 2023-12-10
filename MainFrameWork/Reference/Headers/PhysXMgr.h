#pragma once
#include "Base.h"
#include "AsTypes.h"
#include "Lock.h"
#include "PhysX\PxPhysics.h"
#include "PhysX\PxPhysicsAPI.h"


using namespace physx;

BEGIN(Engine)

class CTransform;
class CGameObject;

class ENGINE_DLL CPhysXMgr : public CBase
{
	DECLARE_SINGLETON(CPhysXMgr);

private:
	explicit CPhysXMgr();
	virtual ~CPhysXMgr() = default;


	USE_LOCK
public:
	HRESULT ReserveManager();

	HRESULT	Add_PlayObject(CGameObject* pObject);
	HRESULT Register_ColMesh(CGameObject* pObject);
	HRESULT Add_ColMesh(_uint iObjectID, const wstring szModelName);
	HRESULT Delete_ColMesh(_uint iObjectID, const wstring szModelName);

	void	LateTick(_float fTimeDelta);


	void	Reset();

private:
	PxTransform Get_ObjectTransform(CGameObject* pObject);
	PxTransform Get_ObjectCapsuleTransform(CGameObject* pObject);



	_bool Find_ColMeshName(_uint iObjectID, const wstring& szName);


public:


private:
	PxFoundation* m_PxFoundation = nullptr;

	PxDefaultAllocator					m_PxAllocator;
	PxDefaultErrorCallback				m_PXErrorCallback;
	PxPhysics* m_PhysX = nullptr;


	PxPvd* m_Pvd = nullptr;
	std::string							m_pvdIPAddress = "127.0.0.1";
	int									m_pvdPortNumber = 5425;
	_uint								m_pvdTimeOutSeconds = 10;

	PxDefaultCpuDispatcher* m_PxDispatcher = nullptr;
	PxScene* m_PxScene = nullptr;


	PxCapsuleGeometry m_PlayerGeom;
	

	map<wstring, vector<PxTriangleMeshGeometry*>> m_ColMesheGeom;


	vector<PxRigidDynamic*> m_PlayerActors;
	unordered_map<_uint, list<wstring>> m_ColMeshes;
	list<CGameObject*> m_Players;



	//Temp
	vector<PxRigidStatic*> m_StaticActors;
public:
	virtual void Free() override;
};

END