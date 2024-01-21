#include "Navigation.h"
#include "DebugDraw.h"
#include "Cell.h"
#include "CellPoint.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "AsFileUtils.h"


CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
{
}


HRESULT CNavigation::Initialize()
{
	if (m_pDevice && m_pContext)
	{
		m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);

		m_pEffect = new BasicEffect(m_pDevice);
		m_pEffect->SetVertexColorEnabled(true);

		const void* pShaderByteCodes = nullptr;
		size_t		iLength = 0;
		m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

		if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		{
			Safe_Delete(m_pBatch);
			Safe_Delete(m_pEffect);
			Safe_Release(m_pInputLayout);
			return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CNavigation::Render()
{
	if (m_pDevice == nullptr)
		return S_OK;

	m_pEffect->SetWorld(XMMatrixIdentity());

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pEffect->SetView(pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	Safe_Release(pGameInstance);

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);


	m_pBatch->Begin();

	if (!m_vecCells.empty())
	{
		for (auto& Cell : m_vecCells)
		{
			Vec3* Points = Cell->Get_Points();

			// 원본 포인트 복사
			Vec3 RenderPoints[3];
			RenderPoints[0] = Points[0];
			RenderPoints[1] = Points[1];
			RenderPoints[2] = Points[2];

			// 렌더링용 포인트만 조정
			RenderPoints[0].y += 0.01f;
			RenderPoints[1].y += 0.01f;
			RenderPoints[2].y += 0.01f;

			// 조정된 포인트로 렌더링

			if (true == Cell->Get_Active())
			{
				DX::DrawTriangle(m_pBatch, RenderPoints[0], RenderPoints[1], RenderPoints[2], Colors::Green);
			}
			else
			{
				DX::DrawTriangle(m_pBatch, RenderPoints[0], RenderPoints[1], RenderPoints[2], Colors::Red);

			}
		
		}
	}



	m_pBatch->End();


	return S_OK;

}

void CNavigation::SetUp_OnCell(CGameObject* pObject)
{
	_int iCurrCell = pObject->Get_CurrCell();

	if (iCurrCell < 0 || iCurrCell >= m_vecCells.size())
		return;


	m_vecCells[pObject->Get_CurrCell()]->SetUp_OnCell(pObject, 0);

}

_bool CNavigation::Is_Outside(CGameObject* pObject, _float fOffset)
{
	_int		iNeighborIndex = 0;
	Vec3 vPosition = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = pObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	vLook.Normalize();
	if (true == m_vecCells[pObject->Get_CurrCell()]->isOut(vPosition + vLook * fOffset, &iNeighborIndex))
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return true;

				if (false == m_vecCells[iNeighborIndex]->isOut(vPosition + vLook * fOffset, &iNeighborIndex))
				{
					break;
				}
			}
			return false;
		}
		else
			return true;
	}
	else
		return false;
}

void CNavigation::Find_FirstCell(CGameObject* pObject)
{
	Vec3 vPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	vPos.y += 1.0f;

	Vec3 vDir(0.0f, -1.0f, 0.0f);
	_float fDist = -1.0f;

	for (auto& Cell : m_vecCells)
	{
		if (Cell->Intersects(vPos, vDir, fDist))
		{
			pObject->Set_CurrCell(Cell->Get_CellIndex());
			return;
		}
	}

	pObject->Set_CurrCell(0);
}

_bool CNavigation::Picking_Cell(Vec3 vRayPos, Vec3 vRayDir, _float& fDist)
{
	_float fResultDist = -1.0f;
	_bool bPick = false;


	for (auto& Cell : m_vecCells)
	{
		_float fCurrDist;
		if (Cell->Intersects(vRayPos, vRayDir, fCurrDist))
		{
			if (fResultDist == -1.0f)
				fResultDist = fCurrDist;
			else
			{
				if (fResultDist > fCurrDist)
					fResultDist = fCurrDist;
			}
		}
	}


	if (fResultDist != -1.0f)
	{
		fDist = fResultDist;
		bPick = true;
	}

	return bPick;
}

