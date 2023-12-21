#include "stdafx.h"
#include "GameInstance.h"
#include "VoidEffect.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "BindShaderDesc.h"

CVoidEffect::CVoidEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, TEXT("void"), -1)
{
}

CVoidEffect::CVoidEffect(const CVoidEffect& rhs)
	: Super(rhs)
{
}

HRESULT CVoidEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVoidEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, 0.f, 0.f));

    return S_OK;
}

void CVoidEffect::Tick(_float fTimeDelta)
{
}

void CVoidEffect::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;
}

HRESULT CVoidEffect::Render()
{
	return S_OK;
}

HRESULT CVoidEffect::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

    return S_OK;
}

CVoidEffect* CVoidEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVoidEffect* pInstance = new CVoidEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVoidEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVoidEffect::Clone(void* pArg)
{
	CVoidEffect* pInstance = new CVoidEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVoidEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVoidEffect::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
