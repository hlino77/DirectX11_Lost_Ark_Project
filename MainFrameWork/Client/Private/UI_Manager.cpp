#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI.h"
#include "UI_Loading.h"
#include "UI_Boss_Hp.h"
#include "UI_Mouse_Cursor.h"
#include "Item.h"
#include "UI_Mouse_EntranceParty.h"
#include "UI_PartyUI.h"
#include "Player_Gunslinger.h"
#include "Player_Slayer.h"
#include "Player_Destroyer.h"
#include "Player_Bard.h"
#include "Player_Doaga.h"
#include "Player_Controller_GN.h"
#include "Controller_MG.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"
#include "Controller_SP.h"
#include "ServerSessionManager.h"
#include "UI_InGame_NamePlate.h"
#include "UI_ChaosDungeon_Clear.h"
#include "UI_ValtanClearWnd.h"
#include "UI_DeadScene.h";
#include "UI_Option.h"
#include "Renderer.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Reserve_Manager()
{
	m_pUIList = new  list<class CUI*>[(_uint)LEVELID::LEVEL_END];



	return S_OK;
}

HRESULT CUI_Manager::Tick(_float fTimeDelta)
{
	return  S_OK;
}

HRESULT CUI_Manager::LateTick(_float fTimeDelta)
{
	return  S_OK;
}

void CUI_Manager::Clear(LEVELID iLevelIndex)
{
	WRITE_LOCK
	m_pUIList[iLevelIndex].clear();
}

void CUI_Manager::Render()
{

}

HRESULT CUI_Manager::Add_UI(LEVELID eLevelIndex, CUI* pUI)
{
	WRITE_LOCK
	auto iter = find(m_pUIList[eLevelIndex].begin(), m_pUIList[eLevelIndex].end(), pUI);
	if (iter != m_pUIList[eLevelIndex].end())
		return E_FAIL;
	
	m_pUIList[eLevelIndex].push_back(pUI);

	return S_OK;
}

HRESULT CUI_Manager::ObjectManager_to_UIManager(LEVELID eLevelIndex)
{
	WRITE_LOCK
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_pUIList[eLevelIndex].clear();

	vector<CGameObject*>pUIList = pGameInstance->Find_GameObjects(eLevelIndex, (_uint)LAYER_TYPE::LAYER_UI);
	for (auto iter : pUIList)
	{
		auto pUI = find(m_pUIList[eLevelIndex].begin(), m_pUIList[eLevelIndex].end(), static_cast<CUI*>(iter));
		if (pUI == m_pUIList[eLevelIndex].end())
			m_pUIList[eLevelIndex].push_back(static_cast<CUI*>(iter));
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CUI_Manager::Loading_UI(_float fSizeX)
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Change_LoadingPer(fSizeX);

	return S_OK;
}

HRESULT CUI_Manager::Set_MaxFiles(_uint iFilesNum)
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Set_MaxFiles(iFilesNum);

	return S_OK;
}

HRESULT CUI_Manager::Add_CurrFile()
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Add_CurrFile();

	return S_OK;
}

HRESULT CUI_Manager::Add_CurrFiles(_uint iFilesNum)
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Add_CurrFiles(iFilesNum);

	return S_OK;
}

CUI* CUI_Manager::Find_UI(LEVELID eLevelIndex, const wstring& UITag)
{
	WRITE_LOCK
	for (auto& iter : m_pUIList[eLevelIndex])
	{
		if (UITag == iter->Get_UITag())
				return iter;
	}

	return nullptr;
}

list<class CUI*>* CUI_Manager::Get_UIList(LEVELID eLevelIndex)
{
	return &m_pUIList[eLevelIndex];
}

vector<CUI*> CUI_Manager::Find_UIParts(LEVELID eLevelIndex, const wstring& UITag)
{
	CUI* pUI = 	Find_UI(eLevelIndex, UITag);
	pUI->Get_UIParts();

	return pUI->Get_UIParts();
}

CUI* CUI_Manager::Find_UIPart(LEVELID eLevelIndex, const wstring& UITag, const wstring& PartTag)
{
	CUI* pUI = Find_UI(eLevelIndex, UITag);
	if (nullptr == pUI)
		return nullptr;

	return pUI->Get_UIPart(PartTag);
}

HRESULT CUI_Manager::Delete_UI(LEVELID eLevelIndex, const wstring& UITag)
{
	WRITE_LOCK
	CUI* pUI = Find_UI(eLevelIndex, UITag);
	if (nullptr == pUI)
		return E_FAIL;

	Safe_Release(pUI);

	return S_OK;
}

