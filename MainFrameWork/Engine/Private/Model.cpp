#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Animation.h"
#include "Shader.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include <filesystem>
#include "tinyxml2.h"
#include "GameInstance.h"
#include <iostream>
#include "BindShaderDesc.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
	, m_eModelType(rhs.m_eModelType)
	/*, m_HierarchyNodes(rhs.m_HierarchyNodes)*/
	, m_iCurrAnim(rhs.m_iCurrAnim)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_ModelBones(rhs.m_ModelBones)
{
	for (auto& pMeshContainer : m_Meshes)
		Safe_AddRef(pMeshContainer);


	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}

	for (auto& Animation : rhs.m_Animations)
		m_Animations.push_back(Animation->Clone(this));
	//for (auto& pAnimation : m_Animations)
	//	Safe_AddRef(pAnimation.second);
	
}


_uint CModel::Get_MaterialIndex(_uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Get_MaterialIndex();
}

void CModel::Set_CurrAnim(_int iCurrAnim)
{
	m_Animations[m_iCurrAnim]->Reset_Animation();
	m_Animations[iCurrAnim]->Reset_Animation();
	m_iCurrAnim = iCurrAnim;
	
	m_bNext = false;
	m_bReserved = false;
	ZeroMemory(&m_tCurrChange, sizeof(CHANGEANIM));
	ZeroMemory(&m_tReserveChange, sizeof(CHANGEANIM));

	m_vPreRootPos = { 0.f, 0.f, 0.f };
	m_vCurRootPos = { 0.f, 0.f, 0.f };
}

_uint CModel::Get_MaxAnimIndex()
{
	return (_uint)(m_Animations.size() - 1);
}

_int CModel::Find_AnimIndex(const wstring& szAnimName)
{
	_int iIndex = 0;

	for (auto& Animation : m_Animations)
	{
		if (Animation->Get_Name() == szAnimName)
			return iIndex;
		++iIndex;
	}

	return -1;
}

_bool CModel::Is_AnimationEnd(_uint iAnimation)
{
	if (m_iCurrAnim != iAnimation)
		return false;

	_bool bEnd = m_Animations[m_iCurrAnim]->Is_End();

	return bEnd;
}

_float CModel::Get_Anim_MaxFrameRatio(_uint iAnimation)
{
	return m_Animations[iAnimation]->Get_MaxFrameRatio();
}

_uint CModel::Get_Anim_Frame(_uint iAnimation)
{
	return m_Animations[iAnimation]->Get_Frame();
}



void CModel::Set_Anim_Speed(_uint iAnimation, _float fSpeed)
{
	m_Animations[iAnimation]->Set_Speed(fSpeed);
}

_int CModel::Find_BoneIndex(const wstring& szBoneName)
{
	_int iIndex = 0;

	for (auto& Bone : m_ModelBones)
	{
		if (Bone->strName == szBoneName)
			return iIndex;
		++iIndex;
	}

	return -1;
}

Matrix CModel::Get_CurrBoneMatrix(_uint iIndex)
{
	return m_matCurrTransforms[iIndex];
}

void CModel::Set_CurrBoneMatrix(_uint iIndex, Matrix matValue)
{
	m_matCurrTransforms[iIndex] = matValue;
}

Matrix CModel::Get_BoneOffsetMatrix(_uint iIndex)
{
	return m_ModelBones[iIndex]->matTransform;
}

_int CModel::Initailize_FindAnimation(const wstring& szAnimName, _float fSpeed)
{
	_uint iAnimationIndex = Find_AnimIndex(szAnimName);

	if (iAnimationIndex == -1)
		return -1;

	m_Animations[iAnimationIndex]->Set_Speed(fSpeed);
	return iAnimationIndex;
}

HRESULT CModel::Initialize_Prototype(Matrix PivotMatrix, const wstring& strFilePath, const wstring& strFileName, _bool bClient, _bool IsMapObject)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	m_IsMapObject = IsMapObject;


	if (FAILED(Load_AssetFile_FromBinary(strFilePath, strFileName, bClient, IsMapObject)))
		return E_FAIL;



	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	if (m_eModelType == TYPE::TYPE_ANIM)
	{
		Matrix matIdentity = Matrix::Identity;
		m_matCurrTransforms.resize(m_ModelBones.size(), matIdentity);

		ModelKeyframeData KeyData;
		ZeroMemory(&KeyData, sizeof(ModelKeyframeData));
		m_CurrKeyFrameDatas.resize(m_ModelBones.size(), KeyData);
		m_PrevKeyFrameDatas.resize(m_ModelBones.size(), KeyData);
	}
		
	if (nullptr != pArg)
	{
		CHANGECOLOR* pColorDesc = (CHANGECOLOR*)pArg;

		for (size_t i = 0; i < 20; i++)
		{
			m_vEm_Color = pColorDesc->vEm_Color;
			m_vColor_R = pColorDesc->vColor_R;
			m_vColor_G = pColorDesc->vColor_G;
			m_vColor_B = pColorDesc->vColor_B;
			m_bChangeColor = true;
		}
	}

	return S_OK;
}

HRESULT CModel::SetUp_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* strConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (m_Materials[iMaterialIndex].pTexture[eTextureType] == nullptr)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Set_SRV(pShader, strConstantName);
}

