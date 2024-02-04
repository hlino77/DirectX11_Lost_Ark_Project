#include "stdafx.h"
#include "UI_DeadScene.h"
#include "GameInstance.h"

CUI_DeadScene::CUI_DeadScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_DeadScene::CUI_DeadScene(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_DeadScene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_DeadScene::Initialize(void* pArg)
{
	return S_OK;
}

void CUI_DeadScene::Tick(_float fTimeDelta)
{
}

void CUI_DeadScene::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_DeadScene::Render()
{
	return S_OK;
}

HRESULT CUI_DeadScene::Ready_Components()
{
	return S_OK;
}

HRESULT CUI_DeadScene::Bind_ShaderResources()
{
	return S_OK;
}

CUI_DeadScene* CUI_DeadScene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_DeadScene* pInstance = new CUI_DeadScene(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_DeadScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_DeadScene::Clone(void* pArg)
{
	CUI_DeadScene* pInstance = new CUI_DeadScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_DeadScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_DeadScene::Free()
{
	__super::Free();
}
