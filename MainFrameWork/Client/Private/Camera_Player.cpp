#include "stdafx.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "Player.h"
#include "AsUtils.h"
#include "Renderer.h"

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

	Ready_FadeInOut();

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
	if (m_bFade)
	{
		Update_FadeInOut(fTimeDelta);
	}
		

	if (m_bFade == true)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI, this);
	}
}

HRESULT CCamera_Player::Render()
{
	if (FAILED(Render_FadeInOut()))
		return E_FAIL;

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

void CCamera_Player::ZoomInOut(_float fCameraLength, _float fSpeed)
{
	m_fTargetCameraLength = fCameraLength; m_fZoomSpeed = fSpeed;
	CRenderer::Set_DOF_Focus(fCameraLength / 1200.f);
}

void CCamera_Player::DefaultLength(_float fSpeed)
{
	m_fTargetCameraLength = m_fDefaultLength; m_fZoomSpeed = fSpeed;
	CRenderer::Set_DOF_Focus(m_fDefaultLength / 1200.f);
}

void CCamera_Player::Set_CameraLength(_float fCameraLength)
{
	m_fCameraLength = m_fTargetCameraLength = fCameraLength;
	CRenderer::Set_DOF_Focus(fCameraLength / 1200.f);
}

void CCamera_Player::Tick_FreeCamera(_float fTimeDelta)
{
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, min(1.0f, m_fZoomSpeed * fTimeDelta));

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

	if (m_bRadialBlurActivate)
	{
		if (m_fRadialBlurTimer <= 0.0f)
		{
			if (m_fRadialBlurIntensity <= 0.0f)
			{
				m_vRadialPos = Vec3();
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, 0.0f);
				m_fRadialBlurIntensity = 0.0f;
				m_bRadialBlurActivate = false;
				m_fRadialBlurTimer = 0.0f;
			}
			else
			{
				m_fRadialBlurIntensity -= m_fRadialBlurDamping * fTimeDelta;
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
			}
		}
		else
		{
			m_fRadialBlurTimer -= fTimeDelta;
			m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
		}
	}

	if (m_bChromatic)
	{
		if (m_fChromaticAcc <= 0.0f)
		{
			if (m_fChromaticIntensity <= 0.0f)
			{
				m_vChromaticPos = Vec3();
				m_pRendererCom->Set_ChromaticData(m_vChromaticPos, 0.0f);
				m_fChromaticIntensity = 0.0f;
				m_bChromatic = false;
				m_fChromaticAcc = 0.0f;
			}
			else
			{
				m_fChromaticIntensity -= m_fChromaticDamping * fTimeDelta;
				m_pRendererCom->Set_ChromaticData(m_vChromaticPos, m_fChromaticIntensity);
			}
		}
		else
		{
			m_fChromaticAcc -= fTimeDelta;
			m_pRendererCom->Set_ChromaticData(m_vChromaticPos, m_fChromaticIntensity);
		}
	}
}	

void CCamera_Player::Tick_DefaultCamera(_float fTimeDelta)
{
	if (m_fCameraLength != m_fTargetCameraLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fTargetCameraLength, min(1.0f, m_fZoomSpeed * fTimeDelta));

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

	if (m_bRadialBlurActivate)
	{
		if (m_fRadialBlurTimer <= 0.0f)
		{
			if (m_fRadialBlurIntensity <= 0.0f)
			{
				m_vRadialPos = Vec3(0.0f, 0.0f, 0.0f);
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, 0.0f);
				m_fRadialBlurIntensity = 0.0f;
				m_bRadialBlurActivate = false;
				m_fRadialBlurTimer = 0.0f;
			}
			else
			{
				m_fRadialBlurIntensity -= m_fRadialBlurDamping * fTimeDelta;
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
			}
		}
		else
		{
			m_fRadialBlurTimer -= fTimeDelta;
			m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
		}
	}

	if (m_bChromatic)
	{
		if (m_fChromaticAcc <= 0.0f)
		{
			if (m_fChromaticIntensity <= 0.0f)
			{
				m_vChromaticPos = Vec3(0.0f, 0.0f, 0.0f);
				m_pRendererCom->Set_ChromaticData(m_vChromaticPos, 0.0f);
				m_fChromaticIntensity = 0.0f;
				m_bChromatic = false;
				m_fChromaticAcc = 0.0f;
			}
			else
			{
				m_fChromaticIntensity -= m_fChromaticDamping * fTimeDelta;
				m_pRendererCom->Set_ChromaticData(m_vChromaticPos, m_fChromaticIntensity);
			}
		}
		else
		{
			m_fChromaticAcc -= fTimeDelta;
			m_pRendererCom->Set_ChromaticData(m_vChromaticPos, m_fChromaticIntensity);
		}
	}
}

