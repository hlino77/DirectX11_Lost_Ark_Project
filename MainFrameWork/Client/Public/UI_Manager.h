#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CUI_Manager final:
    public CBase
{
    DECLARE_SINGLETON(CUI_Manager)

private:
    CUI_Manager();
    virtual ~CUI_Manager() = default;

public:
    HRESULT Tick(_float fTimeDelta);
    HRESULT LateTick(_float fTimeDelta);
    void    Clear(LEVELID iLevelIndex);
    void    Render();

public:
    HRESULT Add_UI(LEVELID eLevelIndex, class CUI* pUI);
    HRESULT Loading_UI(LEVELID eLevelIndex, const _uint& iLayerType, const wstring& UITag);
    class CUI* Find_UI(LEVELID eLevelIndex, const _uint& iLayerType, const wstring& UITag);

private:

private:
    list<class CUI*>*    m_pUIList;
    _uint   m_iCurrentLevel = { 0 };
    _uint   m_iNextLevel = { 0 };

public:
    virtual void Free() override;
};

END