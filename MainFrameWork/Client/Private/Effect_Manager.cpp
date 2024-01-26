#include "stdafx.h"
#include "Effect_Manager.h"
#include "Client_Defines.h"
#include "Effect_Mesh.h"
#include "Effect_Texture.h"
#include "Effect_Particle.h"
#include "Effect_Decal.h"
#include "Effect_Trail.h"
#include <filesystem>
#include "tinyxml2.h"
#include "GameInstance.h"
#include "Utils.h"

IMPLEMENT_SINGLETON(CEffect_Manager);

namespace fs = std::filesystem;

CEffect_Manager::CEffect_Manager()
{
}

HRESULT CEffect_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	m_pGameInstance = GET_INSTANCE(CGameInstance)

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	CUtils* pUtils = GET_INSTANCE(CUtils)

	wstring strBundlePath = TEXT("../Bin/Resources/Effects/EffectData/");
	for (const auto& bundle : fs::directory_iterator(strBundlePath))
	{
		wstring strBundleTag = bundle.path().stem().generic_wstring();
		vector<wstring>	vecEffects;

		for (const auto& entry : fs::directory_iterator(bundle))
		{
			if (!entry.is_regular_file() || ".xml" != entry.path().extension())
				continue;

			shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
			tinyxml2::XMLError error = document->LoadFile(entry.path().generic_string().c_str());
			assert(error == tinyxml2::XML_SUCCESS);

			tinyxml2::XMLElement* root = nullptr;
			root = document->FirstChildElement();
			tinyxml2::XMLElement* node = nullptr;
			node = root->FirstChildElement();

			CEffect::tagEffectDesc tDesc;
			_bool IsMesh = false;

			if (!node) { MSG_BOX("Fail to Load"); return E_FAIL; }

			tinyxml2::XMLElement* element = nullptr;
			element = node->FirstChildElement();
			tDesc.iEffectType = element->IntAttribute("EffectType");

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strBaseMesh = pUtils->ToWString(element->GetText());
				if (strBaseMesh.length() > 0)
				{
					tDesc.protoModel = strBaseMesh;
					IsMesh = true;
				}
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strBaseTexture = pUtils->ToWString(element->GetText());
				if (strBaseTexture.length() > 0)
					tDesc.protoDiffuseTexture = strBaseTexture;
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strNoiseTexture = pUtils->ToWString(element->GetText());
				if (strNoiseTexture.length() > 0)
					tDesc.protoNoiseTexture = strNoiseTexture;
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strMaskTexture = pUtils->ToWString(element->GetText());
				if (strMaskTexture.length() > 0)
					tDesc.protoMaskTexture = strMaskTexture;
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strEmissiveTexture = pUtils->ToWString(element->GetText());
				if (strEmissiveTexture.length() > 0)
					tDesc.protoEmissiveTexture = strEmissiveTexture;
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strDissolveTexture = pUtils->ToWString(element->GetText());
				if (strDissolveTexture.length() > 0)
					tDesc.protoDissolveTexture = strDissolveTexture;
			}

			node = node->NextSiblingElement();
			{
				tinyxml2::XMLElement* element = nullptr;

				element = node->FirstChildElement();
				tDesc.vPosition_Start.x = element->FloatAttribute("X");
				tDesc.vPosition_Start.y = element->FloatAttribute("Y");
				tDesc.vPosition_Start.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.vPosition_End.x = element->FloatAttribute("X");
				tDesc.vPosition_End.y = element->FloatAttribute("Y");
				tDesc.vPosition_End.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.bPosition_Lerp = element->BoolAttribute("Lerp");

				element = element->NextSiblingElement();
				tDesc.vRotation_Start.x = element->FloatAttribute("X");
				tDesc.vRotation_Start.y = element->FloatAttribute("Y");
				tDesc.vRotation_Start.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.vRotation_End.x = element->FloatAttribute("X");
				tDesc.vRotation_End.y = element->FloatAttribute("Y");
				tDesc.vRotation_End.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.bRotation_Lerp = element->BoolAttribute("Lerp");
				
				element = element->NextSiblingElement();
				tDesc.vRevolution_Start.x = element->FloatAttribute("X");
				tDesc.vRevolution_Start.y = element->FloatAttribute("Y");
				tDesc.vRevolution_Start.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.vRevolution_End.x = element->FloatAttribute("X");
				tDesc.vRevolution_End.y = element->FloatAttribute("Y");
				tDesc.vRevolution_End.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.bRevolution_Lerp = element->BoolAttribute("Lerp");

				element = element->NextSiblingElement();
				tDesc.vScaling_Start.x = element->FloatAttribute("X");
				tDesc.vScaling_Start.y = element->FloatAttribute("Y");
				tDesc.vScaling_Start.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.vScaling_End.x = element->FloatAttribute("X");
				tDesc.vScaling_End.y = element->FloatAttribute("Y");
				tDesc.vScaling_End.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.bScaling_Lerp = element->BoolAttribute("Lerp");

				element = element->NextSiblingElement();
				tDesc.vVelocity_Start.x = element->FloatAttribute("X");
				tDesc.vVelocity_Start.y = element->FloatAttribute("Y");
				tDesc.vVelocity_Start.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.vVelocity_End.x = element->FloatAttribute("X");
				tDesc.vVelocity_End.y = element->FloatAttribute("Y");
				tDesc.vVelocity_End.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				tDesc.bVelocity_Lerp = element->BoolAttribute("Lerp");

				element = element->NextSiblingElement();
				tDesc.vColor_Start.x = element->FloatAttribute("X");
				tDesc.vColor_Start.y = element->FloatAttribute("Y");
				tDesc.vColor_Start.z = element->FloatAttribute("Z");
				tDesc.vColor_Start.w = element->FloatAttribute("W");

				element = element->NextSiblingElement();
				tDesc.vColor_End.x = element->FloatAttribute("X");
				tDesc.vColor_End.y = element->FloatAttribute("Y");
				tDesc.vColor_End.z = element->FloatAttribute("Z");
				tDesc.vColor_End.w = element->FloatAttribute("W");

				element = element->NextSiblingElement();
				tDesc.bColor_Lerp = element->BoolAttribute("Lerp");

				element = element->NextSiblingElement();
				tDesc.vColor_Clip.x = element->FloatAttribute("X");
				tDesc.vColor_Clip.y = element->FloatAttribute("Y");
				tDesc.vColor_Clip.z = element->FloatAttribute("Z");
				tDesc.vColor_Clip.w = element->FloatAttribute("W");

				element = element->NextSiblingElement();
				tDesc.vColor_Mul.x = element->FloatAttribute("X");
				tDesc.vColor_Mul.y = element->FloatAttribute("Y");
				tDesc.vColor_Mul.z = element->FloatAttribute("Z");
				tDesc.vColor_Mul.w = element->FloatAttribute("W");

				element = element->NextSiblingElement();
				tDesc.fLifeTime = element->FloatAttribute("LifeTime");
				element = element->NextSiblingElement();
				tDesc.fWaitingTime = element->FloatAttribute("WaitingTime");
				element = element->NextSiblingElement();
				tDesc.fRemainTime = element->FloatAttribute("RemainTime");
				
				element = element->NextSiblingElement();
				tDesc.bParentPivot = element->BoolAttribute("ParentPivot");
			}

			node = node->NextSiblingElement();
			{
				tinyxml2::XMLElement* element = nullptr;

				element = node->FirstChildElement();
				tDesc.vUV_Start.x = element->FloatAttribute("X");
				tDesc.vUV_Start.y = element->FloatAttribute("Y");

				element = element->NextSiblingElement();
				tDesc.vUV_Speed.x = element->FloatAttribute("X");
				tDesc.vUV_Speed.y = element->FloatAttribute("Y");

				element = element->NextSiblingElement();
				tDesc.iUV_Wave = element->IntAttribute("Wave");
				tDesc.fUV_WaveSpeed = element->FloatAttribute("WaveSpeed");

				element = element->NextSiblingElement();
				tDesc.IsSequence = element->BoolAttribute("IsSequence");

				element = element->NextSiblingElement();
				tDesc.IsLoop = element->BoolAttribute("IsLoop");

				element = element->NextSiblingElement();
				tDesc.fSequenceTerm = element->FloatAttribute("Sequence_Term");

				element = element->NextSiblingElement();
				tDesc.fDissolveStart = element->FloatAttribute("Dissolve_Start");

				element = element->NextSiblingElement();
				tDesc.vUV_TileCount.x = element->FloatAttribute("X");
				tDesc.vUV_TileCount.y = element->FloatAttribute("Y");

				element = element->NextSiblingElement();
				tDesc.vUV_TileIndex.x = element->FloatAttribute("X");
				tDesc.vUV_TileIndex.y = element->FloatAttribute("Y");
			}

			node = node->NextSiblingElement();
			{
				tinyxml2::XMLElement* element = nullptr;

				element = node->FirstChildElement();
				tDesc.fBloom = element->FloatAttribute("Intensity");

				element = element->NextSiblingElement();
				tDesc.fDistortion = element->FloatAttribute("Intensity");
				tDesc.iDistortionOnBaseMaterial = element->IntAttribute("OnBaseMaterial");

				element = element->NextSiblingElement();
				tDesc.fDissolveAmount = element->FloatAttribute("Amount");

				element = element->NextSiblingElement();
				tDesc.fRadialTime = element->FloatAttribute("Time");
				tDesc.fRadialIntensity = element->FloatAttribute("Intensity");
			}

			node = node->NextSiblingElement();
			{
				tinyxml2::XMLElement* element = nullptr;

				element = node->FirstChildElement();
				tDesc.iBillboard = element->IntAttribute("Billboard");
			}

			node = node->NextSiblingElement();
			{
				if (2 == tDesc.iEffectType)
				{
					tinyxml2::XMLElement* element = nullptr;

					element = node->FirstChildElement();
					tDesc.vEmitDirection.x = element->FloatAttribute("X");
					tDesc.vEmitDirection.y = element->FloatAttribute("Y");
					tDesc.vEmitDirection.z = element->FloatAttribute("Z");

					element = element->NextSiblingElement();
					tDesc.vRandomMul.x = element->FloatAttribute("X");
					tDesc.vRandomMul.y = element->FloatAttribute("Y");
					tDesc.vRandomMul.z = element->FloatAttribute("Z");

					element = element->NextSiblingElement();
					tDesc.fSpreadSpeed = element->FloatAttribute("SpreadSpeed");

					element = element->NextSiblingElement();
					tDesc.fEmitTerm = element->FloatAttribute("EmitTerm");

					element = element->NextSiblingElement();
					tDesc.fParticleLifeTime = element->FloatAttribute("ParticleLifeTime");
				}
			}

			node = node->NextSiblingElement();
			{
				tinyxml2::XMLElement* element = nullptr;

				element = node->FirstChildElement();
				if (element->GetText())
				{
					string strPassName = element->GetText();
					if (strPassName.length() > 0)
						tDesc.strPassName = strPassName;
				}
			}

			if (4 == tDesc.iEffectType)
			{
				node = node->NextSiblingElement();
				{
					tinyxml2::XMLElement* element = nullptr;

					element = node->FirstChildElement();
					tDesc.iTrailVtxCount = element->IntAttribute("Count");
				}
			}

			wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + strBundleTag + TEXT("_") + entry.path().stem().generic_wstring();
			wstring strEffectTag = strBundleTag + TEXT("_") + entry.path().stem().generic_wstring();
			tDesc.EffectTag = strEffectTag;
			if (0 == tDesc.iEffectType)
			{
				if (FAILED(m_pGameInstance->Add_Prototype(strProtoTag, CEffect_Mesh::Create(m_pDevice, m_pContext, &tDesc))))
					return E_FAIL;
			}
			else if (1 == tDesc.iEffectType)
			{
				if (FAILED(m_pGameInstance->Add_Prototype(strProtoTag, CEffect_Texture::Create(m_pDevice, m_pContext, &tDesc))))
					return E_FAIL;
			}
			else if (2 == tDesc.iEffectType)
			{
				if (FAILED(m_pGameInstance->Add_Prototype(strProtoTag, CEffect_Particle::Create(m_pDevice, m_pContext, &tDesc))))
					return E_FAIL;
			}
			else if (3 == tDesc.iEffectType)
			{
				if (FAILED(m_pGameInstance->Add_Prototype(strProtoTag, CEffect_Decal::Create(m_pDevice, m_pContext, &tDesc))))
					return E_FAIL;
			}
			else if (4 == tDesc.iEffectType)
			{
				if (FAILED(m_pGameInstance->Add_Prototype(strProtoTag, CEffect_Trail::Create(m_pDevice, m_pContext, &tDesc))))
					return E_FAIL;
			}

			vecEffects.push_back(strEffectTag);
		}

		m_hashEffectBundles.emplace(strBundleTag, vecEffects);
	}
	
	RELEASE_INSTANCE(CUtils);

	return S_OK;
}

