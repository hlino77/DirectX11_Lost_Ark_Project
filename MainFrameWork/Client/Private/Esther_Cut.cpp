#include "stdafx.h"
#include "GameInstance.h"
#include "Esther_Cut.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "PickingMgr.h"
#include "Struct.pb.h"
#include "NavigationMgr.h"
#include "Pool.h"

#include "Player.h"
#include "Esther.h"
#include "PartObject.h"

#include "Camera_Cut.h"

CEsther_Cut::CEsther_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Esther_Cut", OBJ_TYPE::ESTHER)
{
}

CEsther_Cut::CEsther_Cut(const CEsther_Cut& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEsther_Cut::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Cut::Initialize(void* pArg)
{
	ESTHERCUTDESC* pDesc = static_cast<ESTHERCUTDESC*>(pArg);
	m_pLeaderPlayer = pDesc->pLeaderPlayer;
	m_pOwnerEsther = pDesc->pOwnerEsther;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_IsFinished = true;

	return S_OK;
}

void CEsther_Cut::Tick(_float fTimeDelta)
{
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);

	if (nullptr != m_pPart)
		m_pPart->Tick(fTimeDelta);
}

void CEsther_Cut::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	m_pModelCom->Set_ToRootPos(m_pTransformCom);

	if (nullptr != m_pPart)
		m_pPart->LateTick(fTimeDelta);

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}

	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}
}

HRESULT CEsther_Cut::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &Get_TransformCom()->Get_WorldMatrix(), sizeof(Matrix))))
		return E_FAIL;

	GlobalDesc gDesc = {
		m_pCutCamera->Get_TransformCom()->Get_WorldMatrixInverse(),
		m_pCutCamera->Get_ProjMatrix(),
		m_pCutCamera->Get_TransformCom()->Get_WorldMatrixInverse() * m_pCutCamera->Get_ProjMatrix(),
		m_pCutCamera->Get_TransformCom()->Get_WorldMatrix()
	};

	if (FAILED(m_pShaderCom->Bind_CBuffer("GlobalBuffer", &gDesc, sizeof(GlobalDesc))))
		return E_FAIL;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}

_bool CEsther_Cut::Stop_VoiceSound()
{
	if (m_fVoiceSoundDelay == 0.0f)
	{
		CGameInstance::GetInstance()->Find_Stop_Sound(m_VoiceSoundKey);
		return true;
	}

	return false;
}

HRESULT CEsther_Cut::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEsther_Cut::Ready_Parts()
{
	return S_OK;
}

void CEsther_Cut::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY, _bool bIgnoreRoot)
{
	if (nullptr == m_pModelCom)
		return;

	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bRootRot, bReverse, bUseY, bIgnoreRoot);
}

void CEsther_Cut::Free()
{
	__super::Free();
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}