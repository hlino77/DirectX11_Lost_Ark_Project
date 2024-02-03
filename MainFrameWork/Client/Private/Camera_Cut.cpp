#include "stdafx.h"
#include "Camera_Cut.h"
#include "GameInstance.h"
#include "Player.h"
#include "AsUtils.h"
#include "Esther_Cut.h"
#include "Esther.h"

CCamera_Cut::CCamera_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag)
{
}

CCamera_Cut::CCamera_Cut(const CCamera_Cut& rhs, CTransform::TRANSFORMDESC* pArg)
	: CCamera(rhs, pArg)
{

}

HRESULT CCamera_Cut::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Cut::Initialize(void* pArg)
{
	CUTCAMERADESC* pDesc = static_cast<CUTCAMERADESC*>(pArg);
	m_pTarget = pDesc->pCutTarget;

	if (FAILED(__super::Initialize(&pDesc->tCameraDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Cut::Tick(_float fTimeDelta)
{


	switch (m_iState)
	{
	case (_uint)CAMERATYPE::OFFSET:
		Tick_Offset(fTimeDelta);
		break;
	case (_uint)CAMERATYPE::LOOKTARGET:
		Tick_LookTarget(fTimeDelta);
		break;
	}
}

void CCamera_Cut::LateTick(_float fTimeDelta)
{
}

void CCamera_Cut::Tick_Offset(_float fTimeDelta)
{
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, m_fZoomSpeed * fTimeDelta);

		if (fabs(m_fTargetCameraLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength;
		}
	}
	if (true == m_tLerpOffset.bActive)
	{
		m_vOffset = m_tLerpOffset.Update_Lerp(fTimeDelta);
	}
	if (true == m_tLerpTarget.bActive)
	{
		m_vTargetOffset = m_tLerpTarget.Update_Lerp(fTimeDelta);
	}

	Vec3 vTargetPos = static_cast<CPlayer*>(m_pTarget)->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vTargetPos = vTargetPos + m_vTargetOffset;

	Vec3 vPos = vTargetPos + (m_vOffset * m_fCameraLength);

	Vec3 vLook = vTargetPos - vPos;
	Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	m_pTransformCom->Set_WorldMatrix(matWorld);
}

void CCamera_Cut::Tick_LookTarget(_float fTimeDelta)
{
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, m_fZoomSpeed * fTimeDelta);

		if (fabs(m_fTargetCameraLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength;
		}
	}
	if (true == m_tLerpTarget.bActive)
	{
		m_vTargetOffset = m_tLerpTarget.Update_Lerp(fTimeDelta);
	}

	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	Vec3 vTargetPos = static_cast<CPlayer*>(m_pTarget)->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vTargetPos = vTargetPos + (m_vTargetOffset);

	Vec3 vLook = vTargetPos - vPos;
	Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	m_pTransformCom->Set_WorldMatrix(matWorld);
}

HRESULT CCamera_Cut::Render()
{
	return S_OK;
}

void CCamera_Cut::Cam_Shake(_float fFirstShake, _float fForce, _float fTime, _float fBreak)
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

void CCamera_Cut::Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight, _float fTimeDelta)
{
	m_vShakeVelocity.x += -m_vShakeOffset.x * m_fShakeForce * CGameInstance::GetInstance()->Random_Float(0.1f, 0.5f) * fTimeDelta;
	m_vShakeVelocity.y += -m_vShakeOffset.y * m_fShakeForce * CGameInstance::GetInstance()->Random_Float(0.1f, 0.5f) * fTimeDelta;

	//m_vShakeVelocity *= (1.0f - m_fBreak);

	m_vShakeOffset += m_vShakeVelocity;

	m_vShakeOffset *= (1.0f - m_fBreak * fTimeDelta);

	vLook += m_vShakeOffset.x * vRight;
	vLook += m_vShakeOffset.y * vUp;
}

HRESULT CCamera_Cut::Ready_Components()
{
	__super::Ready_Components();
	return S_OK;
}

CCamera_Cut* CCamera_Cut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Cut* pInstance = new CCamera_Cut(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Cut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Cut::Clone(void* pArg)
{
	CCamera::CAMERADESC* pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Cut* pInstance = new CCamera_Cut(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Cut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Cut::Free()
{
	__super::Free();
}
