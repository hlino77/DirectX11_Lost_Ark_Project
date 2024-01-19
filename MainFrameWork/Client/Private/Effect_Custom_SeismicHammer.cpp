#include "stdafx.h"
#include "Effect_Custom_SeismicHammer.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"

CEffect_Custom_SeismicHammer::CEffect_Custom_SeismicHammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_SeismicHammer::CEffect_Custom_SeismicHammer(const CEffect_Custom_SeismicHammer& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_SeismicHammer::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_SeismicHammer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	EffectDesc* pDesc = static_cast<EffectDesc*>(pArg);

	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XM_PI * 0.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPos);

	m_fLifeTime = 10.3f;
	m_fTimeAcc = 0.0f;

	return S_OK;
}

void CEffect_Custom_SeismicHammer::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fLifeTime)
	{
		Set_Active(true);
		Set_Dead(true);
		return;
	}

}

void CEffect_Custom_SeismicHammer::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_SeismicHammer::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Custom_SeismicHammer::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_SeismicHammer";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	Vec3 vScale;
	vScale.y = 1.0f;
	vScale.z = 1.0f;
	vScale.x = 1.0f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

CEffect_Custom_SeismicHammer* CEffect_Custom_SeismicHammer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_SeismicHammer* pInstance = new CEffect_Custom_SeismicHammer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_SeismicHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_SeismicHammer::Clone(void* pArg)
{
	CEffect_Custom_SeismicHammer* pInstance = new CEffect_Custom_SeismicHammer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_SeismicHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_SeismicHammer::Free()
{
	Super::Free();
}
