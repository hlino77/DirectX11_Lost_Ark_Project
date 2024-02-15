#include "stdafx.h"
#include "UI_WatchingMode.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "UI_Manager.h"
#include "Player.h"
#include "Party.h"
#include "Camera_Player.h"
#include "Sound_Manager.h"

CUI_WatchingMode::CUI_WatchingMode(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_WatchingMode::CUI_WatchingMode(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WatchingMode::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WatchingMode::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_WatchingMode");
	m_fSizeX = 695.f;
	m_fSizeY = 140.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = 750.f;

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_Button
	m_pTransform_Button->Set_Scale(Vec3(101.f * 1.5f, 34.f, 1.f));
	m_pTransform_Button->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX - 100.f) - g_iWinSizeX * 0.5f, -(100.f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_NextButton
	m_pTransform_NextButton->Set_Scale(Vec3(23.f, 30.f, 1.f));
	m_pTransform_NextButton->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(180.f));
	m_pTransform_NextButton->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + m_fSizeX * 0.35f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//Vec3((m_fX + m_fSizeX*0.35f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_BackButton
	m_pTransform_BackButton->Set_Scale(Vec3(23.f, 30.f, 1.f));
	m_pTransform_BackButton->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - m_fSizeX * 0.35f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_TextBox()))
		return E_FAIL;
	m_pTextBox->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_pReturnTextBox->Get_TransformCom()->Set_Scale(Vec3(101.f * 1.5f, 34.f, 1.f));
	m_pReturnTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX - 100.f) - g_iWinSizeX * 0.5f, -(100.f) + g_iWinSizeY * 0.5f, 0.f));

	m_bActive = false;

	return S_OK;
}

HRESULT CUI_WatchingMode::Initialize_TextBox()
{
	m_strFont = TEXT("넥슨Lv1고딕Bold");
	if (FAILED(Ready_TextBox()))
		return E_FAIL;

	m_pTextBox->Set_Active(false);
	m_pReturnTextBox->Set_Active(false);
	return S_OK;
}

void CUI_WatchingMode::Tick(_float fTimeDelta)
{
}

void CUI_WatchingMode::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_WatchingMode::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Button()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pReturnTextBox->Render();

	if (FAILED(Bind_ShaderResources_NextButton()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BackButton()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pTextBox->Render();

	return S_OK;
}

void CUI_WatchingMode::Set_WatchingMode(_bool bWatching)
{
	CUI* pUI = nullptr;

	if (TEXT("Gunslinger") == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_IdentityGN"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_QuickSlot"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
	}
	else if (TEXT("WD") == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_WRIdentity"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_QuickSlot"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
	}
	else if (TEXT("WDR") == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("IdentityWDR_UI"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_QuickSlot"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
	}
	else if (TEXT("SP") == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_IdentitySP"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
		pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
			TEXT("UI_QuickSlot"));
		for (auto& iter : pUI->Get_UIParts())
		{
			iter->Set_Active(!bWatching);
		}
	}
	pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
		TEXT("UI_PlayerHP"));
	for (auto& iter : pUI->Get_UIParts())
	{
		iter->Set_Active(!bWatching);
	}
	pUI = CUI_Manager::GetInstance()->Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(),
		TEXT("UI_PlayerMp"));
	for (auto& iter : pUI->Get_UIParts())
	{
		iter->Set_Active(!bWatching);
	}
	
	m_pTextBox->Set_Active(bWatching);
	m_pReturnTextBox->Set_Active(bWatching);
	m_bWathingMode = true;
	m_bActive = bWatching;

}

void CUI_WatchingMode::Update_CCTV(_float fTimeDelta, POINT pt)
{
	Create_BackButton();
	Create_ReturnButton();
	Create_NextButton();

	Is_Picking_BackButton(pt);
	Is_Picking_ReturnButton(pt);
	Is_Picking_NextButton(pt);

}

void CUI_WatchingMode::LateUpdate_CCTV(_float fTimeDelta)
{
}

