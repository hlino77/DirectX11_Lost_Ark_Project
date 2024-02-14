#include "stdafx.h"
#include "ClientEvent_BernStart.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Camera_Free.h"
#include "EventMgr.h"
#include "Player_Doaga.h"
#include "Player_Destroyer.h"
#include "Player_Slayer.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Controller_SP.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"
#include "Player_Controller_GN.h"
#include "UI_Manager.h"
#include "AsUtils.h"
#include "AsFileUtils.h"
#include <filesystem>

CClientEvent_BernStart::CClientEvent_BernStart(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CClientEvent(iID, pDevice, pContext)
{
}

HRESULT CClientEvent_BernStart::Initialize()
{
	Load_CameraData();

	m_fCameraSpeed = 1.0f;

	return S_OK;
}

void CClientEvent_BernStart::Enter_Event()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSound_Manager::GetInstance()->PlayBGM_AddChannel(L"CastleBern", L"BGM", L"CastleBern.wav", 0.5f);
	//앞에 2초 자른 버전
	//CSound_Manager::GetInstance()->PlayBGM_AddChannel(L"CastleBern", L"BGM", L"CastleBern_Edit.wav", 0.5f);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	CCamera_Player* pCamera = pPlayer->Get_Camera();

	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
	
	pCamera->Set_TargetPos(Vec3(159.4f, 5.33f, 99.75f));
	pCamera->Set_Offset(Vec3(-1.0f, 1.0f, 0.2f));
	pCamera->ZoomInOut(80.0f, 0.5f);
	pCamera->Set_FadeIntensity(1.0f);
	pCamera->Set_FadeInOut(2.0f, false, Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	m_iCurrPos = 1;
	m_fTimeRatio = 0.0f;
	m_iCurrScene = 0;
	m_bFadeIn = false;

	m_iState = (_uint)EVENTSTATE::READY;

	Update_Camera(0.0f);

	if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Control_Active(false);
	}
	else if (TEXT("WR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Control_Active(false);
	}
	else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Control_Active(false);
	}
	else if (TEXT("SP") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Control_Active(false);
	}

	CUI_Manager::GetInstance()->Set_UIs_Active(false, LEVELID::LEVEL_BERN);
	CUI_Manager::GetInstance()->Set_RenderNickName(false, LEVELID::LEVEL_BERN);
	CUI_Manager::GetInstance()->Set_RenderNickName(false, LEVELID::LEVEL_STATIC);

	Safe_Release(pGameInstance);
}

void CClientEvent_BernStart::Exit_Event()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	CCamera_Player* pCamera = pPlayer->Get_Camera();

	pCamera->Set_ResetSpeed(2.0f);
	pCamera->Set_Mode(CCamera_Player::CameraState::RESET);

	if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Control_Active(true);
	}
	else if (TEXT("WR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Control_Active(true);
	}
	else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Control_Active(true);
	}
	else if (TEXT("SP") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Control_Active(true);
	}

	CUI_Manager::GetInstance()->Set_UIs_Active(true, LEVELID::LEVEL_BERN);
	CUI_Manager::GetInstance()->Set_RenderNickName(true, LEVELID::LEVEL_BERN);
	CUI_Manager::GetInstance()->Set_RenderNickName(true, LEVELID::LEVEL_STATIC);

	Safe_Release(pGameInstance);
}

void CClientEvent_BernStart::Tick(_float fTimeDelta)
{
	if (m_iState == (_uint)EVENTSTATE::READY)
	{
		m_iState = (_uint)EVENTSTATE::EVENT;
		return;
	}

	Update_Camera(fTimeDelta);
}

void CClientEvent_BernStart::LateTick(_float fTimeDelta)
{

}


HRESULT CClientEvent_BernStart::Render()
{
	return S_OK;
}

