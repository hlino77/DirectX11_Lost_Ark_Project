#include "stdafx.h"
#include "SkyBoxNight.h"
#include "Client_Defines.h"
#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Pool.h"



CSkyBoxNight::CSkyBoxNight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)
{
}

CSkyBoxNight::CSkyBoxNight(const CSkyBoxNight& rhs)
	: CEffect(rhs)
{
}

HRESULT CSkyBoxNight::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CSkyBoxNight::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_szModelName = L"SkyBoxNight";

	m_bActive = true;


	Vec3 vScale = Vec3(2.0f, 2.0f, 2.0f);
	m_pTransformCom->Set_Scale(vScale);


	return S_OK;
}

void CSkyBoxNight::Tick(_float fTimeDelta)
{
	//m_fUVX += 0.01f * fTimeDelta;
}

void CSkyBoxNight::LateTick(_float fTimeDelta)
{
	Vec3 vCamPos = CGameInstance::GetInstance()->Get_CamPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSkyBoxNight::Render()
{
	
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return S_OK;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_fUVX", &m_fUVX, sizeof(_float))))
		return S_OK;


	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;*/


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return S_OK;
	}

	Safe_Release(pGameInstance);
	
	return S_OK;
}

HRESULT CSkyBoxNight::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SkyBox"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_SkyBoxDay";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkyBoxNight"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	Safe_Release(pGameInstance);



	return S_OK;
}


CGameObject* CSkyBoxNight::Clone(void* pArg)
{
	CSkyBoxNight* pInstance = new CSkyBoxNight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkyBoxDay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyBoxNight* CSkyBoxNight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyBoxNight* pInstance = new CSkyBoxNight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyBoxDay");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CSkyBoxNight::Free()
{
	__super::Free();
}