HRESULT CModel::SetUpAnimation_OnShader(CShader* pShader)
{
	if (FAILED(pShader->Bind_Matrices("g_BoneMatrices", m_matCurrTransforms.data(), (size_t)m_matCurrTransforms.size())))
		return S_OK;

	return S_OK;
}

HRESULT CModel::Reserve_NextAnimation(_int iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY, _bool bIgnoreRoot)
{
	WRITE_LOCK

	if (iAnimIndex >= m_iNumAnimations || iAnimIndex < 0)
		return E_FAIL;

	m_bReverse = bReverse;

	if (false == m_bReverse)
	{
		if (-1 == iStartFrame)
			iStartFrame = m_Animations[iAnimIndex]->Get_MaxFrame();
		if (-1 == iChangeFrame)
			iChangeFrame = m_Animations[iAnimIndex]->Get_MaxFrame();

		m_Animations[iAnimIndex]->Reset_Animation();
		m_tReserveChange.m_fSumTime = 0.0f;
		m_tReserveChange.m_iNextAnim = iAnimIndex;
		m_tReserveChange.m_iNextAnimFrame = iStartFrame;
		m_tReserveChange.m_iChangeFrame = iChangeFrame;
		m_tReserveChange.m_fChangeTime = fChangeTime;
		m_tReserveChange.m_fChangeRatio = 0.0f;
	}
	else if (true == m_bReverse)
	{
		if (-1 == iStartFrame)
			iStartFrame = m_Animations[iAnimIndex]->Get_MaxFrame();
		if (-1 == iChangeFrame)
			iChangeFrame = m_Animations[iAnimIndex]->Get_MaxFrame();

		m_Animations[iAnimIndex]->Reset_Reverse_Animation();
		m_tReserveChange.m_fSumTime = 0.0f;
		m_tReserveChange.m_iNextAnim = iAnimIndex;
		m_tReserveChange.m_iNextAnimFrame = iStartFrame;
		m_tReserveChange.m_iChangeFrame = iChangeFrame;
		m_tReserveChange.m_fChangeTime = fChangeTime;
		m_tReserveChange.m_fChangeRatio = 0.0f;
	}
	
	m_bIgnoreRoot = bIgnoreRoot;
	m_fRootDist = fRootDist;
	m_bUseRootY = bUseY;
	m_IsPreRootRot = m_bRootRotation;
	m_bRootRotation = bRootRot;
	if (true == m_bRootRotation)
	{
		m_OriginMatrix = XMMatrixIdentity();
	}

	m_bReserved = true;
	return S_OK;
}

HRESULT CModel::Set_NextAnimation()
{
	WRITE_LOCK

	m_PrevKeyFrameDatas = m_CurrKeyFrameDatas;

	m_bNext = true;
	m_tCurrChange = m_tReserveChange;
	ZeroMemory(&m_tReserveChange, sizeof(CHANGEANIM));

	if (false == m_bReverse)
		m_Animations[m_tCurrChange.m_iNextAnim]->Set_Frame(m_tCurrChange.m_iNextAnimFrame);
	else if (true == m_bReverse)
		m_Animations[m_tCurrChange.m_iNextAnim]->Set_Reverse_Frame(m_tCurrChange.m_iNextAnimFrame);

	m_bReserved = false;

	return S_OK;
}

