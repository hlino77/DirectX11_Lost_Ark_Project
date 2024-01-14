#include "stdafx.h"
#include "GameInstance.h"
#include "Guide_Npc.h"
#include "UI_SpeechBubble.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"

CGuide_Npc::CGuide_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CFunction_Npc(pDevice, pContext)
{
}

CGuide_Npc::CGuide_Npc(const CGuide_Npc& rhs)
	: CFunction_Npc(rhs)
{
}

HRESULT CGuide_Npc::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CGuide_Npc::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize(pArg);

	return S_OK;
}

void CGuide_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGuide_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CGuide_Npc::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CGuide_Npc::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CGuide_Npc::Render_Debug()
{
	__super::Render_Debug();

	return S_OK;
}

HRESULT CGuide_Npc::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

HRESULT CGuide_Npc::Ready_Parts()
{
	__super::Ready_Parts();

	return S_OK;
}

HRESULT CGuide_Npc::Render_Model()
{
	__super::Render_Model();

	return S_OK;
}

HRESULT CGuide_Npc::Render_Model_Shadow()
{
	__super::Render_Model_Shadow();

	return S_OK;
}

HRESULT CGuide_Npc::Render_PartModel()
{
	__super::Render_PartModel();

	return S_OK;
}

HRESULT CGuide_Npc::Render_PartModel_Shadow()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pNpcPartCom[i]) continue;

		_uint		iNumMeshes = m_pNpcPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{

			if (FAILED(m_pNpcPartCom[i]->Render(m_pShaderCom, j, "ShadowPass")))
				return S_OK;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CGuide_Npc* CGuide_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGuide_Npc* pInstance = new CGuide_Npc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGuide_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGuide_Npc::Clone(void* pArg)
{
	CGuide_Npc* pInstance = new CGuide_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CGuide_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGuide_Npc::Free()
{
	__super::Free();
}
