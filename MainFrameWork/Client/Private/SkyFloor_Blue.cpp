#include "stdafx.h"
#include "SkyFloor_Blue.h"
#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"
#include "RigidBody.h"

CSkyFloor_Blue::CSkyFloor_Blue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, TEXT("SkyFloor"), OBJ_TYPE::PROP)
{
}

CSkyFloor_Blue::CSkyFloor_Blue(const CSkyFloor_Blue& rhs)
	: Super(rhs)
{
}

HRESULT CSkyFloor_Blue::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CSkyFloor_Blue::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_tSKyFloorDesc = *reinterpret_cast<SkyFloorDescription*>(pArg);
	m_vFloorSpeed = m_tSKyFloorDesc.vFloorUVoffset;

	return S_OK;
}

void CSkyFloor_Blue::Tick(_float fTimeDelta)
{
	m_tSKyFloorDesc.vFloorUVoffset += fTimeDelta * m_vFloorSpeed;

	while (m_tSKyFloorDesc.vFloorUVoffset.x > 1.f)
		m_tSKyFloorDesc.vFloorUVoffset.x -= 1.f;

	while (m_tSKyFloorDesc.vFloorUVoffset.y > 1.f)
		m_tSKyFloorDesc.vFloorUVoffset.y -= 1.f;
}

void CSkyFloor_Blue::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSkyFloor_Blue::Render()
{
	/*Vec3 vCamPos = m_pGameInstance->Get_CamPosition();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos);*/
	
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	_int iMeshIndex = (_int)m_pModelCom->Get_Meshes().size();
	for (_int i = 0; i < iMeshIndex; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_CBuffer("cbSkyFloorDesc", &m_tSKyFloorDesc, sizeof(SkyFloorDescription))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Texture("g_NoiseTexture", m_pTexture->Get_SRV())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColorMul", &m_tSKyFloorDesc.vFloorColorMul, sizeof(Vec3))))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, "SkyFloor")))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyFloor_Blue::Ready_Components()
{
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	wstring strComName = L"Prototype_Component_Model_SkyFloor_Blue";
	if (FAILED(Super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SkyDome"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkyFloorNoise"), TEXT("Com_Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

CGameObject* CSkyFloor_Blue::Clone(void* pArg)
{
	CSkyFloor_Blue* pInstance = new CSkyFloor_Blue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkyFloor_Blue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyFloor_Blue* CSkyFloor_Blue::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyFloor_Blue* pInstance = new CSkyFloor_Blue(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyFloor_Blue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyFloor_Blue::Free()
{
	Super::Free();
}
