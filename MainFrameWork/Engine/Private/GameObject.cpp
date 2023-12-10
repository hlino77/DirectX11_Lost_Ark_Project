#include "GameObject.h"
#include "GameInstance.h"
#include "Component.h"
#include "ColliderBase.h"
#include "ColliderSphere.h"
#include "StateMachine.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, wstring strObjectTag, _int iObjType)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_strObjectTag(strObjectTag)
	, m_iObjType(iObjType)
	, m_vTargetPos(Vec3(0.0f, 0.0f, 0.0f))
	, m_bActive(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_strObjectTag(rhs.m_strObjectTag)
	, m_iObjType(rhs.m_iObjType)
	, m_matTargetWorld(rhs.m_matTargetWorld.load())
	, m_bActive(rhs.m_bActive)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{
	m_matTargetWorld = XMMatrixIdentity();
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::Get_Component(const wstring& strComponentTag)
{
	auto iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;

}

void CGameObject::Set_NoneControlState(const wstring& szName)
{
	m_pStateMachine->Change_State(szName);
}



HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CComponent* pComponent = pGameInstance->Clone_Component(this, iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring& strComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(strComponentTag.c_str()));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Compute_CamZ(Vec4 vWorldPos)
{
	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	Vec4		vCamPos = XMLoadFloat4(&pPipeLine->Get_CamPosition());

	m_fCamDistance = XMVectorGetX(XMVector3Length(vWorldPos - vCamPos));

	Safe_Release(pPipeLine);

	return S_OK;

}


void CGameObject::Add_CollisionStay(_uint iColLayer, CCollider* pCollider)
{
	WRITE_LOCK
	COLLISIONSTAY tCollision;
	tCollision.iColLayer = iColLayer;
	tCollision.pCollider = pCollider;

	m_CollisionList.push_back(tCollision);
}

void CGameObject::Delete_CollisionStay(_uint iColLayer, CCollider* pCollider)
{
	for (auto& Collision = m_CollisionList.begin(); Collision != m_CollisionList.end();)
	{
		if (Collision->iColLayer == iColLayer && Collision->pCollider == pCollider)
		{
			WRITE_LOCK
			Collision = m_CollisionList.erase(Collision);
		}
		else
			++Collision;
	}

}

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
