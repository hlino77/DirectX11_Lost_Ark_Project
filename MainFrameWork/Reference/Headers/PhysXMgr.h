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
public:

	DECLARE_SINGLETON(CPhysXMgr);

	typedef struct BranchDesc
	{
		vector<_uint> Bones;
		vector<PxRigidDynamic*> Frames;
		vector<Matrix> RelativeMatrix;
	}BRANCHDESC;


	typedef struct PhysxPlayerDesc
	{
		CGameObject* pPlayer = nullptr;
		PxRigidDynamic* pPlayerActor = nullptr;
		vector<BRANCHDESC> Branches;
	}PLAYERDESC;


	
private:
	explicit CPhysXMgr();
	virtual ~CPhysXMgr() = default;



public:
	HRESULT ReserveManager();


	void	LateTick(_float fTimeDelta);
	void	Update_Branches();
	void	Set_BranchesToBone();

	void	Reset();

	void	Add_Player(CGameObject* pPlayer);
	void	Add_BoneBranch(CGameObject* pPlayer, vector<_uint>& Bones);

private:
	PxTransform Get_ObjectTransform(CGameObject* pObject);

	PxTransform MatrixToPxTrans(Matrix matValue);
	Matrix TransToMatrix(PxTransform transValue);

	PxTransform Get_ObjectCapsuleTransform(CGameObject* pObject);


	PLAYERDESC* Find_PlayerInfo(CGameObject* pPlayer);

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



	map<wstring, vector<PxTriangleMeshGeometry*>> m_ColMesheGeom;
	vector<PLAYERDESC> m_PlayerInfos;


	

	vector<PxRigidDynamic*> m_TestBones;
	vector<PxRigidDynamic*> m_TestFrames;
	vector<PxSphericalJoint*> m_Joints;
	USE_LOCK
public:
	virtual void Free() override;
};

END