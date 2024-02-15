#include "stdafx.h"
#include "Valtan_BT_Dead.h"
#include "Monster.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Boss_Valtan.h"

CValtan_BT_Dead::CValtan_BT_Dead()
{
}

void CValtan_BT_Dead::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Deactivate_AllColliders();
	static_cast<CMonster*>(m_pGameObject)->Disable_HpUI();

	m_bDeadEffect = false;
	m_iCameraSequence = 0;
	m_fSlowMotionSpeed = 0.02f;
}

CBT_Node::BT_RETURN CValtan_BT_Dead::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 38 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
	}

	if (m_bDeadEffect == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 23)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_Dead", &tDesc);

		m_bDeadEffect = true;
	}

	Update_Camera(fTimeDelta);

	Add_Sound(0, 0, L"Effect", L"Boss_Down");
	Add_Sound(0, 1, L"Effect", L"Valtan#2 (613355816)");
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Dead::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster*>(m_pGameObject)->Set_Die(4.f);
}

void CValtan_BT_Dead::Update_Camera(_float fTimeDelta)
{
	CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

	if (m_iCurrAnimation == 2 && m_iCameraSequence == 0 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vLook = matWorld.Backward();
		Vec3 vRight = matWorld.Right();
		Vec3 vUp(0.0f, 1.0f, 0.0f);

		Vec3 vOffset = vLook + vUp * 0.2f;
		vOffset.Normalize();

		Vec3 vTargetPos = matWorld.Translation() + vUp * 1.0f;

		pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
		pCamera->Set_CameraLength(15.0f);
		pCamera->Set_TargetPos(vTargetPos);
		pCamera->Set_Offset(vOffset);
		pCamera->ZoomInOut(5.0f, 0.3f);

		m_iCameraSequence = 1;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		_uint iAnimFrame = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex);

		if (m_iCameraSequence == 1)
		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			Vec3 vLook = matWorld.Backward();
			Vec3 vRight = matWorld.Right();
			Vec3 vUp(0.0f, 1.0f, 0.0f);
			Vec3 vPos = matWorld.Translation();

			m_vCameraPos = vPos + vLook * 3.0f - vRight * 0.5f + vUp * 1.2f;
			m_vCamTargetPos = vPos + vUp * 1.0f;

			Vec3 vOffset = m_vCameraPos - m_vCamTargetPos;
			_float fLength = vOffset.Length();
			vOffset.Normalize();

			pCamera->Set_Offset(vOffset);
			pCamera->Set_TargetPos(m_vCamTargetPos);
			pCamera->Set_CameraLength(fLength);

			m_iCameraSequence = 2;
		}
		else if (m_iCameraSequence == 2)
		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			Vec3 vLook = matWorld.Backward();
			Vec3 vRight = matWorld.Right();
			Vec3 vUp(0.0f, 1.0f, 0.0f);
			Vec3 vPos = matWorld.Translation();

			Vec3 vTargetPos = vPos + vUp * 3.0f;

			m_vCamTargetPos = Vec3::Lerp(m_vCamTargetPos, vTargetPos, 0.5f * fTimeDelta);

			Vec3 vOffset = m_vCameraPos - m_vCamTargetPos;
			_float fLength = vOffset.Length();
			vOffset.Normalize();

			pCamera->Set_Offset(vOffset);
			pCamera->Set_TargetPos(m_vCamTargetPos);
			pCamera->Set_CameraLength(fLength);

			if (iAnimFrame >= 54)
			{
				m_iCameraSequence = 3;
			}

		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex)
	{
		_uint iAnimFrame = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex);

		if (m_iCameraSequence == 3)
		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			Vec3 vLook = matWorld.Backward();
			Vec3 vRight = matWorld.Right();
			Vec3 vUp(0.0f, 1.0f, 0.0f);

			Vec3 vOffset = vLook + vUp * 0.2f - vRight;
			vOffset.Normalize();

			Vec3 vTargetPos = matWorld.Translation() + vUp * 0.4f;

			pCamera->Set_CameraLength(15.0f);
			pCamera->Set_TargetPos(vTargetPos);
			pCamera->Set_Offset(vOffset);
			pCamera->ZoomInOut(5.0f, 1.0f);

			m_iCameraSequence = 4;
			m_fAnimSpeed = static_cast<CBoss_Valtan*>(m_pGameObject)->Get_AnimationSpeed();
		}
		
		

		if (iAnimFrame >= 28 && iAnimFrame < 36)
		{
			if (g_fTimeDeltaRatio > 0.1f)
			{
				g_fTimeDeltaRatio -= m_fSlowMotionSpeed;

				if (g_fTimeDeltaRatio < 0.1f)
					g_fTimeDeltaRatio = 0.1f;
			}

			_float fAnimSpeed = static_cast<CBoss_Valtan*>(m_pGameObject)->Get_AnimationSpeed();
			if (fAnimSpeed > 0.5f)
			{
				fAnimSpeed -= m_fSlowMotionSpeed;
				if (fAnimSpeed < 0.5f)
					fAnimSpeed = 0.5f;

				static_cast<CBoss_Valtan*>(m_pGameObject)->Set_AnimationSpeed(fAnimSpeed);
			}
		}

		if (iAnimFrame >= 36)
		{
			if (g_fTimeDeltaRatio < 1.0f)
			{
				g_fTimeDeltaRatio += m_fSlowMotionSpeed;
				if (g_fTimeDeltaRatio > 1.0f)
					g_fTimeDeltaRatio = 1.0f;
			}

			_float fAnimSpeed = static_cast<CBoss_Valtan*>(m_pGameObject)->Get_AnimationSpeed();
			if (fAnimSpeed < m_fAnimSpeed)
			{
				fAnimSpeed += m_fSlowMotionSpeed;
				if (fAnimSpeed > m_fAnimSpeed)
					fAnimSpeed = m_fAnimSpeed;

				static_cast<CBoss_Valtan*>(m_pGameObject)->Set_AnimationSpeed(fAnimSpeed);
			}
		}
	}

	if (m_iCameraSequence == 4)
	{
		Vec3 vOffset = pCamera->Get_Offset();

		vOffset = XMVector3Rotate(vOffset, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), 0.5f * fTimeDelta));
		pCamera->Set_Offset(vOffset);



		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex)
		{
			m_iCameraSequence = 5;
			g_fTimeDeltaRatio = 1.0f;
			static_cast<CBoss_Valtan*>(m_pGameObject)->Set_AnimationSpeed(m_fAnimSpeed);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex)
	{

		if (m_iCameraSequence == 5)
		{
			Matrix matCamera = pCamera->Get_TransformCom()->Get_WorldMatrix();
		
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			Vec3 vLook = matWorld.Backward();
			Vec3 vRight = matWorld.Right();
			Vec3 vUp(0.0f, 1.0f, 0.0f);

			Vec3 vCurrOffset = -matCamera.Backward();
			Vec3 vCurrPos = matCamera.Translation();
			Vec3 vCurrTargetPos = pCamera->Get_TargetPos();

			Vec3 vOffset = matWorld.Backward() * 0.8f + matWorld.Right() * 0.8f + vUp * 0.2f;
			vOffset.Normalize();
			Vec3 vTargetPos = matWorld.Translation();

			vCurrOffset = Vec3::Lerp(vCurrOffset, vOffset, 1.0f * fTimeDelta);
			vCurrTargetPos = Vec3::Lerp(vCurrTargetPos, vTargetPos, 1.0f * fTimeDelta);

			pCamera->Set_Offset(vCurrOffset);
			pCamera->Set_TargetPos(vCurrTargetPos);
		}
	}

}



CValtan_BT_Dead* CValtan_BT_Dead::Create(void* pArg)
{
	CValtan_BT_Dead* pInstance = new CValtan_BT_Dead;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Dead::Free()
{
	__super::Free();
}