HRESULT CModel::Play_Animation(_float fTimeDelta)
{
	if (false == m_bReverse)
	{
		if (FAILED(Play_Proceed_Animation(fTimeDelta)))
			return E_FAIL;
	}
	else if (true == m_bReverse)
	{
		if (FAILED(Play_Reverse_Animation(fTimeDelta)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Play_Proceed_Animation(_float fTimeDelta)
{
	if (m_bReserved)
	{
		if (m_Animations[m_iCurrAnim]->Get_Frame() >= m_tReserveChange.m_iChangeFrame)
			Set_NextAnimation();
	}

	if (m_bNext)
	{
		m_tCurrChange.m_fSumTime += fTimeDelta;
		if (m_tCurrChange.m_fSumTime >= m_tCurrChange.m_fChangeTime)
			Change_NextAnimation();
		else
		{
			m_Animations[m_tCurrChange.m_iNextAnim]->Play_Animation(fTimeDelta);
			m_tCurrChange.m_fChangeRatio = m_tCurrChange.m_fSumTime / m_tCurrChange.m_fChangeTime;
		}
	}
	else
	{
		if (true == m_Animations[m_iCurrAnim]->Is_End())
		{
			m_Animations[m_iCurrAnim]->Reset_End();
			m_vPreRootPos = { 0.f, 0.f, 0.f };
			m_vCurRootPos = { 0.f, 0.f, 0.f };
		}

		m_Animations[m_iCurrAnim]->Play_Animation(fTimeDelta);
	}

	if (m_bNext)
		Set_AnimationBlend_Transforms();
	else
		Set_Animation_Transforms();

	m_vPreRootPos = m_vCurRootPos;
	m_vCurRootPos = m_vRootPos;
	if (0 == m_vPreRootPos.z)
		m_vPreRootPos = m_vCurRootPos;

	return S_OK;
}

HRESULT CModel::Play_Reverse_Animation(_float fTimeDelta)
{
	if (m_bReserved)
	{
		if (m_Animations[m_iCurrAnim]->Get_Frame() >= m_tReserveChange.m_iChangeFrame)
			Set_NextAnimation();
	}

	if (m_bNext)
	{
		m_tCurrChange.m_fSumTime += fTimeDelta;
		if (m_tCurrChange.m_fSumTime >= m_tCurrChange.m_fChangeTime)
			Change_NextAnimation();
		else
		{
			m_Animations[m_tCurrChange.m_iNextAnim]->Play_Reverse_Animation(fTimeDelta);
			m_tCurrChange.m_fChangeRatio = m_tCurrChange.m_fSumTime / m_tCurrChange.m_fChangeTime;
		}
	}
	else
	{
		if (true == m_Animations[m_iCurrAnim]->Is_End())
		{
			m_Animations[m_iCurrAnim]->Reset_End();
			m_vPreRootPos = { 0.f, 0.f, 0.f };
			m_vCurRootPos = { 0.f, 0.f, 0.f };
		}

		m_Animations[m_iCurrAnim]->Play_Reverse_Animation(fTimeDelta);
	}

	if (m_bNext)
		Set_AnimationBlend_Transforms();
	else
		Set_Animation_Transforms();

	m_vPreRootPos = m_vCurRootPos;
	m_vCurRootPos = m_vRootPos;
	if (0 == m_vPreRootPos.z)
		m_vPreRootPos = m_vCurRootPos;

	return S_OK;
}

HRESULT CModel::Set_ToRootPos(CTransform* pTransform)
{
	if (nullptr == pTransform)
		return E_FAIL;
	if (true == m_bIgnoreRoot)
		return S_OK;

	KEY_DESC tCurrKeyDesc = m_Animations[m_iCurrAnim]->Get_KeyDesc();
	if (true == m_bNext || 0 == tCurrKeyDesc.iNextFrame || 0.f == m_fRootDist)
	{
		m_vPreRootPos = { 0.f, 0.f, 0.f };
		m_vCurRootPos = { 0.f, 0.f, 0.f };
		return S_OK;
	}

	Vec3 vPos = pTransform->Get_State(CTransform::STATE_POSITION);
	Vec3 vLocalDir = m_vCurRootPos - m_vPreRootPos;
	Vec3 vLocalPivotDir = XMVector3TransformNormal(vLocalDir, m_PivotMatrix);
	Vec3 vWorldDir = XMVector3TransformNormal(vLocalPivotDir, pTransform->Get_WorldMatrix());
	_float fDist = vWorldDir.Length();
	vWorldDir.Normalize();

	Vec3 vCalculePos = vPos;

	vCalculePos += vWorldDir * fDist * m_fRootDist;
	if (false == m_bUseRootY)
		vCalculePos.y = vPos.y;

	if (false == m_bRootRotation)
	{
		pTransform->Set_State(CTransform::STATE_POSITION, vCalculePos);
	}
	else
	{
		if (m_OriginMatrix == XMMatrixIdentity())
		{
			m_OriginMatrix = pTransform->Get_WorldMatrix();
		}

		Matrix RootWorld = pTransform->Q_Rotation(m_RootRotation, m_OriginMatrix);
		pTransform->Set_WorldMatrix(RootWorld);

		pTransform->Set_State(CTransform::STATE_POSITION, vCalculePos);
	}
		
	return S_OK;
}

HRESULT CModel::Bind_ChangeColor(CShader* pShader, _uint iMeshIndex)
{
	if (false == m_bChangeColor)
		return E_FAIL;

	pShader->Bind_RawValue("g_vBloomColor", &m_vEm_Color, sizeof(Vec4));
	pShader->Bind_RawValue("g_vColor_R", &m_vColor_R, sizeof(Vec4));
	pShader->Bind_RawValue("g_vColor_G", &m_vColor_G, sizeof(Vec4));
	pShader->Bind_RawValue("g_vColor_B", &m_vColor_B, sizeof(Vec4));

	return S_OK;
}

_int CModel::Is_HairTexture()
{
	for (size_t i = 0; i < m_Materials.size(); i++)
	{
		string strMaterialName = Get_Material_Name(i);

		string search_str = "hair";
		size_t hair_pos = strMaterialName.find(search_str);

		if (hair_pos != string::npos)
			return (_int)i;
	}

	return -1;
}

HRESULT CModel::Set_Animation_Transforms()
{
	KEY_DESC tCurrKeyDesc = m_Animations[m_iCurrAnim]->Get_KeyDesc();
	vector<ModelKeyframeData>& CurrKeyDatas = m_Animations[m_iCurrAnim]->Get_KeyDatas(tCurrKeyDesc.iCurrFrame);
	vector<ModelKeyframeData>& NextKeyDatas = m_Animations[m_iCurrAnim]->Get_KeyDatas(tCurrKeyDesc.iNextFrame);
	_float fRatio = tCurrKeyDesc.fRatio;

	Matrix matResult;

	for (_uint i = 0; i < m_ModelBones.size(); ++i)
	{
		m_CurrKeyFrameDatas[i].vTranslation = Vec3::Lerp(CurrKeyDatas[i].vTranslation, NextKeyDatas[i].vTranslation, fRatio);
		m_CurrKeyFrameDatas[i].vScale = Vec3::Lerp(CurrKeyDatas[i].vScale, NextKeyDatas[i].vScale, fRatio);
		m_CurrKeyFrameDatas[i].vRotation = Quaternion::Slerp(CurrKeyDatas[i].vRotation, NextKeyDatas[i].vRotation, fRatio);


		matResult = XMMatrixAffineTransformation(XMLoadFloat3(&m_CurrKeyFrameDatas[i].vScale),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMLoadFloat4(&m_CurrKeyFrameDatas[i].vRotation),
			XMVectorSetW(XMLoadFloat3(&m_CurrKeyFrameDatas[i].vTranslation), 1.f));


		int32 iParentIndex = m_ModelBones[i]->iParentID;

		if (iParentIndex < 0)
			m_matCurrTransforms[i] = matResult;
		else
			m_matCurrTransforms[i] = m_CombinedMatrix[i] = matResult * m_matCurrTransforms[iParentIndex];


		if (true == m_bActivateRoot && TEXT("b_root") == m_ModelBones[i]->strName)
		{
			if (false == m_bIgnoreRoot)
			{
				if (false == m_bRootRotation)
				{
					// �̵� ����
					memcpy(&m_vRootPos, &m_matCurrTransforms[i].m[3], sizeof(Vec4));
					Vec4 Zero;
					if (true == m_bUseRootY)
					{
						Zero = Vec4(0.f, 0.f, 0.f, 1.f);
					}
					else
					{
						Zero = Vec4(0.f, m_vRootPos.y, 0.f, 1.f);
					}
					memcpy(&m_matCurrTransforms[i].m[3], &Zero, sizeof(Vec4));
				}
				else if (true == m_bRootRotation)
				{
					// �̵�, ȸ�� ����
					memcpy(&m_vRootPos, &m_matCurrTransforms[i].m[3], sizeof(Vec4));
					m_RootMatrix = m_matCurrTransforms[i];

					Vec3 vScale, vPos;
					m_matCurrTransforms[i].Decompose(vScale, m_RootRotation, vPos);
					Quaternion rotationQuaternion = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));
					m_RootRotation = m_RootRotation * rotationQuaternion;

					m_matCurrTransforms[i] = XMMatrixScalingFromVector(vScale) * XMMatrixRotationY(XMConvertToRadians(-90.0f));

					Vec4 Zero;
					if (true == m_bUseRootY)
					{
						Zero = Vec4(0.f, 0.f, 0.f, 1.f);
					}
					else
					{
						Zero = Vec4(0.f, m_vRootPos.y, 0.f, 1.f);
					}
					memcpy(&m_matCurrTransforms[i].m[3], &Zero, sizeof(Vec4));
				}
			}
			else
			{
				// ��Ʈ ����
				Vec4 Zero = Vec4(0.f, 0.f, 0.f, 1.f);
				memcpy(&m_matCurrTransforms[i].m[3], &Zero, sizeof(Vec4));
			}
		}

	}

	for (_uint i = 0; i < m_ModelBones.size(); ++i)
		m_matCurrTransforms[i] = m_ModelBones[i]->matTransform * m_matCurrTransforms[i] * m_PivotMatrix;

	return S_OK;
}

HRESULT CModel::Set_AnimationBlend_Transforms()
{
	KEY_DESC tCurrKeyDesc = m_Animations[m_tCurrChange.m_iNextAnim]->Get_KeyDesc();
	vector<ModelKeyframeData>& CurrKeyDatas = m_PrevKeyFrameDatas;
	vector<ModelKeyframeData>& NextKeyDatas = m_Animations[m_tCurrChange.m_iNextAnim]->Get_KeyDatas(tCurrKeyDesc.iCurrFrame);
	_float fRatio = m_tCurrChange.m_fChangeRatio;

	Matrix matResult;

	for (_uint i = 0; i < m_ModelBones.size(); ++i)
	{
		m_CurrKeyFrameDatas[i].vTranslation = Vec3::Lerp(CurrKeyDatas[i].vTranslation, NextKeyDatas[i].vTranslation, fRatio);
		m_CurrKeyFrameDatas[i].vScale = Vec3::Lerp(CurrKeyDatas[i].vScale, NextKeyDatas[i].vScale, fRatio);
		m_CurrKeyFrameDatas[i].vRotation = Quaternion::Slerp(CurrKeyDatas[i].vRotation, NextKeyDatas[i].vRotation, fRatio);

		if (true == m_IsPreRootRot && TEXT("b_root") == m_ModelBones[i]->strName)
		{
			m_CurrKeyFrameDatas[i].vRotation = NextKeyDatas[i].vRotation;
		}
		
		matResult = XMMatrixAffineTransformation(XMLoadFloat3(&m_CurrKeyFrameDatas[i].vScale),
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMLoadFloat4(&m_CurrKeyFrameDatas[i].vRotation),
			XMVectorSetW(XMLoadFloat3(&m_CurrKeyFrameDatas[i].vTranslation), 1.f));

		int32 iParentIndex = m_ModelBones[i]->iParentID;

		if (iParentIndex < 0)
			m_matCurrTransforms[i] = matResult;
		else
			m_matCurrTransforms[i] = m_CombinedMatrix[i] = matResult * m_matCurrTransforms[iParentIndex];
	}

	for (_uint i = 0; i < m_ModelBones.size(); ++i)
		m_matCurrTransforms[i] = m_ModelBones[i]->matTransform * m_matCurrTransforms[i] * m_PivotMatrix;

	return S_OK;
}

HRESULT CModel::Render(CShader*& pShader, const _uint& iMeshIndex, const _uint iPassIndex)
{
	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	if (FAILED(m_Meshes[iMeshIndex]->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render(CShader*& pShader, const _uint& iMeshIndex, const string& strPassName)
{
	if (FAILED(pShader->Begin(strPassName)))
		return E_FAIL;

	if (FAILED(m_Meshes[iMeshIndex]->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::Render(CShader*& pShader, const string& strPassName)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (FAILED(Render_AlpahBlendSingleMesh(pShader, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Render(CShader*& pShader)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (FAILED(Render_SingleMesh(pShader, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Render_Outline(CShader*& pShader, _bool bToon)
{
	if (true == bToon)
	{
		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			if (FAILED(Render_ToonMesh(pShader, i)))
				return E_FAIL;
		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			if (FAILED(Render_OutlineMesh(pShader, i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CModel::Render_Inline(CShader*& pShader)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (FAILED(Render_InlineMesh(pShader, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Render_Alpha(CShader*& pShader, const _int& iMeshIndex)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
	{
		if (FAILED(Render(pShader, iMeshIndex, "Diffuse")))
			return E_FAIL;

		return S_OK;
	}

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render(pShader, iMeshIndex, "Alphablend")))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render_ToonMesh(CShader*& pShader, const _int& iMeshIndex)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
	{
		if (FAILED(Render(pShader, iMeshIndex, "Diffuse")))
			return E_FAIL;

		return S_OK;
	}

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render(pShader, iMeshIndex, "Toon")))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render_InlineMesh(CShader*& pShader, const _int& iMeshIndex)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
	{
		if (FAILED(Render(pShader, iMeshIndex, "Diffuse")))
			return E_FAIL;

		return S_OK;
	}

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render(pShader, iMeshIndex, "Inline")))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render_OutlineMesh(CShader*& pShader, const _int& iMeshIndex)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
	{
		if (FAILED(Render(pShader, iMeshIndex, "Diffuse")))
			return E_FAIL;

		return S_OK;
	}

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render(pShader, iMeshIndex, "Outline")))
		return E_FAIL;



	return S_OK;
}

HRESULT CModel::Render_SingleMesh(CShader*& pShader, const _int& iMeshIndex)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
	{
		if (FAILED(Render(pShader, iMeshIndex, "Diffuse")))
			return E_FAIL;

		return S_OK;
	}

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (true == m_bChangeColor)
	{
		if (FAILED(Bind_ChangeColor(pShader, iMeshIndex)))
			return E_FAIL;

		if (FAILED(Render(pShader, iMeshIndex, "ChangeColor")))
			return E_FAIL;

		pShader->Bind_RawValue("g_vBloomColor", &Vec4::One, sizeof(Vec4));
	}
	else
	{
		if (FAILED(Render(pShader, iMeshIndex, "PBR")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Render_AlpahBlendSingleMesh(CShader*& pShader, const _int& iMeshIndex)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
	{
		if (FAILED(Render(pShader, iMeshIndex, "Diffuse")))
			return E_FAIL;

		return S_OK;
	}

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (true == m_bChangeColor)
	{
		if (FAILED(Bind_ChangeColor(pShader, iMeshIndex)))
			return E_FAIL;

		if (FAILED(Render(pShader, iMeshIndex, "ChangeColor")))
			return E_FAIL;

		pShader->Bind_RawValue("g_vBloomColor", &Vec4::One, sizeof(Vec4));
	}
	else
	{
		if (FAILED(Render(pShader, iMeshIndex, "Alpha")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader*& pShader, _uint iStride)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
		Render_SingleMeshInstance(pInstanceBuffer, iSize, pShader, i, iStride);

	return S_OK;
}

HRESULT CModel::Render_GrassInstance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader*& pShader, _uint iStride)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
		Render_SingleMeshGrassInstance(pInstanceBuffer, iSize, pShader, i, iStride);

	return S_OK;
}

HRESULT CModel::Render_SingleMeshInstance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader*& pShader, const _int& iMeshIndex, _uint iStride)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
		return E_FAIL;

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render_Instance(pInstanceBuffer, iSize, pShader, iMeshIndex, iStride, "PBRInstance")))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render_SingleMeshGrassInstance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader*& pShader, const _int& iMeshIndex, _uint iStride)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
		return E_FAIL;

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render_Instance(pInstanceBuffer, iSize, pShader, iMeshIndex, iStride, "GrassInstance")))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render_AlpahBlendInstance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader*& pShader, _uint iStride)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
		Render_AlphaBlendSingleMeshInstance(pInstanceBuffer, iSize, pShader, i, iStride);

	return S_OK;
}


HRESULT CModel::Render_AlphaBlendSingleMeshInstance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader*& pShader, const _int& iMeshIndex, _uint iStride)
{
	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_NORMALS, "g_NormalTexture")))
		return E_FAIL;

	MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_SPECULAR, "g_SpecularTexture")))
		tFlag.SpecMaskEmisExtr.x = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_DIFFUSE_ROUGHNESS, "g_MRMaskTexture")))
		tFlag.SpecMaskEmisExtr.y = 1.f;

	if (SUCCEEDED(SetUp_OnShader(pShader, Get_MaterialIndex(iMeshIndex), aiTextureType_EMISSIVE, "g_EmissiveTexture")))
		tFlag.SpecMaskEmisExtr.z = 1.f;

	if (FAILED(pShader->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
		return E_FAIL;

	if (FAILED(Render_Instance(pInstanceBuffer, iSize, pShader, iMeshIndex, iStride, "Alpha")))
		return E_FAIL;

	return S_OK;
}


HRESULT CModel::Load_AssetFile_FromBinary(const wstring& pFilePath, const wstring& pFileName, _bool bClient, _bool bIsMapObject)
{

	m_strFileName = pFileName;
	m_strFilePath = pFilePath;

	if (FAILED(Load_ModelData_FromFile(XMLoadFloat4x4(&m_PivotMatrix), bClient, bIsMapObject)))
	{
		return E_FAIL;
	}

	if (bClient)
	{
		if (m_strFilePath == L"../Bin/Resources/Export/Bern/" ||
			m_strFilePath == L"../Bin/Resources/Export/Chaos1/" ||
			m_strFilePath == L"../Bin/Resources/Export/Chaos2/" ||
			m_strFilePath == L"../Bin/Resources/Export/Chaos3/" ||
			m_strFilePath == L"../Bin/Resources/Export/Boss/" ||
			m_strFilePath == L"../Bin/Resources/Export/Lobby/" || 
			m_strFilePath == L"../Bin/Resources/Export/Nature/")
		{
			if (FAILED(Load_MapMaterialData_FromFile()))
				return E_FAIL;
		}
		else
		{
			if (FAILED(Load_MaterialData_FromFile()))
				return E_FAIL;
		}

	}

	
	if (m_eModelType == TYPE::TYPE_ANIM)
	{
		if (FAILED(Load_AnimationData_FromFile(XMLoadFloat4x4(&m_PivotMatrix), bClient)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CModel::Load_ModelData_FromFile(Matrix PivotMatrix, _bool bClient, _bool bIsMapObject)
{
	wstring strfullPath = m_strFilePath + m_strFileName + L"/" + m_strFileName + L".mesh";

	shared_ptr<CAsFileUtils> pFileUtils = make_shared<CAsFileUtils>();
	pFileUtils->Open(strfullPath, FileMode::Read);

	int32 iAnimationCount = pFileUtils->Read<int32>();

	if (iAnimationCount > 0)
		m_eModelType = TYPE::TYPE_ANIM;
	else
		m_eModelType = TYPE::TYPE_NONANIM;

	uint32 iBoneCount = pFileUtils->Read<uint32>();

	for (uint32 i = 0; i < iBoneCount; i++)
	{
		shared_ptr<ModelBone> tBone = make_shared<ModelBone>();
		tBone->iIndex = pFileUtils->Read<int32>();
		tBone->strName = CAsUtils::ToWString(pFileUtils->Read<string>());
		tBone->iParentID = pFileUtils->Read<int32>();
		tBone->matTransform = pFileUtils->Read<Matrix>();
		tBone->matTransform = tBone->matTransform.Invert();

		m_ModelBones.push_back(tBone);
	}

	if (bClient)
	{
		m_iNumMeshes = pFileUtils->Read<uint32>();

		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			CMesh* pMeshContainer = CMesh::Create(m_pDevice, m_pContext, m_eModelType, this, PivotMatrix);
			if (nullptr == pMeshContainer)
				return E_FAIL;

			pMeshContainer->LoadData_FromMeshFile(m_eModelType, pFileUtils.get(), PivotMatrix, bIsMapObject);

			m_Meshes.push_back(pMeshContainer);
		}
	}

	// Bone ���� ���� ä���
	if (m_RootBone == nullptr && m_ModelBones.size() > 0)
	{
		m_RootBone = m_ModelBones[0];

		for (const auto& Bone : m_ModelBones)
		{
			if (Bone->iParentID >= 0)
			{
				Bone->tParent = m_ModelBones[Bone->iParentID];
				Bone->tParent->tChildrens.push_back(Bone);
			}
			else
			{
				Bone->tParent = nullptr;
			}
		}
	}

	return S_OK;
} 

HRESULT CModel::Load_MaterialData_FromFile()
{
	wstring strFullPath = m_strFilePath + m_strFileName + L"/" + m_strFileName + L".xml";
	auto parentPath = filesystem::path(strFullPath).parent_path();

	tinyxml2::XMLDocument* Document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = Document->LoadFile(CAsUtils::ToString(strFullPath).c_str());

#ifdef _DEBUG
	assert(error == tinyxml2::XML_SUCCESS);
#else
#endif


	tinyxml2::XMLElement* Root = Document->FirstChildElement();
	tinyxml2::XMLElement* MaterialNode = Root->FirstChildElement();

	while (MaterialNode)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		tinyxml2::XMLElement* Node = nullptr;

		Node = MaterialNode->FirstChildElement();

		strncpy_s(MaterialDesc.strName, Node->GetText(), strlen(Node->GetText()));

		// Diffuse Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				wstring szFullPath = L"";

				if(m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;


				MaterialDesc.pTexture[aiTextureType_DIFFUSE] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_DIFFUSE])
					return E_FAIL;

			}
		}

		// Specular Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;

				MaterialDesc.pTexture[aiTextureType_SPECULAR] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_SPECULAR])
					return E_FAIL;
			}
		}

		// Normal Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;

				MaterialDesc.pTexture[aiTextureType_NORMALS] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_NORMALS])
					return E_FAIL;
			}
		}

		// Emissive Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;

				MaterialDesc.pTexture[aiTextureType_EMISSIVE] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_EMISSIVE])
					return E_FAIL;
			}
		}

		// Metalic Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;

				MaterialDesc.pTexture[aiTextureType_METALNESS] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_METALNESS])
					return E_FAIL;
			}
		}

		// Roughness Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = m_strFilePath + m_strFileName + L"/" + strTexture;

				MaterialDesc.pTexture[aiTextureType_DIFFUSE_ROUGHNESS] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_DIFFUSE_ROUGHNESS])
					return E_FAIL;
			}
		}

		// Ambient
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vAmbient.x = Node->FloatAttribute("R");
			MaterialDesc.vAmbient.y = Node->FloatAttribute("G");
			MaterialDesc.vAmbient.z = Node->FloatAttribute("B");
			MaterialDesc.vAmbient.w = Node->FloatAttribute("A");
		}

		// Diffuse
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vDiffuse.x = Node->FloatAttribute("R");
			MaterialDesc.vDiffuse.y = Node->FloatAttribute("G");
			MaterialDesc.vDiffuse.z = Node->FloatAttribute("B");
			MaterialDesc.vDiffuse.w = Node->FloatAttribute("A");
		}

		// Specular
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vSpecular.x = Node->FloatAttribute("R");
			MaterialDesc.vSpecular.y = Node->FloatAttribute("G");
			MaterialDesc.vSpecular.z = Node->FloatAttribute("B");
			MaterialDesc.vSpecular.w = Node->FloatAttribute("A");
		}

		// Emissive
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vEmissive.x = Node->FloatAttribute("R");
			MaterialDesc.vEmissive.y = Node->FloatAttribute("G");
			MaterialDesc.vEmissive.z = Node->FloatAttribute("B");
			MaterialDesc.vEmissive.w = Node->FloatAttribute("A");
		}

		m_Materials.push_back(MaterialDesc);

		// Next Material
		MaterialNode = MaterialNode->NextSiblingElement();
	}

	m_iNumMaterials = m_Materials.size();

	return S_OK;
}