void CClientEvent_BernStart::Update_Camera(_float fTimeDelta)
{
	_bool bNextScene = false;

	CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

	m_fTimeRatio += m_fCameraSpeed * fTimeDelta;
	if (m_fTimeRatio >= 1.0f)
	{
		m_fTimeRatio -= 1.0f;
		++m_iCurrPos;
		
		if (m_iCurrPos == m_CameraPos[m_iCurrScene].size() - 2)
		{
			if (m_iCurrScene == m_CameraPos.size() - 1)
			{
				CEventMgr::GetInstance()->End_Event(m_iEventID);
				return;
			}
			else
			{
				--m_iCurrPos;
				m_fTimeRatio = 1.0f;
				bNextScene = true;
			}
		}
	}

	Vec3 vPos = XMVectorCatmullRom(m_CameraPos[m_iCurrScene][m_iCurrPos - 1].vPos,
		m_CameraPos[m_iCurrScene][m_iCurrPos].vPos,
		m_CameraPos[m_iCurrScene][m_iCurrPos + 1].vPos,
		m_CameraPos[m_iCurrScene][m_iCurrPos + 2].vPos,
		m_fTimeRatio);

	Vec3 vLook = XMVectorCatmullRom(m_CameraPos[m_iCurrScene][m_iCurrPos - 1].vLook,
		m_CameraPos[m_iCurrScene][m_iCurrPos].vLook,
		m_CameraPos[m_iCurrScene][m_iCurrPos + 1].vLook,
		m_CameraPos[m_iCurrScene][m_iCurrPos + 2].vLook,
		m_fTimeRatio);

	m_fCameraSpeed = CAsUtils::Lerpf(m_CameraPos[m_iCurrScene][m_iCurrPos].fSpeed, m_CameraPos[m_iCurrScene][m_iCurrPos].fSpeed, m_fTimeRatio);

	if (m_iCurrPos == m_CameraPos[m_iCurrScene].size() - 3 && m_bFadeIn == false && m_fTimeRatio > 0.8f)
	{
		if (m_iCurrScene != 3)
			pCamera->Set_FadeInOut(2.0f, true, Vec4(1.0f, 1.0f, 1.0f, 1.0f));

		m_bFadeIn = true;
	}

	vLook.Normalize();

	Vec3 vTargetPos = vPos + vLook;
	Vec3 vOffset = vPos - vTargetPos;
	_float fLength = vOffset.Length();
	vOffset.Normalize();

	pCamera->Set_TargetPos(vTargetPos);
	pCamera->Set_Offset(vOffset);
	pCamera->Set_CameraLength(fLength);

	if (bNextScene == true)
	{
		++m_iCurrScene;
		m_iCurrPos = 1;
		m_fTimeRatio = 0.0f;
		m_bFadeIn = false;
		pCamera->Set_FadeInOut(2.0f, false, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void CClientEvent_BernStart::Load_CameraData()
{
	CAsFileUtils* pFileUtil = new CAsFileUtils();
	wstring strDirectory = TEXT("../Bin/Resources/CameraData/");
	strDirectory += L"Bern.data";

	filesystem::path Path(strDirectory);

	if (filesystem::exists(Path))
	{
		pFileUtil->Open(strDirectory, FileMode::Read);

		_uint iSize = pFileUtil->Read<_uint>();

		for (_uint i = 0; i < iSize; ++i)
		{
			wstring szName = CAsUtils::ToWString(pFileUtil->Read<string>());
			_uint iPosSize = pFileUtil->Read<_uint>();

			vector<CameraPos> PosList;

			for (_uint i = 0; i < iPosSize; ++i)
			{
				_float fSpeed = pFileUtil->Read<_float>();

				Vec3 vPos;
				vPos.x = pFileUtil->Read<_float>();
				vPos.y = pFileUtil->Read<_float>();
				vPos.z = pFileUtil->Read<_float>();

				Vec3 vLook;
				vLook.x = pFileUtil->Read<_float>();
				vLook.y = pFileUtil->Read<_float>();
				vLook.z = pFileUtil->Read<_float>();

				CameraPos tDesc;
				tDesc.fSpeed = fSpeed;
				tDesc.vPos = vPos;
				tDesc.vLook = vLook;

				PosList.push_back(tDesc);
			}

			m_CameraPos.push_back(PosList);
		}
	}
}

void CClientEvent_BernStart::Free()
{
	__super::Free();
}