HRESULT CUI_Manager::Delete_UIPart(LEVELID eLevelIndex, const wstring& UITag, const wstring& PartTag)
{
	WRITE_LOCK
	CUI* pUI = Find_UI(eLevelIndex, UITag);
	if (nullptr != pUI)
	{
		CUI* pPart = pUI->Get_UIPart(PartTag);
		if (nullptr != pPart)
			Safe_Release(pPart);

		return S_OK;
	}
	else
		return E_FAIL;
}

void CUI_Manager::Sorting_UI()
{
}

void CUI_Manager::Set_UIState(LEVELID iLevelIndex, CUI::UISTATE eState)
{
	WRITE_LOCK
	for (auto iter : m_pUIList[iLevelIndex])
	{
		iter->Set_State(eState);
	}
}

void CUI_Manager::Set_UIState(LEVELID iLevelIndex, const wstring& strUITag, CUI::UISTATE eState)
{
	WRITE_LOCK
	for (auto iter : m_pUIList[iLevelIndex])
	{
		if (strUITag == iter->Get_UITag())
		{
			iter->Set_State(eState);
		}
	}

}

void CUI_Manager::Set_CurrHPUI(CUI* pUI)
{
	if (pUI == m_pCurrentBossHpUI)
		return;

	if (pUI != m_pCurrentBossHpUI)
	{
		if (nullptr != m_pCurrentBossHpUI)
			static_cast<CUI_Boss_Hp*>(m_pCurrentBossHpUI)->Set_Active(false);//m_pCurrentBossHpUI->Set_Render(false);
		if (nullptr != pUI)
		{
			static_cast<CUI_Boss_Hp*>(pUI)->Set_Active(true);
			m_pCurrentBossHpUI = pUI;
		}
	}
}

void CUI_Manager::SetDead_CurrHPUI(CUI* pUI)
{
	if (pUI == m_pCurrentBossHpUI)
	{
		m_pCurrentBossHpUI->Set_Dead(true);
		m_pCurrentBossHpUI = nullptr;
	}
	else
	{
		pUI->Set_Dead(true);
	}
}

void CUI_Manager::Picked_ItemIcon(const wstring& strObjectTag, CTexture* pTexture_Icon, _uint  iItemGrade)
{
	m_strPickedTag = strObjectTag;

	if ((nullptr == pTexture_Icon)||((_uint)CItem::GRADE::_END == iItemGrade))
		return;

	m_pMouseCursor = Find_UI(LEVEL_STATIC, TEXT("Mouse_Cursor"));
	if (nullptr == m_pMouseCursor)
		return;

	static_cast<CUI_Mouse_Cursor*>(m_pMouseCursor)->Picked_Icon(pTexture_Icon, iItemGrade);
}

void CUI_Manager::Reset_ItemIcon()
{
	m_strPickedTag = TEXT("");
	m_pMouseCursor = Find_UI(LEVEL_STATIC, TEXT("Mouse_Cursor"));
	if (nullptr == m_pMouseCursor)
		return;

	static_cast<CUI_Mouse_Cursor*>(m_pMouseCursor)->Reset_Icon();
}

_bool CUI_Manager::Is_PickedIcon()
{
	m_pMouseCursor = Find_UI(LEVEL_STATIC, TEXT("Mouse_Cursor"));
	if (nullptr != m_pMouseCursor)
		return static_cast<CUI_Mouse_Cursor*>(m_pMouseCursor)->Is_PickedIcon();

	return false;
}

void CUI_Manager::Set_PickedIcon(_bool bPickedIcon)
{
	m_pMouseCursor = Find_UI(LEVEL_STATIC, TEXT("Mouse_Cursor"));
	if (nullptr == m_pMouseCursor)
		return;

	static_cast<CUI_Mouse_Cursor*>(m_pMouseCursor)->Set_PickedIcon(bPickedIcon);
}

void CUI_Manager::Set_EntranceParty_Player(_bool bEntrance, class CPlayer* pPlayer)
{
	m_pMouseCursor = Find_UI(LEVEL_STATIC, TEXT("Mouse_Cursor"));
	if (nullptr == m_pMouseCursor)
		return;

	static_cast<CUI_Mouse_Cursor*>(m_pMouseCursor)->Entrance_Party(bEntrance, pPlayer);
}

