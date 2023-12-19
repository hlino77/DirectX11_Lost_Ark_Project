#include "stdafx.h"
#include "UI_IdentityGN_ShineFrame.h"
#include "GameInstance.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "ServerSessionManager.h"

CUI_IdentityGN_ShineFrame::CUI_IdentityGN_ShineFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_ShineFrame::CUI_IdentityGN_ShineFrame(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_ShineFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_ShineFrame::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		m_pMainFrame = static_cast<CUI*>(pArg);
	}
	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_strUITag = TEXT("IdentityGN_ShineFrame");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) +(m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_IdentityGN_ShineFrame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	CGameInstance* pInstance = CGameInstance::GetInstance();
	Safe_AddRef(pInstance);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != pPlayer)
	{
		CPlayer_Controller_GN::GN_IDENTITY eIDentity = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller())->Get_GN_Identity();
		if (CPlayer_Controller_GN::GN_IDENTITY::HAND == eIDentity)
			m_iTextureIndex = (_uint)CPlayer_Controller_GN::GN_IDENTITY::HAND;
		else if (CPlayer_Controller_GN::GN_IDENTITY::SHOT == eIDentity)
			m_iTextureIndex = (_uint)CPlayer_Controller_GN::GN_IDENTITY::SHOT;
		else if (CPlayer_Controller_GN::GN_IDENTITY::LONG == eIDentity)
			m_iTextureIndex = (_uint)CPlayer_Controller_GN::GN_IDENTITY::LONG;
	}

	Matrix matWorld = m_pTransformCom->Get_WorldMatrix()* m_pMainFrame->Get_TransformCom()->Get_WorldMatrixInverse();
	Vec3 vScale = m_pMainFrame->Get_TransformCom()->Get_Scale();

	matWorld.Translation(Vec3(0.f, 48.f/vScale.y , 0.f));
	matWorld *= m_pMainFrame->Get_TransformCom()->Get_WorldMatrix();
	m_pTransformCom->Set_WorldMatrix(matWorld);

	m_pTransformCom->My_Rotation(Vec3(0.f, 0.f, -1.f));
	Safe_Release(pInstance);
}

void CUI_IdentityGN_ShineFrame::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_ShineFrame::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

Matrix CUI_IdentityGN_ShineFrame::CombineRotations(const Matrix& rotationSelf, const Matrix& rotationOrbit) {
	return XMMatrixMultiply(rotationSelf, rotationOrbit);
}

HRESULT CUI_IdentityGN_ShineFrame::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_ShineEffect"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_OffSetTransform"), (CComponent**)&m_pOffSetTrasformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_ShineFrame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);

	return S_OK;
}

CUI_IdentityGN_ShineFrame* CUI_IdentityGN_ShineFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_ShineFrame* pInstance = new CUI_IdentityGN_ShineFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_ShineFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_ShineFrame::Clone(void* pArg)
{
	CUI_IdentityGN_ShineFrame* pInstance = new CUI_IdentityGN_ShineFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_ShineFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_ShineFrame::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Delete(m_pOffSetTrasformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
