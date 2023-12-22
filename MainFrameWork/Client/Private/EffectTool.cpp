#include "stdafx.h"
#include "EffectTool.h"
//#include "ViewMediator.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"
#include "Transform.h"
#include "Utils.h"

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

	return S_OK;
}

HRESULT CEffectTool::Tick()
{
	Input();

	ImGui::Begin("Effect Tool");

	InfoView();
	//Categories();
	SelectBaseMaterial();
	SelectNoiseTexture();
	SelectMaskTexture();
	SelectEmissiveTexture();
	SelectDissolveTexture();

	ImGui::End();

	return S_OK;
}

HRESULT CEffectTool::LateTick()
{

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
		m_szCurrentCategory = "";
	ImGui::SameLine();
	if (ImGui::RadioButton("Texture", &m_iCurrentEffectType, 1))
		m_szCurrentCategory = "";
}

void CEffectTool::Categories()
{
}

void CEffectTool::SelectBaseMaterial()
{
	ImGui::Text("Category :");
	ImGui::SameLine();
	if (!m_iCurrentEffectType) // Meshes
	{
		if (ImGui::BeginCombo(" ", m_szCurrentCategory))
		{
			for (int n = 0; n < m_vecMeshes.size(); ++n)
			{
				_bool isSelected = (m_szCurrentCategory == m_vecMeshes[n].first); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(m_vecMeshes[n].first, isSelected))
					m_szCurrentCategory = m_vecMeshes[n].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		m_strCurrentPath = TEXT("../Bin/Resources/Effects/FX_Meshes/");
	}
	else // Textures
	{
		if (ImGui::BeginCombo(" ", m_szCurrentCategory))
		{
			for (int n = 0; n < m_vecTextures.size(); ++n)
			{
				_bool isSelected = (m_szCurrentCategory == m_vecTextures[n].first);
				if (ImGui::Selectable(m_vecTextures[n].first, isSelected))
					m_szCurrentCategory = m_vecTextures[n].first;
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		m_strCurrentPath = TEXT("../Bin/Resources/Effects/FX_Textures/");
	}

	m_strCurrentPath += m_pUtils->ToWString(string(m_szCurrentCategory)) + TEXT("/");

	ImGui::NewLine();
}

void CEffectTool::SelectNoiseTexture()
{
}

void CEffectTool::SelectMaskTexture()
{
}

void CEffectTool::SelectEmissiveTexture()
{
}

void CEffectTool::SelectDissolveTexture()
{
}

void CEffectTool::ShowTextures(wstring strPath)
{
	
}

void CEffectTool::Meshes()
{
}

void CEffectTool::Textures()
{
}

void CEffectTool::TreeGroups()
{
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

	ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);							// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

	if (ImGui::TreeNode("Tool"))
	{
		if (ImGui::ImageButton("", size))
		{
			//ImGuiIO& io = ImGui::GetIO();
			
			ImGui::Image(m_vecMaskTextureIDs[m_Item_Current], ImVec2(96.0f, 96.0f), uv0, uv1, tint_col, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			ImGui::SameLine();

			//Tex_Mask();
			/*Tex_Noise();
			Tex_Gradient();
			Tex_Ex();*/
		}
		ImGui::TreePop();
		//ImGui::Spacing();
	}
}

void CEffectTool::Tex_Mask()
{
	ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);							// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

	if (ImGui::TreeNode("Mask"))
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				_int iIndex = 8 * i + j;
				if (iIndex >= m_vecMaskTextureIDs.size())
					break;

				ImGui::PushID(iIndex);

				if (ImGui::ImageButton("", m_vecMaskTextureIDs[iIndex], size))
				{
					//m_pTextureMask = m_vecMaskTextureIDs[iIndex];
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}
	}
}

void CEffectTool::Tex_Noise()
{
	ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);							// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

	if (ImGui::TreeNode("Noise"))
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				_int iIndex = 8 * i + j;
				if (iIndex >= m_vecNoiseTextureIDs.size())
					break;

				ImGui::PushID(iIndex);

				if (ImGui::ImageButton("", m_vecNoiseTextureIDs[iIndex], size))
				{
					m_pTextureNoise = m_vecNoiseTextureIDs[iIndex];
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}
	}
}

void CEffectTool::Tex_Gradient()
{
	ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);							// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
	
	if (ImGui::TreeNode("Gredient"))
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				_int iIndex = 8 * i + j;
				if (iIndex >= m_vecGradientTextureIDs.size())
					break;

				ImGui::PushID(iIndex);

				if (ImGui::ImageButton("", m_vecGradientTextureIDs[iIndex], size))
				{
					m_pTextureGradient = m_vecGradientTextureIDs[iIndex];
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}
	}
}

void CEffectTool::Tex_Ex()
{
	ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);							// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
	
	if (ImGui::TreeNode("Extra"))
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				_int iIndex = 8 * i + j;
				if (iIndex >= m_vecMaskTextureIDs.size())
					break;

				ImGui::PushID(iIndex);

				if (ImGui::ImageButton("", m_vecExtraTextureIDs[iIndex], size))
				{
					m_pTextureEx0 = m_vecExtraTextureIDs[iIndex];
				}
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();
		}
	}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

HRESULT CEffectTool::LoadMeshes()
{
	vector<const _char*> vecItems;
	wstring strDirectory = TEXT("../Bin/Resources/Effects/FX_Meshes/");

	if (fs::exists(strDirectory) && fs::is_directory(strDirectory))
	{
		for (const auto& category : fs::directory_iterator(strDirectory))
		{
			if (category.is_directory())
			{
				const _char* szKey = s2c(category.path().stem().generic_string());
				vector<const _char*> meshes;
				
				for (const auto& mesh : fs::directory_iterator(category))
				{
					const _char* szMeshFileName = s2c(mesh.path().stem().generic_string());
					if (mesh.is_regular_file() && strcmp("desktop", szMeshFileName))
					{
						meshes.push_back(s2c(mesh.path().stem().generic_string()));
					}
				}
				m_vecMeshes.push_back(make_pair(szKey, meshes));
			}
		}
	}

	return S_OK;
}

HRESULT CEffectTool::LoadTextures()
{
	vector<const _char*> vecItems;
	wstring strDirectory = TEXT("../Bin/Resources/Effects/FX_Textures/");
	
	if (fs::exists(strDirectory) && fs::is_directory(strDirectory))
	{
		for (const auto& category : fs::directory_iterator(strDirectory))
		{
			if (category.is_directory())
			{
				const _char* szKey = s2c(category.path().stem().generic_string());
				vector<const _char*> textures;

				for (const auto& texture : fs::directory_iterator(category))
				{
					const _char* szTextureFileName = s2c(texture.path().stem().generic_string());
					if (texture.is_regular_file() && strcmp("desktop", szTextureFileName))
					{
						textures.push_back(s2c(texture.path().stem().generic_string()));
					}
				}
				m_vecTextures.push_back(make_pair(szKey, textures));
			}
		}
	}

	return S_OK;
}

void CEffectTool::PlaceObject(const LAYER_TYPE& eLayerTag, const wstring& strPrototypeTag, const Vec3& vPickPosition)
{
	if (eLayerTag == LAYER_TYPE::LAYER_END) return;

	//const wstring strPrototypeTag = TEXT("Prototype_GameObject_") + strObjectTag;
	CGameObject* pGameObject = m_pGameInstance->Add_GameObject(LEVEL_TOOL, (_int)LAYER_TYPE::LAYER_EFFECT, strPrototypeTag);

	//pGameObject->Get_TransformCom()->Translate(vPickPosition);
	pGameObject->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION,  vPickPosition);
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
