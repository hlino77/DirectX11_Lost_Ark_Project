#include "stdafx.h"
#include "King_Server.h"
#include "BehaviorTree.h"

CKing_Server::CKing_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss_Server(pDevice, pContext)
{
}

CKing_Server::CKing_Server(const CKing_Server& rhs)
	: CBoss_Server(rhs)
{
}

HRESULT CKing_Server::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKing_Server::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CKing_Server::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CKing_Server::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CKing_Server::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}


void CKing_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CKing_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CKing_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CKing_Server::Set_Colliders(_float fTimeDelta)
{
	__super::Set_Colliders(fTimeDelta);
}

HRESULT CKing_Server::Ready_Coliders()
{
	if (FAILED(__super::Ready_Coliders()))
		return E_FAIL;
	return S_OK;
}

HRESULT CKing_Server::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CBoss_Server* CKing_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CKing_Server* pInstance = new CKing_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CKing_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKing_Server::Clone(void* pArg)
{
	CKing_Server* pInstance = new CKing_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CKing_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CKing_Server::Free()
{
	__super::Free();
}
