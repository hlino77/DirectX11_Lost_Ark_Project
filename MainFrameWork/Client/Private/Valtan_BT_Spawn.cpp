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
#include "UI_Manager.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "GameInstance.h"
#include "Effect_Particle.h"
#include "Effect_Trail.h"
#include "Sound_Manager.h"
#include "UI_Manager.h"

CValtan_BT_Spawn::CValtan_BT_Spawn()
{
}

void CValtan_BT_Spawn::OnStart()
{
	__super::OnStart(0);
	m_bShoot = true;

	Init_StartScene();

	m_iCameraSequence = 0;
	m_fCameraTurnSpeed = 0.0f;
	m_fCamShakeTimeAcc = m_fCamShakeTimeDelay = 0.23f;

	m_fLightningParticleTime = 0.05f;
	CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"RiseValtan.wav", 0.5f);
	//CGameInstance::GetInstance()->SetChannelVolume(1.f, 1.f);
	Effect_SpawnStart();
}

CBT_Node::BT_RETURN CValtan_BT_Spawn::OnUpdate(const _float& fTimeDelta)
{
	Update_StartCamera(fTimeDelta);

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
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex&& m_bSoundOn[0] == false)
	{
		m_bSoundOn[0] = true;
		CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"Scream.wav", 0.5f);
		CSound_Manager::GetInstance()->PlayBGM_AddChannel(L"Commander of Beast Valtan", L"BGM", L"Commander of Beast Valtan.wav", 0.5f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_bSoundOn[1] == false)
	{
		m_bSoundOn[1] = true;
		CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"IamCommanderOfBeastValtan.wav", 0.5f);
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
	
	for (auto& Effect : m_BlackSmoke)
	{
		dynamic_cast<CEffect_Particle*>(Effect)->ParticleEnd();
	}
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
	 
	if (m_bFirstInit == true)
		return;

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

	pCamera->Set_FadeInOut(0.1f, false);

	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);

	Vec3 vTargetPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vTargetPos.y += 2.0f;
	vTargetPos.x += 0.5f;
	pCamera->Set_TargetPos(vTargetPos);

	Vec3 vOffset = Vec3(-0.01f, 0.0f, -1.0f);
	vOffset.Normalize();
	pCamera->Set_Offset(vOffset);

	m_fCameraLength = 18.0f;
	pCamera->Set_CameraLength(m_fCameraLength);

	m_bFirstInit = true;

}

