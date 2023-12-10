#include "PhysXMgr.h"
#include "PipeLine.h"
#include "Transform.h"
#include "GameObject.h"
#include "VIBuffer.h"
#include "Mesh.h"
#include "Model.h"
#include "PhysX\PxPhysics.h"
#include "PhysX\PxPhysicsAPI.h"
#include "RigidBody.h"

IMPLEMENT_SINGLETON(CPhysXMgr)

CPhysXMgr::CPhysXMgr()
{
}

HRESULT CPhysXMgr::ReserveManager()
{
	m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxAllocator, m_PXErrorCallback);

	m_Pvd = PxCreatePvd(*m_PxFoundation);
	//The PVD needs connection via a socket. It will run on the Address defined, in our case it's our machine
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(m_pvdIPAddress.c_str(), m_pvdPortNumber, m_pvdTimeOutSeconds);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);


	m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true, m_Pvd);
	PxInitExtensions(*m_PhysX, m_Pvd);
	
	PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	//This creates CPU dispatcher threads or worker threads. We will make 2
	m_PxDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_PxDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//Create the scene now by passing the scene's description
	m_PxScene = m_PhysX->createScene(sceneDesc);

	
	

	PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient();
	if (pvdClient)
	{
		//I have a PVD client, so set some flags that it needs
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}



	m_PlayerGeom = PxCapsuleGeometry(0.5f, 0.3f);


	return S_OK;
}

HRESULT CPhysXMgr::Add_PlayObject(CGameObject* pObject)
{
	m_Players.push_back(pObject);

	PxRigidDynamic* pActor = m_PhysX->createRigidDynamic(Get_ObjectTransform(pObject));

	PxMaterial* Material = m_PhysX->createMaterial(0.5f, 0.5f, 0.6f);
	PxShape* pShape = m_PhysX->createShape(m_PlayerGeom, *Material);

	pActor->attachShape(*pShape);


	m_PxScene->addActor(*pActor);
	m_PlayerActors.push_back(pActor);

	return S_OK;
}

HRESULT CPhysXMgr::Register_ColMesh(CGameObject* pObject)
{
	CModel* pModel = pObject->Get_ModelCom();


	for (auto* Mesh : pModel->Get_Meshes())
	{
		VTXANIMMODEL* pVB = Mesh->Get_Vertices();
		FACEINDICES32* pIB = Mesh->Get_Indices();
		_uint iNumVertices = Mesh->Get_NumVertices();
		_uint iNumPrimitives = Mesh->Get_NumPrimitives();
		_uint iNumIndices = iNumPrimitives * 3;

		vector<PxVec3> Vertices;
		Vertices.reserve(iNumVertices);

		Matrix matWorld = pObject->Get_TransformCom()->Get_WorldMatrix();

		for (_uint i = 0; i < iNumVertices; ++i)
		{
			Vec3 vMeshVertex = pVB[i].vPosition;

			vMeshVertex = XMVector3TransformCoord(vMeshVertex, matWorld);

			Vertices.push_back(PxVec3(vMeshVertex.x, vMeshVertex.y, vMeshVertex.z));
		}



		vector<PxU32> Indices;
		Indices.reserve(iNumIndices);

		for (_uint i = 0; i < iNumPrimitives; ++i)
		{
			FACEINDICES32 Index = pIB[i];

			Indices.push_back(Index._0);
			Indices.push_back(Index._1);
			Indices.push_back(Index._2);
		}

		PxTriangleMeshDesc tDesc;

		tDesc.points.count = iNumVertices;
		tDesc.points.stride = sizeof(PxVec3);
		tDesc.points.data = Vertices.data();

		tDesc.triangles.count = iNumPrimitives;
		tDesc.triangles.stride = sizeof(PxU32) * 3;
		tDesc.triangles.data = Indices.data();


		PxTriangleMesh* pTriangleMesh = PxCreateTriangleMesh(PxCookingParams(PxTolerancesScale(0.0f, 0.0f)), tDesc);

		PxTriangleMeshGeometry* pGeometry = new PxTriangleMeshGeometry(pTriangleMesh);


		m_ColMesheGeom[pObject->Get_ModelName()].push_back(pGeometry);



		PxRigidStatic* pActor = m_PhysX->createRigidStatic(Get_ObjectTransform(pObject));


		
		PxMaterial* Material = m_PhysX->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* pShape = m_PhysX->createShape(*pGeometry, *Material);

		pActor->attachShape(*pShape);


		m_PxScene->addActor(*pActor);

		m_StaticActors.push_back(pActor);
	}
	return S_OK;
}

HRESULT CPhysXMgr::Add_ColMesh(_uint iObjectID, const wstring szModelName)
{
	WRITE_LOCK

	if (m_ColMesheGeom.find(szModelName) == m_ColMesheGeom.end())
		return S_OK;


	if (Find_ColMeshName(iObjectID, szModelName))
		return S_OK;

	m_ColMeshes[iObjectID].push_back(szModelName);

	return S_OK;
}

