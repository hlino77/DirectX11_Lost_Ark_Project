#pragma once


#include "VIBuffer.h"
#include "Model.h"
#include "AsFileUtils.h"

/* 모델을 구성하는 하나의 메시. */
/* 이 메시를 구성하는 정점, 인덱스 버퍼를 보관한다. */

BEGIN(Engine)


class CMesh final : public CVIBuffer
{

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(class CModel* pModel, Matrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);	

public:
	HRESULT	LoadData_FromMeshFile(CModel::TYPE eModelType, CAsFileUtils* pFileUtils, Matrix PivotMatrix, _bool bColMesh);
	HRESULT	LoadData_FromConverter(CModel::TYPE eModelType, shared_ptr<asMesh> pMesh, Matrix PivotMatrix);


	VTXANIMMODEL* Get_Vertices() { return m_pVertices; }
	FACEINDICES32* Get_Indices() { return m_pIndices; }
private:
	//char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = 0;
private:
	/* 이 메시에 영향ㅇ르 주는 뼈의 갯수. */
	_uint							m_iNumBones = 0;

	/* 이 메시에 영향을 주는 뼈들을 모아놓느다. */
	/* why ? : 메시 별로 렌더링할 때 이 메시에 영향ㅇ르 주는 뼏르의 행렬을 모아서 셰{ㅇ디러ㅗ 도ㅓㄴ질려고. */

	_uint							m_iBoneIndex;
	wstring							m_szName;
	wstring							m_szMaterialName;

	VTXANIMMODEL*					m_pVertices = nullptr;
	FACEINDICES32*					m_pIndices = nullptr;
private:
	HRESULT Ready_Vertices(VTXANIMMODEL* pVertices, Matrix PivotMatrix);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType , class CModel* pModel, Matrix PivotMatrix);
	virtual void Free() override;
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
};

END