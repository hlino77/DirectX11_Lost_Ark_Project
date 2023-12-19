#include "stdafx.h"
#include "UI_Tool.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include <filesystem>
#include "UI_Manager.h"
#include "UI_ChatFrame.h"
#include "UI_ChatWriteFrame.h"
#include "UI_ChatLanguageIcon.h"
#include "UI_ChatSmallerButton.h"
#include "UI_IdentityGN_MainFrame.h"

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

	if ((KEY_HOLD(KEY::CTRL)) && KEY_TAP(KEY::U))
	{
		_int iA = 0;
		m_bToolMode = !m_bToolMode;
	}
	if (m_bToolMode)
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
	m_UIList_CurrentLevel = *pUIManager->Get_UIList(eLevel);

	Safe_Release(pUIManager);
	return S_OK;
}

void CUI_Tool::UI_Tool()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Tool_Manager");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;

	if (ImGui::Button("Add_CurrentLevelUI"))
	{
		Reset_CurrentUI();

		m_iCurrentLevel = CGameInstance::GetInstance()->Get_CurrLevelIndex();
		CUI_Manager::GetInstance()->ObjectManager_to_UIManager((LEVELID)m_iCurrentLevel);

		m_UIList_CurrentLevel = *CUI_Manager::GetInstance()->Get_UIList((LEVELID)m_iCurrentLevel);
		if (nullptr != &m_UIList_CurrentLevel)
		{
			MSG_BOX("성공");
			m_bHierachy = true;
		}
		else if (nullptr == &m_UIList_CurrentLevel)
		{
			MSG_BOX("실패");
			m_bHierachy = false;
		}
		
	}
	UI_Hierachy();

	Modify_UIPannel();

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
	if (!m_bHierachy)
		return;

	ImGui::Begin("Hierachy");
	ImGui::BeginListBox("UI_List");

	m_UIList_CurrentLevel = *CUI_Manager::GetInstance()->Get_UIList((LEVELID)m_iCurrentLevel);
	if (nullptr == &m_UIList_CurrentLevel)
		return;
	
	_uint iIndex = 0;
	for (auto iter : m_UIList_CurrentLevel)
	{
		if(ImGui::Selectable(CGameInstance::GetInstance()->wstring_to_string(iter->Get_UITag()).c_str(), iIndex == m_iCurrIndex))
		{
			m_pCurrentUI = iter;
			m_iCurrIndex = iIndex;
		}
		++iIndex;
	}
	ImGui::EndListBox();


	ImGui::BeginListBox("Part_List");
	if (nullptr != m_pCurrentUI)
	{
		m_vecUIPart_CurrentLevel = m_pCurrentUI->Get_UIParts();
	}
	if (nullptr != &m_vecUIPart_CurrentLevel)
	{
		_int iPartIndex = 0;
		for (auto iter : m_vecUIPart_CurrentLevel)
		{
			if (ImGui::Selectable(CGameInstance::GetInstance()->wstring_to_string(iter->Get_UITag()).c_str(), iPartIndex == m_iCurrentPartIndex))
			{
				m_pPartUI = iter;
				if ((nullptr != m_pBeforePartUI)&&(m_pPartUI != m_pBeforePartUI))
					m_pBeforePartUI->Set_ToolMode(false);
				m_pBeforePartUI = iter;
				m_pPartUI->Set_ToolMode(true);
				m_iCurrentPartIndex = iPartIndex;
			}
			++iPartIndex;
		}
	}

	ImGui::EndListBox();
	ImGui::End();
}

void CUI_Tool::UI_Inspector()
{
}

void CUI_Tool::Reset_CurrentUI()
{

	m_UIList_CurrentLevel.clear();

	m_vecUIPart_CurrentLevel.clear();
}

void CUI_Tool::Change_Mode()
{
}

