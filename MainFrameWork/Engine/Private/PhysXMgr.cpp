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
#include "GameInstance.h"

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
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
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

	/*PxMaterial* Material = m_PhysX->createMaterial(0.5f, 0.5f, 0.6f);


	PxRigidStatic* groundPlane = PxCreatePlane(*m_PhysX, PxPlane(0, 1, 0, 0), *Material);
	m_PxScene->addActor(*groundPlane);



	PxVec3 vStartPos = PxVec3(0.0f, 10.0f, 0.0f);

	PxShape* pFrameShape = m_PhysX->createShape(PxCapsuleGeometry(0.1f, 0.5f), *Material);

	PxShape* pSphereShape = m_PhysX->createShape(PxSphereGeometry(0.1f), *Material);


	vector<PxTransform> Bones;

	for (_uint i = 0; i < 5; ++i)
	{
		Bones.push_back(PxTransform(vStartPos));
		vStartPos.y -= 2.0f;
	}*/
	


	
	//for (_uint i = 0; i < Bones.size() - 1; ++i)
	//{
	//	PxTransform FirstBone = Bones[i];
	//	PxTransform SecondBone = Bones[i + 1];

	//	PxTransform FrameTrans = PxTransform(PxTransformFromSegment(FirstBone.p, SecondBone.p).p);

	//	PxRigidDynamic* pChild = m_PhysX->createRigidDynamic(FrameTrans);

	//	PxTransform pShapeTransform = PxTransformFromSegment(PxVec3(0.0f, -1.0f, 0.0f), PxVec3(0.0f, 1.0f, 0.0f));
	//	pFrameShape->setLocalPose(pShapeTransform);
	//	pChild->attachShape(*pFrameShape);

	//	pChild->setMass(1.0f);
	//	pChild->setLinearDamping(5.0f);
	//	pChild->setAngularDamping(0.05f);


	//	if(i == 0)
	//		pChild->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	//	m_PxScene->addActor(*pChild);
	//	m_TestFrames.push_back(pChild);

	//	
	//	if (i > 0)
	//	{
	//		PxRigidDynamic* pParent = m_TestFrames[i - 1];

	//		PxSphericalJoint* pJoint = PxSphericalJointCreate(*m_PhysX, pParent, FirstBone * pParent->getGlobalPose().getInverse(), pChild, FirstBone * FrameTrans.getInverse());

	//		_float fAngle = XMConvertToRadians(1.0f);
	//		

	//		pJoint->setLimitCone(PxJointLimitCone(0.0f, PxPi * 2.0f, PxSpring(20.0f, 1.0f)));
	//		//pJoint->setLimitCone(PxJointLimitCone(0.0f, PxPi * 2.0f));
	//		pJoint->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);

	//		m_Joints.push_back(pJoint);
	//	}
	//}


	return S_OK;
}


void CPhysXMgr::LateTick(_float fTimeDelta)
{
	Update_Branches();

	m_PxScene->simulate(fTimeDelta);
	m_PxScene->fetchResults(true);
	
}

void CPhysXMgr::Update_Branches()
{
	for (auto& Player : m_PlayerInfos)
	{
		CModel* pModel = Player.pPlayer->Get_ModelCom();
		Matrix matPivot = pModel->Get_PivotMatrix();
		Matrix matPlayerWorld = Player.pPlayer->Get_TransformCom()->Get_WorldMatrix();

		for (auto& Branch : Player.m_Branches)
		{
			Matrix matFirst = pModel->Get_CombinedMatrix(Branch.second.m_Bones[0]) * matPivot * matPlayerWorld;
			Matrix matSecond = pModel->Get_CombinedMatrix(Branch.second.m_Bones[1]) * matPivot * matPlayerWorld;

			PxTransform FirstBone = MatrixToPxTrans(matFirst);
			PxTransform SecondBone = MatrixToPxTrans(matSecond);


			PxVec3 vPos = FirstBone.p + ((SecondBone.p - FirstBone.p) * 0.5f);
			PxQuat vQuat = FirstBone.q;

			PxTransform FrameTrans = PxTransform(vPos, vQuat);

			Branch.second.m_Frames[0]->setKinematicTarget(FrameTrans);


			for (_uint i = 0; i < 4; ++i)
			{
				if (i == 0)
				{
					Matrix matBone = pModel->Get_CombinedMatrix(Branch.second.m_Bones[i]) * matPivot * matPlayerWorld;
					PxTransform BoneTrans = MatrixToPxTrans(matBone);

					m_TestBones[i]->setKinematicTarget(BoneTrans);
				}
				else
				{
					PxTransform BoneTrans = Branch.second.m_Joints[i - 1]->getLocalPose(PxJointActorIndex::eACTOR0) * Branch.second.m_Frames[i - 1]->getGlobalPose();

					m_TestBones[i]->setKinematicTarget(BoneTrans);
				}
				
			}

		}
	}

}

void CPhysXMgr::Reset()
{
}

