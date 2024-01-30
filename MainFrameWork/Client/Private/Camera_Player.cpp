#include "stdafx.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"
#include "AsUtils.h"

CCamera_Player::CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag)
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& rhs, CTransform::TRANSFORMDESC* pArg)
	: CCamera(rhs, pArg)
{

}

HRESULT CCamera_Player::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CCamera_Player::Initialize(void* pArg)
{
	PlayerCameraDesc* pDesc = static_cast<PlayerCameraDesc*>(pArg);
	m_pTarget = pDesc->pPlayer;

	if (FAILED(__super::Initialize(&pDesc->tCameraDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_vOffset = pDesc->vOffset;
	m_vOffset.Normalize();

	Vec3 vRight = m_vOffset.Cross(Vec3(0.0f, 1.0f, 0.0f));
	
	m_vDefaultOffset = m_vOffset = XMVector3TransformNormal(m_vOffset, Matrix::CreateFromAxisAngle(vRight, XMConvertToRadians(50.0f)));

	m_fDefaultLength = m_fTargetCameraLength = m_fCameraLength = pDesc->fDefaultLength;

	Vec3 vPos = m_pTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION) + (m_vOffset * m_fCameraLength);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	
	m_eState = CameraState::DEFAULT;

	return S_OK;
}

void CCamera_Player::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case CameraState::FREE:
		Tick_FreeCamera(fTimeDelta);
		break;
	case CameraState::DEFAULT:
		Tick_DefaultCamera(fTimeDelta);
		break;
	case CameraState::RESET:
		Tick_ResetCamera(fTimeDelta);
		break;
	}

	__super::Tick(fTimeDelta);
}

void CCamera_Player::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}

void CCamera_Player::Cam_Shake(_float fFirstShake, _float fForce, _float fTime, _float fBreak)
{
	m_bShake = true;

	m_fShakeTime = fTime;
	m_fCurrShakeTime = 0.0f;

	m_fShakeForce = fForce;
	m_fBreak = fBreak;

	m_vShakeOffset = Vec2();
	m_vShakeVelocity.x = CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f);
	m_vShakeVelocity.y = CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f);

	m_vShakeVelocity.Normalize();

	m_vShakeVelocity *= fFirstShake;
}

void CCamera_Player::Tick_FreeCamera(_float fTimeDelta)
{
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, m_fZoomSpeed * fTimeDelta);

		if (fabs(m_fTargetCameraLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength;
		}
	}


	Vec3 vTargetPos = m_vTargetPos;
	Vec3 vPos = vTargetPos + (m_vOffset * m_fCameraLength);
	Vec3 vLook = vTargetPos - vPos;
	Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	if (m_bShake)
	{
		m_fCurrShakeTime += fTimeDelta;
		if (m_fCurrShakeTime >= m_fShakeTime)
		{
			m_bShake = false;
		}

		Update_ShakeLook(vPos, matWorld.Up(), matWorld.Right(), fTimeDelta);
		matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
	}

	m_pTransformCom->Set_WorldMatrix(matWorld);

	if (m_bMotionBlur)
	{
		m_fMotionBlurAcc -= fTimeDelta;
		if (m_fMotionBlurAcc <= 0.0f)
		{
			CGameInstance::GetInstance()->Set_MotionBlur(false);
			m_fMotionBlurIntensity = 0.0f;
			m_bMotionBlur = false;
			m_fMotionBlurAcc = 0.0f;
		}
		else
		{
			CGameInstance::GetInstance()->Set_MotionBlur(true, m_fMotionBlurIntensity);
		}
	}

	if (m_bRadialBlur)
	{
		if (m_fRadialBlurAcc <= 0.0f)
		{
			if (m_fRadialBlurIntensity <= 0.0f)
			{
				m_vRadialPos = Vec3();
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, 0.0f);
				m_fRadialBlurIntensity = 0.0f;
				m_bRadialBlur = false;
				m_fRadialBlurAcc = 0.0f;
			}
			else
			{
				m_fRadialBlurIntensity -= m_fRadialBlurDamping * fTimeDelta;
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
			}
		}
		else
		{
			m_fRadialBlurAcc -= fTimeDelta;
			m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
		}
	}
}	

