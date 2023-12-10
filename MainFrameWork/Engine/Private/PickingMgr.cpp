#include "PickingMgr.h"
#include "PipeLine.h"
#include "Transform.h"
#include "GameObject.h"
#include "VIBuffer.h"
#include "Mesh.h"
#include "Model.h"

IMPLEMENT_SINGLETON(CPickingMgr)

CPickingMgr::CPickingMgr()
{
}

HRESULT CPickingMgr::Ready_PickingMgr()
{
	return S_OK;
}

void CPickingMgr::Update_PickingMgr()
{
	if (m_pPlayer)
	{
		if (m_pPlayer->Is_Picking())
		{
			WRITE_LOCK
			TRIAGLEDESC tResult;
			tResult.fDist = -1.0f;

			for (auto& ColMesh : m_ColMeshList)
			{
				TRIAGLEDESC tTriangle;
				tTriangle.fDist = -1.0f;
				if (IsPicking(ColMesh, &tTriangle))
				{
					if (tResult.fDist == -1.0f || tTriangle.fDist < tResult.fDist)
						tResult = tTriangle;
				}
			}


			if (tResult.fDist != -1.0f)
			{
				Vec3 vDir1 = tResult.vPos1 - tResult.vPos0;
				Vec3 vDir2 = tResult.vPos2 - tResult.vPos1;

				tResult.vNormal = vDir1.Cross(vDir2);
				tResult.vNormal.Normalize();
			}

			m_pPlayer->Set_Pick(tResult);
		}
	}
}

void CPickingMgr::Add_ColMesh(CGameObject* pObj)
{
	WRITE_LOCK
	if (Find_ColMesh(pObj) != nullptr)
		return;
		
	m_ColMeshList.push_back(pObj);
}

void CPickingMgr::Delete_ColMesh(CGameObject* pObj)
{
	WRITE_LOCK

	for (auto& iter = m_ColMeshList.begin(); iter != m_ColMeshList.end();)
	{
		if (pObj == *iter)
			iter = m_ColMeshList.erase(iter);
		else
			++iter;
	}
}

void CPickingMgr::Set_Ray(Vec3 vRayPos, Vec3 vRayDir)
{
	WRITE_LOCK
	m_vRayPos = vRayPos;
	m_vRayDir = vRayDir;
	m_vRayDir.Normalize();
}

void CPickingMgr::Reset()
{
	m_ColMeshList.clear();
	m_bStop = false;
}

void CPickingMgr::Compute_LocalLayInfo(Vec3* pDir, Vec3* pRayPos, CTransform* pTransform)
{
	Matrix matWorldInv = pTransform->Get_WorldMatrixInverse();

	*pRayPos = XMVector3TransformCoord(m_vRayPos, matWorldInv);
	*pDir = XMVector3TransformNormal(m_vRayDir, matWorldInv);
}

void CPickingMgr::Compute_LocalLayInfo(Vec3* pOutDir, Vec3* pOutRayPos, Vec3 vRayPos, Vec3 vRayDir, CTransform* pTransform)
{
	Matrix matWorldInv = pTransform->Get_WorldMatrixInverse();

	*pOutRayPos = XMVector3TransformCoord(vRayPos, matWorldInv);
	*pOutDir = XMVector3TransformNormal(vRayDir, matWorldInv);
}


CGameObject* CPickingMgr::Find_ColMesh(CGameObject* pObj)
{
	for (auto& ColMesh : m_ColMeshList)
	{
		if (ColMesh == pObj)
			return ColMesh;
	}

	return nullptr;
}

