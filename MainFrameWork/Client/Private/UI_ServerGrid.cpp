#include "stdafx.h"
#include "UI_ServerGrid.h"
#include "GameInstance.h"

CUI_ServerGrid::CUI_ServerGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ServerGrid::CUI_ServerGrid(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ServerGrid::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ServerGrid::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Server_Grid");

	if(nullptr != pArg)
	{
		UIDESC* pDesc = static_cast<UIDESC*>(pArg);

		m_fSizeX = pDesc->fSizeX;
		m_fSizeY = pDesc->fSizeY;
		m_fX = g_iWinSizeX * 0.5f;
		m_fY = pDesc->fY;

		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	}

	m_vColor = Vec4(1.f, 1.f, 1.f, 1.f);

		XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	
		return S_OK;
}

void CUI_ServerGrid::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//Picking_UI();
	//Move_UI(fTimeDelta);
}

void CUI_ServerGrid::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ServerGrid::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_ServerGrid::UI_Tick(_float fTimeDelta)
{
	if ((GRID_PICKING == m_eState) && (KEY_TAP(KEY::LBTN)))
		m_eState = GRID_SELECTED;
	else if ((m_bPick) && (GRID_SELECTED != m_eState))
		m_eState = GRID_PICKING;
	else if ((!m_bPick) && (GRID_SELECTED != m_eState) && (GRID_PICKING == m_eState))
		m_eState = GRID_NORMAL;
}

HRESULT CUI_ServerGrid::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Logo_Grids"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ServerGrid::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture",(_uint)m_eState);

	return S_OK;
}

Vec4 CUI_ServerGrid::Linear_Color(_float fTimeDelta)
{
	return m_vColor + (m_vEndColor - m_vColor) * fTimeDelta;
}

void CUI_ServerGrid::Move_UI(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_fX -= 1.0f;
	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_fX += 1.0f;
	}

	if (KEY_HOLD(KEY::UP_ARROW))
	{
		m_fY -= 1.0f;
	}

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		m_fY += 1.0f;
	}

	if (KEY_HOLD(KEY::U))
	{
		m_fSizeX -= 1.0f;
	}

	if (KEY_HOLD(KEY::I))
	{
		m_fSizeX += 1.0f;
	}

	if (KEY_HOLD(KEY::O))
	{
		m_fSizeY -= 1.0f;
	}

	if (KEY_HOLD(KEY::P))
	{
		m_fSizeY += 1.0f;
	}

	if (KEY_TAP(KEY::Y))
	{
		cout << "Pos : " << m_fX << " " << m_fY << endl;
		cout << "Size : " << m_fSizeX << " " << m_fSizeY << endl;
	}

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, m_vUITargetPos.z));

}

CUI_ServerGrid* CUI_ServerGrid::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ServerGrid* pInstance = new CUI_ServerGrid(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ServerGrid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ServerGrid::Clone(void* pArg)
{
	CUI_ServerGrid* pInstance = new CUI_ServerGrid(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ServerGrid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ServerGrid::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