void CUI_Manager::Set_Active_EntranceParty(CPlayer* pPartyLeader, CPlayer* pPlayer)
{
	CUI* pUI_PartyWnd = Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(), TEXT("UI_PartyWnd"));
	if (nullptr == pUI_PartyWnd)
		return;

	m_pPartyLeader = pPartyLeader;

	static_cast<CUI_PartyUI*>(pUI_PartyWnd)->Set_Active_EntranceParty(pPartyLeader, pPlayer);
}

void CUI_Manager::Set_Add_PartyInfo(vector<_uint> vecPartys)
{
	CUI* pUI_PartyWnd = Find_UI((LEVELID)CGameInstance::GetInstance()->Get_CurrLevelIndex(), TEXT("UI_PartyWnd"));
	if (nullptr == pUI_PartyWnd)
		return;
	
	static_cast<CUI_PartyUI*>(pUI_PartyWnd)->Add_PartyInfo(vecPartys);
}

CPlayer* CUI_Manager::Get_PartyLeader()
{
	if(nullptr == m_pPartyLeader)
		return nullptr;

	return m_pPartyLeader;
}

void CUI_Manager::Set_RenderUIs(_bool bRender, LEVELID iLevelIndex)
{
	for (auto iter : m_pUIList[iLevelIndex])
	{
		iter->Set_UIParts_Render(bRender);
	}
}

void CUI_Manager::Set_UIs_Active(_bool bRender, LEVELID iLevelIndex)
{
	for (auto iter : m_pUIList[iLevelIndex])
	{
		if((TEXT("UI_DeadScene") != iter->Get_UITag()) && (string::npos == iter->Get_UITag().find(TEXT("Clear"))))
			iter->Set_UIParts_Active(bRender);
	}
}

void CUI_Manager::Set_RenderNickName(_bool bRender, LEVELID iLevelIndex)
{
	for (auto iter : m_pUIList[(_uint)iLevelIndex])
	{
		if (TEXT("NamePlate") == iter->Get_UITag())
			static_cast<CUI_InGame_NamePlate*>(iter)->Set_TextOn(bRender);
	}
}

void CUI_Manager::Clear_ChaosGate()
{
	for (auto iter : m_pUIList[(_uint)LEVEL_CHAOS_3])
	{
		if (TEXT("ChaosDungeon_Clear") == iter->Get_UITag())
			static_cast<CUI_ChaosDungeon_Clear*>(iter)->Set_ClearUIStart(true);
	}
}

void CUI_Manager::Clear_Valtan()
{
	for (auto iter : m_pUIList[(_uint)LEVEL_VALTANMAIN])
	{
		if (TEXT("ChaosDungeon_Clear") == iter->Get_UITag())
			static_cast<CUI_ChaosDungeon_Clear*>(iter)->Set_ClearUIStart(true);
	}
}

void CUI_Manager::Player_DeadScene(_bool bAvtice, LEVELID iLevelIndex)
{
	for (auto iter : m_pUIList[(_uint)iLevelIndex])
	{
		if (TEXT("UI_DeadScene") == iter->Get_UITag())
		{	
			for (auto & iterDst : iter->Get_UIParts())
			{
				iterDst->Set_Active(bAvtice);
			}
			iter->Set_Active(bAvtice);
		}
	}

}

void CUI_Manager::Set_Player_Control(_bool bControl)
{
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return;

	if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Set_Key_Active(bControl);
	}
	else if (TEXT("WR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Set_Key_Active(bControl);
	}
	else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Set_Key_Active(bControl);
	}
	else if (TEXT("SP") == pPlayer->Get_ObjectTag())
	{
		static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Set_Key_Active(bControl);
	}

}

_float CUI_Manager::Get_ChannelVolume(_uint iChannelID)
{
	for (auto iter : m_pUIList[(_uint)LEVEL_STATIC])
	{
		if (TEXT("UI_Option") == iter->Get_UITag())
		{
			return static_cast<CUI_Option*>(iter)->Get_ChannelSound(iChannelID);
		}
	}
	return 0.f;
}

void CUI_Manager::Set_LevelCustomOption()
{
	for (auto iter : m_pUIList[(_uint)LEVEL_STATIC])
	{
		if (TEXT("UI_Option") == iter->Get_UITag())
		{
			static_cast<CUI_Option*>(iter)->Set_LevelCustomOption();
		}
	}
}

void CUI_Manager::Free()
{
	__super::Free();
	m_pPartyLeader = nullptr;
	for (size_t i = 0; i < (_uint)LEVEL_END; i++)
	{
		m_pUIList[i].clear();
	}
	Safe_Delete_Array(m_pUIList);
}
