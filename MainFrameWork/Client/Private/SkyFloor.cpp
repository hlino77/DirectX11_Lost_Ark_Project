#include "stdafx.h"
#include "SkyFloor.h"
#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"
#include "RigidBody.h"

CSkyFloor::CSkyFloor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, TEXT("SkyFloor"), OBJ_TYPE::PROP)
{
}

CSkyFloor::CSkyFloor(const CSkyFloor& rhs)
	: Super(rhs)
{
}

HRESULT CSkyFloor::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CSkyFloor::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_tSKyFloorDesc = *reinterpret_cast<SkyFloorDescription*>(pArg);
	m_vFloorSpeed = m_tSKyFloorDesc.vFloorUVoffset;

	return S_OK;
}

void CSkyFloor::Tick(_float fTimeDelta)
{
	m_tSKyFloorDesc.vFloorUVoffset += fTimeDelta * m_vFloorSpeed;

	while (m_tSKyFloorDesc.vFloorUVoffset.x > 1.f)
		m_tSKyFloorDesc.vFloorUVoffset.x -= 1.f;

	while (m_tSKyFloorDesc.vFloorUVoffset.y > 1.f)
		m_tSKyFloorDesc.vFloorUVoffset.y -= 1.f;
}

void CSkyFloor::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSkyFloor::Render()
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

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, "SkyFloor")))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyFloor::Ready_Components()
{
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	wstring strComName = L"Prototype_Component_Model_SkyFloor";
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

CGameObject* CSkyFloor::Clone(void* pArg)
{
	CSkyFloor* pInstance = new CSkyFloor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkyFloor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyFloor* CSkyFloor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyFloor* pInstance = new CSkyFloor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyFloor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyFloor::Free()
{
	Super::Free();
}
