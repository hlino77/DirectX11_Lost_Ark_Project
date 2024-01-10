#include "stdafx.h"
#include "UI_Lobby_NickName.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Player_Select.h"
#include "UI_Lobby_NickNameChange.h"
#include "UI_Manager.h"

CUI_Lobby_NickName::CUI_Lobby_NickName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Lobby_NickName::CUI_Lobby_NickName(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Lobby_NickName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Lobby_NickName::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_uint* strIndex = static_cast<_uint*>(pArg);
		m_strUITag = TEXT("Lobby_NickName");
		m_strUITag += to_wstring(*strIndex + 1);
		Initialize_Index(*strIndex);
	}
	else
		m_strUITag = TEXT("Lobby_NickName");

	Initialize_Textbox();
	Random_NickName();
	return S_OK;
}

HRESULT CUI_Lobby_NickName::Initialize_Textbox()
{
	m_strFont = L"던파연마된칼날";
	Ready_TextBox();
	m_pNickNameWnd->Set_Active(false);
	return S_OK;
}

void CUI_Lobby_NickName::Tick(_float fTimeDelta)
{
	InitialIze_EmblemPos();
	__super::Tick(fTimeDelta);
	Moving_Rect();
	Picking_UI();
	Update_NickNameFrame();
}

void CUI_Lobby_NickName::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	LateUpdate_NickNameFrame();
	Get_NickName_Changer();
}

HRESULT CUI_Lobby_NickName::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom_NickNameShine->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Emblem()))
		return E_FAIL;
	m_pTextureCom_Emblem->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iClassIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	Print_NickName();

	return S_OK;
}

HRESULT CUI_Lobby_NickName::Ready_Components()
{
	__super::Ready_Components();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Emblem"), (CComponent**)&m_pTransformCom_Emblem)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_ShineEffect"),
		TEXT("Com_Texture_Shine"), (CComponent**)&m_pTextureCom_NickNameShine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emblem"),
		TEXT("Com_Texture_Emblem"), (CComponent**)&m_pTextureCom_Emblem)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CUI_Lobby_NickName::Bind_ShaderResources()
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


	return S_OK;
}

HRESULT CUI_Lobby_NickName::Bind_ShaderResources_Emblem()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Emblem->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	return S_OK;
}

void CUI_Lobby_NickName::Update_NickNameFrame()
{
	//UI Clicked
	if ((m_bPick) && (BUTTON_STATE::BUTTON_NORMAL == m_iButtonState))
		m_iButtonState = BUTTON_STATE::BUTTON_PICKING;
	if ((BUTTON_STATE::BUTTON_PICKING == m_iButtonState) && ((KEY_HOLD(KEY::LBTN)) || (KEY_TAP(KEY::LBTN))))
	{
		m_iButtonState = BUTTON_STATE::BUTTON_PICKED;
		static_cast<CPlayer_Select*>(m_pLobbyCharacter)->Clicked();
	}

	if ((!m_bPick) && (BUTTON_STATE::BUTTON_PICKING == m_iButtonState))
		m_iButtonState = BUTTON_STATE::BUTTON_NORMAL;
	
}

void CUI_Lobby_NickName::Get_NickName_Changer()
{
	CUI* pUI = CUI_Manager::GetInstance()->Find_UIPart(LEVEL_LOBBY, TEXT("UI_Lobby"), TEXT("Lobby_NameChanger"));

	if ((nullptr != pUI)&&(BUTTON_STATE::BUTTON_PICKED == m_iButtonState))
		static_cast<CUI_Lobby_NickNameChange*>(pUI)->Set_NickNmaeFrame(this);
}

void CUI_Lobby_NickName::LateUpdate_NickNameFrame()
{
	//Player Clicked
	if (static_cast<CPlayer_Select*>(m_pLobbyCharacter)->Is_Selected())
		m_iButtonState = BUTTON_STATE::BUTTON_PICKED;
	else
		m_iButtonState = BUTTON_STATE::BUTTON_NORMAL;

	if (BUTTON_STATE::BUTTON_PICKED == m_iButtonState)
		m_iTextureIndex = 1;
	else
		m_iTextureIndex = 0;
}

void CUI_Lobby_NickName::InitialIze_EmblemPos()
{
	if (m_bInitializeEmblem)
		return;

	Vec3 vPos = {};
	vPos.x = (m_fX - (m_fSizeX * 0.40f) - g_iWinSizeX * 0.5f);
	vPos.y = (-(m_fY - 10.f)) + (g_iWinSizeY * 0.5f);
	vPos.z = 0.2f;

	m_pTransformCom_Emblem->Set_Scale(Vec3(32.f, 32.f, 0.2f));
	m_pTransformCom_Emblem->Set_State(CTransform::STATE_POSITION, vPos);

	m_bInitializeEmblem = true;
}

