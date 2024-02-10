#include "stdafx.h"
#include "UI_DeadScene.h"
#include "GameInstance.h"
#include "UI_DeadWnd.h"
#include "UI_WatchingMode.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"

CUI_DeadScene::CUI_DeadScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_DeadScene::CUI_DeadScene(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_DeadScene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_DeadScene::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_DeadScene");

	return S_OK;
}

void CUI_DeadScene::Tick(_float fTimeDelta)
{
	Update_UIs(fTimeDelta);
}

void CUI_DeadScene::LateTick(_float fTimeDelta)
{
	Late_Update_UIs(fTimeDelta);
}

HRESULT CUI_DeadScene::Render()
{
	return S_OK;
}

HRESULT CUI_DeadScene::UI_Set()
{
	m_pDeadWnd = static_cast<CUI_DeadWnd*>(CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
		_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_UI_DeadWnd")));
	if (nullptr == m_pDeadWnd)
		return E_FAIL;
	else
		m_vecUIParts.push_back(m_pDeadWnd);

	m_pWatchingMode = static_cast<CUI_WatchingMode*>(CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(),
		_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_UI_WatchingMode")));
	if (nullptr == m_pDeadWnd)
		return E_FAIL;
	else
		m_vecUIParts.push_back(m_pDeadWnd);

	return S_OK;
}

void CUI_DeadScene::Update_UIs(_float fTimeDelta)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;

	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort);

	if (0 == m_DeadMode)
	{
		m_pDeadWnd->Update_DeadWnd(fTimeDelta, pt);
	}
	else if(1 == m_DeadMode)
	{
		m_pWatchingMode->Update_CCTV(fTimeDelta, pt);
	}
}

void CUI_DeadScene::Late_Update_UIs(_float fTimeDelta)
{
	if (0 == m_DeadMode)
	{
		m_pDeadWnd->LateUpdate_DeadWnd(fTimeDelta);
		if (true == m_pDeadWnd->Is_WatchingMode())
		{
			m_DeadMode = 1;
			m_pWatchingMode->Set_TargetPlayer();
			m_pWatchingMode->Set_WatchingMode(true);
			m_pWatchingMode->Print_TextBox();
			m_pDeadWnd->Set_Active(false);
		}
	}
	if (1 == m_DeadMode)
	{
		m_pWatchingMode->LateUpdate_CCTV(fTimeDelta);
		if (!m_pWatchingMode->Get_WatchingMode())
		{
			m_DeadMode = 0;
			m_pWatchingMode->Reset_TargetPlayer();
			m_pWatchingMode->Set_WatchingMode(false);
			m_pDeadWnd->Set_WatchingMode(false);
			m_pDeadWnd->Print_TextBox();
			m_pDeadWnd->Print_TextBoxWatching();
			m_pDeadWnd->Set_Active(true);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_FadeInOut(1.f, false);
		}
	}
}

HRESULT CUI_DeadScene::Ready_Components()
{
	return S_OK;
}

HRESULT CUI_DeadScene::Bind_ShaderResources()
{
	return S_OK;
}

CUI_DeadScene* CUI_DeadScene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_DeadScene* pInstance = new CUI_DeadScene(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_DeadScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_DeadScene::Clone(void* pArg)
{
	CUI_DeadScene* pInstance = new CUI_DeadScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_DeadScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_DeadScene::Free()
{
	__super::Free();
	m_pWatchingMode->Set_Dead(true);
	m_pDeadWnd->Set_Dead(true);
}