HRESULT CPhysXMgr::Delete_ColMesh(_uint iObjectID, const wstring szModelName)
{
	WRITE_LOCK

	for (auto iter = m_ColMeshes[iObjectID].begin(); iter != m_ColMeshes[iObjectID].end();)
	{
		if (*iter == szModelName)
			iter = m_ColMeshes[iObjectID].erase(iter);
		else
			++iter;
	}
	
	return S_OK;
}

void CPhysXMgr::LateTick(_float fTimeDelta)
{
	READ_LOCK

	_uint iIndex = 0;
	for (auto& Player : m_Players)
	{
		PxVec3 vDir;
		PxReal fLength;

		PxTransform PlayerTransform = Get_ObjectCapsuleTransform(Player);

		_uint iPlayerID = Player->Get_ObjectID();

		_bool bCollision = false;
		_bool bGround = false;

		for (auto& szName : m_ColMeshes[iPlayerID])
		{
			for (auto& Mesh : m_ColMesheGeom[szName])
			{
				if (PxComputeTriangleMeshPenetration(vDir, fLength, m_PlayerGeom, PlayerTransform, *Mesh, PxTransform(PxIDENTITY::PxIdentity), 1))
				{
					

					if (fLength > 0.1f)
					{
						Vec3 vResultDir(vDir.x, vDir.y, vDir.z);
						vResultDir.Normalize();


						Vec3 vDown(0.0f, 1.0f, 0.0f);
						_float fAngle = XMConvertToDegrees(acosf(vDown.Dot(vResultDir)));
						if (fAngle <= 45.0f)
						{
							bGround = true;
						}


						fLength -= 0.1f;

						vResultDir *= fLength;

						Vec3 vPlayerPos = Player->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

						vPlayerPos += vResultDir;

						Player->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPlayerPos);

						bCollision = true;
					}
				}
			}
		}

		m_PlayerActors[iIndex]->setGlobalPose(PlayerTransform);
		++iIndex;

		if (!bCollision && Player->Is_Gravity())
		{
			Player->Get_RigidBody()->UseGravity(true);
			Player->Get_RigidBody()->Set_Ground(false);
		}
		else if (bGround)
		{
			Player->Get_RigidBody()->Set_Ground(true);
			Player->Get_RigidBody()->UseGravity(false);
		}
	}

	





	m_PxScene->simulate(fTimeDelta);
	m_PxScene->fetchResults(true);

}

void CPhysXMgr::Reset()
{
	for (auto& Meshes : m_ColMesheGeom)
	{
		for (auto& Mesh : Meshes.second)
		{
			Safe_Delete(Mesh);
			Mesh = nullptr;
		}
	}
	m_ColMesheGeom.clear();
	m_ColMeshes.clear();
	m_Players.clear();


	for (auto& Actor : m_PlayerActors)
	{
		m_PxScene->removeActor(*Actor);
		Actor->release();
	}
	m_PlayerActors.clear();
	

	for (auto& Actor : m_StaticActors)
	{
		m_PxScene->removeActor(*Actor);
		Actor->release();
	}
	m_StaticActors.clear();

}

PxTransform CPhysXMgr::Get_ObjectTransform(CGameObject* pObject)
{
	Matrix matWorld = pObject->Get_TransformCom()->Get_WorldMatrix();

	//matWorld.m[3][0] *= -1.0f;



	Vec3 vPos, vScale;

	Quaternion vQuat;

	matWorld.Decompose(vScale, vQuat, vPos);

	return PxTransform(PxVec3(vPos.x, vPos.y, vPos.z), PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w));
}

PxTransform CPhysXMgr::Get_ObjectCapsuleTransform(CGameObject* pObject)
{
	Matrix matWorld = pObject->Get_TransformCom()->Get_WorldMatrix();

	//matWorld.m[3][0] *= -1.0f;

	/*matWorld = matOffset * matWorld;

	Vec3 vPos, vScale;

	Quaternion vQuat;

	matWorld.Decompose(vScale, vQuat, vPos);

	vPos.y = 0.0f;*/


	Vec3 vUp(matWorld.m[1]);

	vUp.Normalize();

	Vec3 vPos(matWorld.m[3]);

	Vec3 vStartPos = vPos + vUp * -0.1f;
	Vec3 vEndPos = vPos + vUp * 1.5f;

	PxTransform Result = PxTransformFromSegment(PxVec3(vStartPos.x, vStartPos.y, vStartPos.z), PxVec3(vEndPos.x, vEndPos.y, vEndPos.z));

	//Result.p.y -= 1.2f;
	//return PxTransform(PxVec3(vPos.x, vPos.y, vPos.z), PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w));
	return Result;
}

_bool CPhysXMgr::Find_ColMeshName(_uint iObjectID, const wstring& szName)
{
	for (auto& Name : m_ColMeshes[iObjectID])
	{
		if (Name == szName)
			return true;
	}

	return false;
}



void CPhysXMgr::Free()
{
}


