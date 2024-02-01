#include "stdafx.h"
#include "Effect_Custom_EarthEaterDecal.h"
#include "Client_Defines.h"
#include "GameInstance.h"

CEffect_Custom_EarthEaterDecal::CEffect_Custom_EarthEaterDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_EarthEaterDecal::CEffect_Custom_EarthEaterDecal(const CEffect_Custom_EarthEaterDecal& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_EarthEaterDecal::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_EarthEaterDecal::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CustomEffectDesc* pDesc = static_cast<CustomEffectDesc*>(pArg);

	m_pTransformCom->Set_Scale(Vec3(2.5f, 5.0f, 2.5f));


	for (_uint i = 0; i < 6; ++i)
	{
		Vec3 vPos = pDesc->vPos;
		Vec3 vOffset = XMVector3Rotate(pDesc->vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(60.0f) * i));
		vPos += vOffset;

		m_pTransformCom->Turn_Axis(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(CGameInstance::GetInstance()->Get_RandomFloat(0.0f, 360.0f)));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_matSubDecal.push_back(m_pTransformCom->Get_WorldMatrix());
	}


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_pTransformCom->Set_Scale(Vec3(3.0f, 5.0f, 3.0f));

	m_fLifeTime = 3.0f;

	m_Variables.vColor_Mul = Vec4(0.1f, 0.1f, 0.1f, 1.0f);

	return S_OK;
}

void CEffect_Custom_EarthEaterDecal::Tick(_float fTimeDelta)
{
	if (m_fTimeAcc >= m_fLifeTime)
	{
		m_Variables.vColor_Mul.w -= fTimeDelta;
		if (m_Variables.vColor_Mul.w <= 0.0f)
		{
			m_Variables.vColor_Mul.w = 0.0f;
			Set_Dead(true);
		}
	}
	else
	{
		m_fTimeAcc += fTimeDelta;
	}
}

void CEffect_Custom_EarthEaterDecal::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_bRender)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DECAL, this)))
			__debugbreak();
	}
}

HRESULT CEffect_Custom_EarthEaterDecal::Render()
{
	
	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Variables", &m_Variables, sizeof(tagFX_Variables))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("EffectMaterialFlag", &m_tNoisMaskEmisDslv.NoisMaskEmisDslv, sizeof(EffectMaterialFlag))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Intensity", &m_Intensity, sizeof(tagFX_Intensity))))
		return E_FAIL;


	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))) ||
		FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")) ||
		FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_Normal"), "g_NormalTarget")) ||
		FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_Properties"), "g_PropertiesTarget")))
		return E_FAIL;

	for (auto& matWorld : m_matSubDecal)
	{
		if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &matWorld, sizeof(Matrix))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_CBuffer("TransformInverse", &matWorld.Invert(), sizeof(Matrix))))
			return E_FAIL;

		if (m_pDiffuseTexture)
		{
			if (FAILED(m_pDecalTexture2->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
			__debugbreak();

		if (FAILED(m_pShaderCom->Begin("AlphaBlend")))
			return E_FAIL;
		if (FAILED(m_pBuffer->Render()))
			return E_FAIL;
	}

	Matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &matWorld, sizeof(Matrix))))
		return E_FAIL;


	Matrix matInverse = matWorld.Invert();

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformInverse", &matInverse, sizeof(Matrix))))
		return E_FAIL;

	if (m_pDiffuseTexture)
	{
		if (FAILED(m_pDiffuseTexture->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
		__debugbreak();

	if (FAILED(m_pShaderCom->Begin("AlphaBlend")))
		return E_FAIL;
	if (FAILED(m_pBuffer->Render()))
		return E_FAIL;

	if (m_pDiffuseTexture)
	{
		if (FAILED(m_pDecalTexture1->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
		__debugbreak();

	if (FAILED(m_pShaderCom->Begin("AlphaBlend")))
		return E_FAIL;
	if (FAILED(m_pBuffer->Render()))
		return E_FAIL;




	return S_OK;
}

HRESULT CEffect_Custom_EarthEaterDecal::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Buffer */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_Buffer"), (CComponent**)&m_pBuffer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectDecal"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal1"),
		TEXT("Com_TextureDecal1"), (CComponent**)&m_pDiffuseTexture)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal2"),
		TEXT("Com_TextureDecal2"), (CComponent**)&m_pDecalTexture1)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal3"),
		TEXT("Com_TextureDecal3"), (CComponent**)&m_pDecalTexture2)))
		return E_FAIL;

	return S_OK;
}

CEffect_Custom_EarthEaterDecal* CEffect_Custom_EarthEaterDecal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_EarthEaterDecal* pInstance = new CEffect_Custom_EarthEaterDecal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_EarthEaterDecal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_EarthEaterDecal::Clone(void* pArg)
{
	CEffect_Custom_EarthEaterDecal* pInstance = new CEffect_Custom_EarthEaterDecal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_EarthEaterDecal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_EarthEaterDecal::Free()
{
	Super::Free();
}
