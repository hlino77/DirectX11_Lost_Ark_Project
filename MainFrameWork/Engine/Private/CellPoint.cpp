#include "CellPoint.h"
#include "GameInstance.h"
#include "DebugDraw.h"

CCellPoint::CCellPoint(Vec3 vPos)
	: m_vPos(vPos)
{
	m_tBoundingSphere.Center = m_vPos;
	m_tBoundingSphere.Radius = 0.3f;
}

CCellPoint::CCellPoint(const CCellPoint& rhs)
	: m_vPos(rhs.m_vPos)
	, m_tBoundingSphere(rhs.m_tBoundingSphere)
{

}

_bool CCellPoint::Intersects(const BoundingSphere& tSphere)
{
	return m_tBoundingSphere.Intersects(tSphere);
}
_bool CCellPoint::Intersects(Vec3 TargetPos)
{
	_float R = m_tBoundingSphere.Radius;

	_float X = m_tBoundingSphere.Center.x;
	_float Y = m_tBoundingSphere.Center.y;
	_float Z = m_tBoundingSphere.Center.z;

	_float TX = TargetPos.x;
	_float TY = TargetPos.y;
	_float TZ = TargetPos.z;

	_float Distance = {};

	Distance = sqrt(pow((TX - X), 2) + pow((TY - Y), 2) + pow((TZ - Z), 2));

	if (Distance <= R)
	{
		return true;
	}
	else
	{
		return false;
	}

}
void CCellPoint::Free()
{
	__super::Free();
}

