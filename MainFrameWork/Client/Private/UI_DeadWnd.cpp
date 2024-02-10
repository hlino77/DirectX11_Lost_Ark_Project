#include "stdafx.h"
#include "UI_DeadWnd.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Player.h"

CUI_DeadWnd::CUI_DeadWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_DeadWnd::CUI_DeadWnd(const CUI& rhs)
	: CUI(rhs)
{
}
HRESULT CUI_DeadWnd::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CUI_DeadWnd::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_DeadWnd");

	m_fSizeX = 460.f;
	m_fSizeY = 240.f;
	m_fX = g_iWinSizeX - m_fSizeX * 0.6f;
	m_fY = 0.f + (m_fSizeY);

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_AnimEffect
	m_pTransform_AnimEffect->Set_Scale(Vec3(600.f, 300.f, 1.f));
	m_pTransform_AnimEffect->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - m_fSizeY* 0.4f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_ResurrectButton
	m_pTransform_ResurrectButton->Set_Scale(Vec3(101.f, 34.f, 1.f));
	m_pTransform_ResurrectButton->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + m_fSizeY* 0.3f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_WatchingButton
	m_pTransform_WatchingButton->Set_Scale(Vec3(101.f * 1.8f, 34.f * 1.5f, 1.f));
	m_pTransform_WatchingButton->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX * 0.5f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f + 180.f) + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	
	if (FAILED(Initialize_TextBox()))
		return E_FAIL;
	m_pTextBox->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_pTextBox_Watching->Get_TransformCom()->Set_Scale(Vec3(101.f * 1.8f, 34.f * 1.5f, 1.f));
	m_pTextBox_Watching->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX * 0.5f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f + 180.f) + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

HRESULT CUI_DeadWnd::Initialize_TextBox()
{
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	if (FAILED(Ready_TextBox()))
		return E_FAIL;
	m_pTextBox->Set_Active(true);
	m_pTextBox_Watching->Set_Active(true);
	Print_TextBox();
	Print_TextBoxWatching();
	return S_OK;
}

void CUI_DeadWnd::Tick(_float fTimeDelta)
{
}

