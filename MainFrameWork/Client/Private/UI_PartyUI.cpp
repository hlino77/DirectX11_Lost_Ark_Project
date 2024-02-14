#include "stdafx.h"
#include "UI_PartyUI.h"
#include "Player.h"
#include "TextBox.h"
#include "GameInstance.h"
#include "UI_PartyHPWnd.h"
#include "UI_PartyEntrance.h"
#include "ServerSessionManager.h"
#include "UI_Manager.h"
#include "Party.h"

CUI_PartyUI::CUI_PartyUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}


CUI_PartyUI::CUI_PartyUI(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PartyUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_PartyUI::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_PartyWnd");

	m_fSizeX = 235.f;
	m_fSizeY = 29.f;
	m_fX = 130.f;
	m_fY = 300.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	if (FAILED(UI_Set()))
		return E_FAIL;

	if (FAILED(Initialize_TextBox(nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PartyUI::Initialize_TextBox(CPlayer* pOwner)
{
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	Ready_NameTextBox(pOwner);
	m_pTextBox->Set_Active(true);
	Print_Text();
	return S_OK;
}

HRESULT CUI_PartyUI::Initialize_Get_Players()
{
	return S_OK;
}

void CUI_PartyUI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_PartInfo();
}

void CUI_PartyUI::LateTick(_float fTimeDelta)
{
	if (nullptr != CServerSessionManager::GetInstance()->Get_Player()->Get_Party())
		__super::LateTick(fTimeDelta);
}

HRESULT CUI_PartyUI::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pTextBox->Render();

	return S_OK;
}

void CUI_PartyUI::Update_PartInfo()
{
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != pPlayer)
	{
		if ((nullptr != pPlayer->Get_Party()) && (0 < pPlayer->Get_Party()->Get_PartyMembers().size()))
			Add_PartyInfo(CServerSessionManager::GetInstance()->Get_Player()->Get_Party()->Get_PartyMembers());
	}
}

void CUI_PartyUI::Print_Text()
{
	if (nullptr == m_pTextBox)
		return;
	m_pTextBox->Clear_Text();
	m_pTextBox->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f);
	m_pTextBox->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));// Vec2(205.f, 53.0f);
	m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("137±â ÆÄÆ¼"));
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pTextBox->Set_Text(m_strWndTag, m_strFont, TEXT("137±â ÆÄÆ¼"), Vec2(m_fSizeX * 0.05f , m_fSizeY * 0.2f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void CUI_PartyUI::Set_Active_EntranceParty(CPlayer* pPartyLeader, CPlayer* pPlayer)
{
	CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Party_Alarm.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
	m_pUI_PartyEntrance->Set_Active_EntranceParty(pPartyLeader, pPlayer);
}

void CUI_PartyUI::Add_PartyInfo(vector<_uint> vecPlayersId)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = { nullptr };
	auto& iterUI = m_vecUIParts.begin();
	++iterUI;
	CUI_PartyHPWnd::PARTYHP_DESC tDesc;
	_uint    iPartyIndex = 0;
	for (auto& iter : vecPlayersId)
	{
		pPlayer = static_cast<CPlayer*>(pInstance->
			Find_GameObject(CServerSessionManager::GetInstance()->Get_Player()->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iter));
		if (nullptr != pPlayer)
		{
			tDesc.pPlayer = pPlayer;
			if (CServerSessionManager::GetInstance()->Get_Player()->Get_CurrLevel() == tDesc.pPlayer->Get_CurrLevel())
			{
				tDesc.iPartyIndex = iPartyIndex;
				tDesc.strObjectTag = tDesc.pPlayer->Get_ObjectTag();
				static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PlayerDesc(tDesc);
				if (0 == iPartyIndex)
					static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PartyLeader(true);
			}
			else
			{
				tDesc.pPlayer = pPlayer;
				tDesc.iPartyIndex = iPartyIndex;
				tDesc.strObjectTag = TEXT("");
				static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PlayerDesc(tDesc);
				if (0 == iPartyIndex)
					static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PartyLeader(true);
			}
		}
		else if (iter == (CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID()))
		{
			tDesc.pPlayer = CServerSessionManager::GetInstance()->Get_Player();
			tDesc.iPartyIndex = iPartyIndex;
			tDesc.strObjectTag = tDesc.pPlayer->Get_ObjectTag();
			static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PlayerDesc(tDesc);
			if (0 == iPartyIndex)
				static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PartyLeader(true);
		}
		else
		{
			tDesc.pPlayer = nullptr;
			tDesc.iPartyIndex = iPartyIndex;
			tDesc.strObjectTag = TEXT("");
			static_cast<CUI_PartyHPWnd*>((*iterUI))->Set_PlayerDesc(tDesc);
		}
		iPartyIndex++;
		iterUI++;
	}
}

HRESULT CUI_PartyUI::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/*m_pTextureCom*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_PartyWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PartyUI::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_PartyUI::UI_Set()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);
	m_vecUIParts.push_back(this);
	CUI* pUI = nullptr;
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex()
		, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_PartyEntrance")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_pUI_PartyEntrance = static_cast<CUI_PartyEntrance*>(pUI);
	}

	pUI = nullptr;
	for (size_t i = 0; i < 4; i++)
	{
		pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex()
			, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_PartyHPWnd"), &i));
		if (nullptr == pUI)
			return E_FAIL;
		else
			m_vecUIParts.push_back(pUI);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CUI_PartyUI::Ready_NameTextBox(CPlayer* pOwner)
{
	if (nullptr == m_pTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("s_PartyNameWnd");
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

CUI_PartyUI* CUI_PartyUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PartyUI* pInstance = new CUI_PartyUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_PartyUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PartyUI::Clone(void* pArg)
{
	CUI_PartyUI* pInstance = new CUI_PartyUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_PartyUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PartyUI::Free()
{
	__super::Free();

	if(nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
}
