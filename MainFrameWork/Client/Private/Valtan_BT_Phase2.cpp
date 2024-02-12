#include "stdafx.h"
#include "Valtan_BT_Phase2.h"
#include "Monster.h"
#include "Model.h"
#include <Boss_Valtan.h>
#include "Transform.h"
#include <AnimModel.h>
#include "GameInstance.h"
#include "Player.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "AsUtils.h"

#include "NavigationMgr.h"
#include "Effect_Manager.h"


CValtan_BT_Phase2::CValtan_BT_Phase2()
{
}

void CValtan_BT_Phase2::OnStart()
{
	__super::OnStart(0);

	m_eCameraState = CameraState::Ready;
	m_bBreak = false;
	m_fCameraSpeed = 8.0f;
	m_bRadial = false;

	m_fCamShakeTimeAcc = m_fCamShakeTimeDelay = 0.23f;

}

CBT_Node::BT_RETURN CValtan_BT_Phase2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}

	_uint iAnimFrame3 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex);

	if (m_iCurrAnimation == 3 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex)
	{
		if (m_bBreak == false && iAnimFrame3 > 41)
		{
			for (auto iter : CGameInstance::GetInstance()->GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_BACKGROUND))
			{
				if (iter->Get_ModelCom()->Get_ModelType() == CModel::TYPE::TYPE_ANIM &&
					iter->Get_ModelName() == TEXT("Wall01") ||
					iter->Get_ModelName() == TEXT("Wall02") ||
					iter->Get_ModelName() == TEXT("Wall03") ||
					iter->Get_ModelName() == TEXT("Wall04"))
				{
					static_cast<CAnimModel*>(iter)->Set_PlayAnim(true);
					
					if (false == m_bSendServerNaviInfo && iter->Get_ModelName() == TEXT("Wall02"))
					{
						static_cast<CAnimModel*>(iter)->Break_OutWall();
						m_bSendServerNaviInfo = true;
					}
				}
			}

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_Phase2", &tDesc);

			m_bBreak = true;
		}

		if (m_eCameraState == CameraState::Ready)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			vPos += Vec3(-0.354f, 0.935f, 0.0f);
			CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();
			pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
			pCamera->Set_TargetPos(vPos);
			pCamera->Set_Offset(Vec3(-0.09f, 0.161f, -0.983f));
			pCamera->Set_CameraLength(2.3f);
			pCamera->ZoomInOut(6.0f, 2.0f);
			m_eCameraState = CameraState::Start;
		}
		else if (m_eCameraState == CameraState::Start)
		{
			CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();
			if (iAnimFrame3 < 39)
			{
				Vec3 vOffset = pCamera->Get_Offset();
				Vec3 vTargetOffset = Vec3(-0.025f, 0.330f, -0.848f);
				vTargetOffset.Normalize();
				vOffset = Vec3::Lerp(vOffset, vTargetOffset, 2.0f * fTimeDelta);
				pCamera->Set_Offset(vOffset);
			}
			else if(iAnimFrame3 >= 39 && iAnimFrame3 < 41)
			{
				_float fLength = pCamera->Get_CameraLength();
				fLength += m_fCameraSpeed * fTimeDelta;
				pCamera->Set_CameraLength(fLength);
			}
			else if (iAnimFrame3 >= 41 && iAnimFrame3 < 42)
			{
				if (m_bRadial == false)
				{
					Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
					vPos.y += 1.0f;
					pCamera->Set_RadialBlur(0.2f, vPos, 0.2f, 0.2f);
					pCamera->Set_Chromatic(0.2f, vPos, 0.2f, 0.2f);
					m_bRadial = false;
				}
				m_fCameraSpeed = 300.0f;
				_float fLength = pCamera->Get_CameraLength();
				fLength += m_fCameraSpeed * fTimeDelta;
				pCamera->Set_CameraLength(fLength);

				Vec3 vOffset = pCamera->Get_Offset();
				Vec3 vTargetOffset = Vec3(-0.025f, 0.830f, -0.848f);
				vTargetOffset.Normalize();
				vOffset = Vec3::Lerp(vOffset, vTargetOffset, 7.0f * fTimeDelta);
				pCamera->Set_Offset(vOffset);

				if (m_fCamShakeTimeAcc >= m_fCamShakeTimeDelay)
				{
					pCamera->Cam_Shake(0.4f, 100.0f, 2.0f, 5.0f);
					m_fCamShakeTimeAcc = 0.0f;
				}
				else
				{
					m_fCamShakeTimeAcc += fTimeDelta;
				}
			}
			else if (iAnimFrame3 >= 42)
			{
				Vec3 vOffset = pCamera->Get_Offset();
				Vec3 vTargetOffset = Vec3(-0.025f, 0.830f, -0.848f);
				vTargetOffset.Normalize();
				vOffset = Vec3::Lerp(vOffset, vTargetOffset, 7.0f * fTimeDelta);
				pCamera->Set_Offset(vOffset);

				m_fCameraSpeed = CAsUtils::Lerpf(m_fCameraSpeed, 4.0f, 10.0f * fTimeDelta);
				_float fLength = pCamera->Get_CameraLength();
				fLength += m_fCameraSpeed * fTimeDelta;
				pCamera->Set_CameraLength(fLength);

				if (m_fCamShakeTimeAcc >= m_fCamShakeTimeDelay)
				{
					pCamera->Cam_Shake(0.4f, 100.0f, 1.0f, 10.0f);
					m_fCamShakeTimeAcc = 0.0f;
				}
				else
				{
					m_fCamShakeTimeAcc += fTimeDelta;
				}
			}
		}
		
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) > 77 && m_iCurrAnimation == 3)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_Invincible(false);

		m_iCurrAnimation++;

		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);

	}
	if (m_iCurrAnimation == 3)
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_iCurrAnimation == 4)
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(true);

		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();
		m_fCameraSpeed = CAsUtils::Lerpf(m_fCameraSpeed, 4.0f, 10.0f * fTimeDelta);
		_float fLength = pCamera->Get_CameraLength();
		fLength += m_fCameraSpeed * fTimeDelta;
		pCamera->Set_CameraLength(fLength);
	}
	Add_Sound(0, 0, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#113 (872104708)", CHANNEL_EFFECT, 23);
	Add_Sound(0, 1, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#153 (153317365)", CHANNEL_EFFECT, 23);

	Add_Sound(2, 2, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#154 (178464551)", CHANNEL_EFFECT, 9);
	Add_Sound(2, 3, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#392 (75108078)", CHANNEL_EFFECT, 9);
	Add_Sound(2, 6, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#43 (1015057785)", CHANNEL_EFFECT, 9);
	
	Add_Sound(3, 4, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#50 (428724023)", CHANNEL_EFFECT, 41);
	Add_Sound(3, 5, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#237 (128183717)", CHANNEL_EFFECT, 65); 
	
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase2::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss*>(m_pGameObject)->Set_Phase(2);
	CServerSessionManager::GetInstance()->Get_Player()->Set_CurValtanPhase(2);


	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_ResetSpeed(5.0f);
	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Mode(CCamera_Player::CameraState::RESET);
}

void CValtan_BT_Phase2::Update_Camera(_float fTimeDelta)
{
	switch (m_eCameraState)
	{
	case CameraState::Ready:
	{

	}
		break;
	case CameraState::Start:
	{

	}
		break;
	case CameraState::ZoomOut:
	{

	}
		break;
	case CameraState::CameraEnd:
	{

	}
		break;
	}
}



CValtan_BT_Phase2* CValtan_BT_Phase2::Create(void* pArg)
{
	CValtan_BT_Phase2* pInstance = new CValtan_BT_Phase2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase2::Free()
{
	__super::Free();
}
