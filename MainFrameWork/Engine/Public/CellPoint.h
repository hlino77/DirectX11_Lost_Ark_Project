#pragma once
#include "Base.h"
#include "AsTypes.h"


BEGIN(Engine)

class CCellPoint : public CBase
{
public:
	CCellPoint(Vec3 vPos);
	CCellPoint(const CCellPoint& rhs);
	virtual ~CCellPoint() = default;


public:
	_bool	Intersects(const BoundingSphere& tSphere);
	_bool	Intersects(Vec3 TargetPos);

	const BoundingSphere& Get_BoundingSphere() { return m_tBoundingSphere; }

	Vec3	Get_Pos() { return m_vPos; }
	void	Set_Pos(Vec3 vector) 
	{
		m_tBoundingSphere.Center = m_vPos = vector;
	
	}

private:
	Vec3 m_vPos;
	BoundingSphere	m_tBoundingSphere;


public:
	virtual void Free() override;

};

END