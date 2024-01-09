#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UI.h"

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


private:
    list<class CUI*>*    m_pUIList;
    _uint   m_iCurrentLevel = { 0 };
    _uint   m_iNextLevel = { 0 };
    POINT   m_pt = {};
public:
    virtual void Free() override;
};

END