void CPhysXMgr::Add_Player(CGameObject* pPlayer)
{
	PhysxPlayerDesc tPlayer;
	tPlayer.pPlayer = pPlayer;

	//tPlayer.pPlayerActor = 

	m_PlayerInfos.push_back(tPlayer);
}

void CPhysXMgr::Add_BoneBranch(CGameObject* pPlayer, vector<_uint>& Bones)
{
	PLAYERDESC* PlayerDesc = Find_PlayerInfo(pPlayer);


	for (auto& BoneIndex : Bones)
	{
		PlayerDesc->m_Branches[L"Test"].m_Bones.push_back(BoneIndex);
	}

	if (PlayerDesc == nullptr)
		return;


	


	PxMaterial* Material = m_PhysX->createMaterial(0.5f, 0.5f, 0.6f);

	CModel* pModel = pPlayer->Get_ModelCom();

	
	Matrix matPivot = pModel->Get_PivotMatrix();
	Matrix matPlayerWorld = pPlayer->Get_TransformCom()->Get_WorldMatrix();

	PxShape* pFrameShape = m_PhysX->createShape(PxCapsuleGeometry(0.01f, 0.05f), *Material);
	PxTransform pShapeTransform = PxTransformFromSegment(PxVec3(0.0f, -1.0f, 0.0f), PxVec3(0.0f, 1.0f, 0.0f));
	pFrameShape->setLocalPose(pShapeTransform);


	PxShape* pSphereShape = m_PhysX->createShape(PxSphereGeometry(0.01f), *Material);



	for (_uint i = 0; i < 5; ++i)
	{
		Matrix matFirst = pModel->Get_CombinedMatrix(Bones[i]) * matPivot * matPlayerWorld;
		PxTransform FirstBone = MatrixToPxTrans(matFirst);


		PxRigidDynamic* pBoneActor = m_PhysX->createRigidDynamic(FirstBone);
		pBoneActor->attachShape(*pSphereShape);

		pBoneActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		m_PxScene->addActor(*pBoneActor);

		m_TestBones.push_back(pBoneActor);
	}



	for (_uint i = 0; i < Bones.size() - 1; ++i)
	{
		Matrix matFirst = pModel->Get_CombinedMatrix(Bones[i]) * matPivot * matPlayerWorld;
		Matrix matSecond = pModel->Get_CombinedMatrix(Bones[i + 1]) * matPivot * matPlayerWorld;

		PxTransform FirstBone = MatrixToPxTrans(matFirst);
		PxTransform SecondBone = MatrixToPxTrans(matSecond);


		PxVec3 vPos = FirstBone.p + ((SecondBone.p - FirstBone.p) * 0.5f);
		PxQuat vQuat = FirstBone.q;
		PxTransform FrameTrans = PxTransform(vPos, vQuat);
		PxRigidDynamic* pChild = m_PhysX->createRigidDynamic(FrameTrans);

		

		pChild->attachShape(*pFrameShape);
		pChild->setMass(1.0f);
		pChild->setLinearDamping(5.0f);
		pChild->setAngularDamping(0.05f);


		if (i == 0)
			pChild->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

		m_PxScene->addActor(*pChild);
		PlayerDesc->m_Branches[L"Test"].m_Frames.push_back(pChild);


		if (i > 0)
		{
			PxRigidDynamic* pParent = PlayerDesc->m_Branches[L"Test"].m_Frames[i - 1];

			PxTransform ParentJointTrans = FirstBone * pParent->getGlobalPose().getInverse();
			PxTransform ChildJointTrans = FirstBone * FrameTrans.getInverse();

			PxTransform ParentJointTransTest = ParentJointTrans * pParent->getGlobalPose();
			PxTransform ChildJointTransTest = ChildJointTrans * FrameTrans;


			PxSphericalJoint* pJoint = PxSphericalJointCreate(*m_PhysX, pParent, ParentJointTrans, pChild, ChildJointTrans);

			//_float fAngle = XMConvertToRadians(1.0f);


			pJoint->setLimitCone(PxJointLimitCone(0.0f, PxPi * 2.0f, PxSpring(20.0f, 1.0f)));
			//pJoint->setLimitCone(PxJointLimitCone(0.0f, PxPi * 2.0f));
			pJoint->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);


			PxTransform JointTrans = pJoint->getLocalPose(PxJointActorIndex::eACTOR0) * pParent->getGlobalPose();



			PlayerDesc->m_Branches[L"Test"].m_Joints.push_back(pJoint);
		}
	}

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

PxTransform CPhysXMgr::MatrixToPxTrans(Matrix matValue)
{
	Vec3 vPos, vScale;

	Quaternion vQuat;

	matValue.Decompose(vScale, vQuat, vPos);

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

CPhysXMgr::PLAYERDESC* CPhysXMgr::Find_PlayerInfo(CGameObject* pPlayer)
{
	for (auto& PlayerInfo : m_PlayerInfos)
	{
		if (PlayerInfo.pPlayer == pPlayer)
			return &PlayerInfo;
	}

	return nullptr;
}


void CPhysXMgr::Free()
{
}