void CValtan_BT_Spawn::Update_StartCamera(_float fTimeDelta)
{
	CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

	if (m_iCameraSequence == 0)
	{
		m_fCameraLength -= 4.0f * fTimeDelta;
		if (m_fCameraLength < 5.0f)
			m_fCameraLength = 5.0f;

		pCamera->ZoomInOut(m_fCameraLength, 0.6f);

		if (m_fLoopTime > 4.0f)
		{
			Effect_Appear();
			m_iCameraSequence = 1;
		}
			

		Update_LightningParticle(fTimeDelta);
	}

	else if (m_iCameraSequence == 1)
	{
		if (m_fCameraLength > 2.0f)
		{
			m_fCameraLength -= 0.5f * fTimeDelta;

			if (m_fCameraLength < 2.0f)
				m_fCameraLength = 2.0f;

			pCamera->ZoomInOut(m_fCameraLength, 0.6f);
		}

		Vec3 vTargetPos = pCamera->Get_TargetPos();
		vTargetPos.y += 0.1f * fTimeDelta;
		if (vTargetPos.y > 3.0f)
			vTargetPos.y = 3.0f;
		pCamera->Set_TargetPos(vTargetPos);
			

		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
			m_iCameraSequence = 2;

		Update_LightningParticle(fTimeDelta);
	}

	else if (m_iCameraSequence == 2)
	{
		m_fCameraTurnSpeed += 0.2f * fTimeDelta;

		if (m_fCameraTurnSpeed > 0.6f)
		{
			m_fCameraTurnSpeed = 0.6f;
		}

		Vec3 vOffset = pCamera->Get_Offset();
		vOffset = XMVector3Rotate(vOffset, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), -m_fCameraTurnSpeed * fTimeDelta));
		pCamera->Set_Offset(vOffset);

		Vec3 vTargetPos = pCamera->Get_TargetPos();
		vTargetPos.y += 0.1f * fTimeDelta;
		if (vTargetPos.y > 3.0f)
			vTargetPos.y = 3.0f;
		pCamera->Set_TargetPos(vTargetPos);

		if (m_fCameraLength > 2.0f)
		{
			m_fCameraLength -= 0.5f * fTimeDelta;

			if (m_fCameraLength < 2.0f)
				m_fCameraLength = 2.0f;

			pCamera->ZoomInOut(m_fCameraLength, 0.6f);
		}

		if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 30)
		{
			m_iCameraSequence = 3;
		}

		Update_LightningParticle_AfterSpawn(fTimeDelta);
	}

	else if (m_iCameraSequence == 3)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vTargetPos = matWorld.Translation();
		vTargetPos.y += 2.4f;
		pCamera->Set_TargetPos(vTargetPos);

		Vec3 vLook = matWorld.Backward();
		Vec3 vRight = matWorld.Right();
		Vec3 vUp(0.0f, 1.0f, 0.0f);

		Vec3 vOffset = (vRight * 0.284f) + (vUp * -0.002f) + (vLook * 0.959f);
		vOffset.Normalize();
		pCamera->Set_Offset(vOffset);

		pCamera->Set_CameraLength(1.76f);
		m_iCameraSequence = 4;
		m_fCameraTurnSpeed = 0.1f;

		Update_LightningParticle_AfterSpawn(fTimeDelta);
	}

	else if (m_iCameraSequence == 4)
	{
		Vec3 vOffset = pCamera->Get_Offset();
		vOffset = XMVector3Rotate(vOffset, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), -m_fCameraTurnSpeed * fTimeDelta));
		pCamera->Set_Offset(vOffset);

		if (m_iCurrAnimation == 2)
		{
			m_iCameraSequence = 5;
		}

		Update_LightningParticle_AfterSpawn(fTimeDelta);
	}

	else if (m_iCameraSequence == 5)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		Vec3 vLook = matWorld.Backward();
		Vec3 vRight = matWorld.Right();
		Vec3 vUp(0.0f, 1.0f, 0.0f);

		Vec3 vCameraTargetPos = pCamera->Get_TargetPos();
		Vec3 vTargetPos = matWorld.Translation() + vRight * 0.163f + vLook * 0.251f;
		vTargetPos.y += 2.0f;
		vCameraTargetPos = Vec3::Lerp(vCameraTargetPos, vTargetPos, 5.0f * fTimeDelta);
		pCamera->Set_TargetPos(vCameraTargetPos);

		Vec3 vCameraOffset = pCamera->Get_Offset();
		Vec3 vOffset = vUp * 0.878f + vLook * 0.479f;
		vCameraOffset = Vec3::Lerp(vCameraOffset, vOffset, 5.0f * fTimeDelta);
		vCameraOffset.Normalize();
		pCamera->Set_Offset(vCameraOffset);

		if (18 < m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex))
		{
			m_iCameraSequence = 6;
		}

		Update_LightningParticle_AfterSpawn(fTimeDelta);
	}

	else if (m_iCameraSequence == 6)
	{
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vPos.y += 2.5f;
		pCamera->ZoomInOut(20.0f, 10.0f);
		m_iCameraSequence = 7;
		pCamera->Set_RadialBlur(0.4f, vPos, 0.2f, 0.1f);
		pCamera->Set_Chromatic(0.4f, vPos, 0.2f, 0.1f);
		pCamera->Cam_Shake(0.2f, 100.0f, 0.5f, 5.0f);
		m_fCamShakeTimeAcc = 0.0f;

		for (auto& Effect : m_SpawnFloor)
		{
			Effect->EffectEnd();
		}

		for (auto& Trail : m_Trails)
		{
			dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(1.0f);
		}

		Update_LightningParticle_AfterSpawn(fTimeDelta);

		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START(L"ValtanShouting", &tDesc);
	}

	else if (m_iCameraSequence == 7)
	{
	_uint iAnimFrame = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex);
	
	if (iAnimFrame < 90)
	{
		if (m_fCamShakeTimeAcc >= m_fCamShakeTimeDelay)
		{
			pCamera->Cam_Shake(0.2f, 100.0f, 0.5f, 5.0f);
			m_fCamShakeTimeAcc = 0.0f;
		}
		else
		{
			m_fCamShakeTimeAcc += fTimeDelta;
		}
	}

		if (iAnimFrame > 102)
		{
			pCamera->ZoomInOut(5.0f, 1.0f);
			m_iCameraSequence = 8;
		}

		Update_LightningParticle_AfterSpawn(fTimeDelta);
	}

	else if (m_iCameraSequence == 8)
	{
	if (m_iCurrAnimation == 3)
	{
		Vec3 vTargetPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vTargetPos.y += 2.1f;
		pCamera->Set_TargetPos(vTargetPos);

		Vec3 vOffset = Vec3(-0.01f, 0.0f, -1.0f);
		vOffset.Normalize();
		pCamera->Set_Offset(vOffset);

		m_fCameraLength = 4.0f;
		pCamera->Set_CameraLength(m_fCameraLength);
		m_iCameraSequence = 9;
	}
	Update_LightningParticle_AfterSpawn(fTimeDelta);
	}

	else if (m_iCameraSequence == 9)
	{
	m_fCameraLength += 4.0f * fTimeDelta;
	if (m_fCameraLength > 6.0f)
		m_fCameraLength = 6.0f;

	pCamera->ZoomInOut(m_fCameraLength, 0.8f);
	Update_LightningParticle_AfterSpawn(fTimeDelta);
	}
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

	CUI_Manager::GetInstance()->Set_UIs_Active(true, LEVELID::LEVEL_VALTANMAIN);
	CUI_Manager::GetInstance()->Set_RenderNickName(true, LEVELID::LEVEL_VALTANMAIN);
	CUI_Manager::GetInstance()->Set_RenderNickName(true, LEVELID::LEVEL_STATIC);
}

