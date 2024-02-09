#include "stdafx.h"
#include "..\Public\Esther_Silian.h"
#include "GameInstance.h"
#include "Esther_Silian_Cut.h"
#include "Esther_Silian_Skill.h"
#include "Player.h"
#include "Esther_Scene.h"

CEsther_Silian::CEsther_Silian(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther(pDevice, pContext)
{
}

CEsther_Silian::CEsther_Silian(const CEsther_Silian& rhs)
	: CEsther(rhs)
{
}

HRESULT CEsther_Silian::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Silian::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Silian");
	m_iEstherType = (_uint)ESTHERTYPE::SA;

	CEsther_Skill::ESTHERSKILLDESC SkillDesc;
	SkillDesc.pLeaderPlayer = m_pLeaderPlayer;
	SkillDesc.pOwnerEsther = this;
	m_pEsther_Skill = static_cast<CEsther_Silian_Skill*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Silian_Skill"), &SkillDesc));

	CEsther_Scene::ESTHERSCENEDESC SceneDesc;
	SceneDesc.fStartFrame = 0.f;
	SceneDesc.fFinalFrame = 138.f;
	SceneDesc.fFrameSpeed = 25.f;
	SceneDesc.strTexutre = TEXT("Esther_Cut_Silian");
	m_pEsther_Scene = static_cast<CEsther_Scene*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Scene"), &SceneDesc));

	/*CEsther_Cut::ESTHERCUTDESC CutDesc;
	CutDesc.pLeaderPlayer = m_pLeaderPlayer;
	CutDesc.pOwnerEsther = this;
	m_pEsther_Cut = static_cast<CEsther_Silian_Cut*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Silian_Cut"), &CutDesc));*/

	return S_OK;
}

void CEsther_Silian::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEsther_Silian::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEsther_Silian::Render()
{
	return S_OK;
}

void CEsther_Silian::Leader_Active_Esther()
{
	__super::Leader_Active_Esther();
}

HRESULT CEsther_Silian::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

CEsther_Silian* CEsther_Silian::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Silian* pInstance = new CEsther_Silian(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Silian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Silian::Clone(void* pArg)
{
	CEsther_Silian* pInstance = new CEsther_Silian(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Silian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Silian::Free()
{
	__super::Free();
}
