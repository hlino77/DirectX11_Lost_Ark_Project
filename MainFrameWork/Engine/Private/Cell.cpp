#include "Cell.h"
#include "GameInstance.h"
#include "Engine_Defines.h"
#include "Transform.h"
#include "Navigation.h"

CCell::CCell()
{
}

CCell::CCell(const CCell& rhs)
{
}

HRESULT CCell::Initialize(class CNavigation* pNavigation, const Vec3* pPoints, _uint iIndex)
{

	m_pNavigation = pNavigation;

	m_iCellIndex = iIndex;

	// Local Point Set
	memcpy(m_vPoints_Original, pPoints, sizeof(Vec3) * POINT_END);

    Vec3 vLine[LINE_END];

	// Make Line   =  Vector - Vector
	XMStoreFloat3(&vLine[LINE_AB], XMLoadFloat3(&m_vPoints_Original[POINT_B]) - XMLoadFloat3(&m_vPoints_Original[POINT_A]));
	XMStoreFloat3(&vLine[LINE_BC], XMLoadFloat3(&m_vPoints_Original[POINT_C]) - XMLoadFloat3(&m_vPoints_Original[POINT_B]));
	XMStoreFloat3(&vLine[LINE_CA], XMLoadFloat3(&m_vPoints_Original[POINT_A]) - XMLoadFloat3(&m_vPoints_Original[POINT_C]));



	for (size_t i = 0; i < LINE_END; i++)
	{
		// Normal == (x, y=0, z) => (-z, y=0, x)                 
		m_vNormals[i] = Vec3(vLine[i].z * -1.f, 0.f, vLine[i].x);

		XMStoreFloat3(&m_vNormals[i], XMVector3Normalize(XMLoadFloat3(&m_vNormals[i])));
	}

	Set_Normal();

	return S_OK;
}

HRESULT CCell::Render()
{
	return S_OK;
}

_bool CCell::Intersects(Vec3 vOrigin, Vec3 vDir, _float& fDist)
{
    return TriangleTests::Intersects(vOrigin, vDir, m_vPoints_Original[0], m_vPoints_Original[1], m_vPoints_Original[2], fDist);
}

void CCell::SetUp_OnCell(CGameObject* pObject, _uint iCount)
{
	Vec3 vPlayerPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = vPlayerPos;
	vPos.y = 0.0f;


	for (_int i = 0; i < CELLLINE::LINE_END; ++i)
	{
		if (m_iNeighbor[i] != -1)
		{
			Vec3 vDir = vPos - m_vPoints[i];
			vDir.Normalize();

			_float fDot = vDir.Dot(m_vNormals[i]);
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

	for (_int i = 0; i < CELLLINE::LINE_END; ++i)
	{
		Vec3 vDir = vPos - m_vPoints[i];
		vDir.Normalize();

		_float fDot = vDir.Dot(m_vNormals[i]);
		if (fDot > 0.001f)
		{
			if (m_iNeighbor[i] == -1)
			{
				Vec3 vDir = vPos - m_vPoints[i];
				_float fLength = vDir.Dot(m_vLine[i]);

				/*	if (fLength < 0.0f)
						fLength = 0.0f;*/

				Vec3 vLinePos = m_vPoints[i] + m_vLine[i] * fLength;

				vPos = vLinePos;
				vPos.y = vPlayerPos.y;

				vPlayerPos = vPos;
				vPos.y = 0.0f;
			}
		}
	}

	Vec3 vDir = vPlayerPos - m_vPoints_Original[0];
	
	_float fDistance = vDir.Dot(m_vOriginNormal);

	Vec3 vDirPlane = -m_vOriginNormal * fDistance;
	Vec3 vPlanePos = vPlayerPos + vDirPlane;
	
	vPlayerPos.y = vPlanePos.y;

	//TRIAGLEDESC tTriangle;
	//tTriangle.vPos0 = m_vPoints_Original[0];
	//tTriangle.vPos1 = m_vPoints_Original[1];
	//tTriangle.vPos2 = m_vPoints_Original[2];
	//tTriangle.vNormal = m_vOriginNormal;

	//pObject->Set_Pick(tTriangle);
	pObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPlayerPos);
}

_bool CCell::isOut(Vec3 vPoint, _int* pNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		Vec3 vDir = vPoint - m_vPoints[i];
		vDir.Normalize();

		_float fDot = vDir.Dot(m_vNormals[i]);
		if (fDot > 0.001f)
		{
			*pNeighborIndex = m_iNeighbor[i];
			return true;
		}
	}

	return false;
}