HRESULT CModel::Load_MapMaterialData_FromFile()
{
	wstring strFullPath = m_strFilePath + m_strFileName + L"/" + m_strFileName + L".xml";
	auto parentPath = filesystem::path(strFullPath).parent_path();

	tinyxml2::XMLDocument* Document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = Document->LoadFile(CAsUtils::ToString(strFullPath).c_str());

#ifdef _DEBUG
	assert(error == tinyxml2::XML_SUCCESS);
#else
#endif


	tinyxml2::XMLElement* Root = Document->FirstChildElement();
	tinyxml2::XMLElement* MaterialNode = Root->FirstChildElement();

	while (MaterialNode)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		tinyxml2::XMLElement* Node = nullptr;

		Node = MaterialNode->FirstChildElement();

		strncpy_s(MaterialDesc.strName, Node->GetText(), strlen(Node->GetText()));



		// Diffuse Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());

			if (strTexture.length() > 0)
			{
				size_t lastDotPos = strTexture.find_last_of(L'.');
				wstring strDDsTexturePath = strTexture.substr(0, lastDotPos) + L".dds";

				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;



				MaterialDesc.pTexture[aiTextureType_DIFFUSE] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_DIFFUSE])
					return E_FAIL;

			}
		}

		// Specular Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				size_t lastDotPos = strTexture.find_last_of(L'.');
				wstring strDDsTexturePath = strTexture.substr(0, lastDotPos) + L".dds";

				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;


				MaterialDesc.pTexture[aiTextureType_SPECULAR] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_SPECULAR])
					return E_FAIL;
			}
		}

		// Normal Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				size_t lastDotPos = strTexture.find_last_of(L'.');
				wstring strDDsTexturePath = strTexture.substr(0, lastDotPos) + L".dds";

				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;


				MaterialDesc.pTexture[aiTextureType_NORMALS] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_NORMALS])
					return E_FAIL;
			}
		}

		// Emissive Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				size_t lastDotPos = strTexture.find_last_of(L'.');
				wstring strDDsTexturePath = strTexture.substr(0, lastDotPos) + L".dds";

				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;


				MaterialDesc.pTexture[aiTextureType_EMISSIVE] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_EMISSIVE])
					return E_FAIL;
			}
		}

		// Metalic Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
			
				size_t lastDotPos = strTexture.find_last_of(L'.');
				wstring strDDsTexturePath = strTexture.substr(0, lastDotPos) + L".dds";

				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;


				MaterialDesc.pTexture[aiTextureType_METALNESS] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_METALNESS])
					return E_FAIL;
			}
		}

		// Roughness Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture = CAsUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				size_t lastDotPos = strTexture.find_last_of(L'.');
				wstring strDDsTexturePath = strTexture.substr(0, lastDotPos) + L".dds";

				wstring szFullPath = L"";

				if (m_eModelType == TYPE::TYPE_ANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;
				else if (m_eModelType == TYPE::TYPE_NONANIM)
					szFullPath = L"../Bin/Resources/Export/ConvertedDDS/" + strDDsTexturePath;

				MaterialDesc.pTexture[aiTextureType_DIFFUSE_ROUGHNESS] = Create_Texture(szFullPath);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_DIFFUSE_ROUGHNESS])
					return E_FAIL;
			}
		}

		// Ambient
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vAmbient.x = Node->FloatAttribute("R");
			MaterialDesc.vAmbient.y = Node->FloatAttribute("G");
			MaterialDesc.vAmbient.z = Node->FloatAttribute("B");
			MaterialDesc.vAmbient.w = Node->FloatAttribute("A");
		}

		// Diffuse
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vDiffuse.x = Node->FloatAttribute("R");
			MaterialDesc.vDiffuse.y = Node->FloatAttribute("G");
			MaterialDesc.vDiffuse.z = Node->FloatAttribute("B");
			MaterialDesc.vDiffuse.w = Node->FloatAttribute("A");
		}

		// Specular
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vSpecular.x = Node->FloatAttribute("R");
			MaterialDesc.vSpecular.y = Node->FloatAttribute("G");
			MaterialDesc.vSpecular.z = Node->FloatAttribute("B");
			MaterialDesc.vSpecular.w = Node->FloatAttribute("A");
		}

		// Emissive
		{
			Node = Node->NextSiblingElement();

			MaterialDesc.vEmissive.x = Node->FloatAttribute("R");
			MaterialDesc.vEmissive.y = Node->FloatAttribute("G");
			MaterialDesc.vEmissive.z = Node->FloatAttribute("B");
			MaterialDesc.vEmissive.w = Node->FloatAttribute("A");
		}

		m_Materials.push_back(MaterialDesc);

		// Next Material
		MaterialNode = MaterialNode->NextSiblingElement();
	}

	m_iNumMaterials = (_uint)m_Materials.size();

	return S_OK;
}

