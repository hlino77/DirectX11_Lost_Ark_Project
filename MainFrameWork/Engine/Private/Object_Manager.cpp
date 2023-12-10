#include "..\Public\Object_Manager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint iNumLevels, _uint iNumLayerTypes)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new vector<CLayer*>[iNumLevels];

	for (_uint i = 0; i < iNumLevels; ++i)
	{
		m_pLayers[i].reserve(iNumLevels + 1);
		for (_uint j = 0; j < iNumLayerTypes; ++j)
			m_pLayers[i].push_back(CLayer::Create());
	}

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}




CGameObject* CObject_Manager::Add_GameObject(_uint iLevelIndex, const _uint iLayerType, const wstring & strPrototypeTag, void * pArg)
{
	/* 복제할 사본을 차즌ㄷ나. */
	CGameObject*		pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	CLayer*		pLayer = Find_Layer(iLevelIndex, iLayerType);
	if (nullptr == pLayer)
		return nullptr;

	//wstring szObjectTag = pGameObject->Get_ObjectTag();


	//CGameObject* pObject = pLayer->Find_GameObject(szObjectTag);
	//_uint iIndex = 0;

	//while (pObject)
	//{
	//	pObject = pLayer->Find_GameObject(szObjectTag);

	//	if (pObject)
	//	{
	//		++iIndex;
	//		_tchar			szName[MAX_PATH] = TEXT("");
	//		szObjectTag = pGameObject->Get_ObjectTag();
	//		szObjectTag += L"_%d";

	//		wsprintf(szName, szObjectTag.c_str(), iIndex);
	//		szObjectTag = szName;
	//	}
	//	else
	//		pGameObject->Set_ObjectTag(szObjectTag);
	//}

	
	pLayer->Add_GameObject(pGameObject);
	
	return pGameObject;
}

CGameObject* CObject_Manager::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);

	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pGameObject = pPrototype->Clone(pArg);

	if (nullptr == pGameObject)
		return nullptr;


	return pGameObject;
}

CGameObject* CObject_Manager::Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, const wstring& strObjectTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerType);
	if (nullptr == pLayer)
		return nullptr;
	
	CGameObject* pObj = pLayer->Find_GameObject(strObjectTag);

	if (nullptr == pObj)
		return nullptr;

	return pObj;
}

CGameObject* CObject_Manager::Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, _int iObjectID)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerType);
	if (nullptr == pLayer)
		return nullptr;

	CGameObject* pObj = pLayer->Find_GameObject(iObjectID);

	if (nullptr == pObj)
		return nullptr;

	return pObj;
}

list<CGameObject*>& CObject_Manager::Find_GameObjects(_uint iLevelIndex, const _uint iLayerType)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerType);
	if (nullptr == pLayer)
	{
		MSG_BOX("Find_GameObejcts Failed.");
		assert(nullptr);
	}

	return m_pLayers[iLevelIndex][iLayerType]->Find_GameObjects();
}

CGameObject* CObject_Manager::Find_NearGameObject(_uint iLevelIndex, const _uint iLayerType, CGameObject* pCallObject)
{
	list<CGameObject*>& ObjectList = Find_Layer(iLevelIndex, iLayerType)->Find_GameObjects();

	CGameObject* pFindObject = nullptr;
	_float fDistance = -1.0f;

	if (ObjectList.empty())
		return nullptr;


	for (auto& Object : ObjectList)
	{
		if (Object->Is_Die() || Object->Is_Active() == false)
			continue;

		if (pFindObject == nullptr)
		{
			pFindObject = Object;
			Vec3 vCallObjectPos = pCallObject->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
			Vec3 vObjectPos = Object->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
			fDistance = (vCallObjectPos - vObjectPos).Length();
			continue;
		}
			
		Vec3 vCallObjectPos = pCallObject->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
		Vec3 vObjectPos = Object->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);

		_float fNewDistance = (vObjectPos - vCallObjectPos).Length();
		
		if (fNewDistance < fDistance)
		{
			pFindObject = Object;
			fDistance = fNewDistance;
		}
	}

	return pFindObject;
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, const _uint iLayerType, const CGameObject* pGameObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerType);
	if (nullptr == pLayer)
		return E_FAIL;


	return pLayer->Delete_GameObject(pGameObject);
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& pLayer : m_pLayers[i])
		{
			pLayer->Tick(fTimeDelta);
		}
	}

}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& pLayer : m_pLayers[i])
		{
			pLayer->LateTick(fTimeDelta);
			
		}
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& pLayer : m_pLayers[iLevelIndex])
	{
		pLayer->Clear();
	}
	//m_pLayers[iLevelIndex].clear();
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, _uint iLayerType)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CLayer* pLayer = m_pLayers[iLevelIndex][iLayerType];

	if (nullptr == pLayer)
		return nullptr;

	return pLayer;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (size_t j = 0; j < m_pLayers[i].size(); ++j)
			Safe_Release(m_pLayers[i][j]);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& pObj : m_Prototypes)
		Safe_Release(pObj.second);

	m_Prototypes.clear();

	
}