void CCell::Set_Normal()
{
	for (_uint i = 0; i < 3; ++i)
	{
		m_vPoints[i] = m_vPoints_Original[i];
		m_vPoints[i].y = 0.0f;
	}

	{
		Vec3 vDir1 = m_vPoints[1] - m_vPoints[0];
		Vec3 vDir2 = m_vPoints[2] - m_vPoints[1];

		Vec3 vNormal = vDir1.Cross(vDir2);
		vNormal.Normalize();

		m_vNormal = vNormal;
	}


	{
		Vec3 vDir1 = m_vPoints_Original[1] - m_vPoints_Original[0];
		Vec3 vDir2 = m_vPoints_Original[2] - m_vPoints_Original[1];

		Vec3 vNormal = vDir1.Cross(vDir2);
		vNormal.Normalize();

		m_vOriginNormal = vNormal;
	}




	if (m_vNormal.y < 0.0f)
	{
		m_vNormal *= -1.0f;

		Vec3 vPoint[3] = { m_vPoints[0], m_vPoints[1], m_vPoints[2] };

		m_vPoints[0] = vPoint[2];
		m_vPoints[1] = vPoint[1];
		m_vPoints[2] = vPoint[0];

		_uint iNeighbor[3] = { (_uint)m_iNeighbor[CELLLINE::LINE_AB], (_uint)m_iNeighbor[CELLLINE::LINE_BC], (_uint)m_iNeighbor[CELLLINE::LINE_CA] };

		m_iNeighbor[CELLLINE::LINE_AB] = iNeighbor[CELLLINE::LINE_BC];
		m_iNeighbor[CELLLINE::LINE_BC] = iNeighbor[CELLLINE::LINE_AB];
		m_iNeighbor[CELLLINE::LINE_CA] = iNeighbor[CELLLINE::LINE_CA];
	}


	m_vLine[CELLLINE::LINE_AB] = m_vPoints[1] - m_vPoints[0];
	m_vLine[CELLLINE::LINE_BC] = m_vPoints[2] - m_vPoints[1];
	m_vLine[CELLLINE::LINE_CA] = m_vPoints[0] - m_vPoints[2];

	m_vLine[CELLLINE::LINE_AB].Normalize();
	m_vLine[CELLLINE::LINE_BC].Normalize();
	m_vLine[CELLLINE::LINE_CA].Normalize();



	for (_uint i = 0; i < CELLLINE::LINE_END; ++i)
	{
		m_vNormals[i].x = m_vLine[i].z * -1.0f;
		m_vNormals[i].z = m_vLine[i].x;
		m_vNormals[i].y = 0.0f;
	}

	//Quaternion vQuat = Quaternion::CreateFromAxisAngle(m_vNormal, -XMConvertToRadians(90.0f));
	//m_vLineNormal[LINE::LINE_AB] = XMVector3Rotate(m_vLine[LINE::LINE_AB], vQuat);
	//m_vLineNormal[LINE::LINE_BC] = XMVector3Rotate(m_vLine[LINE::LINE_BC], vQuat);
	//m_vLineNormal[LINE::LINE_CA] = XMVector3Rotate(m_vLine[LINE::LINE_CA], vQuat);
}



_bool CCell::Compare_Points(const Vec3* pSourPoint, const Vec3* pDestPoint)
{
    if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_A]), XMLoadFloat3(pSourPoint)))
    {
        // AB LINE
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_B]), XMLoadFloat3(pDestPoint)))
        {
            return true;
        }


        // CA LINE
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_C]), XMLoadFloat3(pDestPoint)))
        {
            return true;
        }

    }

    if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_B]), XMLoadFloat3(pSourPoint)))
    {
        // AB LINE
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_A]), XMLoadFloat3(pDestPoint)))
            return true;

        // BC LINE
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_C]), XMLoadFloat3(pDestPoint)))
            return true;
    }

    if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_C]), XMLoadFloat3(pSourPoint)))
    {
        // CA LINE
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_A]), XMLoadFloat3(pDestPoint)))
            return true;

        // BC LINE
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints_Original[POINT_B]), XMLoadFloat3(pDestPoint)))
            return true;
    }

    return false;
}





	

CCell* CCell::Create(class CNavigation* pNavigation, const Vec3* pPoints, _uint iIndex)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(pNavigation, pPoints, iIndex)))
	{
		MSG_BOX("Failed To Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
