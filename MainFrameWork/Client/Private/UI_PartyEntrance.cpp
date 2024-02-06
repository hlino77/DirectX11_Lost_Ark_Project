#include "stdafx.h"
#include "UI_PartyEntrance.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Player_Gunslinger.h"
#include "Player_Slayer.h"
#include "Player_Destroyer.h"
#include "Player_Doaga.h"
#include "Player_Controller_GN.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"
#include "Controller_SP.h"
#include "ServerSessionManager.h"
#include "UI_Manager.h"
#include "Party.h"

CUI_PartyEntrance::CUI_PartyEntrance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_PartyEntrance::CUI_PartyEntrance(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PartyEntrance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_PartyEntrance::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_PartyEntrance");

	m_fSizeX = 487.f * 0.6f;
	m_fSizeY = 224.f * 0.6f;
	m_fX = g_iWinSizeX + (m_fSizeX * 0.5f);
	m_fY = 700.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.1f));

	m_fSizeX_Button = 101.f * 0.6f;
	m_fSizeY_Button = 34.f * 0.6f;
	m_fX_Button = 55.5f * 0.6f;
	m_fY_Button = 80.f * 0.6f;
	m_pTransform_AcceptButton->Set_Scale(Vec3(m_fSizeX_Button, m_fSizeY_Button, 1.f));
	m_pTransform_AcceptButton->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - (m_fX_Button)) - g_iWinSizeX * 0.5f, -(m_fY + m_fY_Button) + g_iWinSizeY * 0.5f, 0.1f));

	m_pTransform_RefuseButton->Set_Scale(Vec3(m_fSizeX_Button, m_fSizeY_Button, 1.f));
	m_pTransform_RefuseButton->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + (m_fX_Button)) - g_iWinSizeX * 0.5f, -(m_fY + m_fY_Button) + g_iWinSizeY * 0.5f, 0.1f));

	Create_RectButton();

	m_pTransform_TimerFrame->Set_Scale(Vec3(294.f * 0.5f, 12.f, 1.f));
	m_pTransform_TimerFrame->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.1f));
	m_pTransform_Timer->Set_Scale(Vec3(294.f * 0.5f, 12.f * 0.5f, 1.f));
	m_pTransform_Timer->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.1f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_TextBox()))
		return E_FAIL;

	m_pTextBoxWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.1f));

	m_bClicked_Entrance = false;

	return S_OK;
}

HRESULT CUI_PartyEntrance::Initialize_TextBox()
{
	Ready_TextBox();
	m_pTextBoxWnd->Set_Active(false);
	Set_Active(false);
	return S_OK;
}

void CUI_PartyEntrance::Tick(_float fTimeDelta)
{
	if (m_bDeActive)
	{
		m_fDeActiceDelay += fTimeDelta;
		if (0.5f < m_fDeActiceDelay)
		{
			m_fDeActiceDelay = 0.f;
			m_fCurrTimer = 20.f;
			Reset_Player_Control();
			m_bRender = true;
			m_bDeActive = false;
			m_bActive = false;
		}
	}

	__super::Tick(fTimeDelta);

	if (true == m_bActive)
	{
		if (0.f < m_fCurrTimer)
		{
			m_fCurrTimer -= fTimeDelta;
		}
		else if (0.f > m_fCurrTimer)
		{
			m_fCurrTimer = 20.f;
			m_pTextBoxWnd->Set_Active(false);
			m_bRender = false;
			m_bDeActive = true;
		}
	}
}

void CUI_PartyEntrance::LateTick(_float fTimeDelta)
{
	if (true == m_IsClicked)
	{
		m_IsClicked = false;
		m_pTextBoxWnd->Set_Active(false);
		m_bRender = false;
		m_bDeActive = true;
		return;
	}
	Set_Player_Control();
	Update_Button();

	__super::LateTick(fTimeDelta);

	m_fTimerRatio = m_fCurrTimer / m_fMaxTimer;
	Update_OnTransform(fTimeDelta);
}

HRESULT CUI_PartyEntrance::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_AcceptButton()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_RefuseButton()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_TimerFrame()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Timer()))
		return E_FAIL;
	m_pShaderCom->Begin(16);
	m_pVIBufferCom->Render();

	m_pTextBoxWnd->Render();

	return S_OK;
}

const _bool CUI_PartyEntrance::Get_IsClicked()
{
	if (true == m_bActive)
		return m_bClicked_Entrance;
	else
		return false;
}

void CUI_PartyEntrance::Set_PartyLeaderName(CPlayer* pPlayer)
{
	m_pPartyLeader = pPlayer;
	m_strText = m_pPartyLeader->Get_NickName() + TEXT("¥‘¿Ã ∆ƒ∆º ∏¡˝ø°");
	Print_Text();
}