_int CNavigation::Check_Pos_InCell(Vec3 vPos)
{
	vPos.y += 1.0f;

	Vec3 vDir(0.0f, -1.0f, 0.0f);
	_float fDist = -1.0f;


	for (auto& Cell : m_vecCells)
	{
		if (Cell->Intersects(vPos, vDir, fDist))
		{
			return Cell->Get_CellIndex();
		}
	}

	return -1;
}

HRESULT CNavigation::Add_Cell(Vec3 PointPos[3])
{
	CCell* pCell = CCell::Create(this, PointPos, m_vecCells.size());

	if (nullptr == pCell)
	{
		return E_FAIL;
	}
	else
	{
		m_vecCells.push_back(pCell);
	}


	return S_OK;
}

HRESULT CNavigation::Delete_Cell()
{
	//if (m_vecCells.size() == 0)
	//{
	//	MessageBox(g_hWnd, L"All Navi Cell is Deleted ", L"Delete Fail", MB_OK);
	//}

	if (m_vecCells.size() > 0)
	{
		CCell* pCell = m_vecCells.back();

		Safe_Release(pCell);

		m_vecCells.pop_back();
	}

	return S_OK;
}

HRESULT CNavigation::Set_Neighbors()
{
	for (auto& pSourCell : m_vecCells)
	{
		for (auto& pDestCell : m_vecCells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}



HRESULT CNavigation::Add_IndexCell(Vec3 PointPos[3], _int Index)
{
	CCell* pCell = CCell::Create(this, PointPos, Index);
	if (nullptr == pCell)
	{
		return E_FAIL;
	}

	if (m_vecCells.size() <= Index)
	{
		m_vecCells.resize(Index + 1);
	}
	m_vecCells[Index] = pCell;

	return S_OK;

}

HRESULT CNavigation::Delete_IndexCell(_int Index)
{

	CCell* pCell = m_vecCells[Index];

	Safe_Release(pCell);

	m_vecCells[Index] = nullptr;

	return S_OK;
}

void CNavigation::Save_Navigation(const wstring& szFileName)
{
	//wstring szFullPath = L"../Bin/Resources/Navigation/";
	//szFullPath += szFileName;

	//auto path = filesystem::path(szFullPath);

	//// 폴더가 없으면 만든다.
	//filesystem::create_directory(path.parent_path());

	//shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	//file->Open(szFullPath, FileMode::Write);


	//file->Write<_uint>(m_Points.size());

	//for (auto& Point : m_Points)
	//{
	//	file->Write<Vec3>(Point->Get_Pos());
	//}


	//file->Write<_uint>(m_vecCells.size());

	//for (auto& Cell : m_vecCells)
	//{
	//	file->Write<_uint>(Cell->Get_CellIndex());

	//	vector<_uint>& Points = Cell->Get_PointIndexes();

	//	for (auto& Point : Points)
	//		file->Write<_uint>(Point);


	//	for (_uint i = 0; i < CCell::LINE_END; ++i)
	//		file->Write<_int>(Cell->Get_Neighbor()[i]);
	//}

}

void CNavigation::Load_Navigation(const wstring& szFileName)
{
	wstring filePath = L"../Bin/Resources/Navigation/";
	wstring FullPath = filePath + szFileName;

	
	ifstream fileStream(FullPath, ios::binary);

	if (fileStream.is_open())
	{
		Cell_FileLoad(fileStream);
		fileStream.close();
	}

}

void CNavigation::Cell_FileLoad(ifstream& file)
{
	Vec3			vPoints[3];

	while (file.read(reinterpret_cast<char*>(vPoints), sizeof(Vec3) * 3))
	{
		Add_Cell(vPoints);
	}

	Set_Neighbors();
}

void CNavigation::Set_SelectCell(_uint iIndex)
{
	m_pSelectCell = m_vecCells[iIndex];
}

void CNavigation::Reset_CellPoint()
{
	for (size_t i = 0; i < 3; i++)
	{
		Safe_Release(m_pCellPoint[i]);
	}

}





CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	for (auto& pCell : m_vecCells)
	{
		Safe_Release(pCell);
	}
	m_vecCells.clear();

}
