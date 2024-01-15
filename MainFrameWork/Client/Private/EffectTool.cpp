#include "stdafx.h"
#include "EffectTool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"
#include "Transform.h"
#include "Utils.h"
#include "VoidEffect.h"
#include "Camera_Free.h"
#include "Level_Tool.h"
#include "tinyxml2.h"

namespace fs = std::filesystem;

CEffectTool::CEffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
	m_pUtils = GET_INSTANCE(CUtils);
}

HRESULT CEffectTool::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	if (FAILED(LoadMeshes()))
		return E_FAIL;
	if (FAILED(LoadTextures()))
		return E_FAIL;

	if (FAILED(Reset()))
		return E_FAIL;

	Ready_Camera();

	return S_OK;
}

HRESULT CEffectTool::Tick(const _float& fTimeDelta)
{
	Input();

	if (FAILED(EffectTool()))
		return E_FAIL;

	if (m_pCurrentEffect && m_vecEffects.size())
	{
		if (FAILED(EffectDetail()))
			return E_FAIL;
	}

	if (FAILED(EffectsList()))
		return E_FAIL;

	if (FAILED(DataFiles()))
		return E_FAIL;

	if (FAILED(CameraTool()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectTool::LateTick(const _float& fTimeDelta)
{
	if (m_pCurrentEffect)
	{
		//Matrix& matCamWorld = m_pCamera->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
		//////

		//m_pCurrentEffect->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vCamPos + 3.f * vCamForward);
		//m_pCamera->Get_TransformCom()->LookAt(Vec3::Zero);
		

	}

	if (m_IsResetReserved)
	{
		Reset();
		m_IsResetReserved = false;
	}

	return S_OK;
}

HRESULT CEffectTool::DebugRender()
{

	return S_OK;
}

void CEffectTool::Input()
{
	//if (ImGui::GetIO().WantCaptureMouse)
	//	return;
}

//extern Color vBackBuffer;

void CEffectTool::InfoView()
{
	ImGui::Text("This window has some useful function for Effect Tool.");
	ImGui::NewLine();

	const POINT& p = m_pGameInstance->GetMousePos();
	ImGui::Text("Mouse Viewport Position : %d, %d", p.x, p.y);
	ImGui::NewLine();

	//ImGui::InputFloat3("BackBuffer", (_float*)&vBackBuffer);
	//ImGui::NewLine();

	ImGui::Text("Effect Type :");
	ImGui::SameLine();
	if (ImGui::RadioButton("Mesh", &m_iCurrentEffectType, 0))
		m_strCurrentMeshCategory = "";
	ImGui::SameLine();
	if (ImGui::RadioButton("Texture", &m_iCurrentEffectType, 1))
		m_strCurrentCategory = "";
	ImGui::SameLine();
	if (ImGui::RadioButton("Particle", &m_iCurrentEffectType, 2))
		m_strCurrentCategory = "";
	ImGui::SameLine();
	if (ImGui::RadioButton("Decal", &m_iCurrentEffectType, 3))
		m_strCurrentCategory = "";
	/*ImGui::SameLine();
	if (ImGui::RadioButton("Trail", &m_iCurrentEffectType, 4))
		m_strCurrentCategory = "";*/
}

void CEffectTool::ShowCurrentMaterials()
{
	if (-1 == m_iCurrentEffectType)
		return;

	if (0 == m_iCurrentEffectType)	// Mesh
	{
		ImGui::Text(m_BaseMesh.second.c_str());
		ImGui::NewLine();
	}

	ImGuiIO& io = ImGui::GetIO();

	//ImTextureID         TerrainTextureID = m_pTerainTexture[0];
	//LPDIRECT3DTEXTURE9  TerrainTexture = nullptr;
	//ImTextureID selected_texture = nullptr;
	ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);							// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);         // 

	if (ImGui::ImageButton("BaseTexture", m_BaseTexture.second.second, ImVec2(75.f, 75.f), uv0, uv1, tint_col, border_col))
	{
		m_BaseTexture.first = TEXT("");
		m_BaseTexture.second.first = TEXT("");
		m_BaseTexture.second.second = nullptr;
	}
	ImGui::SameLine();

	if (ImGui::ImageButton("CurrentNoise", m_CurrentNoise.second.second, ImVec2(75.f, 75.f), uv0, uv1, tint_col, border_col))
	{
		m_CurrentNoise.first = TEXT("");
		m_CurrentNoise.second.first = TEXT("");
		m_CurrentNoise.second.second = nullptr;
	}
	ImGui::SameLine();

	if(ImGui::ImageButton("CurrentMask", m_CurrentMask.second.second, ImVec2(75.f, 75.f), uv0, uv1, tint_col, border_col))
	{
		m_CurrentMask.first = TEXT("");
		m_CurrentMask.second.first = TEXT("");
		m_CurrentMask.second.second = nullptr;
	}
	ImGui::SameLine();

	if(ImGui::ImageButton("CurrentEmissive", m_CurrentEmissive.second.second, ImVec2(75.f, 75.f), uv0, uv1, tint_col, border_col))
	{
		m_CurrentEmissive.first = TEXT("");
		m_CurrentEmissive.second.first = TEXT("");
		m_CurrentEmissive.second.second = nullptr;
	}
	ImGui::SameLine();

	if (ImGui::ImageButton("CurrentDissolve", m_CurrentDissolve.second.second, ImVec2(75.f, 75.f), uv0, uv1, tint_col, border_col))
	{
		m_CurrentDissolve.first = TEXT("");
		m_CurrentDissolve.second.first = TEXT("");
		m_CurrentDissolve.second.second = nullptr;
	}
}

void CEffectTool::SelectBaseMesh()
{
	ImGui::Text("Category :");
	ImGui::SameLine();
	if (0 == m_iCurrentEffectType) // Meshes
	{
		if (ImGui::BeginCombo(" ", m_strCurrentMeshCategory.c_str()))
		{
			for (size_t i = 0; i < m_vecMeshes.size(); ++i)
			{
				//_bool isSelected = (m_strCurrentCategory == m_vecMeshes[i].first); // You can store your selection however you want, outside or inside your objects
				_bool isSelected = (m_strCurrentMeshCategory.c_str() == m_vecMeshes[i].first);
				if (ImGui::Selectable(m_vecMeshes[i].first.c_str(), isSelected))
					m_strCurrentMeshCategory = m_vecMeshes[i].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		SelectMesh(m_strCurrentMeshCategory);
	}
	ImGui::NewLine();
}

void CEffectTool::SelectBaseTexture()
{
	if (ImGui::BeginTabItem("Base"))
	{
		// Textures
		if (ImGui::BeginCombo("BaseTextures", m_strCurrentCategory.c_str()))
		{
			for (size_t i = 0; i < m_vecTextures.size(); ++i)
			{
				_bool isSelected = (m_strCurrentCategory == m_vecTextures[i].first);
				if (ImGui::Selectable(m_vecTextures[i].first.c_str(), isSelected))
					m_strCurrentCategory = m_vecTextures[i].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		pair<wstring, ID3D11ShaderResourceView*> selected = SelectTexture(m_strCurrentCategory);
		if (nullptr == m_BaseTexture.second.second)
		{
			m_BaseTexture.first = TEXT("../Bin/Resources/Effects/FX_Textures/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
			m_BaseTexture.second = selected;
		}
		ImGui::EndTabItem();
	}
}

void CEffectTool::SelectNoiseTexture()
{
	if (ImGui::BeginTabItem("Noise"))
	{
		// Textures
		if (ImGui::BeginCombo("NoiseTextures", m_strCurrentCategory.c_str()))
		{
			for (size_t i = 0; i < m_vecTextures.size(); ++i)
			{
				_bool isSelected = (m_strCurrentCategory == m_vecTextures[i].first);
				if (ImGui::Selectable(m_vecTextures[i].first.c_str(), isSelected))
					m_strCurrentCategory = m_vecTextures[i].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		pair<wstring, ID3D11ShaderResourceView*> selected = SelectTexture(m_strCurrentCategory);
		if (nullptr == m_CurrentNoise.second.second)
		{
			m_CurrentNoise.first = TEXT("../Bin/Resources/Effects/FX_Textures/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
			m_CurrentNoise.second = selected;
		}
		ImGui::EndTabItem();
	}
}

void CEffectTool::SelectMaskTexture()
{
	if (ImGui::BeginTabItem("Mask"))
	{
		// Textures
		if (ImGui::BeginCombo("MaskTextures", m_strCurrentCategory.c_str()))
		{
			for (size_t i = 0; i < m_vecTextures.size(); ++i)
			{
				_bool isSelected = (m_strCurrentCategory == m_vecTextures[i].first);
				if (ImGui::Selectable(m_vecTextures[i].first.c_str(), isSelected))
					m_strCurrentCategory = m_vecTextures[i].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		pair<wstring, ID3D11ShaderResourceView*> selected = SelectTexture(m_strCurrentCategory);
		if (nullptr == m_CurrentMask.second.second)
		{
			m_CurrentMask.first = TEXT("../Bin/Resources/Effects/FX_Textures/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
			m_CurrentMask.second = selected;
		}
		ImGui::EndTabItem();
	}
}

void CEffectTool::SelectEmissiveTexture()
{
	if (ImGui::BeginTabItem("Emissive"))
	{
		// Textures
		if (ImGui::BeginCombo("EmissiveTextures", m_strCurrentCategory.c_str()))
		{
			for (size_t i = 0; i < m_vecTextures.size(); ++i)
			{
				_bool isSelected = (m_strCurrentCategory == m_vecTextures[i].first);
				if (ImGui::Selectable(m_vecTextures[i].first.c_str(), isSelected))
					m_strCurrentCategory = m_vecTextures[i].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		pair<wstring, ID3D11ShaderResourceView*> selected = SelectTexture(m_strCurrentCategory);
		if (nullptr == m_CurrentEmissive.second.second)
		{
			m_CurrentEmissive.first = TEXT("../Bin/Resources/Effects/FX_Textures/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
			m_CurrentEmissive.second = selected;
		}
		ImGui::EndTabItem();
	}
}

void CEffectTool::SelectDissolveTexture()
{
	if (ImGui::BeginTabItem("Dissolve"))
	{
		// Textures
		if (ImGui::BeginCombo("DissolveTextures", m_strCurrentCategory.c_str()))
		{
			for (size_t i = 0; i < m_vecTextures.size(); ++i)
			{
				_bool isSelected = (m_strCurrentCategory == m_vecTextures[i].first);
				if (ImGui::Selectable(m_vecTextures[i].first.c_str(), isSelected))
					m_strCurrentCategory = m_vecTextures[i].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		pair<wstring, ID3D11ShaderResourceView*> pair = SelectTexture(m_strCurrentCategory);
		if (nullptr == m_CurrentDissolve.second.second)
		{
			m_CurrentDissolve.first = TEXT("../Bin/Resources/Effects/FX_Textures/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
			m_CurrentDissolve.second = pair;
		}
		ImGui::EndTabItem();
	}
}

void CEffectTool::SelectMesh(string& strCurrentCategory)
{
	vector<string>* vecCategoryMeshes = nullptr;
	for (size_t i = 0; i < m_vecMeshes.size(); ++i)
	{
		if (strCurrentCategory == m_vecMeshes[i].first)
			vecCategoryMeshes = &m_vecMeshes[i].second;
	}

	if (ImGui::BeginListBox("BaseMeshes", ImVec2(395, 150)))
	{
		if (nullptr != vecCategoryMeshes)
		{
			for (size_t i = 0; i < vecCategoryMeshes->size(); i++)
			{
				_bool isSelected = (m_strCurrentCategory == (*vecCategoryMeshes)[i]);
				if (ImGui::Selectable((*vecCategoryMeshes)[i].c_str(), isSelected))
				{
					m_BaseMesh.first = TEXT("../Bin/Resources/Effects/FX_Meshes/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
					m_BaseMesh.second = (*vecCategoryMeshes)[i];
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}

pair<wstring, ID3D11ShaderResourceView*> CEffectTool::SelectTexture(string& strCurrentCategory)
{
	pair<wstring, ID3D11ShaderResourceView*> selected;
	selected.first = TEXT("");
	selected.second = nullptr;
	vector<pair<wstring, ID3D11ShaderResourceView*>>* vecCategoryTextures = nullptr;

	for (size_t i = 0; i < m_vecTextures.size(); ++i)
	{
		if (strCurrentCategory == m_vecTextures[i].first)
			vecCategoryTextures = &m_vecTextures[i].second;
	}

	if (ImGui::BeginListBox(" ", ImVec2(395, 390)))
	{
		if (nullptr != vecCategoryTextures)
		{
			for (size_t i = 0; i < vecCategoryTextures->size(); i++)
			{
				if (i % 4 != 0)	ImGui::SameLine();
				if (ImGui::ImageButton((*vecCategoryTextures)[i].second, ImVec2(80.f, 80.f)))
				{
					selected.first = (*vecCategoryTextures)[i].first;
					selected.second = (*vecCategoryTextures)[i].second;
				}
			}
		}
		ImGui::EndListBox();
	}
	return selected;
}

HRESULT CEffectTool::EffectTool()
{
	ImGui::Begin("Effect Tool");

	InfoView();
	//Categories();
	if (-1 == m_iCurrentEffectType)
	{
		ImGui::End();
		return S_OK;
	}

	if (ImGui::Button("Reset"))
	{	//Reset(); ImGui::SameLine();
		m_IsResetReserved = true;
		ImGui::SameLine();
	}
	if (ImGui::Button("CreateEffect"))
		CreateEffect();

	ShowCurrentMaterials();

	m_strCurrentMeshPath = TEXT("../Bin/Resources/Effects/FX_Meshes/") + m_pUtils->ToWString(m_strCurrentMeshCategory) + TEXT("/");
	m_BaseMesh.first = m_strCurrentMeshPath;
	if (0 == m_iCurrentEffectType)
		SelectBaseMesh();

	m_strCurrentPath = TEXT("../Bin/Resources/Effects/FX_Textures/") + m_pUtils->ToWString(m_strCurrentCategory) + TEXT("/");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Material", tab_bar_flags))
	{
		SelectBaseTexture();
		SelectNoiseTexture();
		SelectMaskTexture();
		SelectEmissiveTexture();
		SelectDissolveTexture();
		ImGui::EndTabBar();
	}

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::EffectDetail()
{
	ImGui::Begin("Effect Detail");

	ImGui::Checkbox("Lerp Position", &m_pCurrentEffect->m_bPosition_Lerp);
	ImGui::InputFloat3("Position_Start", (_float*)&m_pCurrentEffect->m_vPosition_Start);
	if (m_pCurrentEffect->m_bPosition_Lerp)
		ImGui::InputFloat3("Position_End", (_float*)&m_pCurrentEffect->m_vPosition_End);
	else
		m_pCurrentEffect->m_vPosition_End = m_pCurrentEffect->m_vPosition_Start;

	ImGui::Checkbox("Lerp Rotation", &m_pCurrentEffect->m_bRotation_Lerp);
	ImGui::InputFloat3("Rotation_Start", (_float*)&m_pCurrentEffect->m_vRotation_Start);
	if (m_pCurrentEffect->m_bRotation_Lerp)
		ImGui::InputFloat3("Rotation_End", (_float*)&m_pCurrentEffect->m_vRotation_End);
	else
		m_pCurrentEffect->m_vRotation_End = m_pCurrentEffect->m_vRotation_Start;

	ImGui::Checkbox("Lerp Scaling", &m_pCurrentEffect->m_bScaling_Lerp);
	ImGui::InputFloat3("Scaling_Start", (_float*)&m_pCurrentEffect->m_vScaling_Start);
	if (m_pCurrentEffect->m_bScaling_Lerp)
		ImGui::InputFloat3("Scaling_End", (_float*)&m_pCurrentEffect->m_vScaling_End);
	else
		m_pCurrentEffect->m_vScaling_End = m_pCurrentEffect->m_vScaling_Start;

	ImGui::Checkbox("Lerp Velocity", &m_pCurrentEffect->m_bVelocity_Lerp);
	ImGui::InputFloat3("Velocity_Start", (_float*)&m_pCurrentEffect->m_vVelocity_Start);
	if (m_pCurrentEffect->m_bVelocity_Lerp)
		ImGui::InputFloat3("Velocity_End", (_float*)&m_pCurrentEffect->m_vVelocity_End);
	else
		m_pCurrentEffect->m_vVelocity_End = m_pCurrentEffect->m_vVelocity_Start;

	ImGui::Checkbox("Lerp ColorOffset", &m_pCurrentEffect->m_bColor_Lerp);
	ImGui::InputFloat4("ColorOffset_Start", (_float*)&m_pCurrentEffect->m_vColor_Start);
	if (m_pCurrentEffect->m_bColor_Lerp)
		ImGui::InputFloat4("ColorOffset_End", (_float*)&m_pCurrentEffect->m_vColor_End);
	else
		m_pCurrentEffect->m_vColor_End = m_pCurrentEffect->m_vColor_Start;

	ImGui::InputFloat4("Color Clip", (_float*)&m_pCurrentEffect->m_Variables.vColor_Clip);

	ImGui::InputFloat4("Color Mul", (_float*)&m_pCurrentEffect->m_Variables.vColor_Mul);

	ImGui::InputFloat("Bloom Intensity", &m_pCurrentEffect->m_Intensity.fBloom);
	ImGui::InputFloat("Radial Intensity", &m_pCurrentEffect->m_Intensity.fRadial);

	ImGui::InputFloat2("UV Speed", (_float*)&m_pCurrentEffect->m_vUV_Speed, "%.7f");

	ImGui::Checkbox("UV_Wave", (_bool*)&m_pCurrentEffect->m_Variables.iUV_Wave);

	if (m_pCurrentEffect->m_Variables.iUV_Wave)
		ImGui::InputFloat("UV_Wave_Speed", (_float*)&m_pCurrentEffect->m_Variables.fUV_WaveSpeed);

	ImGui::Checkbox("IsSequence", &m_pCurrentEffect->m_IsSequence);
	ImGui::Checkbox("IsLoop", &m_pCurrentEffect->m_IsLoop);

	if (m_pCurrentEffect->m_IsSequence)
		ImGui::InputFloat("Sequence Term", &m_pCurrentEffect->m_fSequenceTerm);

	ImGui::InputFloat2("UV_TileCount", (_float*)&m_pCurrentEffect->m_Variables.vUV_TileCount);
	ImGui::InputFloat2("UV_TileIndex", (_float*)&m_pCurrentEffect->m_Variables.vUV_TileIndex);

	ImGui::InputFloat("Life Time", &m_pCurrentEffect->m_fLifeTime);
	ImGui::InputFloat("Waiting Time", &m_pCurrentEffect->m_fWaitingTime);
	ImGui::InputFloat("Remain Time", &m_pCurrentEffect->m_fRemainTime);

	ImGui::Checkbox("Billboard", (_bool*)&m_pCurrentEffect->m_Billboard.iBillboard);
	/*ImGui::Text("UV_Rotation_Offset");
	ImGui::Text("UV_Speed_Offset");
	ImGui::Text("UV_Tiles_Offset");*/

	// 여기에 디졸브
	if (TEXT("") != m_pCurrentEffect->m_tVoidEffectDesc.protoDissolveTexture)
		ImGui::InputFloat("Dissolve Start (ratio)", &m_pCurrentEffect->m_fDissolveStart);
	//

	if (2 == m_pCurrentEffect->m_iEffectType)
	{
		ImGui::InputFloat3("Particle Direction", (_float*)&m_pCurrentEffect->m_vOriginEmitDir);
		Vec3	vRandomMag = Vec3(1.f, 1.f, 1.f);
		_float	fSpreadSpeed = 1.f;
		_float	fEmitTerm = 0.005f;
		_float	fParticleLifeTime = 1.f;
		ImGui::InputFloat3("Particle RandomMultiplier", (_float*)&m_pCurrentEffect->m_Particle.vRandomMul);
		ImGui::InputFloat("Particle SpreadSpeed", (_float*)&m_pCurrentEffect->m_Particle.fSpreadSpeed);
		ImGui::InputFloat("Particle EmittingTerm", (_float*)&m_pCurrentEffect->m_fOriginEmitTerm);
		ImGui::InputFloat("Particle LifeTime", (_float*)&m_pCurrentEffect->m_Particle.fParticleLifeTime);
	}

	const _char* hint = "Pass Name";
	static _char szPassName[128] = "";
	if (ImGui::Button("Select Pass"))
		m_pCurrentEffect->m_strPassName = szPassName;

	ImGui::InputTextWithHint("Pass Name", hint, szPassName, IM_ARRAYSIZE(szPassName));

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::EffectsList()
{
	ImGui::Begin("All Effects");

	if (ImGui::Button("Reset All"))
	{
		for (auto& Effect : m_vecEffects)
		{
			Effect->Reset();
		}
	}

	if (ImGui::Button("Delete"))
	{
		if (m_vecEffects.empty())
		{
			ImGui::End();
			return S_OK;
		}

		m_vecEffects[m_iSelectedEffectIndex]->Set_Dead(true);

		auto& iter = m_vecEffects.begin() + m_iSelectedEffectIndex;
		m_vecEffects.erase(iter);

		if (m_iSelectedEffectIndex >= m_vecEffects.size())
			m_iSelectedEffectIndex = ::max(0, (_int)(m_vecEffects.size() - 1));
	}

	//vector<const _char*> vecItems;
	//m_vecEffects; // 이펙트가 자기 이름 저장하게 하고(오브젝트 아이디나)
	//vecItems.push_back();

	if (ImGui::BeginListBox("Effects", ImVec2(150, 100)))
	{
		for (size_t i = 0; i < m_vecEffects.size(); i++)
		{
			_bool isSelected = (m_vecEffects[i] == m_vecEffects[m_iSelectedEffectIndex]);

			string strItem = "";
			if (0 == m_vecEffects[i]->m_iEffectType)
				strItem = "Effect_" + to_string(i) + "_M";
			else if(1 == m_vecEffects[i]->m_iEffectType)
				strItem = "Effect_" + to_string(i) + "_T";
			else if(2 == m_vecEffects[i]->m_iEffectType)
				strItem = "Effect_" + to_string(i) + "_P";
			else if(3 == m_vecEffects[i]->m_iEffectType)
				strItem = "Effect_" + to_string(i) + "_D";

			if (ImGui::Selectable(strItem.c_str(), isSelected))
			{
				m_iSelectedEffectIndex = i;
				m_pCurrentEffect = m_vecEffects[i];
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	/*ImGui::ListBox("Effects", &m_iSelectedDataIndex, m_vecEffects.data(), m_vecEffects.size(), 5);
	if (!vecItems.empty())
		m_strSelectedGroup = m_pUtils->ToWString(vecItems[m_iSelectedDataIndex]);*/

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::CameraTool()
{
	ImGui::Begin("CameraTool");

	ImGui::InputFloat("First Shake", &m_fFirstShake);
	ImGui::InputFloat("Shake Time", &m_fShakeTime);
	ImGui::InputFloat("Shake Force", &m_fShakeForce);
	ImGui::InputFloat("Shake Break", &m_fShakeBreak);

	if (ImGui::Button("Shake"))
	{
		m_pCamera->Cam_Shake(m_fFirstShake, m_fShakeForce, m_fShakeTime, m_fShakeBreak);
	}


	ImGui::End();
	return S_OK;
}

HRESULT CEffectTool::DataFiles()
{
	ImGui::Begin("Data Files");

	const _char* hint = "Enter the Name of New file";
	ImGui::InputTextWithHint("NewFileName", hint, m_szBundleNameBuf, IM_ARRAYSIZE(m_szBundleNameBuf));

	if (ImGui::Button("Save"))
	{
		if (FAILED(Save(m_szBundleNameBuf)))
			MSG_BOX("Save Failed");
	}ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		if (FAILED(Load()))
			MSG_BOX("Load Failed");
	}

	vector<const _char*> vecItems;
	wstring strDirectory = TEXT("../Bin/Resources/Effects/EffectData/");

	if (fs::exists(strDirectory) && fs::is_directory(strDirectory))
		for (const auto& entry : fs::directory_iterator(strDirectory))
			if (entry.is_directory())
				s2cPushBack(vecItems, m_pUtils->ToString(entry.path().filename()));

	ImGui::ListBox("Saved Data", &m_iSelectedDataIndex, vecItems.data(), vecItems.size(), 5);
	if(!vecItems.empty())
		m_strSelectedGroup = m_pUtils->ToWString(vecItems[m_iSelectedDataIndex]);

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::CreateEffect()
{
	CVoidEffect::tagVoidEffectDesc tDesc;
	if (0 == m_iCurrentEffectType)
	{
		tDesc.protoModel = m_pUtils->ToWString(m_BaseMesh.second);
	}
	else if (1 == m_iCurrentEffectType)
	{
		//tDesc.protoDiffuseTexture = TEXT("Prototype_Component_Texture_") + fs::path(m_BaseTexture.second.first).stem().generic_wstring();
	}
	else if (2 == m_iCurrentEffectType)
	{
	}
	else if (3 == m_iCurrentEffectType)
	{
	}
	else
	{
		MSG_BOX("You need to select BaseMaterial");
	}

	tDesc.iEffectType = m_iCurrentEffectType;
	tDesc.protoDiffuseTexture = m_BaseTexture.first + fs::path(m_BaseTexture.second.first).generic_wstring() + TEXT(".png");

	if (TEXT("") != m_CurrentNoise.second.first)
		tDesc.protoNoiseTexture = m_CurrentNoise.first + fs::path(m_CurrentNoise.second.first).generic_wstring() + TEXT(".png");
	if (TEXT("") != m_CurrentMask.second.first)
		tDesc.protoMaskTexture = m_CurrentMask.first + fs::path(m_CurrentMask.second.first).generic_wstring() + TEXT(".png");
	if (TEXT("") != m_CurrentEmissive.second.first)
		tDesc.protoEmissiveTexture = m_CurrentEmissive.first + fs::path(m_CurrentEmissive.second.first).generic_wstring() + TEXT(".png");
	if (TEXT("") != m_CurrentDissolve.second.first)
		tDesc.protoDissolveTexture = m_CurrentDissolve.first + fs::path(m_CurrentDissolve.second.first).generic_wstring() + TEXT(".png");

	m_pCurrentEffect = static_cast<CVoidEffect*>(m_pGameInstance->Add_GameObject((_uint)LEVEL_TOOL, (_uint)LAYER_TYPE::LAYER_EFFECT, TEXT("Prototype_GameObject_VoidEffect"), &tDesc));
	if (nullptr == m_pCurrentEffect)
		return E_FAIL;

	m_pCurrentEffect->SetEffectTool(this);

	m_vecEffects.push_back(m_pCurrentEffect);

	//Matrix& matCamWorld = m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	//Vec3 vCamForward = matCamWorld.Backward();
	//Vec3 vCamPos = matCamWorld.Translation();

	//m_pCurrentEffect->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vCamPos + 3.f * vCamForward);

	return S_OK;
}

HRESULT CEffectTool::Reset()
{
	m_BaseMesh.first = TEXT("");
	m_BaseMesh.second = "";
	m_BaseTexture.first = TEXT("");
	m_BaseTexture.second.second = nullptr;
	m_CurrentNoise.first = TEXT("");
	m_CurrentNoise.second.second = nullptr;
	m_CurrentMask.first = TEXT("");
	m_CurrentMask.second.second = nullptr;
	m_CurrentEmissive.first = TEXT("");
	m_CurrentEmissive.second.second = nullptr;
	m_CurrentDissolve.first = TEXT("");
	m_CurrentDissolve.second.second = nullptr;

	m_iCurrentEffectType = -1;
	m_strCurrentCategory = "";
	m_strCurrentMeshCategory = "";
	m_strCurrentMeshPath = TEXT("");
	m_strCurrentPath = TEXT("");

	/*if (m_pCurrentEffect)
	{
		m_pCurrentEffect->Set_Dead(true);
		m_pCurrentEffect = nullptr;
	}*/

	return S_OK;
}

HRESULT CEffectTool::Save(_char* szGroupName)
{
	fs::path strPath;
	if (m_vecEffects.size())
		strPath = fs::path("../Bin/Resources/Effects/EffectData/" + string(szGroupName) + "/");
	else
		MSG_BOX("You need to Create Effect");

	fs::create_directories(strPath);

	for (_int i = 0; i < m_vecEffects.size(); ++i)
	{
		shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

		fs::path finalPath;
		if(0 == m_vecEffects[i]->m_iEffectType)
			finalPath = strPath.generic_string() + "Effect_" + to_string(i) + "_M" + ".xml";
		else if (1 == m_vecEffects[i]->m_iEffectType)
			finalPath = strPath.generic_string() + "Effect_" + to_string(i) + "_T" + ".xml";
		else if (2 == m_vecEffects[i]->m_iEffectType)
			finalPath = strPath.generic_string() + "Effect_" + to_string(i) + "_P" + ".xml";
		else if (3 == m_vecEffects[i]->m_iEffectType)
			finalPath = strPath.generic_string() + "Effect_" + to_string(i) + "_D" + ".xml";

		tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
		document->LinkEndChild(decl);

		tinyxml2::XMLElement* root = document->NewElement("Properties");
		document->LinkEndChild(root);

		tinyxml2::XMLElement* node = document->NewElement("ProtoNames");
		tinyxml2::XMLElement* element = nullptr;
		
		root->LinkEndChild(node);
		{
			element = document->NewElement("EffectType");
			element->SetAttribute("EffectType", m_vecEffects[i]->m_iEffectType);
			node->LinkEndChild(element);

			element = document->NewElement("BaseMesh");
			if(TEXT("") == m_vecEffects[i]->m_tVoidEffectDesc.protoModel)
				element->SetText("");
			else
				element->SetText(m_pUtils->ToString(m_vecEffects[i]->m_tVoidEffectDesc.protoModel).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("BaseTexture");
			element->SetText(m_pUtils->ToString(m_vecEffects[i]->m_tVoidEffectDesc.protoDiffuseTexture).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("NoiseTexture");
			element->SetText(m_pUtils->ToString(m_vecEffects[i]->m_tVoidEffectDesc.protoNoiseTexture).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("MaskTexture");
			element->SetText(m_pUtils->ToString(m_vecEffects[i]->m_tVoidEffectDesc.protoMaskTexture).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("EmissiveTexture");
			element->SetText(m_pUtils->ToString(m_vecEffects[i]->m_tVoidEffectDesc.protoEmissiveTexture).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("DissolveTexture");
			element->SetText(m_pUtils->ToString(m_vecEffects[i]->m_tVoidEffectDesc.protoDissolveTexture).c_str());
			node->LinkEndChild(element);
		}

		node = document->NewElement("Transform");
		root->LinkEndChild(node);
		{
			element = document->NewElement("Position_Start");
			element->SetAttribute("X", m_vecEffects[i]->m_vPosition_Start.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vPosition_Start.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vPosition_Start.z);
			node->LinkEndChild(element);

			element = document->NewElement("Position_End");
			element->SetAttribute("X", m_vecEffects[i]->m_vPosition_End.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vPosition_End.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vPosition_End.z);
			node->LinkEndChild(element);

			element = document->NewElement("Position_Lerp");
			element->SetAttribute("Lerp", m_vecEffects[i]->m_bPosition_Lerp);
			node->LinkEndChild(element);

			element = document->NewElement("Rotation_Start");
			element->SetAttribute("X", m_vecEffects[i]->m_vRotation_Start.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vRotation_Start.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vRotation_Start.z);
			node->LinkEndChild(element);

			element = document->NewElement("Rotation_End");
			element->SetAttribute("X", m_vecEffects[i]->m_vRotation_End.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vRotation_End.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vRotation_End.z);
			node->LinkEndChild(element);

			element = document->NewElement("Rotation_Lerp");
			element->SetAttribute("Lerp", m_vecEffects[i]->m_bRotation_Lerp);
			node->LinkEndChild(element);

			element = document->NewElement("Scaling_Start");
			element->SetAttribute("X", m_vecEffects[i]->m_vScaling_Start.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vScaling_Start.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vScaling_Start.z);
			node->LinkEndChild(element);

			element = document->NewElement("Scaling_End");
			element->SetAttribute("X", m_vecEffects[i]->m_vScaling_End.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vScaling_End.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vScaling_End.z);
			node->LinkEndChild(element);

			element = document->NewElement("Scaling_Lerp");
			element->SetAttribute("Lerp", m_vecEffects[i]->m_bScaling_Lerp);
			node->LinkEndChild(element);

			element = document->NewElement("Velocity_Start");
			element->SetAttribute("X", m_vecEffects[i]->m_vVelocity_Start.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vVelocity_Start.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vVelocity_Start.z);
			node->LinkEndChild(element);

			element = document->NewElement("Velocity_End");
			element->SetAttribute("X", m_vecEffects[i]->m_vVelocity_End.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vVelocity_End.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vVelocity_End.z);
			node->LinkEndChild(element);

			element = document->NewElement("Velocity_Lerp");
			element->SetAttribute("Lerp", m_vecEffects[i]->m_bVelocity_Lerp);
			node->LinkEndChild(element);

			element = document->NewElement("Color_Start");
			element->SetAttribute("X", m_vecEffects[i]->m_vColor_Start.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vColor_Start.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vColor_Start.z);
			element->SetAttribute("W", m_vecEffects[i]->m_vColor_Start.w);
			node->LinkEndChild(element);

			element = document->NewElement("Color_End");
			element->SetAttribute("X", m_vecEffects[i]->m_vColor_End.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vColor_End.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vColor_End.z);
			element->SetAttribute("W", m_vecEffects[i]->m_vColor_End.w);
			node->LinkEndChild(element);

			element = document->NewElement("Color_Lerp");
			element->SetAttribute("Lerp", m_vecEffects[i]->m_bColor_Lerp);
			node->LinkEndChild(element);

			element = document->NewElement("Color_Clip");
			element->SetAttribute("X", m_vecEffects[i]->m_Variables.vColor_Clip.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_Variables.vColor_Clip.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_Variables.vColor_Clip.z);
			element->SetAttribute("W", m_vecEffects[i]->m_Variables.vColor_Clip.w);
			node->LinkEndChild(element);

			element = document->NewElement("Color_Mul");
			element->SetAttribute("X", m_vecEffects[i]->m_Variables.vColor_Mul.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_Variables.vColor_Mul.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_Variables.vColor_Mul.z);
			element->SetAttribute("W", m_vecEffects[i]->m_Variables.vColor_Mul.w);
			node->LinkEndChild(element);

			element = document->NewElement("LifeTime");
			element->SetAttribute("LifeTime", m_vecEffects[i]->m_fLifeTime);
			node->LinkEndChild(element);
			element = document->NewElement("WaitingTime");
			element->SetAttribute("WaitingTime", m_vecEffects[i]->m_fWaitingTime);
			node->LinkEndChild(element);
			element = document->NewElement("RemainTime");
			element->SetAttribute("RemainTime", m_vecEffects[i]->m_fRemainTime);
			node->LinkEndChild(element);
			
			element = document->NewElement("ParentPivot");
			element->SetAttribute("ParentPivot", m_vecEffects[i]->m_bParentPivot);
			node->LinkEndChild(element);
		}

		node = document->NewElement("UV_Variables");
		root->LinkEndChild(node);
		{
			element = document->NewElement("UV_Speed");
			element->SetAttribute("X", m_vecEffects[i]->m_vUV_Speed.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vUV_Speed.y);
			node->LinkEndChild(element);

			element = document->NewElement("UV_Wave");
			element->SetAttribute("Wave", m_vecEffects[i]->m_Variables.iUV_Wave);
			element->SetAttribute("WaveSpeed", m_vecEffects[i]->m_Variables.fUV_WaveSpeed);
			node->LinkEndChild(element);

			element = document->NewElement("Is_Sequence");
			element->SetAttribute("IsSequence", m_vecEffects[i]->m_IsSequence);
			node->LinkEndChild(element);

			element = document->NewElement("Is_Loop");
			element->SetAttribute("IsLoop", m_vecEffects[i]->m_IsLoop);
			node->LinkEndChild(element);

			element = document->NewElement("Sequence_Term");
			element->SetAttribute("Sequence_Term", m_vecEffects[i]->m_fSequenceTerm);
			node->LinkEndChild(element);
			
			element = document->NewElement("Dissolve_Start");
			element->SetAttribute("Dissolve_Start", m_vecEffects[i]->m_fDissolveStart);
			node->LinkEndChild(element);

			element = document->NewElement("UV_TileCount");
			element->SetAttribute("X", m_vecEffects[i]->m_Variables.vUV_TileCount.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_Variables.vUV_TileCount.y);
			node->LinkEndChild(element);

			element = document->NewElement("UV_TileIndex");
			element->SetAttribute("X", m_vecEffects[i]->m_Variables.vUV_TileIndex.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_Variables.vUV_TileIndex.y);
			node->LinkEndChild(element);
		}

		node = document->NewElement("Intensity");
		root->LinkEndChild(node);
		{
			element = document->NewElement("Bloom");
			element->SetAttribute("Intensity", m_vecEffects[i]->m_Intensity.fBloom);
			node->LinkEndChild(element);

			element = document->NewElement("Radial");
			element->SetAttribute("Intensity", m_vecEffects[i]->m_Intensity.fRadial);
			node->LinkEndChild(element);
			
			element = document->NewElement("Dissolve");
			element->SetAttribute("Amount", m_vecEffects[i]->m_Intensity.fDissolveAmount);
			node->LinkEndChild(element);
		}

		node = document->NewElement("Billboard");
		root->LinkEndChild(node);
		{
			element = document->NewElement("Billboard");
			element->SetAttribute("Billboard", m_vecEffects[i]->m_Billboard.iBillboard);
			node->LinkEndChild(element);
		}

		node = document->NewElement("Particle");
		root->LinkEndChild(node);
		{
			element = document->NewElement("EmitDirection");
			element->SetAttribute("X", m_vecEffects[i]->m_vOriginEmitDir.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_vOriginEmitDir.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_vOriginEmitDir.z);
			node->LinkEndChild(element);
			
			element = document->NewElement("RandomMul");
			element->SetAttribute("X", m_vecEffects[i]->m_Particle.vRandomMul.x);
			element->SetAttribute("Y", m_vecEffects[i]->m_Particle.vRandomMul.y);
			element->SetAttribute("Z", m_vecEffects[i]->m_Particle.vRandomMul.z);
			node->LinkEndChild(element);

			element = document->NewElement("SpreadSpeed");
			element->SetAttribute("SpreadSpeed", m_vecEffects[i]->m_Particle.fSpreadSpeed);
			node->LinkEndChild(element);

			element = document->NewElement("EmitTerm");
			element->SetAttribute("EmitTerm", m_vecEffects[i]->m_fOriginEmitTerm);
			node->LinkEndChild(element);

			element = document->NewElement("ParticleLifeTime");
			element->SetAttribute("ParticleLifeTime", m_vecEffects[i]->m_Particle.fParticleLifeTime);
			node->LinkEndChild(element);
		}

		node = document->NewElement("PassName");
		root->LinkEndChild(node);
		{
			element = document->NewElement("PassName");
			element->SetText(m_vecEffects[i]->m_strPassName.c_str());
			node->LinkEndChild(element);
		}		

		document->SaveFile(m_pUtils->ToString(finalPath).c_str());
	}

	return S_OK;
}

HRESULT CEffectTool::Load()
{
	Reset();

	wstring strPath = TEXT("../Bin/Resources/Effects/EffectData/") + m_strSelectedGroup + TEXT("/");

	for (const auto& entry : fs::directory_iterator(strPath))
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

		CVoidEffect::tagVoidEffectDesc tDesc;
		_bool IsMesh = false;

		if (!node) { MSG_BOX("Fail to Load"); return E_FAIL; }

		tinyxml2::XMLElement* element = nullptr;

		element = node->FirstChildElement();
		tDesc.iEffectType = element->IntAttribute("EffectType");
	
		element = element->NextSiblingElement();
		if (element->GetText())
		{
			wstring strBaseMesh = m_pUtils->ToWString(element->GetText());
			if (strBaseMesh.length() > 0)
			{
				tDesc.protoModel = strBaseMesh;
				IsMesh = true;
			}
		}

		element = element->NextSiblingElement();
		if (element->GetText())
		{
			wstring strBaseTexture = m_pUtils->ToWString(element->GetText());
			if (strBaseTexture.length() > 0)
				tDesc.protoDiffuseTexture = strBaseTexture;
		}

		element = element->NextSiblingElement();
		if (element->GetText())
		{
			wstring strNoiseTexture = m_pUtils->ToWString(element->GetText());
			if (strNoiseTexture.length() > 0)
				tDesc.protoNoiseTexture = strNoiseTexture;
		}

		element = element->NextSiblingElement();
		if (element->GetText())
		{
			wstring strMaskTexture = m_pUtils->ToWString(element->GetText());
			if (strMaskTexture.length() > 0)
				tDesc.protoMaskTexture = strMaskTexture;
		}

		element = element->NextSiblingElement();
		if (element->GetText())
		{
			wstring strEmissiveTexture = m_pUtils->ToWString(element->GetText());
			if (strEmissiveTexture.length() > 0)
				tDesc.protoEmissiveTexture = strEmissiveTexture;
		}

		element = element->NextSiblingElement();
		if (element->GetText())
		{
			wstring strDissolveTexture = m_pUtils->ToWString(element->GetText());
			if (strDissolveTexture.length() > 0)
				tDesc.protoDissolveTexture = strDissolveTexture;
		}

		m_pCurrentEffect = static_cast<CVoidEffect*>(m_pGameInstance->Add_GameObject((_uint)LEVEL_TOOL, (_uint)LAYER_TYPE::LAYER_EFFECT, TEXT("Prototype_GameObject_VoidEffect"), &tDesc));
		if (nullptr == m_pCurrentEffect)
			return E_FAIL;

		m_pCurrentEffect->SetEffectTool(this);

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			m_pCurrentEffect->m_vPosition_Start.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vPosition_Start.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vPosition_Start.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vPosition_End.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vPosition_End.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vPosition_End.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_bPosition_Lerp = element->BoolAttribute("Lerp");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vRotation_Start.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vRotation_Start.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vRotation_Start.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vRotation_End.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vRotation_End.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vRotation_End.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_bRotation_Lerp = element->BoolAttribute("Lerp");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vScaling_Start.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vScaling_Start.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vScaling_Start.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vScaling_End.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vScaling_End.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vScaling_End.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_bScaling_Lerp = element->BoolAttribute("Lerp");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vVelocity_Start.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vVelocity_Start.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vVelocity_Start.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vVelocity_End.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vVelocity_End.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vVelocity_End.z = element->FloatAttribute("Z");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_bVelocity_Lerp = element->BoolAttribute("Lerp");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vColor_Start.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vColor_Start.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vColor_Start.z = element->FloatAttribute("Z");
			m_pCurrentEffect->m_vColor_Start.w = element->FloatAttribute("W");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_vColor_End.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vColor_End.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_vColor_End.z = element->FloatAttribute("Z");
			m_pCurrentEffect->m_vColor_End.w = element->FloatAttribute("W");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_bColor_Lerp = element->BoolAttribute("Lerp");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Variables.vColor_Clip.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_Variables.vColor_Clip.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_Variables.vColor_Clip.z = element->FloatAttribute("Z");
			m_pCurrentEffect->m_Variables.vColor_Clip.w = element->FloatAttribute("W");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Variables.vColor_Mul.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_Variables.vColor_Mul.y = element->FloatAttribute("Y");
			m_pCurrentEffect->m_Variables.vColor_Mul.z = element->FloatAttribute("Z");
			m_pCurrentEffect->m_Variables.vColor_Mul.w = element->FloatAttribute("W");
			
			element = element->NextSiblingElement();
			m_pCurrentEffect->m_fLifeTime = element->FloatAttribute("LifeTime");
			element = element->NextSiblingElement();
			m_pCurrentEffect->m_fWaitingTime = element->FloatAttribute("WaitingTime");
			element = element->NextSiblingElement();
			m_pCurrentEffect->m_fRemainTime = element->FloatAttribute("RemainTime");
			
			element = element->NextSiblingElement();
			m_pCurrentEffect->m_bParentPivot = element->BoolAttribute("ParentPivot");
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			m_pCurrentEffect->m_vUV_Speed.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_vUV_Speed.y = element->FloatAttribute("Y");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Variables.iUV_Wave = element->FloatAttribute("Wave");
			m_pCurrentEffect->m_Variables.fUV_WaveSpeed = element->FloatAttribute("WaveSpeed");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_IsSequence = element->BoolAttribute("IsSequence");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_IsLoop = element->BoolAttribute("IsLoop");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_fSequenceTerm = element->FloatAttribute("Sequence_Term");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_fDissolveStart = element->FloatAttribute("Dissolve_Start");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Variables.vUV_TileCount.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_Variables.vUV_TileCount.y = element->FloatAttribute("Y");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Variables.vUV_TileIndex.x = element->FloatAttribute("X");
			m_pCurrentEffect->m_Variables.vUV_TileIndex.y = element->FloatAttribute("Y");
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			m_pCurrentEffect->m_Intensity.fBloom = element->FloatAttribute("Intensity");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Intensity.fRadial = element->FloatAttribute("Intensity");

			element = element->NextSiblingElement();
			m_pCurrentEffect->m_Intensity.fDissolveAmount = element->FloatAttribute("Amount");
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			m_pCurrentEffect->m_Billboard.iBillboard = element->IntAttribute("Billboard");
		}

		node = node->NextSiblingElement();
		{
			if (2 == tDesc.iEffectType)
			{
				tinyxml2::XMLElement* element = nullptr;

				element = node->FirstChildElement();
				m_pCurrentEffect->m_vOriginEmitDir.x = element->FloatAttribute("X");
				m_pCurrentEffect->m_vOriginEmitDir.y = element->FloatAttribute("Y");
				m_pCurrentEffect->m_vOriginEmitDir.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				m_pCurrentEffect->m_Particle.vRandomMul.x = element->FloatAttribute("X");
				m_pCurrentEffect->m_Particle.vRandomMul.y = element->FloatAttribute("Y");
				m_pCurrentEffect->m_Particle.vRandomMul.z = element->FloatAttribute("Z");

				element = element->NextSiblingElement();
				m_pCurrentEffect->m_Particle.fSpreadSpeed = element->FloatAttribute("SpreadSpeed");

				element = element->NextSiblingElement();
				m_pCurrentEffect->m_fOriginEmitTerm = element->FloatAttribute("EmitTerm");

				element = element->NextSiblingElement();
				m_pCurrentEffect->m_Particle.fParticleLifeTime = element->FloatAttribute("ParticleLifeTime");
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
					m_pCurrentEffect->m_strPassName = strPassName;
			}
		}

		m_pCurrentEffect->Reset();
		m_vecEffects.push_back(m_pCurrentEffect);
	}

	return S_OK;
}

HRESULT CEffectTool::LoadMeshes()
{
	wstring strDirectory = TEXT("../Bin/Resources/Effects/FX_Meshes/");

	if (fs::exists(strDirectory) && fs::is_directory(strDirectory))
	{
		for (const auto& category : fs::directory_iterator(strDirectory))
		{
			if (category.is_directory())
			{
				string strKey = category.path().stem().generic_string();
				vector<string> meshes;
				
				for (const auto& mesh : fs::directory_iterator(category))
				{
					string strMeshFileName = mesh.path().stem().generic_string();
					if (mesh.is_directory() && "desktop" != strMeshFileName)
					{
						meshes.push_back(mesh.path().stem().generic_string());
					}
				}
				m_vecMeshes.push_back(make_pair(strKey, meshes));
			}
		}
	}

	return S_OK;
}

HRESULT CEffectTool::LoadTextures()
{
	wstring strDirectory = TEXT("../Bin/Resources/Effects/FX_Textures/");
	
	if (fs::exists(strDirectory) && fs::is_directory(strDirectory))
	{
		for (const auto& category : fs::directory_iterator(strDirectory))
		{
			if (category.is_directory())
			{
				string strKey = category.path().stem().generic_string();
				vector<pair<wstring, ID3D11ShaderResourceView*>> textures;

				for (const auto& texture : fs::directory_iterator(category))
				{
					string strTextureFileName = texture.path().stem().generic_string();
					if (texture.is_regular_file() && "desktop" != strTextureFileName)
					{
						HRESULT hr = 0;
						ID3D11ShaderResourceView* pSRV = nullptr;

						const _tchar* path = texture.path().c_str();
						wstring filename = texture.path().stem();
						wstring extension = texture.path().extension();

						if (TEXT(".dds") == extension)
							hr = DirectX::CreateDDSTextureFromFile(m_pDevice, path, nullptr, &pSRV);
						else if (TEXT(".tga") == extension)
							hr = E_FAIL;
						else
							hr = DirectX::CreateWICTextureFromFile(m_pDevice, path, nullptr, &pSRV);

						if (FAILED(hr))
							return E_FAIL;

						textures.push_back(make_pair(filename, pSRV));
						/*wstring filename = texture.path().stem();
						wstring extension = texture.path().extension();

						wstring strPrototypeTag = TEXT("Prototype_Component_Texture_") + filename;
						CTexture* pTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Component(nullptr, (_uint)LEVEL_TOOL, strPrototypeTag));
						ID3D11ShaderResourceView* pSRV = pTexture->Get_SRV();

						textures.push_back(make_pair(filename, pSRV));
						Safe_Release(pTexture);*/
					}
				}
				m_vecTextures.push_back(make_pair(strKey, textures));
			}
		}
	}

	return S_OK;
}

HRESULT CEffectTool::Ready_Camera()
{
	/*CCamera::CAMERADESC tCameraDesc;

	tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	tCameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	tCameraDesc.fNear = 0.2f;
	tCameraDesc.fFar = 1200.0f;
	tCameraDesc.TransformDesc.fRotationPerSec = 10.0f;
	tCameraDesc.TransformDesc.fSpeedPerSec = 10.0f;

	CGameObject* pCamera = CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free", &tCameraDesc);*/

	m_pCamera = dynamic_cast<CCamera_Free*>(CGameInstance::GetInstance()->Find_GameObejct(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free"));


	return S_OK;
}

CEffectTool* CEffectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool* pLevel_Tool)
{
	CEffectTool* pInstance = new CEffectTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pLevel_Tool)))
	{
		MSG_BOX("Failed to Created : CEffectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectTool::Free()
{
	Super::Free();
	RELEASE_INSTANCE(CUtils);
}