void CUI_PartyEntrance::Set_Active_EntranceParty(CPlayer* pPartyLeader, CPlayer* pPlayer)
{
	if (!pPlayer->Is_Control())
		return;

	m_tLerp.Init_Lerp(0.3f, g_iWinSizeX + (m_fSizeX * 0.5f), (g_iWinSizeX - (m_fSizeX * 0.5f)), LERP_MODE::SMOOTHSTEP);
	Set_PartyLeaderName(pPartyLeader);
	Set_Active(true);
	m_pTextBoxWnd->Set_Active(true);
}

void CUI_PartyEntrance::Print_Text()
{
	if (nullptr != m_pTextBoxWnd)
	{
		m_pTextBoxWnd->Clear_Text();
		m_pTextBoxWnd->Set_Alpha(1.f);
		m_pTextBoxWnd->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("≥ÿΩºLv1∞ÌµÒBold"), m_strText);
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pTextBoxWnd->Set_Text(m_strTag + TEXT("_FirstLine"), TEXT("≥ÿΩºLv1∞ÌµÒBold"), m_strText, Vec2(m_fSizeX * 0.5f, m_fSizeY * 0.4f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
		
		vMeasure = CGameInstance::GetInstance()->MeasureString(TEXT("≥ÿΩºLv1∞ÌµÒBold"), TEXT("√ ¥Î«œø¥Ω¿¥œ¥Ÿ."));
		vOrigin = vMeasure * 0.5f;
		m_pTextBoxWnd->Set_Text(m_strTag + TEXT("SecondLine"), TEXT("≥ÿΩºLv1∞ÌµÒBold"), TEXT("√ ¥Î«œø¥Ω¿¥œ¥Ÿ."), Vec2(m_fSizeX * 0.5f, m_fSizeY * 0.5f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
	}
}

HRESULT CUI_PartyEntrance::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pTextBoxWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("Party_Entrance_NewWnd");
		m_strTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(m_fSizeX, m_fSizeY);
		m_pTextBoxWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
		if (nullptr == m_pTextBoxWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pTextBoxWnd->Set_Render(false);
		m_pTextBoxWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pTextBoxWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_PartyEntrance::Update_OnTransform(_float fTimeDelta)
{
	m_fX = m_tLerp.Update_Lerp(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.1f));
	m_pTransform_AcceptButton->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - (m_fX_Button)) - g_iWinSizeX * 0.5f, -(m_fY + m_fY_Button) + g_iWinSizeY * 0.5f, 0.1f));
	m_pTransform_RefuseButton->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + (m_fX_Button)) - g_iWinSizeX * 0.5f, -(m_fY + m_fY_Button) + g_iWinSizeY * 0.5f, 0.1f));
	m_pTransform_TimerFrame->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.1f));
	m_pTransform_Timer->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 15.f) + g_iWinSizeY * 0.5f, 0.1f));
	Create_RectButton();

	m_pTextBoxWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.1f));
}

void CUI_PartyEntrance::Create_RectButton()
{
	m_rcButton[0].left = LONG((m_fX - m_fX_Button) - (m_fSizeX_Button / 2));
	m_rcButton[0].top = LONG((m_fY + m_fY_Button) - (m_fSizeY_Button / 2));
	m_rcButton[0].right = LONG((m_fX - m_fX_Button) + (m_fSizeX_Button / 2));
	m_rcButton[0].bottom = LONG((m_fY + m_fY_Button) + (m_fSizeY_Button / 2));

	for (size_t i = 0; i < 2; i++)
	{
		if (0 == i)
		{
			m_rcButton[0].left = LONG((m_fX - m_fX_Button) - (m_fSizeX_Button / 2));
			m_rcButton[0].top = LONG((m_fY + m_fY_Button) - (m_fSizeY_Button / 2));
			m_rcButton[0].right = LONG((m_fX - m_fX_Button) + (m_fSizeX_Button / 2));
			m_rcButton[0].bottom = LONG((m_fY + m_fY_Button) + (m_fSizeY_Button / 2));
		}
		if (1 == i)
		{
			m_rcButton[1].left = LONG((m_fX + m_fX_Button) - (m_fSizeX_Button / 2));
			m_rcButton[1].top = LONG((m_fY + m_fY_Button) - (m_fSizeY_Button / 2));
			m_rcButton[1].right = LONG((m_fX + m_fX_Button) + (m_fSizeX_Button / 2));
			m_rcButton[1].bottom = LONG((m_fY + m_fY_Button) + (m_fSizeY_Button / 2));
		}
	}
}

