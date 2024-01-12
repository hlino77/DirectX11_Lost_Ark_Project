#include "stdafx.h"
#include "Effect_Custom_PerpectShotBullet.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"

CEffect_Custom_PerpectShotBullet::CEffect_Custom_PerpectShotBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_PerpectShotBullet::CEffect_Custom_PerpectShotBullet(const CEffect_Custom_PerpectShotBullet& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_PerpectShotBullet::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_PerpectShotBullet::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	PerpectShotBulletDesc* pDesc = static_cast<PerpectShotBulletDesc*>(pArg);
	m_pTransformCom->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pDesc->vPos + pDesc->vLook * 1.2f);
	m_vTargetPos = pDesc->vLook;

	m_pTransformCom->LookAt_Dir(m_vTargetPos);

	m_fLifeTime = 0.3f;
	m_fTimeAcc = 0.0f;

	vector<CEffect*> EffectList;

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &m_pTransformCom->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START_OUTLIST(TEXT("PerpectShotBullet"), &desc, EffectList);

	m_pBulletEffect = EffectList.front();

	return S_OK;
}

void CEffect_Custom_PerpectShotBullet::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fLifeTime)
	{
		m_pBulletEffect->EffectEnd();
		Set_Active(true);
		Set_Dead(true);
		return;
	}

	m_pTransformCom->Go_Straight(30.0f, fTimeDelta);
	m_pBulletEffect->Update_Pivot(m_pTransformCom->Get_WorldMatrix());
}

void CEffect_Custom_PerpectShotBullet::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_PerpectShotBullet::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Custom_PerpectShotBullet::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Effect_Custom_Grenade";
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

CEffect_Custom_PerpectShotBullet* CEffect_Custom_PerpectShotBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_PerpectShotBullet* pInstance = new CEffect_Custom_PerpectShotBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_PerpectShotBullet::Clone(void* pArg)
{
	CEffect_Custom_PerpectShotBullet* pInstance = new CEffect_Custom_PerpectShotBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_PerpectShotBullet::Free()
{
	Super::Free();
}
