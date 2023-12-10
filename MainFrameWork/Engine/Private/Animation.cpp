#include "..\Public\Animation.h"
#include "Model.h"
#include "AsUtils.h"
#include "Shader.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation & rhs)
	: m_fDuration(rhs.m_fDuration)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_fPlayTime(rhs.m_fPlayTime)
	, m_iFrameCount(rhs.m_iFrameCount)
	, m_fSpeed(rhs.m_fSpeed)
	, m_KeyFrameBones(rhs.m_KeyFrameBones)
	, m_tKeyDesc(rhs.m_tKeyDesc)
	, m_szName(rhs.m_szName)
	, m_iBoneSize(rhs.m_iBoneSize)
{
	ZeroMemory(&m_tKeyDesc, sizeof(KEY_DESC));
	m_tKeyDesc.iNextFrame = 1;
}

HRESULT CAnimation::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CAnimation::Initialize(CModel* pModel)
{
	Reset_Animation();
	return S_OK;
}

HRESULT CAnimation::LoadData_FromAnimationFile(CAsFileUtils* pFileUtils, Matrix PivotMatrix)
{
	m_szName = CAsUtils::ToWString(pFileUtils->Read<string>());
	m_fDuration = pFileUtils->Read<_float>();
	m_fTickPerSecond = pFileUtils->Read<_float>();
	m_iFrameCount = pFileUtils->Read<uint32>();

	uint32 iKeyframesCount = pFileUtils->Read<uint32>();
	vector<shared_ptr<ModelKeyframe>>	KeyFrames;

	for (uint32 i = 0; i < iKeyframesCount; i++)
	{
		shared_ptr<ModelKeyframe> Keyframe = make_shared<ModelKeyframe>();
		Keyframe->szBoneName = CAsUtils::ToWString(pFileUtils->Read<string>());

		uint32 iSize = pFileUtils->Read<uint32>();

		if (iSize > 0)
		{
			Keyframe->KeyData.resize(iSize);
			void* ptr = &Keyframe->KeyData[0];
			pFileUtils->Read(&ptr, sizeof(ModelKeyframeData) * iSize);
		}

		KeyFrames.push_back(Keyframe);
	}

	Make_KeyframeData(KeyFrames);

	for (auto& ModelKey : KeyFrames)
	{
		ModelKey->KeyData.clear();
		ModelKey = nullptr;
	}

	return S_OK;
}


HRESULT CAnimation::Play_Animation(_float fTimeDelta)
{
	m_tKeyDesc.fSumTime += fTimeDelta * m_fSpeed;
	
	float fTimePerFrame = 1.f / (m_fTickPerSecond);

	if (m_tKeyDesc.fSumTime >= fTimePerFrame)
	{
		_uint iFrame = m_tKeyDesc.fSumTime / fTimePerFrame;
		m_tKeyDesc.fSumTime -= fTimePerFrame * iFrame;
		m_tKeyDesc.iCurrFrame = (m_tKeyDesc.iCurrFrame + iFrame) % m_iFrameCount;
		m_tKeyDesc.iNextFrame = (m_tKeyDesc.iCurrFrame + 1) % m_iFrameCount;

		if (m_tKeyDesc.iCurrFrame >= m_iFrameCount - 3)
			m_bEnd = true;
	}

	m_tKeyDesc.fRatio = (m_tKeyDesc.fSumTime / fTimePerFrame);

	if (m_tKeyDesc.fRatio > 1.0f)
		m_tKeyDesc.fRatio = 1.0f;

	return S_OK;
}

_float CAnimation::Get_MaxFrameRatio()
{
	return (m_tKeyDesc.iCurrFrame % m_iFrameCount) / (_float)m_iFrameCount;
}

void CAnimation::Set_Frame(_uint iFrame)
{
	m_tKeyDesc.iCurrFrame = iFrame % m_iFrameCount;
	m_tKeyDesc.iNextFrame = (m_tKeyDesc.iCurrFrame + 1) % m_iFrameCount;
}


HRESULT CAnimation::Make_KeyframeData(vector<shared_ptr<ModelKeyframe>>& KeyFrames)
{
	m_KeyFrameBones = new vector<vector<ModelKeyframeData>>();
	m_KeyFrameBones->resize(m_iFrameCount);

	for (auto& Bones : *m_KeyFrameBones)
		Bones.reserve(KeyFrames.size());

	for (_uint i = 0; i < m_iFrameCount; ++i)
	{
		for (_uint j = 0; j < KeyFrames.size(); ++j)
		{
			ModelKeyframeData tModelKey;
			tModelKey = KeyFrames[j]->KeyData[i];

			(*m_KeyFrameBones)[i].push_back(tModelKey);
		}
	}
	
	return S_OK;
}

HRESULT CAnimation::SetUpAnimation_OnShader(CShader* pShader, const char* strMapname)
{

	return S_OK;
}

void CAnimation::Reset_Animation()
{
	m_tKeyDesc.fRatio = 0.0f;
	m_tKeyDesc.fSumTime = 0.0f;
	m_tKeyDesc.iCurrFrame = 0;
	m_tKeyDesc.iNextFrame = 1;
	m_bEnd = false;
}



CAnimation * CAnimation::Create()
{
	CAnimation*			pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Clone(CModel* pModel)
{
	CAnimation*			pInstance = new CAnimation(*this);

	if (FAILED(pInstance->Initialize(pModel)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& KeyData : *m_KeyFrameBones)
		KeyData.clear();
}