void CUI_Lobby_NickName::Update_Equipment_Level()
{
}

void CUI_Lobby_NickName::Initialize_Index(_uint iIndex)
{
	m_fSizeX = 205.f;
	m_fSizeY = 53.f;

	switch (iIndex)
	{
	case 0:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObejct((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Gunslinger"));
		m_iClassIndex = (_uint)CHR_CLASS::GUNSLINGER;
		m_fX = 330.f;
		m_fY = 800.f;
		break;

	case 1:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObejct((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("WDR"));
		m_iClassIndex = (_uint)CHR_CLASS::DESTROYER;
		m_fX = 630.f;
		m_fY = 800.f;
		break;

	case 2:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObejct((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("MG"));
		m_iClassIndex = (_uint)CHR_CLASS::BARD;
		m_fX = 930.f;
		m_fY = 800.f;
		break;

	case 3:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObejct((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("WR"));
		m_iClassIndex = (_uint)CHR_CLASS::SLAYER;
		m_fX = 1230.f;
		m_fY = 800.f;
		break;
	}

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	m_pTransformCom_Emblem->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom_Emblem->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
}

void CUI_Lobby_NickName::Print_NickName()
{
	if (nullptr == m_pNickNameWnd)
		return;

	m_pNickNameWnd->Set_Active(true);
	m_pNickNameWnd->Clear_Text();
	m_pNickNameWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3((m_fX+10.f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
	m_pNickNameWnd->Get_TransformCom()->Set_Scale(Vec3(102.5f, 26.5f, 0.f));
	m_pNickNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"빛의계승자", m_strNickName);
	Vec2 vOrigin = vMeasure * 0.5f;

	m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex) + TEXT("-1"), m_strFont, m_strNickName, Vec2(102.5f - 2.f, 26.5f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 0.5f));
	m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex) + TEXT("-2"), m_strFont, m_strNickName, Vec2(102.5f+2.f, 26.5f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 0.5f));
	m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex),	m_strFont,	m_strNickName,	Vec2(102.5f, 26.5f),	Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}            

HRESULT CUI_Lobby_NickName::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pNickNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("NickNameWnd") + to_wstring(m_iClassIndex);
		tTextDesc.vSize = Vec2(205.f, 53.0f);
		m_pNickNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
	
		if (nullptr == m_pNickNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pNickNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pNickNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_Lobby_NickName::Start_NickNameText()
{
	m_strNickName.clear();
	m_pNickNameWnd->Set_Active(true);
}

void CUI_Lobby_NickName::End_NickNameText()
{
	m_strNickName.clear();
	m_pNickNameWnd->Set_Active(false);
}

void CUI_Lobby_NickName::Set_NickName(const wstring& strStageName)
{
	m_strNickName.clear();
	m_strNickName = strStageName;
}

void CUI_Lobby_NickName::Random_NickName()
{
	_int iRandomIndex =  CGameInstance::GetInstance()->Random_Int(0,10);

	switch (iRandomIndex)	
	{
	case 0 :
		m_strNickName = TEXT("쥬신147기");
		break;
	case 1:
		m_strNickName = TEXT("범부여");
		break;
	case 2:
		m_strNickName = TEXT("내뇌가Nullptr");
		break;
	case 3:
		m_strNickName = TEXT("이거왜안돼");
		break;
	case 4:
		m_strNickName = TEXT("왜터지지");
		break;
	case 5:
		m_strNickName = TEXT("도와줘Gpt");
		break;
	case 6:
		m_strNickName = TEXT("할게너무많은데");
		break;
	case 7:
		m_strNickName = TEXT("로아가뜯기네");
		break;
	case 8:
		m_strNickName = TEXT("책임자는팀장");
		break;
	case 9:
		m_strNickName = TEXT("내잘못아님");
		break;
	case 10:
		m_strNickName = TEXT("살려주세요");
		break;
	}

	if((_uint)CHR_CLASS::DESTROYER ==  m_iClassIndex)
		m_strNickName = TEXT("디붕이");
}

CUI_Lobby_NickName* CUI_Lobby_NickName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Lobby_NickName* pInstance = new CUI_Lobby_NickName(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Lobby_NickName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Lobby_NickName::Clone(void* pArg)
{
	CUI_Lobby_NickName* pInstance = new CUI_Lobby_NickName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Lobby_NickName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Lobby_NickName::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pNickNameWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_NickNameShine);
	Safe_Release(m_pTextureCom_Emblem);
	Safe_Release(m_pTextureCom_Equipment);
	Safe_Release(m_pTransformCom_Emblem);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
