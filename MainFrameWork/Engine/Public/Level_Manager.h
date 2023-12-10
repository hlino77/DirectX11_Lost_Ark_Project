#pragma once

#include "Base.h"
#include "Lock.h"
/* Open_Level*/
/* 현재 게임내에서 활용하고 있는 레벨객체의 주소를 보관한다. */
/* 레벨 교체시에 기존 레벨을 파괴한다. + 기존 레벨용으로 만들어놓은 자원들을 삭제한다. */

/* 보관하고 있는 레벨의 틱을 반복적으로 호출해준다. */


BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;


	USE_LOCK
public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	HRESULT Render_Debug();


	void Set_Loading(_bool bLoading) { m_bLoading = bLoading; }
	void Set_LoadingNext(_uint iNextLevel) { m_iLoadingNext = iNextLevel; }

	class CLevel* Get_CurrLevel() { return m_pCurrentLevel; }

	_uint Get_CurrLevelIndex();
public:
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

private:
	_uint						m_iCurrentLevelIndex = { 0 };
	_uint						m_iNextLevelIndex = { 0 };

	class CLevel*				m_pCurrentLevel = { nullptr };
	class CLevel*				m_pReserveLevel = { nullptr };


	_bool						m_bLoading = false;
	_uint						m_iLoadingNext = 0;
public:
	virtual void Free() override;
};

END