void CCamera_Player::Tick_DefaultCamera(_float fTimeDelta)
{
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, m_fZoomSpeed * fTimeDelta);

		if (fabs(m_fTargetCameraLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength;
		}
	}


	Vec3 vPlayerPos = m_pTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = vPlayerPos + (m_vDefaultOffset * m_fCameraLength);
	Vec3 vLook = vPlayerPos - vPos;
	Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	if (m_bShake)
	{
		m_fCurrShakeTime += fTimeDelta;
		if (m_fCurrShakeTime >= m_fShakeTime)
		{
			m_bShake = false;
		}

		Update_ShakeLook(vPos, matWorld.Up(), matWorld.Right(), fTimeDelta);
		matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
	}

	m_pTransformCom->Set_WorldMatrix(matWorld);

	if (m_bMotionBlur)
	{
		m_fMotionBlurAcc -= fTimeDelta;
		if (m_fMotionBlurAcc <= 0.0f)	
		{
			CGameInstance::GetInstance()->Set_MotionBlur(false);
			m_fMotionBlurIntensity = 0.0f;
			m_bMotionBlur = false;
			m_fMotionBlurAcc = 0.0f;
		}
		else
		{
			CGameInstance::GetInstance()->Set_MotionBlur(true, m_fMotionBlurIntensity);
		}
	}


	if (m_bRadialBlur)
	{
		if (m_fRadialBlurAcc <= 0.0f)
		{
			if (m_fRadialBlurIntensity <= 0.0f)
			{
				m_vRadialPos = Vec3();
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, 0.0f);
				m_fRadialBlurIntensity = 0.0f;
				m_bRadialBlur = false;
				m_fRadialBlurAcc = 0.0f;
			}
			else
			{
				m_fRadialBlurIntensity -= m_fRadialBlurDamping * fTimeDelta;
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
			}
		}
		else
		{
			m_fRadialBlurAcc -= fTimeDelta;
			m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
		}
	}
}

void CCamera_Player::Tick_ResetCamera(_float fTimeDelta)
{
	if (m_fCameraLength != m_fDefaultLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fDefaultLength, m_fResetSpeed * fTimeDelta);

		if (fabs(m_fDefaultLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength = m_fDefaultLength;
		}
	}

	Vec3 vPlayerPos = m_pTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	Vec3 vTargetPos = vPlayerPos + (m_vDefaultOffset * m_fCameraLength);
	Vec3 vCurrPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec3 vTargetLook = vPlayerPos - vTargetPos;
	vTargetLook.Normalize();

	_float fDistance = (vTargetPos - vCurrPos).Length();

	Vec3 vPos;
	Vec3 vLook;

	if (fDistance < 0.1f)
	{
		vPos = vTargetPos;
		vLook = vTargetLook;
		m_fCameraLength = m_fTargetCameraLength = m_fDefaultLength;
		m_eState = CameraState::DEFAULT;
	}
	else
	{
		vPos = Vec3::Lerp(vCurrPos, vTargetPos, m_fResetSpeed * fTimeDelta);
		vLook = Vec3::Lerp(vCurrLook, vTargetLook, m_fResetSpeed * fTimeDelta);
	}

	Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	if (m_bShake)
	{
		m_fCurrShakeTime += fTimeDelta;
		if (m_fCurrShakeTime >= m_fShakeTime)
		{
			m_bShake = false;
		}

		Update_ShakeLook(vPos, matWorld.Up(), matWorld.Right(), fTimeDelta);
		matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
	}

	m_pTransformCom->Set_WorldMatrix(matWorld);

	if (m_bMotionBlur)
	{
		m_fMotionBlurAcc -= fTimeDelta;
		if (m_fMotionBlurAcc <= 0.0f)
		{
			CGameInstance::GetInstance()->Set_MotionBlur(false);
			m_fMotionBlurIntensity = 0.0f;
			m_bMotionBlur = false;
			m_fMotionBlurAcc = 0.0f;
		}
		else
		{
			CGameInstance::GetInstance()->Set_MotionBlur(true, m_fMotionBlurIntensity);
		}
	}

	if (m_bRadialBlur)
	{
		if (m_fRadialBlurAcc <= 0.0f)
		{
			if (m_fRadialBlurIntensity <= 0.0f)
			{
				m_vRadialPos = Vec3();
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, 0.0f);
				m_fRadialBlurIntensity = 0.0f;
				m_bRadialBlur = false;
				m_fRadialBlurAcc = 0.0f;
			}
			else
			{
				m_fRadialBlurIntensity -= m_fRadialBlurDamping * fTimeDelta;
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
			}
		}
		else
		{
			m_fRadialBlurAcc -= fTimeDelta;
			m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
		}
	}
}

void CCamera_Player::Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight, _float fTimeDelta)
{
	m_vShakeVelocity.x += -m_vShakeOffset.x * m_fShakeForce * CGameInstance::GetInstance()->Random_Float(0.1f, 0.5f) * fTimeDelta;
	m_vShakeVelocity.y += -m_vShakeOffset.y * m_fShakeForce * CGameInstance::GetInstance()->Random_Float(0.1f, 0.5f) * fTimeDelta;

	//m_vShakeVelocity *= (1.0f - m_fBreak);

	m_vShakeOffset += m_vShakeVelocity;

	m_vShakeOffset *= (1.0f - m_fBreak * fTimeDelta);

	vLook += m_vShakeOffset.x * vRight;
	vLook += m_vShakeOffset.y * vUp;
}

HRESULT CCamera_Player::Ready_Components()
{
	__super::Ready_Components();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

CCamera_Player* CCamera_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Player* pInstance = new CCamera_Player(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
	CCamera::CAMERADESC* pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Player* pInstance = new CCamera_Player(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();


}
