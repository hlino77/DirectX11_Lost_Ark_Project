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

	m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true);
	
	PxSceneDesc SceneDesc(m_PhysX->getTolerancesScale());
	SceneDesc.gravity = PxVec3(0.0f, -9.81f * 10.0f, 0.0f);
	m_PxDispatcher = PxDefaultCpuDispatcherCreate(2);
	SceneDesc.cpuDispatcher = m_PxDispatcher;
	SceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_PxScene = m_PhysX->createScene(SceneDesc);

	return S_OK;
}


void CPhysXMgr::LateTick(_float fTimeDelta)
{
	WRITE_LOCK

	Update_Branches();

	m_PxScene->simulate(fTimeDelta);
	m_PxScene->fetchResults(true);
	
	Set_BranchesToBone();
}

void CPhysXMgr::Update_Branches()
{
	for (auto& Player : m_PlayerInfos)
	{
		CModel* pModel = Player.pPlayer->Get_ModelCom();
		Matrix matPivot = pModel->Get_PivotMatrix();
		Matrix matPlayerWorld = Player.pPlayer->Get_TransformCom()->Get_WorldMatrix();

		for (auto& Branch : Player.Branches)
		{
			Matrix matFirst = pModel->Get_CombinedMatrix(Branch.Bones[0]) * matPivot * matPlayerWorld;
			Branch.Frames[0]->setKinematicTarget(MatrixToPxTrans(matFirst));
		}
	}

}

void CPhysXMgr::Set_BranchesToBone()
{
	for (auto& Player : m_PlayerInfos)
	{
		CModel* pModel = Player.pPlayer->Get_ModelCom();
		Matrix matPlayerWorld = Player.pPlayer->Get_TransformCom()->Get_WorldMatrix();

		for (auto& Branch : Player.Branches)
		{
			for (_uint i = 1; i < Branch.Bones.size(); ++i)
			{
				Matrix matFrame = TransToMatrix(Branch.Frames[i]->getGlobalPose());

				Matrix matResult = (Branch.RelativeMatrix[i] * matFrame) * matPlayerWorld.Invert();
				matResult = pModel->Get_BoneOffsetMatrix(Branch.Bones[i]) * matResult;

				pModel->Set_CurrBoneMatrix(Branch.Bones[i], matResult);
			}
		}
	}
}

void CPhysXMgr::Reset()
{
	WRITE_LOCK
	for (auto& Player : m_PlayerInfos)
	{
		for (auto& Branch : Player.Branches)
		{
			for (auto& Frame : Branch.Frames)
			{
				m_PxScene->removeActor(*Frame);
				Frame->release();
				Frame = nullptr;
			}
		}
		if (Player.pPlayerActor)
		{
			m_PxScene->removeActor(*Player.pPlayerActor);
			Player.pPlayerActor->release();
			Player.pPlayerActor = nullptr;
		}
	}

	m_PlayerInfos.clear();
}

void CPhysXMgr::Add_Player(CGameObject* pPlayer)
{
	WRITE_LOCK
	PhysxPlayerDesc tPlayer;
	tPlayer.pPlayer = pPlayer;

	m_PlayerInfos.push_back(tPlayer);
}

