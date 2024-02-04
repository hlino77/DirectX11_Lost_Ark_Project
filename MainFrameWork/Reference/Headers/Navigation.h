#pragma once
#include "Base.h"
#include "AsTypes.h"
#include <filesystem>
#include <fstream>
#include <commdlg.h>

BEGIN(Engine)

class CNavigation : public CBase
{


private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;


public:
	HRESULT Initialize();
	HRESULT Render();


public: // Hwang's Founction

	void	SetUp_OnCell(CGameObject* pObject);
	_float Get_Height_OnCell(CGameObject* pObject);
	_bool	Is_Outside(CGameObject* pObject, _float fOffset);
	void	Find_FirstCell(CGameObject* pObject);
	_bool	Picking_Cell(Vec3 vRayPos, Vec3 vRayDir, _float& fDist);
	_int	Check_Pos_InCell(Vec3 vPos);
	CCell*	Find_Cell(_uint iIndex) { return m_vecCells[iIndex]; }


public:
	HRESULT Add_Cell(Vec3 PointPos[3]);
	HRESULT Delete_Cell();

	HRESULT	Set_Neighbors();

	HRESULT Add_IndexCell(Vec3 PointPos[3], _int Index);
	HRESULT Delete_IndexCell( _int Index);

	void	Save_Navigation(const wstring& szFileName);
	void	Load_Navigation(const wstring& szFileName);

	void	Cell_FileLoad(ifstream& file);


public:  // Get Set
	vector <class CCell*> Get_Cells() { return m_vecCells; }

	CCell* Get_Cell(_int Index) { return m_vecCells[Index]; }

	void Set_SelectCell(_uint iIndex);

	void Reset_CellPoint();

	void Set_Active_Specific_Cell(_uint Index, _bool Active) { m_vecCells[Index]->Set_Active(Active); }
	void Set_Active_All_Cell(_bool Active) 
	{
		for (auto Cell : m_vecCells)
		{
			Cell->Set_Active(Active);
		}
	}

	_bool Is_NeighborActive(CGameObject* pObject);

	Vec3 Find_CloseCell_Middle(CGameObject* pObject);

private:

	vector<class CCell*>			m_vecCells;

	class CCellPoint*			   m_pCellPoint[3] = { nullptr, nullptr, nullptr };


	_int				   m_iCurrentIndex = { -1 };

	CCell*				   m_pCurrnetCell = nullptr;
	CCell*				   m_pSelectCell = nullptr;

private: // For Render
	ID3D11Device*							 m_pDevice = { nullptr };
	ID3D11DeviceContext*					 m_pContext = { nullptr };

	PrimitiveBatch<VertexPositionColor>*	 m_pBatch = nullptr;
	BasicEffect*							 m_pEffect = nullptr;
	ID3D11InputLayout*						 m_pInputLayout = nullptr;
	
	_bool									 m_IsFirstRender = true;

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END