HRESULT CModel::Load_AnimationData_FromFile(Matrix PivotMatrix, _bool bClient)
{
	wstring szFullPath = m_strFilePath + m_strFileName + L"/" + m_strFileName + L".anim";

	shared_ptr<CAsFileUtils> pFileUtils = make_shared<CAsFileUtils>();
	pFileUtils->Open(szFullPath, FileMode::Read);

	m_iNumAnimations = pFileUtils->Read<uint32>();

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create();
		if (nullptr == pAnimation)
			return E_FAIL;

		if (FAILED(pAnimation->LoadData_FromAnimationFile(pFileUtils.get(), PivotMatrix)))
			return E_FAIL;
		

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

_int CModel::FindMaterialIndexByName(string strMaterialName)
{
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		string strFullName = "";
		strFullName = Get_Material_Name(i);

		size_t pos = strFullName.find("_");
		while (pos != string::npos)
		{
			strFullName.replace(pos, 1, " ");
			pos = (pos + 1 < strFullName.size()) ? strFullName.find("_", pos + 1) : string::npos;
		}

		size_t hair_pos = strMaterialName.find(strMaterialName);

		if (hair_pos != string::npos)
			return (_int)i;
	}

	return -1;
}



void CModel::Change_NextAnimation()
{
	m_iCurrAnim = m_tCurrChange.m_iNextAnim;
	m_bNext = false;
	ZeroMemory(&m_tCurrChange, sizeof(CHANGEANIM));

	m_vPreRootPos = { 0.f, 0.f, 0.f };
	m_vCurRootPos = { 0.f, 0.f, 0.f };
}


CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strFilePath, const wstring& strFileName, _bool bClient, _bool bColMesh, Matrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(PivotMatrix, strFilePath, strFileName, bClient, bColMesh)))
	{
		MSG_BOX("Failed To Created : CTexture");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CModel::Clone(CGameObject* pObject, void * pArg)
{
	CModel*			pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();


	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_Materials.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);

	m_Meshes.clear();

	//for (auto& pAnimation : m_Animations)
	//	Safe_Release(pAnimation.second);

	//m_Animations.clear();



	for (auto& Bone : m_ModelBones)
	{
		Bone = nullptr;
	}

	m_ModelBones.clear();
}

_uint CModel::Get_Anim_MaxFrame(_uint iAnimation)
{
	return m_Animations[iAnimation]->Get_MaxFrame();
}

CTexture* CModel::Create_Texture(const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_tchar			szFileName[MAX_PATH] = TEXT("");
	_wsplitpath_s(szFullPath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);


	wstring szComName = L"Prototype_Component_Texture_";
	szComName += szFileName;


	CComponent* pComponent = pGameInstance->Clone_Component(m_pOwner, 0, szComName);


	if (pComponent)
	{
		CTexture* pTexture = dynamic_cast<CTexture*>(pComponent);

		Safe_Release(pGameInstance);
		return pTexture;
	}

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, szFullPath);

	pGameInstance->Add_Prototype(0, szComName, pTexture);

	Safe_Release(pGameInstance);
	return pTexture;
}

Vec3 CModel::ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	_float sinr_cosp = 2.f * (q.w * q.x + q.y * q.z);
	_float cosr_cosp = 1.f - 2.f * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	_float sinp = std::sqrt(1.f + 2.f * (q.w * q.y - q.x * q.z));
	_float cosp = std::sqrt(1.f - 2.f * (q.w * q.y - q.x * q.z));
	angles.y = 2.f * std::atan2(sinp, cosp) - 3.14159265f / 2.f;

	// yaw (z-axis rotation)
	_float siny_cosp = 2.f * (q.w * q.z + q.x * q.y);
	_float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

HRESULT CModel::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader* pShader, _uint iMeshIndex, _uint iStride, _uint iPassIndex)
{
	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	if (FAILED(m_Meshes[iMeshIndex]->Render_Instance(pInstanceBuffer, iSize, iStride)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, CShader* pShader, _uint iMeshIndex, _uint iStride, const string& strPassName)
{
	if (FAILED(pShader->Begin(strPassName)))
		return E_FAIL;

	if (FAILED(m_Meshes[iMeshIndex]->Render_Instance(pInstanceBuffer, iSize, iStride)))
		return E_FAIL;

	return S_OK;
}