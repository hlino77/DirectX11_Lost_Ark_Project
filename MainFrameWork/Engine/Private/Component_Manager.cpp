#include "Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{

}

HRESULT CComponent_Manager::Reserve_Manager(_uint iNumLevels)
{
    if (nullptr != m_Prototypes)
        return S_OK;

    m_iNumLevels = iNumLevels;

    m_Prototypes = new PROTOTYPES[iNumLevels];

    return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag, CComponent* pPrototype)
{
    if (iLevelIndex >= m_iNumLevels)
        return E_FAIL;

    if (nullptr != Find_Component(iLevelIndex, strProtoTypeTag))
        return E_FAIL;

    m_Prototypes[iLevelIndex].emplace(strProtoTypeTag, pPrototype);

    return S_OK;
}

CComponent* CComponent_Manager::Clone_Component(CGameObject* pObject, _uint iLevelIndex, const wstring& strProtoTypeTag, void* pArg)
{
    CComponent* pPrototype = Find_Component(iLevelIndex, strProtoTypeTag);
    if (nullptr == pPrototype)
        return nullptr;

    return pPrototype->Clone(pObject, pArg);
}

HRESULT CComponent_Manager::Check_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag)
{
    CComponent* pPrototype = Find_Component(iLevelIndex, strProtoTypeTag);
    if (nullptr == pPrototype)
        return S_OK;

    return E_FAIL;
}

CComponent* CComponent_Manager::Find_Component(_uint iLevelIndex, const wstring& strProtoTypeTag)
{
    auto	iter = m_Prototypes[iLevelIndex].find(strProtoTypeTag);

    if (iter == m_Prototypes[iLevelIndex].end())
        return nullptr;

    return iter->second;
}



void CComponent_Manager::Free()
{
    for (_uint i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_Prototypes[i])
            Safe_Release(Pair.second);

        m_Prototypes[i].clear();
    }

    Safe_Delete_Array(m_Prototypes);
}
