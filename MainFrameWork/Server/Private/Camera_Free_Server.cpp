#include "stdafx.h"
#include "Camera_Free_Server.h"
#include "GameInstance.h"

CCamera_Free_Server::CCamera_Free_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag)
{
}

CCamera_Free_Server::CCamera_Free_Server(const CCamera_Free_Server & rhs, CTransform::TRANSFORMDESC * pArg)
	: CCamera(rhs, pArg)
{

}

HRESULT CCamera_Free_Server::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free_Server::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	

	return S_OK;
}

void CCamera_Free_Server::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	

	Safe_Release(pGameInstance);

	__super::Tick(fTimeDelta);
}

void CCamera_Free_Server::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Free_Server::Render()
{
	return S_OK;
}

HRESULT CCamera_Free_Server::Ready_Components()
{
	__super::Ready_Components();
	return S_OK;
}

CCamera_Free_Server * CCamera_Free_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Free_Server*		pInstance = new CCamera_Free_Server(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free_Server::Clone(void* pArg)
{
	CCamera::CAMERADESC*		pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Free_Server*		pInstance = new CCamera_Free_Server(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free_Server::Free()
{
	__super::Free();


}
