#include "stdafx.h"
#include "Camera_Free.h"
#include "Engine_Defines.h"
#include "GameInstance.h"


CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs, CTransform::TRANSFORMDESC * pArg)
	: CCamera(rhs, pArg)
{
	
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_strObjectTag = L"Prototype_GameObject_Camera_Free";
	
	m_bActive = false;

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	

	if (KEY_TAP(KEY::TAB))
	{
		m_bMouse = !m_bMouse;
		ShowCursor(m_bMouse);
	}


	if (!m_bMouse)
	{
		if (KEY_HOLD(KEY::UP_ARROW))
		{
			m_pTransformCom->Go_Straight(5.0f, fTimeDelta);
		}

		if (KEY_HOLD(KEY::DOWN_ARROW))
		{
			m_pTransformCom->Go_Backward(5.0f, fTimeDelta);
		}

		if (KEY_HOLD(KEY::LEFT_ARROW))
		{
			m_pTransformCom->Go_Left(5.0f, fTimeDelta);
		}

		if (KEY_HOLD(KEY::RIGHT_ARROW))
		{
			m_pTransformCom->Go_Right(5.0f, fTimeDelta);
		}

		if (KEY_HOLD(KEY::P))
		{
			m_pTransformCom->Go_Up(fTimeDelta * 5.f);
		}

		if (KEY_HOLD(KEY::O))
		{
			m_pTransformCom->Go_Down(fTimeDelta * 5.f);
		}
		
		if (KEY_TAP(KEY::F2))
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_CameraDesc.vEye));
			m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));
		}



		POINT MousePos;

		MousePos.x = g_iWinSizeX / 2;
		MousePos.y = g_iWinSizeY / 2;

		ClientToScreen(g_hWnd, &MousePos);
		SetCursorPos(MousePos.x, MousePos.y);

		_long	MouseMove  = 0;

		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		{
			if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
				m_pTransformCom->Move_Dir(-m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta, MouseMove * 3.f);
			else
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * 0.05f);
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
		{
			if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
				m_pTransformCom->Move_Dir(m_pTransformCom->Get_State(CTransform::STATE_UP), fTimeDelta, MouseMove * 3.f);
			else	
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * 0.05f);

		}
	}


	if (m_bShake)
	{
		

		m_fCurrShakeTime += fTimeDelta;
		if (m_fCurrShakeTime >= m_fShakeTime)
		{
			m_bShake = false;
			m_pTransformCom->Set_WorldMatrix(m_matOrigin);
		}
		else
		{
			Vec3 vLook = m_matOrigin.Backward();
			Vec3 vPos = m_matOrigin.Translation();

			Update_ShakeLook(vPos, m_matOrigin.Up(), m_matOrigin.Right(), fTimeDelta);
			Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

			m_pTransformCom->Set_WorldMatrix(matWorld);
		}
	}

	__super::Tick(fTimeDelta);
	Safe_Release(pGameInstance);
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::Cam_Shake(_float fFirstShake, _float fForce, _float fTime, _float fBreak)
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

	m_matOrigin = m_pTransformCom->Get_WorldMatrix();
}

void CCamera_Free::Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight, _float fTimeDelta)
{
	m_vShakeVelocity.x += -m_vShakeOffset.x * m_fShakeForce * CGameInstance::GetInstance()->Random_Float(0.1f, 0.5f) * fTimeDelta;
	m_vShakeVelocity.y += -m_vShakeOffset.y * m_fShakeForce * CGameInstance::GetInstance()->Random_Float(0.1f, 0.5f) * fTimeDelta;

	//m_vShakeVelocity *= (1.0f - m_fBreak);

	m_vShakeOffset += m_vShakeVelocity;

	m_vShakeOffset *= (1.0f - m_fBreak * fTimeDelta);

	vLook += m_vShakeOffset.x * vRight;
	vLook += m_vShakeOffset.y * vUp;
}

HRESULT CCamera_Free::Ready_Components()
{
	__super::Ready_Components();
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Free*		pInstance = new CCamera_Free(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void* pArg)
{
	CCamera::CAMERADESC*		pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Free*		pInstance = new CCamera_Free(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();


}

