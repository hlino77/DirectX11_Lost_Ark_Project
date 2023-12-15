#include "stdafx.h"
#include "UI_Tool.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_ChatFrame.h"
#include "UI_ChatWriteFrame.h"
#include "UI_ChatLanguageIcon.h"
#include "UI_ChatSmallerButton.h"
#include "UI_Chat.h"

IMPLEMENT_SINGLETON(CUI_Tool)

CUI_Tool::CUI_Tool()
{
}


HRESULT CUI_Tool::Reserve_Manager(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_hWnd = hWnd;
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::StyleColorsDark();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\batang.ttc", 15.f, NULL, io.Fonts->GetGlyphRangesKorean());

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	return S_OK;
}

HRESULT CUI_Tool::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CUI_Tool::Tick()
{
	if (KEY_AWAY(KEY::F6))
		m_bToolMode = true;
	if (KEY_AWAY(KEY::F7))
		m_bToolMode = false;
	UI_Tool();

	return S_OK;
}

HRESULT CUI_Tool::LateTick()
{
	return S_OK;
}

HRESULT CUI_Tool::Add_UI_CurrentLevel()
{
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	LEVELID eLevel = (LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex();
	pUIManager->ObjectManager_to_UIManager(eLevel);
	m_UIList_pCurrentLevel = *pUIManager->Get_pUIList(eLevel);

	Safe_Release(pUIManager);
	return S_OK;
}

void CUI_Tool::UI_Tool()
{
	if (!m_bToolMode)
		return;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Tool_Manager");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	ImGui::Text("Tool Test");
	_bool	bShowDemo = true;
	ImGui::Checkbox("Demo_Window", &bShowDemo);

	if (ImGui::Button("Test"))
	{

	}

	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CUI_Tool::Find_PickedUI()
{


}

void CUI_Tool::UI_Hierachy()
{
}

void CUI_Tool::UI_Inspector()
{
}

void CUI_Tool::Reset_CurrentUI()
{
}

void CUI_Tool::Change_Mode()
{
}

void CUI_Tool::Free()
{
	__super::Free();
	Safe_Delete(m_hWnd);
	m_hWnd = nullptr;

	for (auto& iter : m_UIList_pCurrentLevel)
	{
		Safe_Release(iter);
	}
	m_UIList_pCurrentLevel.clear();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
