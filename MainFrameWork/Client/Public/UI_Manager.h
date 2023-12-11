#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

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
    void    Clear(_uint iLevelIndex);
    void    Render();

public:
    HRESULT Draw_UI(LEVELID eLevelId);
    HRESULT Ready_NextLevel_UI(_uint iLevelId);
private:

private:
    map<const LEVELID, vector<CGameObject*>>    m_mapUIs;//레벨에 따라 돌릴예정
    _uint   m_iCurrentLevel = { 0 };
    _uint   m_iNextLevel = { 0 };

public:
    virtual void Free() override;
};

END