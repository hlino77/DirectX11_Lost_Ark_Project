#pragma once
#include "Base.h"
#include "AsTypes.h"

BEGIN(Engine)

class CCell;
class CGameObject;

class CNavigation : public CBase
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;




public:
	HRESULT Render();
	HRESULT Initialize();
public:
	CCell*		Find_Cell(_uint iIndex) { return m_Cells[iIndex]; }
	void		Find_FirstCell(CGameObject* pObject);
	_int		Check_Pos_InCell(Vec3 vPos);

	void		Load_Navigation(const wstring& szFileName);


	void		SetUp_OnCell(CGameObject* pObject);
private:
	vector<CCell*>			m_Cells;

	CCell*					m_pCurrCell = nullptr;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;


public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END