#include "stdafx.h"
#include "SkyDOme.h"
#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"
#include "RigidBody.h"

CSkyDome::CSkyDome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, TEXT("SkyDome0"), OBJ_TYPE::PROP)
{
}

CSkyDome::CSkyDome(const CSkyDome& rhs)
	: Super(rhs)
{
}

HRESULT CSkyDome::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CSkyDome::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSkyDome::Tick(_float fTimeDelta)
{
	
}

void CSkyDome::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSkyDome::Render()
{
	Vec3 vCamPos = m_pGameInstance->Get_CamPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos);
	
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	_int iMeshIndex = m_pModelCom->Get_Meshes().size();
	for (_int i = 0; i < iMeshIndex; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, "SkyDome")))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyDome::Ready_Components()
{
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (LEVEL_BERN == m_pGameInstance->Get_CurrLevelIndex())
		m_strObjectTag = TEXT("SkyDome0");
	else if(LEVEL_CHAOS_1 == m_pGameInstance->Get_CurrLevelIndex())
		m_strObjectTag = TEXT("SkyDome1");
	else if(LEVEL_CHAOS_2 == m_pGameInstance->Get_CurrLevelIndex())
		m_strObjectTag = TEXT("SkyDome1");
	else if(LEVEL_CHAOS_3 == m_pGameInstance->Get_CurrLevelIndex())
		m_strObjectTag = TEXT("SkyDome0");

	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(Super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SkyDome"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

CGameObject* CSkyDome::Clone(void* pArg)
{
	CSkyDome* pInstance = new CSkyDome(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkyDome");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyDome* CSkyDome::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyDome* pInstance = new CSkyDome(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyDome");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyDome::Free()
{
	Super::Free();
}
