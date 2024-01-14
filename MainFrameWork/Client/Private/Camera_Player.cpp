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
	m_pPlayer = pDesc->pPlayer;


	if (FAILED(__super::Initialize(&pDesc->tCameraDesc)))
		return E_FAIL;

	m_vOffset = pDesc->vOffset;
	m_vOffset.Normalize();

	Vec3 vRight = m_vOffset.Cross(Vec3(0.0f, 1.0f, 0.0f));
	
	m_vOffset = XMVector3TransformNormal(m_vOffset, Matrix::CreateFromAxisAngle(vRight, XMConvertToRadians(50.0f)));

	m_fDefaultLength = m_fTargetCameraLength = m_fCameraLength = 7.5f;

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION) + (m_vOffset * m_fCameraLength);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	

	return S_OK;
}

void CCamera_Player::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	//오프셋을 매번 업데이트 한다
	// 속도로 회전 
	
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, m_fZoomSpeed * fTimeDelta);

		if (fabs(m_fTargetCameraLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength;
		}
	}


	Vec3 vPlayerPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = vPlayerPos + (m_vOffset * m_fCameraLength);
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

	
	Safe_Release(pGameInstance);

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
