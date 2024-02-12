#include "stdafx.h"
#include "GameInstance.h"
#include "Esther_Skill.h"
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
#include "Skill.h"
#include "Pool.h"
#include "Player.h"
#include "Esther.h"
#include "PartObject.h"

#include "Sound_Manager.h"

CEsther_Skill::CEsther_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Eshter_Skill", OBJ_TYPE::ESTHER)
{
}

CEsther_Skill::CEsther_Skill(const CEsther_Skill& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEsther_Skill::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CEsther_Skill::Initialize(void* pArg)
{
	ESTHERSKILLDESC* pDesc = static_cast<ESTHERSKILLDESC*>(pArg);
	m_pLeaderPlayer = pDesc->pLeaderPlayer;
	m_pOwnerEsther = pDesc->pOwnerEsther;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_tCullingSphere.Radius = 2.0f;

	m_bSoundOn = new _bool[m_iNumSound];
	return S_OK;
}

void CEsther_Skill::Tick(_float fTimeDelta)
{
	if (m_bNavi)
	{
		//CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
	}

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
}

void CEsther_Skill::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	m_pModelCom->Set_ToRootPos(m_pTransformCom);

	if (nullptr != m_pPart)
		m_pPart->Tick(fTimeDelta);

	CullingObject();

	if (nullptr != m_pPart)
		m_pPart->LateTick(fTimeDelta);

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

HRESULT CEsther_Skill::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}

HRESULT CEsther_Skill::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}


void CEsther_Skill::Add_Sound(_int iAnimIndex, _int iSoundOnIndex, const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume, _int iAnimFrame)
{
	if (m_pModelCom->Get_CurrAnim() == iAnimIndex && m_pModelCom->Get_Anim_Frame(iAnimIndex) >= iAnimFrame && !m_bSoundOn[iSoundOnIndex])
	{
		m_bSoundOn[iSoundOnIndex] = true;
		CSound_Manager::GetInstance()->PlaySoundFile(szChannelGroup, strSoundKey + L".wav", fVolume);
	}
}

void CEsther_Skill::Add_Sound(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume)
{
	CSound_Manager::GetInstance()->PlaySoundFile(szChannelGroup, strSoundKey + L".wav", fVolume);
}

HRESULT CEsther_Skill::Ready_Components()
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

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEsther_Skill::Ready_ModelPart()
{
	return S_OK;
}

HRESULT CEsther_Skill::Ready_Parts()
{
	return S_OK;
}

HRESULT CEsther_Skill::Ready_Projectile()
{
	return S_OK;
}

void CEsther_Skill::CullingObject()
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}

}

void CEsther_Skill::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY, _bool bIgnoreRoot)
{
	if (nullptr == m_pModelCom)
		return;

	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bRootRot, bReverse, bUseY, bIgnoreRoot);
}

void CEsther_Skill::Free()
{
	__super::Free();

	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
