#include "stdafx.h"
#include "EffectTool.h"
//#include "ViewMediator.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"
#include "Transform.h"

CEffectTool::CEffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

HRESULT CEffectTool::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	return S_OK;
}

HRESULT CEffectTool::Tick()
{
	Input();

	ImGui::Begin("Effect Tool");

	InfoView();
	TreeGroups();

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
	if (ImGui::GetIO().WantCaptureMouse)
		return;
}

void CEffectTool::InfoView()
{
	ImGui::Text("This window has some useful function for Effect Tool.");
	ImGui::NewLine();

	const POINT& p = m_pGameInstance->GetMousePos();
	ImGui::Text("Mouse Viewport Position : %d, %d", p.x, p.y);
	ImGui::NewLine();
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

			Tex_Mask();
			/*Tex_Noise();
			Tex_Gradient();
			Tex_Ex();*/
		}
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
}
