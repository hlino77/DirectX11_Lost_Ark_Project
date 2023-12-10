#include "stdafx.h"
#include "GameInstance.h"
#include "Player_Lobby.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"

CPlayer_Lobby::CPlayer_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Player_Lobby", OBJ_TYPE::PLAYER)
{
}

CPlayer_Lobby::CPlayer_Lobby(const CPlayer_Lobby& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer_Lobby::Initialize_Prototype()
{
	
    return S_OK;
}

HRESULT CPlayer_Lobby::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_matView = XMMatrixIdentity();

	m_matProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.2f, 1200.0f);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.38f, -1.0f, 1.08f));

    return S_OK;
}

void CPlayer_Lobby::Tick(_float fTimeDelta)
{
	/*Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (KEY_TAP(KEY::LEFT_ARROW))
	{
		vPos.x -= 0.02f;
	}

	if (KEY_TAP(KEY::RIGHT_ARROW))
	{
		vPos.x += 0.02f;
	}

	if (KEY_TAP(KEY::UP_ARROW))
	{
		vPos.z += 0.02f;
	}

	if (KEY_TAP(KEY::DOWN_ARROW))
	{
		vPos.z -= 0.02f;
	}


	if (KEY_TAP(KEY::P))
	{
		vPos.y += 0.02f;
	}

	if (KEY_TAP(KEY::O))
	{
		vPos.y -= 0.02f;
	}


	if (KEY_TAP(KEY::Y))
	{
		cout << vPos.x << vPos.y << vPos.z << endl;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);*/

}

void CPlayer_Lobby::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pModelCom || nullptr == m_pRendererCom)
		return;


	m_pModelCom->Play_Animation(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

}

HRESULT CPlayer_Lobby::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
				return S_OK;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
				return S_OK;
		}
	}

	Safe_Release(pGameInstance);


    return S_OK;
}

void CPlayer_Lobby::Set_CharacterName(const wstring& szCharacterName)
{
	m_pModelCom = m_Models[szCharacterName];

	_uint iAppearIndex = m_pModelCom->Find_AnimIndex(L"Appear");
	_uint iIdleIndex = m_pModelCom->Find_AnimIndex(L"Idle_Loop");

	_uint iAppearMaxFrame = m_pModelCom->Get_Anim_MaxFrame(iAppearIndex);

	m_pModelCom->Set_CurrAnim(iAppearIndex);
	m_pModelCom->Reserve_NextAnimation(iIdleIndex, 0.1f, 0, iAppearMaxFrame - 1);
}





HRESULT CPlayer_Lobby::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	///* For.Com_Model */


	{
		wstring strComName = L"Prototype_Component_Model_Naruto";
		CModel* pModel = nullptr;
		if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Naruto"), (CComponent**)&pModel)))
			return E_FAIL;

		m_Models.emplace(L"Naruto", pModel);
	}
	
	{
		wstring strComName = L"Prototype_Component_Model_Sasuke";
		CModel* pModel = nullptr;
		if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Sasuke"), (CComponent**)&pModel)))
			return E_FAIL;

		m_Models.emplace(L"Sasuke", pModel);
	}


	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}




CGameObject* CPlayer_Lobby::Clone(void* pArg)
{
	CPlayer_Lobby* pInstance = new CPlayer_Lobby(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Lobby::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}

CPlayer_Lobby* CPlayer_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Lobby* pInstance = new CPlayer_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}