void CUI_Tool::Modify_UIPannel()
{
	if (nullptr == m_pPartUI)
		return;
	ImGui::SetNextWindowPos(ImVec2(g_iWinSizeX - 300.f, 310.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 590), ImGuiCond_Always);
	ImGui::Begin(CGameInstance::GetInstance()->wstring_to_string(m_pPartUI->Get_UITag()).c_str());

	if (ImGui::Button("Change_X"))
	{
		m_eMode = UIMODE_X;
		m_strModeName = L"Change_X";
	}
	ImGui::SameLine();
	if (ImGui::Button("Change_Y"))
	{
		m_eMode = UIMODE_Y;
		m_strModeName = L"Change_Y";
	}
	if (ImGui::Button("Change_L"))
	{
		m_eMode = UIMODE_L;
		m_strModeName = L"Change_L";
	}
	ImGui::SameLine();
	if (ImGui::Button("Change_R"))
	{
		m_eMode = UIMODE_R;
		m_strModeName = L"Change_R";
	}
	if (ImGui::Button("Change_T"))
	{
		m_eMode = UIMODE_T;
		m_strModeName = L"Change_T";
	}
	ImGui::SameLine();
	if (ImGui::Button("Change_B"))
	{
		m_eMode = UIMODE_B;
		m_strModeName = L"Change_B";
	}
	if (ImGui::Button("Move_UI"))
	{
		m_eMode = UIMODE_MOVE;
		m_strModeName = L"Move_UI";
	}

	ImGui::InputText("##FileName : ", m_szFileName, MAX_PATH);

	if (ImGui::Button("Save CurrentUI"))
	{
		if (nullptr == m_pCurrentUI)
			return;
		_bool bResult;
		if (MessageBox(NULL, L"저장", L"취소", MB_YESNO | MB_ICONQUESTION))
		{
			if (nullptr == m_pCurrentUI)
				return;
			string strFilePath = "../Bin/Resources/Textures/UI/Binary_UIDesc/";
			string strFileExtention = ".dat";
			strFilePath += m_szFileName;
			strFilePath += strFileExtention;
			Save_UIData(CGameInstance::GetInstance()->string_to_wstring(strFilePath));
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load CurrentUI"))
	{
		if (nullptr == m_pCurrentUI)
			return;
		_bool bResult;
		if (bResult = MessageBox(NULL, L"불러오기", L"취소", MB_YESNO | MB_ICONQUESTION))
		{
			if (bResult == IDNO)
				return;
			string strFilePath = "../Bin/Resources/Textures/UI/Binary_UIDesc/";
			string strFileExtention = ".dat";
			strFilePath += m_szFileName;
			strFilePath += strFileExtention;
			Load_UIData(CGameInstance::GetInstance()->string_to_wstring(strFilePath));
		}
	}
	ImGui::BeginChild("UI_Modify_Controller");
	Modify_UIController();
	ImGui::EndChild();

	ImGui::End();
}

void CUI_Tool::Modify_UIController()
{
	ImGui::Text("");
	ImGui::Text(CGameInstance::GetInstance()->wstring_to_string(m_strModeName.c_str()).c_str(), " Mode");
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_float	MouseMoveX, MouseMoveY;
	MouseMoveX = CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);
	MouseMoveY = CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_Y);

	ImGui::Text("Mouse moving Length:\t");
	ImGui::DragFloat("##MouseMoveX", &MouseMoveX);

	Vec2	vPos;
	if (nullptr != m_pPartUI)
	{
		vPos.x = m_pPartUI->Get_UIDesc().fX;
		vPos.y = m_pPartUI->Get_UIDesc().fY;
	}
	ImGui::Text("UI Pos");
	ImGui::Text("Set UI Pos");
	ImGui::DragFloat2("##Set UI Pos :", m_fPos, 0.01f);
	ImGui::Text("UI Pos Origin");
	ImGui::DragFloat2("##UI Pos Origin :", &vPos.x, vPos.y);
	if (ImGui::Button("Set Position"))
	{
		Set_Pos((Vec2)m_fPos);
	}

	Vec2	vSize;
	if (nullptr != m_pPartUI)
	{
		vSize.x = m_pPartUI->Get_UIDesc().fSizeX;
		vSize.y = m_pPartUI->Get_UIDesc().fSizeY;
	}
	ImGui::Text("UI Size");
	ImGui::Text("UI Set Size");
	ImGui::DragFloat2("##UI Set Size :", m_fSize, 0.01f);
	ImGui::Text("UI Origin Size");
	ImGui::DragFloat2("##UI Origin Size :", &vSize.x, vSize.y);
	if (ImGui::Button("Set Size"))
	{
		Set_Size(m_fSize[0], m_fSize[1]);
	}

	_float	fAlpha;
	if (nullptr != m_pPartUI)
		fAlpha  = m_pPartUI->Get_Alpha();
	ImGui::Text("Change Alpha");
	ImGui::DragFloat("##Change Alpha : ", &m_fAlpha, 0.01f);
	ImGui::Text("Origin Alpha");
	ImGui::DragFloat("##Origin Alpha : ", &fAlpha);
	if (ImGui::Button("Change Alpha"))
	{
		Set_Alpha(m_fAlpha);
	}
	
	//m_vColor
	Vec4	vColor;
	if (nullptr != m_pPartUI)
		vColor = m_pPartUI->Get_Color();
	ImGui::Text("Change Color ");
	ImGui::DragFloat4("##Change Color : ", m_fColor, 0.01f);
	ImGui::Text("Origin Color ");
	ImGui::DragFloat4("##Origin Color : ", &vColor.x, vColor.y, vColor.z, vColor.w);
	if (ImGui::Button("Change Color"))
	{
		Set_Color((Vec4)m_fColor);
	}
	
	if ((nullptr != m_pPartUI)&&(TEXT("IdentityGN_MainFrame") == m_pPartUI->Get_UITag()))
		static_cast<CUI_IdentityGN_MainFrame*>(m_pPartUI)->Set_Angle(m_fAngle);
	ImGui::Text("Change Angle ");
	ImGui::DragFloat("##Change Angle : ", &m_fAngle, 0.1f);

	switch (m_eMode)
	{
	case UIMODE_X:
		Change_SizeX(MouseMoveX);
		break;
	case UIMODE_Y:
		Change_SizeY(MouseMoveX);
		break;
	case UIMODE_L:
		Change_SizeL(MouseMoveX);
		break;
	case UIMODE_R:
		Change_SizeR(MouseMoveX);
		break;
	case UIMODE_T:
		Change_SizeT(MouseMoveX);
		break;
	case UIMODE_B:
		Change_SizeB(MouseMoveX);
		break;
	case UIMODE_MOVE:
		Move_UI(pt);
		break;
	}
}

