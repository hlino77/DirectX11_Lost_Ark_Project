#include "stdafx.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"

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

	m_fCameraLength = 7.5f;

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

	Vec3 vPlayerPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = vPlayerPos + (m_vOffset * m_fCameraLength);
	Vec3 vLook = vPlayerPos - vPos;

	Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
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

void CCamera_Player::Cam_Shake(_float fForce, _float fTime)
{
	m_bShake = true;
	
	m_fShakeForce = fForce;
	m_fShakeTime = fTime;
	m_fCurrShakeTime = 0.0f;
	m_vOriginLook = Vec3(0.0f, 0.0f, 0.0f);
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
