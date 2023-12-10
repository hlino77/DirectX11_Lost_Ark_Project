#include "stdafx.h"
#include "ExplosionRing.h"
#include "Client_Defines.h"
#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"
#include "RigidBody.h"
#include "Pool.h"



CExplosionRing::CExplosionRing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)
{
}

CExplosionRing::CExplosionRing(const CExplosionRing& rhs)
	: CEffect(rhs)
{
}

HRESULT CExplosionRing::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CExplosionRing::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_szModelName = L"Effect_ExplosionRing";

	m_bActive = false;


	m_vColor = Vec4(1.0f, 0.3f, 0.0f, 1.0f);
	m_vBlurColor = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

	m_vBlurColor *= 0.02f;

	m_vScale = Vec3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

void CExplosionRing::Tick(_float fTimeDelta)
{
	m_vColor.w -= 1.0f * fTimeDelta;
	if (m_vColor.w < 0.0f)
	{
		Set_Active(false);
		CPool<CExplosionRing>::Return_Obj(this);
	}
	


	Vec3 vScale = m_pTransformCom->Get_Scale();
	vScale = Vec3::Lerp(vScale, m_vScale, 2.0f * fTimeDelta);
	m_pTransformCom->Set_Scale(vScale);


	BillBoard();
}

void CExplosionRing::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CExplosionRing::Render()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(Vec4))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBlurColor", &m_vBlurColor, sizeof(Vec4))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;

	m_pShaderCom->Begin(3);

	m_pVIBufferCom->Render();


	Safe_Release(pGameInstance);
	return S_OK;
}


void CExplosionRing::Appear(Vec3 vPos, Vec3 vScale)
{
	m_bActive = true;
	m_bEnd = false;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pTransformCom->Set_Scale(Vec3(0.1f, 0.1f, 0.0f));

	m_vScale = vScale;



	BillBoard();
}

void CExplosionRing::DisAppear()
{
	m_bEnd = true;
}


void CExplosionRing::BillBoard()
{
	Vec3 vCamPos = CGameInstance::GetInstance()->Get_CamPosition();

	m_pTransformCom->LookAt(vCamPos);
}


HRESULT CExplosionRing::Ready_Components()
{
	__super::Ready_Components();



	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_ExplosionRing"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Tex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;




	return S_OK;
}


CGameObject* CExplosionRing::Clone(void* pArg)
{
	CExplosionRing* pInstance = new CExplosionRing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CExplosionRing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CExplosionRing* CExplosionRing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosionRing* pInstance = new CExplosionRing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CExplosionRing");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CExplosionRing::Free()
{
	__super::Free();
}