HRESULT CEffect_Manager::Effect_Start(wstring strEffectBundle, EFFECTPIVOTDESC* pDesc)
{
	auto& Effects = m_hashEffectBundles.find(strEffectBundle);

	if (Effects == m_hashEffectBundles.end())
		return E_FAIL;
	else
	{
		for (auto& EffectTag : Effects->second)
		{
			CEffect* pEffect = nullptr;

			if (m_hashEffectPools[EffectTag].empty())
			{
				wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + EffectTag;

				pEffect = static_cast<CEffect*>(m_pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, strProtoTag, pDesc));
				if (nullptr == pEffect)
					return E_FAIL;
			}
			else
			{
				pEffect = m_hashEffectPools[EffectTag].front();
				m_hashEffectPools[EffectTag].pop();
			}

			pEffect->Reset(*pDesc);
			pEffect->Tick(0.0f);
			pEffect->Set_Active(true);
		}
	}

	return S_OK;
}

HRESULT CEffect_Manager::Effect_Start(wstring strEffectBundle, EFFECTPIVOTDESC* pDesc, vector<CEffect*>& EffectList)
{
	auto& Effects = m_hashEffectBundles.find(strEffectBundle);

	if (Effects == m_hashEffectBundles.end())
		return E_FAIL;
	else
	{
		for (auto& EffectTag : Effects->second)
		{
			CEffect* pEffect = nullptr;

			if (m_hashEffectPools[EffectTag].empty())
			{
				wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + EffectTag;

				pEffect = static_cast<CEffect*>(m_pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, strProtoTag, pDesc));
				if (nullptr == pEffect)
					return E_FAIL;
			}
			else
			{
				pEffect = m_hashEffectPools[EffectTag].front();
				m_hashEffectPools[EffectTag].pop();
			}

			pEffect->Reset(*pDesc);
			pEffect->Tick(0.0f);
			pEffect->Set_Active(true);

			EffectList.push_back(pEffect);
		}
	}

	return S_OK;
}