void CCamera_Player::Tick_ResetCamera(_float fTimeDelta)
{
	if (m_fCameraLength != m_fDefaultLength)
	{
		m_fCameraLength = CAsUtils::Lerpf(m_fCameraLength, m_fDefaultLength, min(1.0f, m_fResetSpeed * fTimeDelta));

		if (fabs(m_fDefaultLength - m_fCameraLength) <= 0.001f)
		{
			m_fCameraLength = m_fTargetCameraLength = m_fDefaultLength;
		}
		CRenderer::Set_DOF_Focus(m_fDefaultLength / 1200.f);
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

	if (m_bRadialBlurActivate)
	{
		if (m_fRadialBlurTimer <= 0.0f)
		{
			if (m_fRadialBlurIntensity <= 0.0f)
			{
				m_vRadialPos = Vec3();
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, 0.0f);
				m_fRadialBlurIntensity = 0.0f;
				m_bRadialBlurActivate = false;
				m_fRadialBlurTimer = 0.0f;
			}
			else
			{
				m_fRadialBlurIntensity -= m_fRadialBlurDamping * fTimeDelta;
				m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
			}
		}
		else
		{
			m_fRadialBlurTimer -= fTimeDelta;
			m_pRendererCom->Set_RadialBlurData(m_vRadialPos, m_fRadialBlurIntensity);
		}
	}

	if (m_bChromatic)
	{
		if (m_fChromaticAcc <= 0.0f)
		{
			if (m_fChromaticIntensity <= 0.0f)
			{
				m_vChromaticPos = Vec3();
				m_pRendererCom->Set_ChromaticData(m_vChromaticPos, 0.0f);
				m_fChromaticIntensity = 0.0f;
				m_bChromatic = false;
				m_fChromaticAcc = 0.0f;
			}
			else
			{
				m_fChromaticIntensity -= m_fChromaticDamping * fTimeDelta;
				m_pRendererCom->Set_ChromaticData(m_vChromaticPos, m_fChromaticIntensity);
			}
		}
		else
		{
			m_fChromaticAcc -= fTimeDelta;
			m_pRendererCom->Set_ChromaticData(m_vChromaticPos, m_fChromaticIntensity);
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

HRESULT CCamera_Player::Render_FadeInOut()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_FadeWorld)))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_FadeVeiw)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_FadeProj)))
		return E_FAIL;

	Vec4 vColor = m_vFadeColor;
	vColor.w = m_fFadeIntensity;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &vColor, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin("Texture_FadeInOut")))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Player::Ready_FadeInOut()
{
	m_FadeWorld = Matrix::CreateScale(Vec3(g_iWinSizeX, g_iWinSizeY, 1.0f));

	XMStoreFloat4x4(&m_FadeVeiw, XMMatrixIdentity());
	XMStoreFloat4x4(&m_FadeProj, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
}

void CCamera_Player::Update_FadeInOut(_float fTimeDelta)
{
	if (m_bFadeInOut == true)
	{	
		if (m_fFadeIntensity >= 1.0f)
		{
			m_fFadeIntensity = 1.0f;
		}
		else
		{
			m_fFadeIntensity += m_fFadeSpeed * fTimeDelta;
		}
	}
	else
	{
		if (m_fFadeIntensity <= 0.0f)
		{
			m_fFadeIntensity = 0.0f;
			m_bFade = false;
		}
		else
		{
			m_fFadeIntensity -= m_fFadeSpeed * fTimeDelta;
		}
	}
}

void CCamera_Player::Set_FadeInOut(_float fSpeed, _bool bInOut, Vec4 vColor)
{
	m_fFadeSpeed = fSpeed;
	m_vFadeColor = vColor;

	m_bFadeInOut = bInOut;

	if (m_bFadeInOut == true)
	{
		m_fFadeIntensity = 0.0f;
	}
	else
	{
		m_fFadeIntensity = 1.0f;
	}

	m_bFade = true;
}

HRESULT CCamera_Player::Ready_Components()
{
	__super::Ready_Components();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_DefaultWhite"),
		TEXT("Com_TextureFade"), (CComponent**)&m_pTextureFade)))
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
