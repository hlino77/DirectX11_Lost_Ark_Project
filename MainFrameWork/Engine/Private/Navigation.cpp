#include "Navigation.h"
#include "Engine_Defines.h"
#include "DebugDraw.h"
#include "Cell.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include <filesystem>
#include "GameObject.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
{
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

	if (!m_Cells.empty())
	{
		for (auto& Cell : m_Cells)
		{
			vector<Vec3> Points = Cell->Get_OriginPoints();

			Points[0].y += 0.01f;
			Points[1].y += 0.01f;
			Points[2].y += 0.01f;

			DX::DrawTriangle(m_pBatch, Points[0], Points[1], Points[2], Colors::Green);
		}
	}

	

	m_pBatch->End();


    return S_OK;
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



void CNavigation::Find_FirstCell(CGameObject* pObject)
{
	Vec3 vPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	vPos.y += 1.0f;

	Vec3 vDir(0.0f, -1.0f, 0.0f);


	for (auto& Cell : m_Cells)
	{
		if (Cell->Intersects(vPos, vDir))
		{
			pObject->Set_CurrCell(Cell->Get_CellIndex());
			return;
		}
	}

	pObject->Set_CurrCell(0);
}

_int CNavigation::Check_Pos_InCell(Vec3 vPos)
{
	vPos.y += 1.0f;

	Vec3 vDir(0.0f, -1.0f, 0.0f);


	for (auto& Cell : m_Cells)
	{
		if (Cell->Intersects(vPos, vDir))
		{
			return Cell->Get_CellIndex();
		}
	}

	return -1;
}



void CNavigation::Load_Navigation(const wstring& szFileName)
{
	wstring szFullPath = L"../Bin/Resources/Navigation/";
	szFullPath += szFileName;

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

	_uint iPointSize = file->Read<_uint>();

	vector<Vec3> PointPositions;

	for (_uint i = 0; i < iPointSize; ++i)
		PointPositions.push_back(file->Read<Vec3>());



	_uint iCellSize = file->Read<_uint>();

	for (_uint i = 0; i < iCellSize; ++i)
	{
		_uint iIndex = file->Read<_uint>();

		CCell* pCell = CCell::Create(this, iIndex);


		for (_uint i = 0; i < 3; ++i)
		{
			_uint iPointIndex = file->Read<_uint>();
			pCell->Add_Point(PointPositions[iPointIndex]);
		}

		for (_uint i = 0; i < CCell::LINE_END; ++i)
			pCell->Set_Neighbor(i, file->Read<_int>());

		pCell->Set_Normal();

		m_Cells.push_back(pCell);
	}

	

}

void CNavigation::SetUp_OnCell(CGameObject* pObject)
{
	_int iCurrCell = pObject->Get_CurrCell();

	if (iCurrCell < 0 || iCurrCell >= m_Cells.size())
		return;

	m_Cells[pObject->Get_CurrCell()]->SetUp_OnCell(pObject, 0);
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
}
