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
	_uint Get_NextAnim() { return m_tCurrChange.m_iNextAnim; }
	_uint Get_ReservedAnim() { return m_tReserveChange.m_iNextAnim; }

	_uint Get_MaxAnimIndex();

	Matrix Get_PivotMatrix() {
		return XMLoadFloat4x4(&m_PivotMatrix);
	}

	Matrix Get_CombinedMatrix(_uint iBoneIndex) {
		return m_CombinedMatrix[iBoneIndex];
	}


	_int	Find_AnimIndex(const wstring& szAnimName);
	_bool	Is_AnimationEnd(_uint iAnimation);
	_float	Get_Anim_MaxFrameRatio(_uint iAnimation);
	_uint	Get_Anim_Frame(_uint iAnimation);
	_uint	Get_Anim_MaxFrame(_uint iAnimation);
	void	Set_Anim_Speed(_uint iAnimation, _float fSpeed);

	_int	Find_BoneIndex(const wstring& szBoneName);
	Matrix  Get_CurrBoneMatrix(_uint iIndex);
	void	Set_CurrBoneMatrix(_uint iIndex, Matrix matValue);
	Matrix	Get_BoneOffsetMatrix(_uint iIndex);

	_int	Initailize_FindAnimation(const wstring& szAnimName, _float fSpeed);

	_int	FindMaterialIndexByName(string strMaterialName);
	_bool	Is_HairTexture(_uint iMaterialIndex);

	_bool	IsNext() { return m_bNext; }
	vector<class CMesh*>& Get_Meshes() { return m_Meshes; }
public:
	virtual HRESULT Initialize_Prototype(Matrix PivotMatrix, const wstring& strFilePath, const wstring& strFileName, _bool bClient, _bool bColMesh);
	virtual HRESULT Initialize(void* pArg);


public:
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* strConstantName);
	HRESULT SetUpAnimation_OnShader(class CShader* pShader);

	HRESULT	Reserve_NextAnimation(_int iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bReverse = false);
	HRESULT Set_NextAnimation();

	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Play_Proceed_Animation(_float fTimeDelta);
	HRESULT Play_Reverse_Animation(_float fTimeDelta);

	HRESULT Set_Animation_Transforms();
	HRESULT Set_AnimationBlend_Transforms();

	HRESULT Render(class CShader*& pShader, const _uint& iMeshIndex, const _uint iPassIndex = 0);
	HRESULT Render(class CShader*& pShader, const _uint& iMeshIndex, const string& strPassName);
	HRESULT Render(class CShader*& pShader);
	HRESULT Render_SingleMesh(class CShader*& pShader, const _int& iMeshIndex);

	//Instancing
	HRESULT Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, class CShader* pShader, _uint iMeshIndex, _uint iStride, _uint iPassIndex = 0);
	HRESULT Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, class CShader* pShader, _uint iMeshIndex, _uint iStride, const string& strPassName);
	HRESULT Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, class CShader*& pShader, _uint iStride);
	HRESULT Render_SingleMeshInstance(ID3D11Buffer* pInstanceBuffer, _uint iSize, class CShader*& pShader, const _int& iMeshIndex, _uint iStride);

	HRESULT Load_AssetFile_FromBinary(const wstring& pFilePath, const wstring& pFileName, _bool bClient, _bool bColMesh);

public: /* ;hj가 추가한 함수 */
	string  Get_Material_Name(_uint iMaterialIndex) { return m_Materials[iMaterialIndex].strName; }

	void	Set_RootDist(_float fDist) { m_fRootDist = fDist; }
	HRESULT	Set_ToRootPos(class CTransform* pTransform, _float fTimeDelta, Vec4 TargetPos = XMVectorZero());


private:
	HRESULT Load_ModelData_FromFile(Matrix PivotMatrix, _bool bClient, _bool bColMesh);
	HRESULT Load_MaterialData_FromFile();
	HRESULT Load_AnimationData_FromFile(Matrix PivotMatrix, _bool bClient);

	void	Change_NextAnimation();


	class CTexture*	Create_Texture(const wstring& szFullPath);
private:
	Matrix						m_CombinedMatrix[500];
	Matrix						m_PivotMatrix;
	TYPE						m_eModelType = TYPE_END;

private:
	/* HJ 추가 */
	Vec3						m_vRootPos;
	Vec3						m_vPreRootPos;
	Vec3						m_vCurRootPos;

	_float						m_fRootDist;
	_bool						m_bRootY = { false };
	_bool						m_bReverse = { false };

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