#include "stdafx.h"
#include "Level_Tool.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI.h"

#include "GameInstance.h"
#include "EffectTool.h"

CLevel_Tool::CLevel_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool::Initialize()
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_Tools()))
		return E_FAIL;

	//if (FAILED(Ready_SoundTrack()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Tick(_float fTimeDelta)
{
	if (!m_IsImGUIReady)
	{
		m_IsImGUIReady = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//
	if (m_pEffectTool)
		m_pEffectTool->Tick();

	return S_OK;
}

HRESULT CLevel_Tool::LateTick(_float fTimeDelta)
{
	


	return S_OK;
}

HRESULT CLevel_Tool::Exit()
{
	CGameInstance::GetInstance()->StopSoundAll();


	return S_OK;
}

HRESULT CLevel_Tool::Render_Debug()
{
	if (m_IsImGUIReady)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_pEffectTool->DebugRender();
	}

	return S_OK;
}

HRESULT CLevel_Tool::Ready_SoundTrack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_BackGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Tools()
{
	m_pEffectTool = CEffectTool::Create(m_pDevice, m_pContext);
	//m_pMediator->SetPrefabsView(m_pPrefabsView);

	return S_OK;
}

CLevel_Tool * CLevel_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Tool*	pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{
	Safe_Release(m_pEffectTool);

	Super::Free();

	RELEASE_INSTANCE(CGameInstance);
}