void CValtan_BT_Spawn::Update_LightningParticle(_float fTimeDelta)
{
	m_fLightningParticleAcc += fTimeDelta;
	if (m_fLightningParticleAcc >= m_fLightningParticleTime)
	{
		Matrix matWorld = XMMatrixIdentity();
		Vec3 vPos = Vec3(100.0f, 0.1f, 100.0f);
		vPos.x += CGameInstance::GetInstance()->Random_Float(-2.0f, 2.0f);
		vPos.z += CGameInstance::GetInstance()->Random_Float(-2.0f, 2.0f);
		vPos.y -= CGameInstance::GetInstance()->Random_Float(0.0f, 0.5f);

		matWorld.Translation(vPos);

		wstring szParticleName = L"ValtanSpawnLightning";
		szParticleName += to_wstring(rand() % 4 + 1);

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		
		EFFECT_START(szParticleName, &tDesc);
		m_fLightningParticleAcc = 0.0f;
	}
}

void CValtan_BT_Spawn::Update_LightningParticle_AfterSpawn(_float fTimeDelta)
{
	m_fLightningParticleAcc += fTimeDelta;
	if (m_fLightningParticleAcc >= m_fLightningParticleTime)
	{
		Matrix matWorld = XMMatrixIdentity();
		Vec3 vPos = Vec3(100.0f, 0.1f, 100.0f);
		vPos.x += CGameInstance::GetInstance()->Random_Float(-1.5f, 1.5f);
		vPos.z += CGameInstance::GetInstance()->Random_Float(-1.5f, 1.5f);
		vPos.y += CGameInstance::GetInstance()->Random_Float(-2.0f, 2.0f);

		matWorld.Translation(vPos);

		wstring szParticleName = L"ValtanSpawnLightning";
		szParticleName += to_wstring(rand() % 4 + 1);

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;

		EFFECT_START(szParticleName, &tDesc);
		m_fLightningParticleAcc = 0.0f;
	}
}

void CValtan_BT_Spawn::Effect_SpawnStart()
{
	Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
	matWorld.Translation(Vec3(100.0f, 0.1f, 100.0f));

	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;

	EFFECT_START_OUTLIST(L"ValtanSpawn1", &tDesc, m_SpawnFloor);
	EFFECT_START_OUTLIST(L"ValtanSpawn2", &tDesc, m_SpawnFloor);
	EFFECT_START_OUTLIST(L"ValtanSpawnBlack", &tDesc, m_BlackSmoke);
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

void CValtan_BT_Spawn::Effect_Appear()
{
	Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

	
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START_OUTLIST(L"ValtanShowBlack", &tDesc, m_BlackSmoke);
	}
	{
		CBoss* pBoss = dynamic_cast<CBoss*>(m_pGameObject);
		auto func = bind(&CBoss::Load_WorldMatrix, pBoss, placeholders::_1);
		TRAIL_START_OUTLIST(TEXT("ValtanSpawnTrail1"), func, m_Trails);
	}

	{
		CBoss* pBoss = dynamic_cast<CBoss*>(m_pGameObject);
		auto func = bind(&CBoss::Load_WorldMatrix, pBoss, placeholders::_1);
		TRAIL_START_OUTLIST(TEXT("ValtanSpawnTrail2"), func, m_Trails);
	}
	

	Vec3 vPos = matWorld.Translation();
	vPos.z -= 3.0f;
	vPos.x += 0.5f;
	
	matWorld.Translation(vPos);

	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START(L"ValtanShow", &tDesc);
	}
}