void CPhysXMgr::Add_BoneBranch(CGameObject* pPlayer, vector<_uint>& Bones)
{
	WRITE_LOCK

	PLAYERDESC* PlayerDesc = Find_PlayerInfo(pPlayer);
	BRANCHDESC tBranch;

	for (auto& BoneIndex : Bones)
	{
		tBranch.Bones.push_back(BoneIndex);
	}

	if (PlayerDesc == nullptr)
		return;

	PxMaterial* Material = m_PhysX->createMaterial(0.5f, 0.5f, 0.6f);
	CModel* pModel = pPlayer->Get_ModelCom();

	Matrix matPivot = pModel->Get_PivotMatrix();
	Matrix matPlayerWorld = pPlayer->Get_TransformCom()->Get_WorldMatrix();

	PxShape* pSphereShape = m_PhysX->createShape(PxSphereGeometry(0.01f), *Material);
	_float fCapsuleRadius = 0.01f;

	for (_uint i = 0; i < Bones.size(); ++i)
	{
		if (i == 0)
		{
			Matrix matFirst = pModel->Get_CombinedMatrix(Bones[i]) * matPivot * matPlayerWorld;
			PxRigidDynamic* pRoot = m_PhysX->createRigidDynamic(MatrixToPxTrans(matFirst));

			pRoot->attachShape(*pSphereShape);
			pRoot->setMass(1.0f);
			pRoot->setLinearDamping(5.0f);
			pRoot->setAngularDamping(0.05f);
			pRoot->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

			m_PxScene->addActor(*pRoot);
			tBranch.Frames.push_back(pRoot);
			tBranch.RelativeMatrix.push_back(XMMatrixIdentity());
			continue;
		}

		Matrix matFirst = pModel->Get_CombinedMatrix(Bones[i - 1]) * matPivot * matPlayerWorld;
		Matrix matSecond = pModel->Get_CombinedMatrix(Bones[i]) * matPivot * matPlayerWorld;
		Matrix matFrame = XMMatrixIdentity();

		Vec3 vUp = Vec3(matFirst.m[3]) - Vec3(matSecond.m[3]);
		_float fDistance = vUp.Length();
		vUp.Normalize();
		if (vUp != Vec3(0.0f, 1.0f, 0.0f))
		{
			Vec3 vRight = Vec3(0.0f, 1.0f, 0.0f).Cross(vUp);
			vRight.Normalize();
			Vec3 vLook = vRight.Cross(vUp);
			vLook.Normalize();

			matFrame.Right(vRight);
			matFrame.Up(vUp);
			matFrame.Backward(vLook);
		}
		matFrame.Translation((Vec3(matFirst.m[3]) + Vec3(matSecond.m[3])) * 0.5f);

		Matrix matRelative = matSecond * matFrame.Invert();
		tBranch.RelativeMatrix.push_back(matRelative);

		PxTransform FirstBone = MatrixToPxTrans(matFirst);
		PxTransform SecondBone = MatrixToPxTrans(matSecond);

		PxTransform FrameTrans = MatrixToPxTrans(matFrame);
		PxRigidDynamic* pChild = m_PhysX->createRigidDynamic(FrameTrans);

		_float fHalfHeight = (fDistance - (fCapsuleRadius * 4.0f)) * 0.5f;
		PxShape* pFrameShape = m_PhysX->createShape(PxCapsuleGeometry(fCapsuleRadius, fHalfHeight), *Material);
		PxTransform pShapeTransform = PxTransformFromSegment(PxVec3(0.0f, -1.0f, 0.0f), PxVec3(0.0f, 1.0f, 0.0f));
		pFrameShape->setLocalPose(pShapeTransform);

		pChild->attachShape(*pFrameShape);
		pChild->setMass(5.0f - (i * 1.0f));
		pChild->setLinearDamping(50.0f - (i * 5.0f));
		pChild->setAngularDamping(i * 0.5f);

		m_PxScene->addActor(*pChild);
		tBranch.Frames.push_back(pChild);

		PxRigidDynamic* pParent = tBranch.Frames[i - 1];

		PxVec3 vJointPos = FirstBone.p;
		_float fParentDistance = (vJointPos - pParent->getGlobalPose().p).magnitude();
		_float fChildDistance = (vJointPos - pChild->getGlobalPose().p).magnitude();

		PxSphericalJoint* pJoint = PxSphericalJointCreate(*m_PhysX, 
			pParent, 
			PxTransform(0.0f, -fParentDistance, 0.0f), 
			pChild, 
			PxTransform(0.0f, fChildDistance, 0.0f));
		
		pJoint->setLimitCone(PxJointLimitCone(0.0001f, PxPi, PxSpring(0.05f, 0.05f)));
		pJoint->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
	}

	PlayerDesc->Branches.push_back(tBranch);
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

Matrix CPhysXMgr::TransToMatrix(PxTransform transValue)
{
	Vec3 vPos(transValue.p.x, transValue.p.y, transValue.p.z);
	Quaternion vQuat(transValue.q.x, transValue.q.y, transValue.q.z, transValue.q.w);

	Matrix matResult = XMMatrixIdentity();

	matResult *= Matrix::CreateFromQuaternion(vQuat);
	matResult.Translation(vPos);

	return matResult;
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