HRESULT CEffect_Manager::Trail_Start(wstring strTrailBundle, std::function<void(Matrix&)> const& PivotGetter)
{
	auto& Effects = m_hashEffectBundles.find(strTrailBundle);

	if (Effects == m_hashEffectBundles.end())
		return E_FAIL;
	else
	{
		for (auto& EffectTag : Effects->second)
		{
			CEffect_Trail* pTrail = nullptr;

			if (m_hashEffectPools[EffectTag].empty())
			{
				wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + EffectTag;

				pTrail = static_cast<CEffect_Trail*>(m_pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, strProtoTag));
				if (nullptr == pTrail)
					return E_FAIL;
			}
			else
			{
				pTrail = static_cast<CEffect_Trail*>(m_hashEffectPools[EffectTag].front());
				m_hashEffectPools[EffectTag].pop();
			}
			
			pTrail->CB_UpdatePivot += PivotGetter;
			pTrail->Reset();
			pTrail->Tick(0.0f);
			pTrail->Set_Active(true);
		}
	}
	return S_OK;
}

HRESULT CEffect_Manager::Trail_Start(wstring strTrailBundle, std::function<void(Matrix&)> const& PivotGetter, vector<CEffect*>& EffectList)
{
	auto& Effects = m_hashEffectBundles.find(strTrailBundle);

	if (Effects == m_hashEffectBundles.end())
		return E_FAIL;
	else
	{
		for (auto& EffectTag : Effects->second)
		{
			CEffect_Trail* pTrail = nullptr;

			if (m_hashEffectPools[EffectTag].empty())
			{
				wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + EffectTag;

				pTrail = static_cast<CEffect_Trail*>(m_pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, strProtoTag));
				if (nullptr == pTrail)
					return E_FAIL;
			}
			else
			{
				pTrail = static_cast<CEffect_Trail*>(m_hashEffectPools[EffectTag].front());
				m_hashEffectPools[EffectTag].pop();
			}

			pTrail->CB_UpdatePivot += PivotGetter;
			pTrail->Reset();
			pTrail->Tick(0.0f);
			pTrail->Set_Active(true);
			EffectList.push_back(pTrail);
		}
	}
	return S_OK;
}