void CUI_DeadWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_DeadWnd::Render()
{
	if (FAILED(Bind_ShaderResources_WathchingButton()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ButtonEffect()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_AnimFrame()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pTextBox_Watching->Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ResurrectButton()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pTextBox->Render();

	return S_OK;
}

void CUI_DeadWnd::Update_DeadWnd(_float fTimeDelta, POINT pt)
{
	if (nullptr != CServerSessionManager::GetInstance()->Get_Player())
		m_pControlPlayer = CServerSessionManager::GetInstance()->Get_Player();
	else
		return;

	if ((TEXT("Dead_Start") != m_pControlPlayer->Get_State()) && (TEXT("Dead_End") != m_pControlPlayer->Get_State()))
		return;

	if (19.f > m_fAnimFrame)
		m_fAnimFrame += 15.f * fTimeDelta;

	Create_MovingRect();
	Is_Picking_MovingRect(pt);
	MovingWnd(pt);
	Create_ResurrectRect();
	Is_Picking_ResurrectRect(pt);
	Create_WatchingRect();
	Is_Picking_WatchingRect(pt);
}

void CUI_DeadWnd::LateUpdate_DeadWnd(_float fTimeDelta)
{
	if (nullptr != CServerSessionManager::GetInstance()->Get_Player())
		m_pControlPlayer = CServerSessionManager::GetInstance()->Get_Player();
	else
		return;

	if ((TEXT("Dead_Start") != m_pControlPlayer->Get_State()) && (TEXT("Dead_End") != m_pControlPlayer->Get_State()))
		return;

	if (19.f <= m_fAnimFrame)
		m_fAnimFrame = 0.f;

	if (!m_bDecreaseAlpha)
	{
		m_fEffectAlpha += fTimeDelta;
		if (1.f <= m_fEffectAlpha)
		{
			m_fEffectAlpha = 1.f;
			m_bDecreaseAlpha = true;
		}
	}
	else
	{
		m_fEffectAlpha -= fTimeDelta;
		if (0.f >= m_fEffectAlpha)
		{
			m_fEffectAlpha = 0.f;
			m_bDecreaseAlpha = false;
		}
	}
}

HRESULT CUI_DeadWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTextureCom
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_DeadSceneUI"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTexture_AnimEffect
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_AnimEffect"),
		TEXT("Com_Texture_AnimEffect"), (CComponent**)&m_pTexture_AnimEffect)))
		return E_FAIL;
	//m_pTexture_Button
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_Button"),
		TEXT("Com_Texture_Button"), (CComponent**)&m_pTexture_Button)))
		return E_FAIL;
	//m_pTexture_Effect
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_Button_Effect"),
		TEXT("Com_Texture_Effect"), (CComponent**)&m_pTexture_Effect)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_AnimEffect"), (CComponent**)&m_pTransform_AnimEffect)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_WatchButton"), (CComponent**)&m_pTransform_WatchingButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Resurrect"), (CComponent**)&m_pTransform_ResurrectButton)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_DeadWnd::Ready_TextBox()
{
	if (nullptr == m_pTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("ResurrectButton");
		m_strWndTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(m_fSizeX, m_fSizeY);
		m_pTextBox = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pTextBox)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pTextBox->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pTextBox->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		m_pTextBox->Set_Render(false);

		Safe_Release(pGameInstance);
	}

	if (nullptr == m_pTextBox_Watching)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("WatchingButton");
		m_strWatchingTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(101.f * 1.8f, 34.f * 1.5f);
		m_pTextBox_Watching = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
		if (nullptr == m_pTextBox_Watching)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pTextBox_Watching->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pTextBox_Watching->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		m_pTextBox_Watching->Set_Render(false);

		Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_AnimFrame()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_AnimEffect->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_AnimEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fAnimFrame);
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_ResurrectButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResurrectButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex_Button[0]);

	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_WathchingButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_WatchingButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_Button[1]);
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_ButtonEffect()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_WatchingButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fEffectAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Effect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

void CUI_DeadWnd::MovingWnd(POINT pt)
{
	if (m_bMovingWnd)
	{
		_float	MouseMoveX;
		MouseMoveX = (_float)CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);
		m_fX += MouseMoveX * 0.8f;
		m_fY = pt.y + 52.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
		//m_pTransform_AnimEffect
		m_pTransform_AnimEffect->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - m_fSizeY * 0.4f) + g_iWinSizeY * 0.5f, 0.f));
		//m_pTransform_ResurrectButton
		m_pTransform_ResurrectButton->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + m_fSizeY * 0.3f) + g_iWinSizeY * 0.5f, 0.f));
		m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
		m_pTextBox_Watching->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
			Vec3((g_iWinSizeX * 0.5f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f + 180.f) + g_iWinSizeY * 0.5f, 0.f));
		if (KEY_AWAY(KEY::LBTN))
			m_bMovingWnd = false;
	}
}

void CUI_DeadWnd::Create_MovingRect()
{
	m_rcMovingWnd.left = LONG(m_fX - (m_fSizeX / 2));
	m_rcMovingWnd.top = LONG((m_fY - 52.f) - (100.f / 2));
	m_rcMovingWnd.right = LONG(m_fX + (m_fSizeX / 2));
	m_rcMovingWnd.bottom = LONG((m_fY - 52.f) + (100.f / 2));

}

void CUI_DeadWnd::Is_Picking_MovingRect(POINT pt)
{
	if (PtInRect(&m_rcMovingWnd, pt))
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_bMovingWnd = true;
		}
	}
}

void CUI_DeadWnd::Create_ResurrectRect()
{
	m_rcResurrect.left = LONG(m_fX - (101.f / 2));
	m_rcResurrect.top = LONG((m_fY + m_fSizeY * 0.3f) - (34.f / 2));
	m_rcResurrect.right = LONG(m_fX + (101.f / 2));
	m_rcResurrect.bottom = LONG((m_fY + m_fSizeY * 0.3f) + (34.f / 2));
}