void CUI_PartyEntrance::Update_Button()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);  // ≈¨∂Û¿Ãæ∆Æ ≥ªø° ∏∂øÏΩ∫ ∆˜¿Œ≈Õ ∞°¡Æø¿±‚ 
	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // ∫‰∆˜∆Æ ∞°¡Æø¿±‚ 

	Update_Buttons(pt);
}

void CUI_PartyEntrance::Update_Buttons(POINT pt)
{
	if (true == Is_Picking_AcceptButton(pt))
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_bClicked_Entrance = true;
			m_IsClicked = true;
			m_iTextureIndex_AcceptButton = 2;

			Send_Join_to_Party();
		}
		m_iTextureIndex_AcceptButton = 1;
	}
	else
		m_iTextureIndex_AcceptButton = 0;

	if (true == Is_Picking_RefuseButton(pt))
	{
		if (KEY_TAP(KEY::LBTN))
		{
			m_bClicked_Entrance = false;
			m_IsClicked = true;
			m_iTextureIndex_RefuseButton = 2;
		}
		m_iTextureIndex_RefuseButton = 1;
	}
	else
		m_iTextureIndex_RefuseButton = 0;
}

_bool CUI_PartyEntrance::Is_Picking_AcceptButton(POINT pt)
{
	if (PtInRect(&m_rcButton[0], pt))
		return true;
	else
		return false;
}

_bool CUI_PartyEntrance::Is_Picking_RefuseButton(POINT pt)
{
	if (PtInRect(&m_rcButton[1], pt))
		return true;
	else
		return false;
}

HRESULT CUI_PartyEntrance::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_EntrancePartyWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_AcceptButton"),
		TEXT("Com_Texture_AcceptButton"), (CComponent**)&m_pTexture_AcceptButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_RefuseButton"),
		TEXT("Com_Texture_RefuseButton"), (CComponent**)&m_pTexture_RefuseButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_Timer_Empty"),
		TEXT("Com_Texture_TimerEmpty"), (CComponent**)&m_pTexture_TimerEmpty)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_Timer"),
		TEXT("Com_Texture_Timer"), (CComponent**)&m_pTexture_TimerGauge)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_AcceptButton"), (CComponent**)&m_pTransform_AcceptButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_RefuseButton"), (CComponent**)&m_pTransform_RefuseButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_TimerFrame"), (CComponent**)&m_pTransform_TimerFrame)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Timer"), (CComponent**)&m_pTransform_Timer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PartyEntrance::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_PartyEntrance::Bind_ShaderResources_AcceptButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_AcceptButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_AcceptButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_AcceptButton);
	return S_OK;
}

HRESULT CUI_PartyEntrance::Bind_ShaderResources_RefuseButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_RefuseButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_RefuseButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_RefuseButton);
	return S_OK;
}

HRESULT CUI_PartyEntrance::Bind_ShaderResources_TimerFrame()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_TimerFrame->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_TimerEmpty->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_PartyEntrance::Bind_ShaderResources_Timer()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Timer->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fTimerRatio, sizeof(_float))))
		return E_FAIL;
	m_pTexture_TimerGauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

void CUI_PartyEntrance::Send_Join_to_Party()
{
	Protocol::S_PARTY pkt;
	auto tJoin = pkt.add_tjoinparty();

	auto tPartyLeader = tJoin->add_tplayer();
	tPartyLeader->set_iid(m_pPartyLeader->Get_ObjectID());
	tPartyLeader->set_ilevel(m_pPartyLeader->Get_CurrLevel());

	auto tPartyMember = tJoin->add_tplayer();
	tPartyMember->set_iid(CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID());
	tPartyMember->set_ilevel(CServerSessionManager::GetInstance()->Get_Player()->Get_CurrLevel());

	CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));
}

CUI_PartyEntrance* CUI_PartyEntrance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PartyEntrance* pInstance = new CUI_PartyEntrance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_PartyEntrance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PartyEntrance::Clone(void* pArg)
{
	CUI_PartyEntrance* pInstance = new CUI_PartyEntrance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_PartyEntrance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PartyEntrance::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_AcceptButton);
	Safe_Release(m_pTexture_RefuseButton);
	Safe_Release(m_pTexture_TimerEmpty);
	Safe_Release(m_pTexture_TimerGauge);
	Safe_Release(m_pTexture_AcceptButton);

	Safe_Release(m_pTransform_AcceptButton);
	Safe_Release(m_pTransform_RefuseButton);
	Safe_Release(m_pTransform_TimerFrame);
	Safe_Release(m_pTransform_Timer);

	if (nullptr != m_pTextBoxWnd)
		m_pTextBoxWnd->Set_Dead(true);
}
