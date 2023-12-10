
#include "Cell.h"
#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Navigation.h"

CCell::CCell()
{
}

CCell::CCell(const CCell& rhs)
{
}

HRESULT CCell::Initialize(CNavigation* pNavigation, _uint iIndex)
{
	for (_uint i = 0; i < LINE::LINE_END; ++i)
	{
		m_iNeighbor[i] = -1;
	}

	m_iIndex = iIndex;
	m_pNavigation = pNavigation;

	return S_OK;
}

HRESULT CCell::Render()
{
	return S_OK;
}


void CCell::Set_Normal()
{
	for (_uint i = 0; i < 3; ++i)
	{
		m_Points.push_back(m_OriginPoint[i]);
		m_Points[i].y = 0.0f;
	}

	{
		Vec3 vDir1 = m_Points[1] - m_Points[0];
		Vec3 vDir2 = m_Points[2] - m_Points[1];

		Vec3 vNormal = vDir1.Cross(vDir2);
		vNormal.Normalize();

		m_vNormal = vNormal;
	}

	
	{
		Vec3 vDir1 = m_OriginPoint[1] - m_OriginPoint[0];
		Vec3 vDir2 = m_OriginPoint[2] - m_OriginPoint[1];

		Vec3 vNormal = vDir1.Cross(vDir2);
		vNormal.Normalize();

		m_vOriginNormal = vNormal;
	}




	if (m_vNormal.y < 0.0f)
	{
		m_vNormal *= -1.0f;

		Vec3 vPoint[3] = { m_Points[0], m_Points[1], m_Points[2] };
	

		m_Points[0] = vPoint[2];
		m_Points[1] = vPoint[1];
		m_Points[2] = vPoint[0];


		_uint iNeighbor[3] = { m_iNeighbor[LINE::LINE_AB], m_iNeighbor[LINE::LINE_BC], m_iNeighbor[LINE::LINE_CA] };

		m_iNeighbor[LINE::LINE_AB] = iNeighbor[LINE::LINE_BC];
		m_iNeighbor[LINE::LINE_BC] = iNeighbor[LINE::LINE_AB];
		m_iNeighbor[LINE::LINE_CA] = iNeighbor[LINE::LINE_CA];
	}


	m_vLine[LINE::LINE_AB] = m_Points[1] - m_Points[0];
	m_vLine[LINE::LINE_BC] = m_Points[2] - m_Points[1];
	m_vLine[LINE::LINE_CA] = m_Points[0] - m_Points[2];

	m_vLine[LINE::LINE_AB].Normalize();
	m_vLine[LINE::LINE_BC].Normalize();
	m_vLine[LINE::LINE_CA].Normalize();



	for (_uint i = 0; i < LINE::LINE_END; ++i)
	{
		m_vLineNormal[i].x = m_vLine[i].z * -1.0f;
		m_vLineNormal[i].z = m_vLine[i].x;
		m_vLineNormal[i].y = 0.0f;
	}

	//Quaternion vQuat = Quaternion::CreateFromAxisAngle(m_vNormal, -XMConvertToRadians(90.0f));
	//m_vLineNormal[LINE::LINE_AB] = XMVector3Rotate(m_vLine[LINE::LINE_AB], vQuat);
	//m_vLineNormal[LINE::LINE_BC] = XMVector3Rotate(m_vLine[LINE::LINE_BC], vQuat);
	//m_vLineNormal[LINE::LINE_CA] = XMVector3Rotate(m_vLine[LINE::LINE_CA], vQuat);

}

void CCell::SetUp_OnCell(CGameObject* pObject, _uint iCount)
{
	Vec3 vPlayerPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = vPlayerPos;
	vPos.y = 0.0f;


	for (_int i = 0; i < LINE::LINE_END; ++i)
	{
		if (m_iNeighbor[i] != -1)
		{
			Vec3 vDir = vPos - m_Points[i];
			vDir.Normalize();

			_float fDot = vDir.Dot(m_vLineNormal[i]);
			if (fDot > 0.001f)
			{
				pObject->Set_CurrCell(m_iNeighbor[i]);
				if (iCount < 15)
				{
					m_pNavigation->Find_Cell(m_iNeighbor[i])->SetUp_OnCell(pObject, iCount + 1);
					return;
				}
			}
		}
	}



	
	for (_int i = 0; i < LINE::LINE_END; ++i)
	{
		Vec3 vDir = vPos - m_Points[i];
		vDir.Normalize();

		_float fDot = vDir.Dot(m_vLineNormal[i]);
		if (fDot > 0.001f)
		{
			if (m_iNeighbor[i] == -1)
			{
				Vec3 vDir = vPos - m_Points[i];
				_float fLength = vDir.Dot(m_vLine[i]);

			/*	if (fLength < 0.0f)
					fLength = 0.0f;*/

				Vec3 vLinePos = m_Points[i] + m_vLine[i] * fLength;

				vPos = vLinePos;
				vPos.y = vPlayerPos.y;

				vPlayerPos = vPos;
				vPos.y = 0.0f;
			}
		}
	}



	//Vec3 vDir = vPlayerPos - m_OriginPoint[0];
	//
	//_float fDistance = vDir.Dot(m_vOriginNormal);

	//Vec3 vDirPlane = -m_vOriginNormal * fDistance;
	//Vec3 vPlanePos = vPlayerPos + vDirPlane;
	//
	//if (vPlayerPos.y < vPlanePos.y)
	//	vPlayerPos.y = vPlanePos.y;
	
	TRIAGLEDESC tTriangle;
	tTriangle.vPos0 = m_OriginPoint[0];
	tTriangle.vPos1 = m_OriginPoint[1];
	tTriangle.vPos2 = m_OriginPoint[2];
	tTriangle.vNormal = m_vOriginNormal;

	pObject->Set_Pick(tTriangle);
	pObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPlayerPos);
}

_bool CCell::Intersects(Vec3 vOrigin, Vec3 vDir)
{
	_float fDist;
	return TriangleTests::Intersects(vOrigin, vDir, m_OriginPoint[0], m_OriginPoint[1], m_OriginPoint[2], fDist);
}

CCell* CCell::Create(CNavigation* pNavigation, _uint iIndex)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(pNavigation, iIndex)))
	{
		MSG_BOX("Failed To Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
}
  