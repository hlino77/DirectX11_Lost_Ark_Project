#include "CollisionManager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CCollisionManager);

CCollisionManager::CCollisionManager()
{
}

void CCollisionManager::LateTick_Collision(const _float& fTimeDelta)
{
	if (m_Colliders.empty())	
		return;

	for (auto& bSort : m_arrSorted)
		bSort = false;


	for (_uint iRow = 0; iRow < m_iNumColLayers; ++iRow)
	{
		for (_uint iCol = iRow; iCol < m_iNumColLayers; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				READ_LOCK
				CheckDynamicCollision(iRow, iCol, fTimeDelta);
				//CheckStaticCollision(reinterpret_cast<LAYERTAG&>(iRow), fTimeDelta);
			}
		}
	}


	for (auto& iter : m_hashColInfo)
		iter.second.bCheck = false;
}

HRESULT CCollisionManager::Reserve_Manager(_uint iNumColTypes)
{

	m_iNumColLayers = iNumColTypes;

	m_Colliders.resize(m_iNumColLayers, vector<CSphereCollider*>());
	//CheckGroup(LAYERTAG::DEFAULT, LAYERTAG::PLAYER);
	//CheckGroup(LAYERTAG::DEFAULT, LAYERTAG::DEFAULT);
	Reset();

	return S_OK;
}

void CCollisionManager::Add_Colider(CSphereCollider* pCollider)
{
	WRITE_LOCK
	m_Colliders[pCollider->Get_ColLayer()].push_back(pCollider);
}

void CCollisionManager::Out_Colider(CSphereCollider* pCollider)
{
	WRITE_LOCK;
	for (auto iter = m_Colliders[pCollider->Get_ColLayer()].begin(); iter != m_Colliders[pCollider->Get_ColLayer()].end();)
	{
		if (*iter == pCollider)
			iter = m_Colliders[pCollider->Get_ColLayer()].erase(iter);
	}
}

void CCollisionManager::CheckGroup(_uint iLeft, _uint iRight)
{
	_uint iRow = iLeft;
	_uint iCol = iRight;

	if (iCol < iRow)
	{
		iRow = iRight;
		iCol = iLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
}

void CCollisionManager::Reset()
{
	m_arrCheck.clear();
	m_arrSorted.clear();


	m_arrCheck.resize(m_iNumColLayers, 0);
	m_arrSorted.resize(m_iNumColLayers, false);

	for (auto& iter : m_Colliders)
	{
		iter.clear();
	}
	m_bStop = false;
}

bool CCollisionManager::IsCollided(CSphereCollider* pLeft, CSphereCollider* pRight)
{
	if (pLeft->Intersects(pRight))
		return true;

    return false;
}

void CCollisionManager::CheckDynamicCollision(_uint iLayerLeft, _uint iLayerRight , const _float& fTimeDelta)
{
	if (m_Colliders[iLayerLeft].empty() || m_Colliders[iLayerRight].empty())
		return;

	vector<CSphereCollider*>& vecLeft = m_Colliders[iLayerLeft];
	vector<CSphereCollider*>& vecRight = m_Colliders[iLayerRight];
	
	if (!m_arrSorted[iLayerRight])
	{	// sweep and prune
		::sort(vecRight.begin(), vecRight.end(), [&](CSphereCollider* pObjL, CSphereCollider* pObjR) ->_bool
			{
				
				BoundingSphere& tBoundingSphereL = pObjL->GetBoundingSphere();
				BoundingSphere& tBoundingSphereR = pObjR->GetBoundingSphere();
				return tBoundingSphereL.Center.z - tBoundingSphereL.Radius < tBoundingSphereR.Center.z - tBoundingSphereR.Radius;
			});

		m_arrSorted[iLayerRight] = true;
	}

	CollisionHash::iterator iter;

	for (auto& iterL : vecLeft)
	{
		if (nullptr == iterL)
			continue;

		_float	fLeftMaxZ = iterL->GetBoundingSphere().Center.z + iterL->GetBoundingSphere().Radius;
		_bool	bIgnoreRest = false;

		for (auto& iterR : vecRight)
		{
			if (nullptr == iterR || iterL == iterR)
				continue;

			if (iterL->Get_Owner() == iterR->Get_Owner())
				continue;

			if (!bIgnoreRest)
			{
				_float fRightMinZ = iterR->GetBoundingSphere().Center.z - iterR->GetBoundingSphere().Radius;

				if (false == CompareMaxMinZ(fLeftMaxZ, fRightMinZ)) // false면 이후 무시 or Exit호출
				{
					bIgnoreRest = true;
				}

				COLLIDER_ID ID;
				ID.Left_id = min(iterL->GetID(), iterR->GetID());
				ID.Right_id = max(iterL->GetID(), iterR->GetID());

				iter = m_hashColInfo.find(ID.ID);

				if (m_hashColInfo.end() == iter)
				{
					if (!iterL->IsActive() || !iterR->IsActive())
						continue;

					IDInfo tID;
					tID.bCheck = true;
					tID.bCollsion = false;

					m_hashColInfo.insert(make_pair(ID.ID, tID));
					iter = m_hashColInfo.find(ID.ID);
				}
				else
				{
					if (iter->second.bCheck)
						continue;
					else
						iter->second.bCheck = true;


					if (!iterL->IsActive() || !iterR->IsActive())
					{
						if (iter->second.bCollsion)	//이전 프레임 충돌
						{
							iterL->OnCollisionExit(iterR);
							iterR->OnCollisionExit(iterL);
							iter->second.bCollsion = false;
						}
						continue;
					}
				}



				if (IsCollided(iterL, iterR))
				{	// 현재 충돌 중
					if (!iter->second.bCollsion)
					{
						iterL->OnCollisionEnter(iterR);
						iterR->OnCollisionEnter(iterL);
						iter->second.bCollsion = true;
					}
				}
				else	// 현재 충돌 x면
				{	
					if (iter->second.bCollsion)	//이전 프레임 충돌
					{	
						iterL->OnCollisionExit(iterR);
						iterR->OnCollisionExit(iterL);
						iter->second.bCollsion = false;
					}
				}
			}
			else	// Z비교 결과 무시해도 된다면
			{
				COLLIDER_ID ID;
				ID.Left_id = min(iterL->GetID(), iterR->GetID());
				ID.Right_id = max(iterL->GetID(), iterR->GetID());

				iter = m_hashColInfo.find(ID.ID);

				if (m_hashColInfo.end() == iter)
				{
					if (!iterL->IsActive() || !iterR->IsActive())
						continue;

					IDInfo tID;
					tID.bCheck = true;
					tID.bCollsion = false;

					m_hashColInfo.insert(make_pair(ID.ID, tID));
					iter = m_hashColInfo.find(ID.ID);
				}
				else
				{
					if (iter->second.bCollsion)	// 이전에는 충돌하고 있었다면
					{
						iterL->OnCollisionExit(iterR);
						iterR->OnCollisionExit(iterL);
						iter->second.bCollsion = false;
					}
				}
			}
		}
	}
}


_bool CCollisionManager::CompareMaxMinZ(_float& fLeftMaxZ, _float& fRightMinZ)
{
	if (fLeftMaxZ < fRightMinZ)
		return false;
	return true;
}

void CCollisionManager::Free()
{
	__super::Free();
}
