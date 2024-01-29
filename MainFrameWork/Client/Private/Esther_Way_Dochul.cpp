#include "stdafx.h"
#include "GameInstance.h"
#include "Esther_Way_Dochul.h"
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

CEsther_Way_Dochul::CEsther_Way_Dochul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Eshter_Skill_Dochul", OBJ_TYPE::ESTHER)
{
}

CEsther_Way_Dochul::CEsther_Way_Dochul(const CEsther_Way_Dochul& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEsther_Way_Dochul::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Way_Dochul::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_tCullingSphere.Radius = 2.0f;

	m_iAnimAct1 = m_pModelCom->Initailize_FindAnimation(L"sk_dochul_01", 1.2f);
	if (m_iAnimAct1 == -1)
		return E_FAIL;

	m_iAnimAct2 = m_pModelCom->Initailize_FindAnimation(L"sk_dochul_02", 1.2f);
	if (m_iAnimAct2 == -1)
		return E_FAIL;

	m_iAnimAct3 = m_pModelCom->Initailize_FindAnimation(L"sk_dochul_03", 1.2f);
	if (m_iAnimAct3 == -1)
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CEsther_Way_Dochul::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (false == m_bActive)
		return;

	if (m_bNavi)
	{
		CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
	}

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);

}

void CEsther_Way_Dochul::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	//m_pRigidBody->Tick(fTimeDelta);

	CullingObject();

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

void CEsther_Way_Dochul::Act1(_float fTimeDelta)
{
	m_iAnimIndex = m_iAnimAct1;
	m_pModelCom->Set_CurrAnim(m_iAnimIndex);

	m_bActive = true;
}

void CEsther_Way_Dochul::Act2(_float fTimeDelta)
{
	m_iAnimIndex = m_iAnimAct2;
	m_pModelCom->Set_CurrAnim(m_iAnimIndex);

	m_bActive = true;
}

void CEsther_Way_Dochul::Act3(_float fTimeDelta)
{
	m_iAnimIndex = m_iAnimAct3;
	m_pModelCom->Set_CurrAnim(m_iAnimIndex);

	m_bActive = true;
}

void CEsther_Way_Dochul::Check_Finish()
{
	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		m_bActive = false;
	}
}

HRESULT CEsther_Way_Dochul::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render_SingleMesh(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEsther_Way_Dochul::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "ShadowPass")))
			return S_OK;
	}


	return S_OK;
}

HRESULT CEsther_Way_Dochul::Ready_Components()
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

	wstring strComName = L"Prototype_Component_Model_ESWY_Skill";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CEsther_Way_Dochul::CullingObject()
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

void CEsther_Way_Dochul::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY)
{
	if (nullptr == m_pModelCom)
		return;

	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bRootRot, bReverse, bUseY);
}

CEsther_Way_Dochul* CEsther_Way_Dochul::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Way_Dochul* pInstance = new CEsther_Way_Dochul(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Way_Dochul");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Way_Dochul::Clone(void* pArg)
{
	CEsther_Way_Dochul* pInstance = new CEsther_Way_Dochul(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Way_Dochul");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Way_Dochul::Free()
{
	__super::Free();


	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
