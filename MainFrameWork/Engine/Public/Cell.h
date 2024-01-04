#pragma once
#include "Base.h"

BEGIN(Engine)

class CCell : public CBase
{
public:
	enum CELLPOINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum CELLLINE  { LINE_AB, LINE_BC, LINE_CA, LINE_END };


private:
	CCell();
	CCell(const CCell& rhs);
	virtual ~CCell() = default;


public:
	HRESULT Initialize(class CNavigation* pNavigation, const Vec3* pPoints, _uint iIndex);
	HRESULT Render();


public:
	_uint				Get_CellIndex() { return m_iCellIndex; }
	const Vec3*			Get_Point(CELLPOINT ePoint) const { return &m_vPoints_Original[ePoint]; }
	Vec3*				Get_Points()  { return m_vPoints_Original; }

	_int*				Get_Neighbor() { return m_iNeighbor; }



public:// Hwang's
	_bool					Intersects(Vec3 vOrigin, Vec3 vDir, _float& fDist);
	void					SetUp_OnCell(CGameObject* pObject, _uint iCount);

	_bool isOut(Vec3 vPoint, _int* pNeighborIndex);

	void					Set_Normal();


public:
	_bool					Compare_Points(const Vec3* pSourPoint, const Vec3* pDestPoint);

	void					SetUp_Neighbor(CELLLINE eLine, CCell* pCell) { m_iNeighbor[eLine] = pCell->m_iCellIndex; }


private:

	_uint					m_iCellIndex = {};
	
	Vec3					m_vPoints_Original[POINT_END];
	
	Vec3					m_vPoints[POINT_END];


	Vec3					m_vNormal;
	Vec3					m_vOriginNormal;


	Vec3					m_vLine[LINE_END];
	

	Vec3					m_vNormals[LINE_END];


	_int					m_iNeighbor[LINE_END] = {-1, -1, -1};



	CNavigation*			m_pNavigation = nullptr;


private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };


public:
	static CCell* Create(class CNavigation* pNavigation, const Vec3* pPoints, _uint iIndex);
	virtual void Free() override;

};

END