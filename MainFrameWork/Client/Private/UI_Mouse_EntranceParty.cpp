#include "stdafx.h"
#include "UI_Mouse_EntranceParty.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "UI_Mouse_EntranceParty.h"
#include "Player.h"
#include "ServerSessionManager.h"

CUI_Mouse_EntranceParty::CUI_Mouse_EntranceParty(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_Mouse_EntranceParty::CUI_Mouse_EntranceParty(const CUI& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Mouse_EntranceParty::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Mouse_EntranceParty::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_Mouse_EntranceParty");

	m_fSizeX = 200.f * 0.5f;
	m_fSizeY = 50.f * 0.5f;

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_EntranceWnd
	m_pTransform_EntranceWnd->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransform_EntranceWnd->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_TextBox()))
		return	E_FAIL;

	return S_OK;
}

HRESULT CUI_Mouse_EntranceParty::Initialize_TextBox()
{
	if (FAILED(Ready_TextBox()))
		return E_FAIL;
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	m_pTextBox->Set_Active(false);
	m_bActive = false;
	return S_OK;
}

void CUI_Mouse_EntranceParty::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bDeActive)
	{
		m_fDeActiveDelay += fTimeDelta;
		if (0.5f < m_fDeActiveDelay)
		{
			m_fDeActiveDelay = 0.f;
			Reset_Player_Control();
			m_bDeActive = false;
			m_bRender = true;
			m_bActive = false;
		}
	}
}

void CUI_Mouse_EntranceParty::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Update_EntranceButton();
}

HRESULT CUI_Mouse_EntranceParty::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_EntranceWnd()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if(nullptr != m_pTextBox)
		m_pTextBox->Render();

	return S_OK;
}

void CUI_Mouse_EntranceParty::Print_NickNameText()
{
	if (nullptr == m_pTextBox)
		return;

	m_pTextBox->Clear_Text();
	m_pTextBox->Set_Alpha(1.0f);
	m_pTextBox->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", m_strNickName);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pTextBox->Set_Text(m_strWndTag, m_strFont, m_strNickName, Vec2(m_fSizeX * 0.5f, m_fSizeY * 0.5f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.f));
}

void CUI_Mouse_EntranceParty::Set_UIPosition(_bool & bPick)
{
	if (!bPick)
		return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	m_fX = pt.x;
	m_fY = pt.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + (m_fSizeX * 0.5f)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_EntranceWnd->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + (m_fSizeX * 0.5f)) - g_iWinSizeX * 0.5f, -(m_fY + m_fSizeY) + g_iWinSizeY * 0.5f, 0.f));
	m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + (m_fSizeX * 0.5f)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	Create_EntranceWnd();

	bPick = false;
}

void CUI_Mouse_EntranceParty::Set_NickName(const wstring& strNickName)
{
	m_strNickName = strNickName;
	Print_NickNameText();
}

void CUI_Mouse_EntranceParty::Set_Active_EntrancePartyWnd(_bool bActive, CPlayer* pPlayer)
{
	m_bInviteParty = false;
	CPlayer* pControlPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr == pControlPlayer)
		return;
	m_pInvitedPlayer = pPlayer;

	if ((TEXT("Dead_Start") != pControlPlayer->Get_State())&&(TEXT("Dead_End") != pControlPlayer->Get_State()))
	{
		if (TEXT("Idle") != pControlPlayer->Get_State())
		{
			pControlPlayer->Set_State(TEXT("Idle"));
		}
	}
	m_bActive = bActive;
	m_pTextBox->Set_Active(bActive);	
	Set_UIPosition(bActive);
}

HRESULT CUI_Mouse_EntranceParty::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EntranceNameWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EntranceWnd"),
		TEXT("Com_Texture_EntranceWnd"), (CComponent**)&m_pTexture_EntranceWnd)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_EntranceWnd"), (CComponent**)&m_pTransform_EntranceWnd)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Mouse_EntranceParty::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(_float))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_Mouse_EntranceParty::Bind_ShaderResources_EntranceWnd()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_EntranceWnd->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	m_pTexture_EntranceWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
	return S_OK;
}

HRESULT CUI_Mouse_EntranceParty::Ready_TextBox()
{
	if (nullptr == m_pTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);

		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("NonePlayer_PartyEntranceWnd");
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

	return S_OK;
}

void CUI_Mouse_EntranceParty::Update_EntranceButton()
{
	Picking_UI();
	if (m_bPick)
	{
		Set_Player_Control(false);
		m_iTextureIndex = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			Send_InvitationPlayer();
			m_strNickName = TEXT("");
			m_bDeActive = true;
			m_bRender = false;
			m_bInviteParty = true;
		}
	}
	else if (!m_bPick)
	{
		CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;
		Set_Player_Control(true);
		m_iTextureIndex = 0;

		if (TEXT("Idle") != pPlayer->Get_State())
		{
			m_strNickName = TEXT("");
			m_bDeActive = true;
			m_bRender = false;
		}
	}
}

void CUI_Mouse_EntranceParty::Create_EntranceWnd()
{
	m_rcUI.left = LONG((m_fX + (m_fSizeY)) - (m_fSizeX / 2));
	m_rcUI.top = LONG((m_fY + m_fSizeY) - (m_fSizeY / 2));
	m_rcUI.right = LONG((m_fX + (m_fSizeY)) + (m_fSizeX / 2));
	m_rcUI.bottom = LONG((m_fY + m_fSizeY) + (m_fSizeY / 2));
}

void CUI_Mouse_EntranceParty::Send_InvitationPlayer()
{
	Protocol::S_PARTY pkt;
	auto tInvitation = pkt.add_tinvitationparty();
	auto tPartyLeader = tInvitation->add_tplayers();
	tPartyLeader->set_iid(CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID());
	tPartyLeader->set_ilevel(CServerSessionManager::GetInstance()->Get_Player()->Get_CurrLevel());

	auto tPartyMember = tInvitation->add_tplayers();
	tPartyMember->set_iid(m_pInvitedPlayer->Get_ObjectID());
	tPartyMember->set_ilevel(m_pInvitedPlayer->Get_CurrLevel());

	CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));
}

CUI_Mouse_EntranceParty* CUI_Mouse_EntranceParty::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Mouse_EntranceParty* pInstance = new CUI_Mouse_EntranceParty(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Mouse_EntranceParty");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Mouse_EntranceParty::Clone(void* pArg)
{
	CUI_Mouse_EntranceParty* pInstance = new CUI_Mouse_EntranceParty(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Mouse_EntranceParty");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Mouse_EntranceParty::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_EntranceWnd);
	Safe_Release(m_pTransform_EntranceWnd);

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
}