void CUI_Tool::Change_SizeX(_float MMX)
{
	if (nullptr == m_pPartUI)
		return;
	if((m_pPartUI->Get_Pick())&&(KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Change_SizeX(MMX);
}

void CUI_Tool::Change_SizeY(_float MMY)
{
	if (nullptr == m_pPartUI)
		return;
	if ((m_pPartUI->Get_Pick()) && (KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Change_SizeY(MMY);
}

void CUI_Tool::Change_SizeR(_float MMX)
{
	if (nullptr == m_pPartUI)
		return;
	if ((m_pPartUI->Get_Pick()) && (KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Change_SizeR(MMX);
}

void CUI_Tool::Change_SizeL(_float MMX)
{
	if (nullptr == m_pPartUI)
		return;
	if ((m_pPartUI->Get_Pick()) && (KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Change_SizeL(MMX);
}

void CUI_Tool::Change_SizeT(_float MMY)
{
	if (nullptr == m_pPartUI)
		return;
	if ((m_pPartUI->Get_Pick()) && (KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Change_SizeT(MMY);
}

void CUI_Tool::Change_SizeB(_float MMY)
{
	if (nullptr == m_pPartUI)
		return;
	if ((m_pPartUI->Get_Pick()) && (KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Change_SizeB(MMY);
}

void CUI_Tool::Set_Size(_float fSizeX, _float fSizeY)
{
	if (nullptr == m_pPartUI)
		return;
	m_pPartUI->Set_Size(fSizeX, fSizeY);
}

void CUI_Tool::Set_Pos(Vec2 vPos)
{
	if (nullptr == m_pPartUI)
		return;
	m_pPartUI->Set_Pos(vPos);
}

void CUI_Tool::Move_UI(POINT pt)
{
	if (nullptr == m_pPartUI)
		return;
	if ((m_pPartUI->Get_Pick()) && (KEY_HOLD(KEY::LBTN)))
		m_pPartUI->Move_UI(pt);
}

void CUI_Tool::Set_Alpha(_float fAlpha)
{
	if (nullptr == m_pPartUI)
		return;
	m_pPartUI->Set_Alpha(fAlpha);
}

void CUI_Tool::Set_Color(Vec4 vColor)
{
	if (nullptr == m_pPartUI)
		return;
	m_pPartUI->Set_Color(vColor);
}

void CUI_Tool::Save_UIData(wstring _strFolderPath)
{
	if (nullptr == m_pCurrentUI)
		return;

	m_vecUIPart_CurrentLevel = m_pCurrentUI->Get_UIParts();

	auto path = filesystem::path(_strFolderPath);
	shared_ptr<CAsFileUtils>file = make_shared<CAsFileUtils>();
	file->Open(_strFolderPath, FileMode::Write);

	filesystem::create_directory(path.parent_path());
	CUI::UIDESC	UIDesc;
	for (auto& iter : m_vecUIPart_CurrentLevel)
	{
		file->Write<CUI::UIDESC>(iter->Get_UIDesc());
	}
	MSG_BOX("Save Success");
}

void CUI_Tool::Load_UIData(wstring _strFolderPath)
{
	if (nullptr == m_pCurrentUI)
		return;

	m_vecUIPart_CurrentLevel = m_pCurrentUI->Get_UIParts();

	shared_ptr<CAsFileUtils>file = make_shared<CAsFileUtils>();
	file->Open(_strFolderPath, FileMode::Read);
	CUI::UIDESC	UIDesc;
	for (auto& iter : m_vecUIPart_CurrentLevel)
	{
		UIDesc = file->Read<CUI::UIDESC>();
		iter->Set_UIDesc(UIDesc);
	}
	MSG_BOX("Load Success");
}

void CUI_Tool::Free()
{
	__super::Free();
	Safe_Delete(m_hWnd);
	m_hWnd = nullptr;

	for (auto& iter : m_UIList_CurrentLevel)
	{
		Safe_Release(iter);
	}
	m_UIList_CurrentLevel.clear();

	for (auto& iter : m_vecUIPart_CurrentLevel)
	{
		Safe_Release(iter);
	}
	m_vecUIPart_CurrentLevel.clear();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
