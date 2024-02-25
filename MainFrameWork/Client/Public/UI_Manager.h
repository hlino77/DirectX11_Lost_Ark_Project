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
    void    Picked_SkillIcon(const wstring& strObjectTag, CTexture* pTexture_Icon, _uint iBindingKey);
    void    Reset_ItemIcon();
    _bool    Is_PickedIcon();
    void    Set_PickedIcon(_bool bPickedIcon);
    const wstring Get_PickedTag() { return m_strPickedTag; }
    void    Set_PickedTag(wstring strPickedTag) { m_strPickedTag = strPickedTag; }
    void    Set_EntranceParty_Player(_bool  bEntrance, class CPlayer* pPlayer);
    _uint   Get_BindingKey() { return m_iSkillKey; }

public://Party
    void    Set_Active_EntranceParty(class CPlayer* pPartyLeader, class CPlayer* pPlayer);
    void    Set_Add_PartyInfo(vector<_uint> vecPartys);
    class CPlayer* Get_PartyLeader();

public://Cut_Scene
    void    Set_RenderUIs(_bool bRender, LEVELID iLevelIndex);
    void    Set_UIs_Active(_bool bRender, LEVELID iLevelIndex);
    void    Set_RenderNickName(_bool bRender, LEVELID iLevelIndex);

public:
    void    Clear_ChaosGate();
    void    Clear_Valtan();
    void    Player_DeadScene(_bool bAvtice, LEVELID iLevelIndex);

public:
    void    Set_Player_Control(_bool bControl);
    void    Set_Player_Mouse(_bool bControl);

public://Option
    _float  Get_ChannelVolume(_uint iChannelID);
    void Set_LevelCustomOption();

public://ChaosDungeon
    void    Set_ChaosGauge(_float fGauge);
    _float    Get_Ratio();

private:
    list<class CUI*>*    m_pUIList;
    _uint   m_iCurrentLevel = { 0 };
    _uint   m_iNextLevel = { 0 };
    POINT   m_pt = {};
    wstring m_strPickedTag = TEXT("");
    _int   m_iSkillKey = { -1 };

    CUI* m_pCurrentBossHpUI = { nullptr };
    CUI* m_pMouseCursor = { nullptr };
    CPlayer* m_pPartyLeader = { nullptr };
        
    USE_LOCK
public:
    virtual void Free() override;
};

END