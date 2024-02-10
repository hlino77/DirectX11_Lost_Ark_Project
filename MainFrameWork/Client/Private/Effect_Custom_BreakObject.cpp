#include "stdafx.h"
#include "Effect_Custom_BreakObject.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Boss_Valtan.h"
#include "StaticModel.h"
#include "AnimModel.h"


CEffect_Custom_BreakObject::CEffect_Custom_BreakObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{

}

CEffect_Custom_BreakObject::CEffect_Custom_BreakObject(const CEffect_Custom_BreakObject& rhs)
	: Super(rhs)
{

}

HRESULT CEffect_Custom_BreakObject::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_BreakObject::Initialize(void* pArg)
{

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	BreakObjectDesc* pDesc = static_cast<BreakObjectDesc*>(pArg);
	m_pTransformCom->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_vStartPos = pDesc->vPos;
	m_vTargetPos = pDesc->vTargetPos;
	m_strModelName = pDesc->strModelName;

	m_bBreakStart = false;

	m_fLifeTime = 2.f;
	m_fTimeAcc = 0.0f;

	if (pDesc->pOwner)
	{
		m_pOwner = pDesc->pOwner;
	}
	
	Init_Projectile_Motion();

	return S_OK;
}

void CEffect_Custom_BreakObject::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn_Speed(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -10.0f, fTimeDelta);

	if (true == m_bBreakStart)
	{
		Effect_BreakObject();
		Set_Active(false);
		Set_Dead(true);
		return;
	}
	else
	{
		Update_BreakPos(fTimeDelta);
	}
}

void CEffect_Custom_BreakObject::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_BreakObject::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Custom_BreakObject::Ready_Components()
{
	return S_OK;
}

HRESULT CEffect_Custom_BreakObject::Ready_Components(void* pArg)
{

	BreakObjectDesc* pDesc = static_cast<BreakObjectDesc*>(pArg);

	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	wstring strComName = pDesc->strModelName;

	if (FAILED(__super::Add_Component(LEVEL_VALTANMAIN, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

void CEffect_Custom_BreakObject::Init_Projectile_Motion()
{

	srand(static_cast<unsigned int>(time(0))); // Random 

	// Standard Value  : 0.15f
	_float fMin_Time = 0.12f;
	_float fMax_Time = 0.18f;

	// Standard Value : 1.f
	_float fMin_Height = 0.7f;
	_float fMax_Height = 1.2f;

	Vec3 vTargetPos = m_vTargetPos;
	_float fEndHeight = vTargetPos.y - m_vStartPos.y;

	// Random Value
	_float fRandom_Time = fMin_Time + static_cast<_float>(rand()) / (static_cast<_float>(RAND_MAX / (fMax_Time - fMin_Time)));
	_float fRandom_Height = fMin_Height + static_cast<_float>(rand()) / (static_cast<_float>(RAND_MAX / (fMax_Height - fMin_Height)));


	m_fGravity = 2.0f * fRandom_Height / (fRandom_Time * fRandom_Time);

	m_fVelocityY = sqrtf(2.0f * m_fGravity * fRandom_Height);

	_float a = m_fGravity;
	_float b = -2.0f * m_fVelocityY;
	_float c = 2.0f * fEndHeight;

	m_fEndTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2.0f * a);

	m_fVelocityX = -(m_vStartPos.x - vTargetPos.x) / m_fEndTime;
	m_fVelocityZ = -(m_vStartPos.z - vTargetPos.z) / m_fEndTime;

	m_fTimeAcc = 0.0f;

}

void CEffect_Custom_BreakObject::Update_BreakPos(_float fTimedelta)
{
	m_fTimeAcc += fTimedelta;

	m_fTimeAcc = min(m_fTimeAcc, m_fEndTime);

	Vec3 vPos;

	vPos.x = m_vStartPos.x + m_fVelocityX * m_fTimeAcc;
	vPos.y = m_vStartPos.y + (m_fVelocityY * m_fTimeAcc) - (0.5f * m_fGravity * m_fTimeAcc * m_fTimeAcc);
	vPos.z = m_vStartPos.z + m_fVelocityZ * m_fTimeAcc;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);


	_float fDistance = (m_vTargetPos - vPos).Length();
	if (fDistance < 0.1f)
	{
		m_bBreakStart = true;		
	}

}

void CEffect_Custom_BreakObject::Effect_BreakObject()
{
	Matrix matWorld = XMMatrixIdentity();
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos.y += 0.5f;
	matWorld.Translation(vPos);

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
}



CEffect_Custom_BreakObject* CEffect_Custom_BreakObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_BreakObject* pInstance = new CEffect_Custom_BreakObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_BreakObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CEffect_Custom_BreakObject::Clone(void* pArg)
{
	CEffect_Custom_BreakObject* pInstance = new CEffect_Custom_BreakObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_BreakObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_BreakObject::Free()
{
	Super::Free();
}


