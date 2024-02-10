#include "stdafx.h"
#include "Level_Studio.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include <filesystem>
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "StaticModel.h"
#include "UI.h"
#include "UI_Manager.h"
#include "Camera_Lobby.h"
#include <filesystem>
#include "QuadTreeMgr.h"
#include "Engine_Defines.h"
#include "Player_Select.h"
#include "UI_Tool.h"
#include "UI_Lobby.h"
#include "UI_Lobby_EntranceServer_Button.h"
#include "UI_Lobby_NickNameChange.h"
#include "Pool.h"

#include "Esther_Bahuntur_Cut.h"
#include "Esther_Way_Cut.h"
#include "Esther_Silian_Cut.h"

CLevel_Studio::CLevel_Studio(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Studio::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Esther()))
		return E_FAIL;

	for (size_t i = 0; i < ESTHER::_END; i++)
	{
		m_bCutStart[i] = false;
	}

	return S_OK;
}

HRESULT CLevel_Studio::Tick(const _float& fTimeDelta)
{
	Start_Esther_Cut();

	if (true == m_bCutStart[SA])
	{
		m_pSilian->Tick(fTimeDelta);
	}
	if (true == m_bCutStart[WY])
	{
		m_pWay->Tick(fTimeDelta);
	}
	if (true == m_bCutStart[BT])
	{
		m_pBahuntur->Tick(fTimeDelta);
	}
	
	return S_OK;
}

HRESULT CLevel_Studio::LateTick(const _float& fTimeDelta)
{
	if (true == m_bCutStart[SA])
	{
		m_pSilian->LateTick(fTimeDelta);
	}
	if (true == m_bCutStart[WY])
	{
		m_pWay->LateTick(fTimeDelta);
	}
	if (true == m_bCutStart[BT])
	{
		m_pBahuntur->LateTick(fTimeDelta);
	}
	
	return S_OK;
}

HRESULT CLevel_Studio::Render_Debug()
{
	
	return S_OK;
}

HRESULT CLevel_Studio::Exit()
{
	return S_OK;
}

HRESULT CLevel_Studio::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = Vec4(0.239f, -0.667f, 0.705f, 0.f);
	LightDesc.vDirection.Normalize();

	LightDesc.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = Vec4(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.vSpecular = Vec4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Vec3 vLook = LightDesc.vDirection;
	vLook.Normalize();

	Vec3 vOffset = Vec3(-117.04f, 110.27f, 36.20f);

	pGameInstance->Ready_StaticLightMatrix(vOffset, vLook);
	vOffset.Normalize();
	vOffset *= 30.0f;
	pGameInstance->Ready_LightMatrix(vOffset, vLook);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Studio::Ready_Esther()
{
	CGameInstance* pGameInstace = GET_INSTANCE(CGameInstance);

	{
		CEsther_Cut::ESTHERCUTDESC CutDesc;
		CutDesc.pLeaderPlayer = nullptr;
		CutDesc.pOwnerEsther = nullptr;
		m_pWay = static_cast<CEsther_Way_Cut*>(pGameInstace->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Way_Cut"), &CutDesc));
	}
	
	{
		CEsther_Cut::ESTHERCUTDESC CutDesc;
		CutDesc.pLeaderPlayer = nullptr;
		CutDesc.pOwnerEsther = nullptr;
		m_pSilian = static_cast<CEsther_Silian_Cut*>(pGameInstace->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Silian_Cut"), &CutDesc));
	}

	{
		CEsther_Cut::ESTHERCUTDESC CutDesc;
		CutDesc.pLeaderPlayer = nullptr;
		CutDesc.pOwnerEsther = nullptr;
		m_pBahuntur = static_cast<CEsther_Bahuntur_Cut*>(pGameInstace->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Bahuntur_Cut"), &CutDesc));
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Studio::Start_Esther_Cut()
{
	if (true == m_pSilian->Is_Finished())
	{
		m_bCutStart[SA] = false;
	}
	if (true == m_pWay->Is_Finished())
	{
		m_bCutStart[WY] = false;
	}
	if (true == m_pBahuntur->Is_Finished())
	{
		m_bCutStart[BT] = false;
	}

	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::Q))
	{
		m_bCutStart[SA] = true;
		m_pSilian->Ready();
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::W))
	{
		m_bCutStart[WY] = true;
		m_pWay->Ready();
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::E))
	{
		m_bCutStart[BT] = true;
		m_pBahuntur->Ready();
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::O))
	{
		m_pBahuntur->Set_ShotState(true);
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::P))
	{
		m_pBahuntur->Set_ShotState(false);
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::K))
	{
		m_pBahuntur->Set_ActionFrame(true);
		m_pBahuntur->Set_TimeFrame(false);
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::L))
	{
		m_pBahuntur->Set_ActionFrame(false);
		m_pBahuntur->Set_TimeFrame(true);
	}

	return S_OK;
}

CLevel_Studio* CLevel_Studio::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Studio* pInstance = new CLevel_Studio(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Studio");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Studio::Free()
{
	__super::Free();
}