void CUI_WatchingMode::Set_TargetPlayer()
{
	if (nullptr == CServerSessionManager::GetInstance()->Get_Player()->Get_Party())
		return;
	_uint iIndex = 0;
	for (auto& iter : CServerSessionManager::GetInstance()->Get_Player()->Get_Party()->Get_PartyMembers())
	{
		if (iter != CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID())
		{
			m_iPartyIds[iIndex] = iter;
			iIndex++;
		}
	}

	Find_IndexPartyMember();
	Print_TextBox();
}

void CUI_WatchingMode::Reset_TargetPlayer()
{
	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->
		Set_Target(CServerSessionManager::GetInstance()->Get_Player());
}

void CUI_WatchingMode::Print_TextBox()
{
	if (nullptr == m_pTextBox)
		return;
	else
	{	
		m_pTextBox->Clear_Text();
		m_pTextBox->Set_Alpha(1.f);

		strNowPlayer = m_pWatchingPlayer->Get_NickName() + TEXT("님의 플레이를 관전 중입니다.");
		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, strNowPlayer);
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strWndTag, m_strFont, strNowPlayer, Vec2((m_fSizeX * 0.5f), (m_fSizeY * 0.5f)), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));
	}

	if (nullptr == m_pReturnTextBox)
		return;
	else
	{
		m_pReturnTextBox->Clear_Text();
		m_pReturnTextBox->Set_Alpha(1.f);

		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_strFont, TEXT("관전 종료"));
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pReturnTextBox->Set_Text(m_strReturnWndTag, m_strFont, TEXT("관전 종료"), Vec2((101.f * 1.5f * 0.5f), (34.f * 0.5f)), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));
	}
}

void CUI_WatchingMode::Create_BackButton()
{
	m_rcButton[0].left = LONG((m_fX - m_fSizeX * 0.35f) - (23.f / 2));
	m_rcButton[0].top = LONG(m_fY - (30.f / 2));
	m_rcButton[0].right = LONG((m_fX - m_fSizeX * 0.35f) + (23.f / 2));
	m_rcButton[0].bottom = LONG(m_fY + (30.f / 2));
}

void CUI_WatchingMode::Is_Picking_BackButton(POINT pt)
{
	if (PtInRect(&m_rcButton[0], pt))
	{
		if (!m_bSound[0])
		{
			m_bSound[0] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		m_iChangeButton[0] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			if (nullptr == CServerSessionManager::GetInstance()->Get_Player()->Get_Party())
				return;

			if ((m_iPartyIndex - 1) < 0)
			{
				for (size_t i = 2; i >= 0; i--)
				{
					if (0 != m_iPartyIds[i])
					{
						m_iPartyIndex = i;
						break;
					}
				}
			}
			else
			{
				m_iPartyIndex--;
				if (0 == m_iPartyIds[m_iPartyIndex])
				{
					for (size_t i = 2; i >= 0; i--)
					{
						if (0 != m_iPartyIds[i])
						{
							m_iPartyIndex = i;
							break;
						}
					}
				}
			}
			Find_IndexPartyMember();
			Print_TextBox();
		}
	}
	else
	{
		m_iChangeButton[0] = 0;
		m_bSound[0] = false;
	}
}

void CUI_WatchingMode::Create_ReturnButton()
{
	m_rcButton[1].left = LONG((g_iWinSizeX - 100.f) - (101.f * 1.5f / 2));
	m_rcButton[1].top = LONG(100.f - (34.f / 2));
	m_rcButton[1].right = LONG((g_iWinSizeX - 100.f) + (101.f * 1.5f / 2));
	m_rcButton[1].bottom = LONG(100.f + (34.f / 2));
}

void CUI_WatchingMode::Is_Picking_ReturnButton(POINT pt)
{
	if (PtInRect(&m_rcButton[1], pt))
	{
		if (!m_bSound[1])
		{
			m_bSound[1] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (m_bWathingMode)
			m_iTextureIndex = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			m_bWathingMode = false;
		}
	}
	else
		m_bSound[1] = false;
}

void CUI_WatchingMode::Create_NextButton()
{
	m_rcButton[2].left = LONG((m_fX + m_fSizeX * 0.35f) - (23.f / 2));
	m_rcButton[2].top = LONG(m_fY - (30.f / 2));
	m_rcButton[2].right = LONG((m_fX + m_fSizeX * 0.35f) + (23.f / 2));
	m_rcButton[2].bottom = LONG(m_fY + (30.f / 2));
}

