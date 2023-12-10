#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CNavigation;

class CCell : public CBase
{
private:
	CCell();
	CCell(const CCell& rhs);
	virtual ~CCell() = default;

public:
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
public:
	HRESULT Initialize(CNavigation* pNavigation, _uint iIndex);
	HRESULT Render();
public:
	void					Add_Point(Vec3 vPos) { m_OriginPoint.push_back(vPos); }

	_uint					Get_CellIndex() { return m_iIndex; }

	void					Set_Neighbor(_uint iLine, _uint iCellIndex) { m_iNeighbor[iLine] = iCellIndex; }

	_int*					Get_Neighbor() { return m_iNeighbor; }

	vector<Vec3>&			Get_Points() { return m_Points; }
	vector<Vec3>&			Get_OriginPoints() { return m_OriginPoint; }

	void					Set_Normal();

	void					SetUp_OnCell(CGameObject* pObject, _uint iCount);


	_bool					Intersects(Vec3 vOrigin, Vec3 vDir);
private:
	vector<Vec3> m_Points;
	vector<Vec3> m_OriginPoint;

	_int m_iNeighbor[LINE_END];

	_uint m_iIndex;

	Vec3 m_vNormal;
	Vec3 m_vOriginNormal;

	Vec3 m_vLine[LINE_END];
	Vec3 m_vLineNormal[LINE_END];


	CNavigation* m_pNavigation = nullptr;

public:
	static CCell* Create(CNavigation* pNavigation, _uint iIndex);
	virtual void Free() override;

};

END