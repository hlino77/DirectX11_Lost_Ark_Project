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

	return S_OK;
}

HRESULT CEffectTool::Tick(const _float& fTimeDelta)
{
	Input();

	if (FAILED(EffectTool()))
		return E_FAIL;

	if (m_pCurrentEffect)
	{
		if (FAILED(EffectDetail()))
			return E_FAIL;
	}

	if (FAILED(DataFIles()))
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
		
		if(KEY_HOLD(KEY::CTRL) && KEY_HOLD(KEY::LEFT_ARROW))
			m_pCurrentEffect->Get_TransformCom()->Turn(Vec3::UnitY, fTimeDelta);
		if(KEY_HOLD(KEY::CTRL) && KEY_HOLD(KEY::RIGHT_ARROW))
			m_pCurrentEffect->Get_TransformCom()->Turn(Vec3::UnitY, -fTimeDelta);
		if(KEY_HOLD(KEY::CTRL) && KEY_HOLD(KEY::UP_ARROW))
			m_pCurrentEffect->Get_TransformCom()->Turn(Vec3::UnitX, fTimeDelta);
		if(KEY_HOLD(KEY::CTRL) && KEY_HOLD(KEY::DOWN_ARROW))
			m_pCurrentEffect->Get_TransformCom()->Turn(Vec3::UnitX, -fTimeDelta);
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

void CEffectTool::InfoView()
{
	ImGui::Text("This window has some useful function for Effect Tool.");
	ImGui::NewLine();

	const POINT& p = m_pGameInstance->GetMousePos();
	ImGui::Text("Mouse Viewport Position : %d, %d", p.x, p.y);
	ImGui::NewLine();

	ImGui::Text("Effect Type :");
	ImGui::SameLine();
	if (ImGui::RadioButton("Mesh", &m_iCurrentEffectType, 0))
		m_strCurrentMeshCategory = "";
	ImGui::SameLine();
	if (ImGui::RadioButton("Texture", &m_iCurrentEffectType, 1))
		m_strCurrentCategory = "";
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

	if (!m_pCurrentEffect->m_Billboard.iBillboard)
	{
		ImGui::Checkbox("Lerp Rotation", &m_pCurrentEffect->m_bRotation_Lerp);
		ImGui::InputFloat3("Rotation_Start", (_float*)&m_pCurrentEffect->m_vRotation_Start);
		if (m_pCurrentEffect->m_bRotation_Lerp)
			ImGui::InputFloat3("Rotation_End", (_float*)&m_pCurrentEffect->m_vRotation_End);
		else
			m_pCurrentEffect->m_vRotation_End = m_pCurrentEffect->m_vRotation_Start;
	}

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

	ImGui::InputFloat2("UV Speed", (_float*)&m_pCurrentEffect->m_vUV_Speed);

	ImGui::InputFloat2("UV_Position_Offset", (_float*)&m_pCurrentEffect->m_Variables.vUV_Offset);
	ImGui::InputFloat2("UV_Direction_Offset", (_float*)&m_pCurrentEffect->m_Variables.vUV_Direction);
	ImGui::Checkbox("IsSequence", &m_pCurrentEffect->m_IsSequence);
	ImGui::InputFloat2("UV_TileCount", (_float*)&m_pCurrentEffect->m_Variables.vUV_TileCount);
	ImGui::InputFloat2("UV_TileIndex", (_float*)&m_pCurrentEffect->m_Variables.vUV_TileIndex);

	ImGui::InputFloat("Life Time", (_float*)&m_pCurrentEffect->m_fLifeTime);

	ImGui::Checkbox("Billboard", (_bool*)&m_pCurrentEffect->m_Billboard.iBillboard);
	/*ImGui::Text("UV_Rotation_Offset");
	ImGui::Text("UV_Speed_Offset");
	ImGui::Text("UV_Tiles_Offset");*/

	//m_pCurrentEffect

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::EffectsList()
{
	ImGui::Begin("All Effects");

	if (ImGui::Button("Delete"))
	{
	//
	}

	//vector<const _char*> vecItems;
	//m_vecEffects; // 이펙트가 자기 이름 저장하게 하고(오브젝트 아이디나)
	//vecItems.push_back();

	if (ImGui::BeginListBox("Effects", ImVec2(150, 100)))
	{
		for (size_t i = 0; i < m_vecEffects.size(); i++)
		{
			_bool isSelected = (m_vecEffects[i] == m_vecEffects[m_iSelectedEffectIndex]);
			if (ImGui::Selectable("%d", i, isSelected))
			{
				m_iSelectedEffectIndex = i;
				m_pCurrentEffect = m_vecEffects[i];
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	/*ImGui::ListBox("Effects", &m_iSelectedDataFileIndex, m_vecEffects.data(), m_vecEffects.size(), 5);
	if (!vecItems.empty())
		m_strSelectedDataFile = m_pUtils->ToWString(vecItems[m_iSelectedDataFileIndex]);*/

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::DataFIles()
{
	ImGui::Begin("Data Files");

	const _char* hint = "Enter the Name of New file";
	ImGui::InputTextWithHint("NewFileName", hint, m_szFileNameBuf, IM_ARRAYSIZE(m_szFileNameBuf));

	if (ImGui::Button("Save"))
	{
		if (FAILED(Save(m_szFileNameBuf)))
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
			if (entry.is_regular_file())
				s2cPushBack(vecItems, m_pUtils->ToString(entry.path().filename()));

	ImGui::ListBox("Data Files", &m_iSelectedDataFileIndex, vecItems.data(), vecItems.size(), 5);
	if(!vecItems.empty())
		m_strSelectedDataFile = m_pUtils->ToWString(vecItems[m_iSelectedDataFileIndex]);

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::CreateEffect()
{
	CVoidEffect::tagVoidEffectDesc tDesc;
	if (0 == m_iCurrentEffectType)
	{
		tDesc.protoModel = TEXT("Prototype_Component_Model_") + m_pUtils->ToWString(m_BaseMesh.second);
	}
	else if (1 == m_iCurrentEffectType)
	{
		//tDesc.protoDiffuseTexture = TEXT("Prototype_Component_Texture_") + fs::path(m_BaseTexture.second.first).stem().generic_wstring();
	}
	else
	{
		MSG_BOX("You need to select BaseMaterial");
	}

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

	//Safe_Release(m_pCurrentEffect);
	m_pCurrentEffect = nullptr;

	return S_OK;
}

HRESULT CEffectTool::Save(_char* szFileName)
{
	fs::path strPath;
	if (0 == m_iCurrentEffectType || 1 == m_iCurrentEffectType)
		strPath = fs::path("../Bin/Resources/Effects/EffectData/" + string(szFileName) + ".xml");
	//else if (1 == m_iCurrentEffectType)
	//	strPath = fs::path("../Bin/Resources/Effects/Data_Textures/");
	else
		MSG_BOX("You need to Create Effect");

	filesystem::create_directories(strPath.parent_path());

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Properties");
	document->LinkEndChild(root);

	tinyxml2::XMLElement* node = document->NewElement("ProtoNames");
	tinyxml2::XMLElement* element = nullptr;
	root->LinkEndChild(node);
	{
		element = document->NewElement("BaseMesh");
		element->SetText(("Prototype_Component_Model_" + m_BaseMesh.second).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("BaseTexture");
		if (m_BaseTexture.second.second)
			element->SetText(m_pUtils->ToString(m_BaseTexture.first + m_BaseTexture.second.first + TEXT(".png")).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NoiseTexture");
		if (m_CurrentNoise.second.second)
			element->SetText(m_pUtils->ToString(m_CurrentNoise.first + m_CurrentNoise.second.first + TEXT(".png")).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("MaskTexture");
		if (m_CurrentMask.second.second)
			element->SetText(m_pUtils->ToString(m_CurrentMask.first + m_CurrentMask.second.first + TEXT(".png")).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("EmissiveTexture");
		if (m_CurrentEmissive.second.second)
			element->SetText(m_pUtils->ToString(m_CurrentEmissive.first + m_CurrentEmissive.second.first + TEXT(".png")).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DissolveTexture");
		if (m_CurrentDissolve.second.second)
			element->SetText(m_pUtils->ToString(m_CurrentDissolve.first + m_CurrentDissolve.second.first + TEXT(".png")).c_str());
		node->LinkEndChild(element);
	}

	node = document->NewElement("Transform");
	root->LinkEndChild(node);
	{
		element = document->NewElement("Position_Start");
		element->SetAttribute("X", m_pCurrentEffect->m_vPosition_Start.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vPosition_Start.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vPosition_Start.z);
		node->LinkEndChild(element);

		element = document->NewElement("Position_End");
		element->SetAttribute("X", m_pCurrentEffect->m_vPosition_End.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vPosition_End.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vPosition_End.z);
		node->LinkEndChild(element);

		element = document->NewElement("Position_Lerp");
		element->SetAttribute("Lerp", m_pCurrentEffect->m_bPosition_Lerp);
		node->LinkEndChild(element);

		element = document->NewElement("Rotation_Start");
		element->SetAttribute("X", m_pCurrentEffect->m_vRotation_Start.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vRotation_Start.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vRotation_Start.z);
		node->LinkEndChild(element);

		element = document->NewElement("Rotation_End");
		element->SetAttribute("X", m_pCurrentEffect->m_vRotation_End.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vRotation_End.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vRotation_End.z);
		node->LinkEndChild(element);

		element = document->NewElement("Rotation_Lerp");
		element->SetAttribute("Lerp", m_pCurrentEffect->m_bRotation_Lerp);
		node->LinkEndChild(element);

		element = document->NewElement("Scaling_Start");
		element->SetAttribute("X", m_pCurrentEffect->m_vScaling_Start.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vScaling_Start.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vScaling_Start.z);
		node->LinkEndChild(element);

		element = document->NewElement("Scaling_End");
		element->SetAttribute("X", m_pCurrentEffect->m_vScaling_End.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vScaling_End.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vScaling_End.z);
		node->LinkEndChild(element);

		element = document->NewElement("Scaling_Lerp");
		element->SetAttribute("Lerp", m_pCurrentEffect->m_bScaling_Lerp);
		node->LinkEndChild(element);

		element = document->NewElement("Velocity_Start");
		element->SetAttribute("X", m_pCurrentEffect->m_vVelocity_Start.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vVelocity_Start.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vVelocity_Start.z);
		node->LinkEndChild(element);

		element = document->NewElement("Velocity_End");
		element->SetAttribute("X", m_pCurrentEffect->m_vVelocity_End.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vVelocity_End.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vVelocity_End.z);
		node->LinkEndChild(element);

		element = document->NewElement("Velocity_Lerp");
		element->SetAttribute("Lerp", m_pCurrentEffect->m_bVelocity_Lerp);
		node->LinkEndChild(element);

		element = document->NewElement("Color_Start");
		element->SetAttribute("X", m_pCurrentEffect->m_vColor_Start.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vColor_Start.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vColor_Start.z);
		element->SetAttribute("W", m_pCurrentEffect->m_vColor_Start.w);
		node->LinkEndChild(element);

		element = document->NewElement("Color_End");
		element->SetAttribute("X", m_pCurrentEffect->m_vColor_End.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vColor_End.y);
		element->SetAttribute("Z", m_pCurrentEffect->m_vColor_End.z);
		element->SetAttribute("W", m_pCurrentEffect->m_vColor_End.w);
		node->LinkEndChild(element);

		element = document->NewElement("Color_Lerp");
		element->SetAttribute("Lerp", m_pCurrentEffect->m_bColor_Lerp);
		node->LinkEndChild(element);

		element = document->NewElement("LifeTime");
		element->SetAttribute("LifeTime", m_pCurrentEffect->m_fLifeTime);
		node->LinkEndChild(element);
	}

	node = document->NewElement("UV_Variables");
	root->LinkEndChild(node);
	{
		element = document->NewElement("UV_Speed");
		element->SetAttribute("X", m_pCurrentEffect->m_vUV_Speed.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_vUV_Speed.y);
		node->LinkEndChild(element);

		element = document->NewElement("UV_Offset");
		element->SetAttribute("X", m_pCurrentEffect->m_Variables.vUV_Offset.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_Variables.vUV_Offset.y);
		node->LinkEndChild(element);

		element = document->NewElement("UV_Direction");
		element->SetAttribute("X", m_pCurrentEffect->m_Variables.vUV_Direction.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_Variables.vUV_Direction.y);
		node->LinkEndChild(element);
		
		element = document->NewElement("Is_Sequence");
		element->SetAttribute("IsSequence", m_pCurrentEffect->m_IsSequence);
		node->LinkEndChild(element);

		element = document->NewElement("UV_TileCount");
		element->SetAttribute("X", m_pCurrentEffect->m_Variables.vUV_TileCount.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_Variables.vUV_TileCount.y);
		node->LinkEndChild(element);

		element = document->NewElement("UV_TileIndex");
		element->SetAttribute("X", m_pCurrentEffect->m_Variables.vUV_TileIndex.x);
		element->SetAttribute("Y", m_pCurrentEffect->m_Variables.vUV_TileIndex.y);
		node->LinkEndChild(element);
	}

	node = document->NewElement("Intensity");
	root->LinkEndChild(node);
	{
		element = document->NewElement("Bloom");
		element->SetAttribute("Intensity", m_pCurrentEffect->m_Intensity.fBloom);
		node->LinkEndChild(element);

		element = document->NewElement("Radial");
		element->SetAttribute("Intensity", m_pCurrentEffect->m_Intensity.fRadial);
		node->LinkEndChild(element);
	}

	document->SaveFile(m_pUtils->ToString(strPath).c_str());

	return S_OK;
}

HRESULT CEffectTool::Load()
{
	Reset();

	wstring strPath = TEXT("../Bin/Resources/Effects/EffectData/") + m_strSelectedDataFile;

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
	tinyxml2::XMLError error = document->LoadFile(m_pUtils->ToString(strPath).c_str());
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
		m_pCurrentEffect->m_fLifeTime = element->FloatAttribute("LifeTime");
	}

	node = node->NextSiblingElement();
	{
		tinyxml2::XMLElement* element = nullptr;

		element = node->FirstChildElement();
		m_pCurrentEffect->m_vUV_Speed.x = element->FloatAttribute("X");
		m_pCurrentEffect->m_vUV_Speed.y = element->FloatAttribute("Y");

		element = element->NextSiblingElement();
		m_pCurrentEffect->m_Variables.vUV_Offset.x = element->FloatAttribute("X");
		m_pCurrentEffect->m_Variables.vUV_Offset.y = element->FloatAttribute("Y");

		element = element->NextSiblingElement();
		m_pCurrentEffect->m_Variables.vUV_Direction.x = element->FloatAttribute("X");
		m_pCurrentEffect->m_Variables.vUV_Direction.y = element->FloatAttribute("Y");

		element = element->NextSiblingElement();
		m_pCurrentEffect->m_IsSequence = element->BoolAttribute("IsSequence");

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
		m_pCurrentEffect->m_Intensity.fRadial = element->FloatAttribute("Intensity");
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
	CCamera::CAMERADESC tCameraDesc;

	tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	tCameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	tCameraDesc.fNear = 0.2f;
	tCameraDesc.fFar = 1200.0f;
	tCameraDesc.TransformDesc.fRotationPerSec = 10.0f;
	tCameraDesc.TransformDesc.fSpeedPerSec = 10.0f;

	CGameObject* pCamera = CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free", &tCameraDesc);

	m_pCamera = dynamic_cast<CCamera_Free*>(pCamera);

	return S_OK;
}

CEffectTool* CEffectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffectTool* pInstance = new CEffectTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
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