void CUI_DeadWnd::Is_Picking_ResurrectRect(POINT pt)
{
	if (PtInRect(&m_rcResurrect, pt))
	{
		m_iTextureIndex_Button[0] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			CServerSessionManager::GetInstance()->Get_Player()->Set_State(TEXT("Resurrect"));
			m_bWatchingMode = false;
			m_bMovingWnd = false;
			m_fAnimFrame = 0.f;
			m_fEffectAlpha = 0.f;
			m_bDecreaseAlpha = false;
			m_iTextureIndex_Button[0] = 0;
			m_iTextureIndex_Button[1] = 0;
			m_bActive = false;
			m_pTextBox->Set_Active(false);
			m_pTextBox_Watching->Set_Active(false);
		}
	}
	else
		m_iTextureIndex_Button[0] = 0;
}

void CUI_DeadWnd::Create_WatchingRect()
{
	m_rcWatching.left = LONG((g_iWinSizeX * 0.5f) - (101.f * 1.8f / 2));
	m_rcWatching.top = LONG((g_iWinSizeY * 0.5f + 180.f) - (34.f * 1.5f / 2));
	m_rcWatching.right = LONG((g_iWinSizeX * 0.5f) + (101.f * 1.8f / 2));
	m_rcWatching.bottom = LONG((g_iWinSizeY * 0.5f + 180.f) + (34.f * 1.5f / 2));
}

void CUI_DeadWnd::Is_Picking_WatchingRect(POINT pt)
{
	if (PtInRect(&m_rcWatching, pt))
	{
		if (2 != m_iTextureIndex_Button[1])
			m_iTextureIndex_Button[1] = 1;

		if (KEY_TAP(KEY::LBTN))
		{
			m_iTextureIndex_Button[1] = 2;
			m_bWatchingMode = true;
		}
	}
	else
	{
		if (2 != m_iTextureIndex_Button[1])
			m_iTextureIndex_Button[1] = 0;
	}
}
void CUI_DeadWnd::Print_TextBox()
{
	if (nullptr == m_pTextBox)
		return;

	m_pTextBox->Clear_Text();
	m_pTextBox->Set_Alpha(1.f);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, TEXT("¼÷ÄÚÀÎÁõ¹öÆ°"));
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pTextBox->Set_Text(m_strWndTag, m_strFont, TEXT("¼÷ÄÚÀÎÁõ¹öÆ°"), Vec2((m_fSizeX * 0.5f), (m_fSizeY - 48.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));
}

void CUI_DeadWnd::Print_TextBoxWatching()
{
	if (nullptr == m_pTextBox_Watching)
		return;
	m_pTextBox_Watching->Clear_Text();
	m_pTextBox_Watching->Set_Alpha(1.f);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, TEXT("°üÀüÇÏ±â"));
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pTextBox_Watching->Set_Text(m_strWatchingTag, m_strFont, TEXT("°üÀüÇÏ±â"), Vec2((101.f * 1.8f * 0.5f), (34.f * 1.5f - 48.f * 0.5f)), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));
}


CUI_DeadWnd* CUI_DeadWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_DeadWnd* pInstance = new CUI_DeadWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_DeadWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_DeadWnd::Clone(void* pArg)
{
	CUI_DeadWnd* pInstance = new CUI_DeadWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_DeadWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_DeadWnd::Free()
{
	__super::Free();
	m_pControlPlayer = nullptr;

	Safe_Release(m_pTexture_AnimEffect);
	Safe_Release(m_pTexture_Button);
	Safe_Release(m_pTexture_Effect);

	Safe_Release(m_pTransform_AnimEffect);
	Safe_Release(m_pTransform_WatchingButton);
	Safe_Release(m_pTransform_ResurrectButton);

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
	if (nullptr != m_pTextBox_Watching)
		m_pTextBox_Watching->Set_Dead(true);
}

