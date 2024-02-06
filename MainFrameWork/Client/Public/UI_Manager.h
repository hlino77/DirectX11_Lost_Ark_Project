#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UI.h"
#include "Lock.h"

BEGIN(Client)

class CUI_Manager final:
    public CBase
{
    DECLARE_SINGLETON(CUI_Manager)

private:
    CUI_Manager();
    virtual ~CUI_Manager() = default;

public:
    HRESULT Reserve_Manager();
    HRESULT Tick(_float fTimeDelta);
    HRESULT LateTick(_float fTimeDelta);
    void    Clear(LEVELID iLevelIndex);
    void    Render();

public:
    HRESULT Add_UI(LEVELID eLevelIndex, class CUI* pUI);
    HRESULT ObjectManager_to_UIManager(LEVELID eLevelIndex);
    HRESULT Loading_UI(_float fSizeX);
    HRESULT Set_MaxFiles(_uint iFilesNum);
    HRESULT    Add_CurrFile();
    HRESULT    Add_CurrFiles(_uint iFilesNum);
    list<class CUI*>* Get_UIList(LEVELID eLevelIndex);
    class CUI* Find_UI(LEVELID eLevelIndex, const wstring& UITag);
    vector<CUI*> Find_UIParts(LEVELID eLevelIndex, const wstring& UITag);
    class CUI* Find_UIPart(LEVELID eLevelIndex, const wstring& UITag, const wstring& PartTag);
    HRESULT Delete_UI(LEVELID eLevelIndex, const wstring& UITag);
    HRESULT Delete_UIPart(LEVELID eLevelIndex, const wstring& UITag, const wstring& PartTag);

public:
    void    Sorting_UI();
    void    Set_UIState(LEVELID iLevelIndex, CUI::UISTATE eState);
    void    Set_UIState(LEVELID iLevelIndex, const wstring& strUITag, CUI::UISTATE eState);

public://BossHp
    void    Set_CurrHPUI(CUI* pUI);
    void    SetDead_CurrHPUI(CUI* pUI);

public://Mouse
    void    Picked_ItemIcon(const wstring& strObjectTag, CTexture* pTexture_Icon , _uint iItemGrade);
    void    Reset_ItemIcon();
    _bool    Is_PickedIcon();
    void    Set_PickedIcon(_bool bPickedIcon);
    const wstring Get_PickedTag() { return m_strPickedTag; }
    void    Set_PickedTag(wstring strPickedTag) { m_strPickedTag = strPickedTag; }
    void    Set_EntranceParty_Player(_bool  bEntrance, class CPlayer* pPlayer);

public://Party
    void    Set_Active_EntranceParty(CPlayer* pPartyLeader, CPlayer* pPlayer);
    void    Set_Add_PartyInfo(vector<_uint> vecPartys);
    CPlayer* Get_PartyLeader();

public://Cut_Scene
    void    Set_RenderUIs(_bool bRender, LEVELID iLevelIndex);
    void Set_UIs_Active(_bool bRender, LEVELID iLevelIndex);

private:
    list<class CUI*>*    m_pUIList;
    _uint   m_iCurrentLevel = { 0 };
    _uint   m_iNextLevel = { 0 };
    POINT   m_pt = {};
    wstring m_strPickedTag = TEXT("");

    CUI* m_pCurrentBossHpUI = { nullptr };
    CUI* m_pMouseCursor = { nullptr };
    CPlayer* m_pPartyLeader = { nullptr };
        
    USE_LOCK
public:
    virtual void Free() override;
};

END