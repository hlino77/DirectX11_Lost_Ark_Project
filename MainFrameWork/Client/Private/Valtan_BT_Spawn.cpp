#include "stdafx.h"
#include "Valtan_BT_Spawn.h"
#include "Boss_Valtan.h"
#include "Model.h"

#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Party.h"
#include "Player_Doaga.h"
#include "Player_Destroyer.h"
#include "Player_Slayer.h"
#include "Player_Gunslinger.h"

#include "Player_Controller_GN.h"
#include "Controller_SP.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"

#include "Transform.h"


CValtan_BT_Spawn::CValtan_BT_Spawn()
{
}

void CValtan_BT_Spawn::OnStart()
{
	__super::OnStart(0);
	m_bShoot = true;

	Init_StartScene();
}

CBT_Node::BT_RETURN CValtan_BT_Spawn::OnUpdate(const _float& fTimeDelta)
{
	if (m_fLoopTime < 4.f)
	{
		Update_StartCamera(fTimeDelta);
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Is_Render() == false && m_fLoopTime > 4.f)
	{
		m_pGameObject->Set_Render(true);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 30 && static_cast<CMonster*>(m_pGameObject)->Get_DissolveIn() == false&& m_bShoot)
	{
		m_bShoot = false;
		static_cast<CMonster*>(m_pGameObject)->Set_DissolveIn(4.f);
	}
	if (m_iCurrAnimation == 4|| m_iCurrAnimation == 3)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Lerp_ForLand(Vec3(0.f, 0.f, -1.f), 3.f, fTimeDelta);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Spawn::OnEnd()
{
	__super::OnEnd();
	
	static_cast<CMonster*>(m_pGameObject)->Set_Spawn(false);
	static_cast<CBoss*>(m_pGameObject)->Set_HpUIRender(true);

	End_Scene();
}

Vec3 CValtan_BT_Spawn::Get_EndPos()
{
	Vec3 vPos;
	_uint iPartyIndex = 0;
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	CParty* pParty = pPlayer->Get_Party();
	if (pParty != nullptr)
	{
		iPartyIndex = pParty->Get_PartyIndex(pPlayer);
	}

	if (iPartyIndex == 1)
	{
		vPos = Vec3(98.06f, 0.25f, 91.67f);
	}
	else if (iPartyIndex == 2)
	{
		vPos = Vec3(99.05f, 0.21f, 91.91f);
	}
	else if (iPartyIndex == 3)
	{
		vPos = Vec3(100.34f, 0.24f, 91.71f);
	}
	else if (iPartyIndex == 4)
	{
		vPos = Vec3(102.08f, 0.23f, 91.68f);
	}
	else
	{
		vPos = Vec3(98.06f, 0.25f, 91.67f);
	}

	return vPos;
}

void CValtan_BT_Spawn::Init_StartScene()
{
	//플레이어
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	pPlayer->Set_TargetPos(Get_EndPos());
	pPlayer->Set_State(L"Run");

	if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Control_Active(false);
		static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Get_MoveSpeedMessage(1.5f);
	}
	else if (TEXT("WR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Control_Active(false);
		static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Get_MoveSpeedMessage(1.5f);
	}
	else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Control_Active(false);
		static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Get_MoveSpeedMessage(1.5f);
	}
	else if (TEXT("SP") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Control_Active(false);
		static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Get_MoveSpeedMessage(1.5f);
	}

	//카메라

	CCamera_Player* pCamera = pPlayer->Get_Camera();

	pCamera->Set_FadeInOut(0.2f, false);

	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);

	Vec3 vTargetPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vTargetPos.y += 2.0f;
	pCamera->Set_TargetPos(vTargetPos);

	Vec3 vOffset = Vec3(-0.01f, 0.0f, -1.0f);
	pCamera->Set_Offset(vOffset);

	m_fCameraLength = 18.0f;
	pCamera->Set_CameraLength(m_fCameraLength);

}

void CValtan_BT_Spawn::Update_StartCamera(_float fTimeDelta)
{
	CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

	m_fCameraLength -= 4.0f * fTimeDelta;
	if (m_fCameraLength < 5.0f)
		m_fCameraLength = 5.0f;

	pCamera->ZoomInOut(m_fCameraLength, 0.6f);
}

void CValtan_BT_Spawn::End_Scene()
{
	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_ResetSpeed(5.0f);
	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Mode(CCamera_Player::CameraState::RESET);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

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
}

CValtan_BT_Spawn* CValtan_BT_Spawn::Create(void* pArg)
{
	CValtan_BT_Spawn* pInstance = new CValtan_BT_Spawn;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Spawn::Free()
{
	__super::Free();
}