void CUI_WatchingMode::Is_Picking_NextButton(POINT pt)
{
	if (PtInRect(&m_rcButton[2], pt))
	{
		if (!m_bSound[2])
		{
			m_bSound[2] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		m_iChangeButton[1] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			if (nullptr == CServerSessionManager::GetInstance()->Get_Player()->Get_Party())
				return;

			if (2 < (m_iPartyIndex + 1))
			{
				for (size_t i = 0; i < 3; i++)
				{
					if (0 != m_iPartyIds[i])
					{
						m_iPartyIndex = i;
						break;
					}
				}
			}
			else
			{
				m_iPartyIndex++;
				if (0 == m_iPartyIds[m_iPartyIndex])
				{
					for (size_t i = 0; i < 3; i++)
					{
						if (0 != m_iPartyIds[i])
						{
							m_iPartyIndex = i;
							break;
						}
					}
				}
			}
			Find_IndexPartyMember();
			Print_TextBox();
		}
	}
	else
	{
		m_iChangeButton[1] = 0;
		m_bSound[2] = false;
	}
}

HRESULT CUI_WatchingMode::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_CCTVWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTexture_Button
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_Button"),
		TEXT("Com_Texture_Button"), (CComponent**)&m_pTexture_Button)))
		return E_FAIL;
	//m_pTexture_ChangeButton
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_Change_PlayerButton"),
		TEXT("Com_Texture_ChangeButton"), (CComponent**)&m_pTexture_ChangeButton)))
		return E_FAIL;
	//m_pTexture_Button
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Button"), (CComponent**)&m_pTransform_Button)))
		return E_FAIL;
	//m_pTransform_NextButton
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_NextButton"), (CComponent**)&m_pTransform_NextButton)))
		return E_FAIL;
	//m_pTransform_BackButton
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BackButton"), (CComponent**)&m_pTransform_BackButton)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WatchingMode::Ready_TextBox()
{
	if (nullptr == m_pTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("CCTV_Button");
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
	
	if (nullptr == m_pReturnTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("CCTV_QuitButton");
		m_strReturnWndTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(101.f * 1.5f, 34.f);
		m_pReturnTextBox = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pReturnTextBox)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pReturnTextBox->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pReturnTextBox->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		m_pReturnTextBox->Set_Render(false);

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CUI_WatchingMode::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_WatchingMode::Bind_ShaderResources_Button()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Button->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
	return S_OK;
}

HRESULT CUI_WatchingMode::Bind_ShaderResources_NextButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_NextButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ChangeButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iChangeButton[1]);
	return S_OK;
}

HRESULT CUI_WatchingMode::Bind_ShaderResources_BackButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BackButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ChangeButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iChangeButton[0]);
	return S_OK;
}

void CUI_WatchingMode::Find_IndexPartyMember()
{
	for (auto& iter : CServerSessionManager::GetInstance()->Get_Player()->Get_Party()->Get_PartyMembers())
	{
		if (m_iPartyIds[m_iPartyIndex] == iter)
		{
			m_pWatchingPlayer = static_cast<CPlayer*>(CGameInstance::GetInstance()->Find_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_PLAYER, iter));
			if (nullptr == m_pWatchingPlayer)
				return;
		}
	}
	if (nullptr != m_pWatchingPlayer)
	{
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_FadeInOut(1.f, false);
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Target(m_pWatchingPlayer);
	}
}

CUI_WatchingMode* CUI_WatchingMode::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WatchingMode* pInstance = new CUI_WatchingMode(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WatchingMode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WatchingMode::Clone(void* pArg)
{
	CUI_WatchingMode* pInstance = new CUI_WatchingMode(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WatchingMode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WatchingMode::Free()
{
	__super::Free();

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
	
	if (nullptr != m_pReturnTextBox)
		m_pReturnTextBox->Set_Dead(true);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexture_Button);
	Safe_Release(m_pTexture_ChangeButton);
	Safe_Release(m_pTransform_Button);
	Safe_Release(m_pTransform_NextButton);
	Safe_Release(m_pTransform_BackButton);
}