BOOL CPickingMgr::IsPicking(CGameObject* _pObj, TRIAGLEDESC* tTriangle)
{
	WRITE_LOCK
	CModel* pModel = _pObj->Get_ModelCom();

	if (pModel == nullptr)
		return false;



	CTransform* pTransform = _pObj->Get_TransformCom();
	vector<CMesh*>& ObjectMeshes = pModel->Get_Meshes();

	BOOL bRayHit = false;

	for (auto& Mesh : ObjectMeshes)
	{
		Matrix matWorld = pTransform->Get_WorldMatrix();
		VTXANIMMODEL* objVB = Mesh->Get_Vertices();
		FACEINDICES32* objIB = Mesh->Get_Indices();
		Matrix matTargetWorld = _pObj->Get_TransformCom()->Get_WorldMatrix();


		_uint iNumVertices = Mesh->Get_NumVertices();
		_uint iNumPrimitives = Mesh->Get_NumPrimitives();


		Vec3 vRayDir, vRayPos;
		Compute_LocalLayInfo(&vRayDir, &vRayPos, pTransform);

		vRayDir.Normalize();

		_float fU, fV, fDist;

		_float fResultDist = -1.0f;
		_float fMinDistance = 8.f;

		for (_uint i = 0; i < iNumPrimitives; ++i)
		{
			if (TriangleTests::Intersects(vRayPos,
				vRayDir, XMLoadFloat3(&objVB[objIB[i]._0].vPosition),
				XMLoadFloat3(&objVB[objIB[i]._1].vPosition),
				XMLoadFloat3(&objVB[objIB[i]._2].vPosition), fDist))
			{
				if (fResultDist == -1.0f || fDist < fResultDist)
				{
					if (fDist < fMinDistance)
					{
						fResultDist = fDist;
						tTriangle->vPos0 = XMVector3TransformCoord(objVB[objIB[i]._0].vPosition, matTargetWorld);
						tTriangle->vPos1 = XMVector3TransformCoord(objVB[objIB[i]._1].vPosition, matTargetWorld);
						tTriangle->vPos2 = XMVector3TransformCoord(objVB[objIB[i]._2].vPosition, matTargetWorld);

						tTriangle->fDist = fDist;

						bRayHit = true;
					}
				}
			}
		}
	}



	return bRayHit;
}

BOOL CPickingMgr::IsPicking(Vec3 vRayPos, Vec3 vRayDir, CGameObject* _pObj, TRIAGLEDESC* tTriangle, _float fMinDistance)
{
	CModel* pModel = _pObj->Get_ModelCom();

	if (pModel == nullptr)
		return false;

	CTransform* pTransform = _pObj->Get_TransformCom();
	vector<CMesh*>& ObjectMeshes = pModel->Get_Meshes();

	BOOL bRayHit = false;

	for (auto& Mesh : ObjectMeshes)
	{
		Matrix matWorld = pTransform->Get_WorldMatrix();
		VTXANIMMODEL* objVB = Mesh->Get_Vertices();
		FACEINDICES32* objIB = Mesh->Get_Indices();
		Matrix matTargetWorld = _pObj->Get_TransformCom()->Get_WorldMatrix();


		_uint iNumVertices = Mesh->Get_NumVertices();
		_uint iNumPrimitives = Mesh->Get_NumPrimitives();


		Vec3 vOutRayDir, vOutRayPos;
		Compute_LocalLayInfo(&vOutRayDir, &vOutRayPos, vRayPos, vRayDir, pTransform);

		vOutRayDir.Normalize();

		_float fU, fV, fDist;

		_float fResultDist = -1.0f;

		for (_uint i = 0; i < iNumPrimitives; ++i)
		{
			if (TriangleTests::Intersects(vOutRayPos,
				vOutRayDir, XMLoadFloat3(&objVB[objIB[i]._0].vPosition),
				XMLoadFloat3(&objVB[objIB[i]._1].vPosition),
				XMLoadFloat3(&objVB[objIB[i]._2].vPosition), fDist))
			{
				if (fResultDist == -1.0f || fDist < fResultDist)
				{
					if (fDist <= fMinDistance)
					{
						fResultDist = fDist;
						tTriangle->vPos0 = XMVector3TransformCoord(objVB[objIB[i]._0].vPosition, matTargetWorld);
						tTriangle->vPos1 = XMVector3TransformCoord(objVB[objIB[i]._1].vPosition, matTargetWorld);
						tTriangle->vPos2 = XMVector3TransformCoord(objVB[objIB[i]._2].vPosition, matTargetWorld);

						tTriangle->fDist = fDist;

						bRayHit = true;
					}
				}
			}
		}
	}

	return bRayHit;
}


void CPickingMgr::Free()
{
}


