#pragma once

#include "Component.h"
#include "AsTypes.h"
#include "Lock.h"


BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	typedef struct ChangeAnimation
	{
		_uint	m_iNextAnim;
		_float	m_fChangeTime = 0.0f, m_fSumTime = 0.0f, m_fChangeRatio = 0.0f;
		_uint	m_iChangeFrame = 0, m_iNextAnimFrame = 0;
	}CHANGEANIM;

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

	
public:

	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_uint Get_MaterialIndex(_uint iMeshIndex);

	void Set_CurrAnim(_int iCurrAnim);
	_uint Get_CurrAnim() { return m_iCurrAnim; }

	_uint Get_MaxAnimIndex();

	Matrix Get_PivotMatrix() {
		return XMLoadFloat4x4(&m_PivotMatrix);
	}


	_int	Find_AnimIndex(const wstring& szAnimName);
	_bool	Is_AnimationEnd(_uint iAnimation);
	_float	Get_Anim_MaxFrameRatio(_uint iAnimation);
	_uint	Get_Anim_Frame(_uint iAnimation);
	_uint	Get_Anim_MaxFrame(_uint iAnimation);


	_int	Find_BoneIndex(const wstring& szBoneName);
	Matrix  Get_CurrBoneMatrix(_uint iIndex);

	_int	Initailize_FindAnimation(const wstring& szAnimName, _float fSpeed);


	vector<class CMesh*>& Get_Meshes() { return m_Meshes; }
public:
	virtual HRESULT Initialize_Prototype(Matrix PivotMatrix, const wstring& strFilePath, const wstring& strFileName, _bool bClient, _bool bColMesh);
	virtual HRESULT Initialize(void* pArg);


public:
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* strConstantName);
	HRESULT SetUpAnimation_OnShader(class CShader* pShader);
	/* 애니메이션을 재생한다. */
	/* 1. 해당 애니메이션에서 사용하는 모든 뼈들의  Transformation 행렬을 갱신한다. */
	/* 2. Transformation를 최상위 부모로부터 자식으로 계속 누적시켜간다.(CombinedTransformation) */
	/* 3. 애니메이션에 의해 움직인 뼈들의 CombinedTransfromation을 셋팅한다. */

	HRESULT	Reserve_NextAnimation(_int iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	HRESULT Set_NextAnimation();
	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Set_Animation_Transforms();
	HRESULT Set_AnimationBlend_Transforms();
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	HRESULT Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	HRESULT Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, class CShader* pShader, _uint iMeshIndex, _uint iStride, _uint iPassIndex = 0);

	HRESULT Load_AssetFile_FromBinary(const wstring& pFilePath, const wstring& pFileName, _bool bClient, _bool bColMesh);



private:
	HRESULT Load_ModelData_FromFile(Matrix PivotMatrix, _bool bClient, _bool bColMesh);
	HRESULT Load_MaterialData_FromFile();
	HRESULT Load_AnimationData_FromFile(Matrix PivotMatrix, _bool bClient);


	void	Change_NextAnimation();


	class CTexture*	Create_Texture(const wstring& szFullPath);
private:

	Matrix						m_PivotMatrix;
	TYPE						m_eModelType = TYPE_END;

private:
	_uint							m_iNumMeshes = 0;
	vector<class CMesh*>			m_Meshes;

private:
	_uint									m_iNumMaterials = 0;
	vector<MATERIALDESC>					m_Materials;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;



private:
	_int								m_iCurrAnim = 0;
	_bool								m_bNext = false, m_bReserved = false;

	uint64								m_iNumAnimations = 0;
	vector<class CAnimation*>			m_Animations;

	//Animation
	vector<Matrix>						m_matCurrTransforms;
	vector<ModelKeyframeData>			m_CurrKeyFrameDatas;
	vector<ModelKeyframeData>			m_PrevKeyFrameDatas;


	CHANGEANIM							m_tCurrChange, m_tReserveChange;

	//Model Load
	wstring								m_strFilePath;
	wstring								m_strFileName;


	vector<shared_ptr<ModelBone>>		m_ModelBones;
	shared_ptr<ModelBone>				m_RootBone;

	_bool								m_bClient = true;
private:
	USE_LOCK

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const wstring& strFilePath, const wstring& strFileName, _bool bClient, _bool bColMesh, Matrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(CGameObject* pObject, void* pArg = nullptr);
	virtual void Free() override;
};

END