HRESULT CEffect_Manager::Effect_LateStart(wstring strEffectBundle, EFFECTPIVOTDESC_LATE* pDesc)
{
	if (0 == m_hashEffectBundles.count(strEffectBundle))
		return E_FAIL;
	else
		m_vecEffectResereved.push_back(make_pair(strEffectBundle, EFFECTPIVOTDESC_LATE(*pDesc)));

	return S_OK;
}

HRESULT CEffect_Manager::Return_Effect(CEffect* pEffect)
{
	m_hashEffectPools[pEffect->Get_ModelName()].push(pEffect);

	return S_OK;
}

HRESULT CEffect_Manager::Ready_EffectPool(_uint iDefaultSize)
{
	for (auto& Bundle : m_hashEffectBundles)
	{
		for (auto& EffectTag : Bundle.second)
		{
			wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + EffectTag;

			for (_uint i = 0; i < iDefaultSize; ++i)
			{
				CEffect* pEffect = dynamic_cast<CEffect*>(m_pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, strProtoTag));

				if (pEffect == nullptr)
					return E_FAIL;

				m_hashEffectPools[EffectTag].push(pEffect);
			}
		}
	}

	return S_OK;
}

void CEffect_Manager::Tick(_float fTimedelta)
{
	for (_int i = 0; i < m_vecEffectResereved.size(); ++i)
	{
		for (auto& EffectTag : m_hashEffectBundles[m_vecEffectResereved[i].first])
		{
			CEffect* pEffect = nullptr;

			EFFECTPIVOTDESC pDesc =
			{
				m_vecEffectResereved[i].second.pPivotTransform,
				&m_vecEffectResereved[i].second.matPivot,
				m_vecEffectResereved[i].second.bParentPivot
			};

			if (m_hashEffectPools[EffectTag].empty())
			{
				wstring strProtoTag = TEXT("Prototype_GameObject_Effect_") + EffectTag;

				pEffect = static_cast<CEffect*>(m_pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, strProtoTag, &pDesc));
				if (nullptr == pEffect)
					return __debugbreak();
			}
			else
			{
				pEffect = m_hashEffectPools[EffectTag].front();
				m_hashEffectPools[EffectTag].pop();
			}

			pEffect->Reset(pDesc);
			pEffect->Tick(0.0f);
			pEffect->Set_Active(true);
		}
	}

	m_vecEffectResereved.clear();
}

void CEffect_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	RELEASE_INSTANCE(CGameInstance)
}

