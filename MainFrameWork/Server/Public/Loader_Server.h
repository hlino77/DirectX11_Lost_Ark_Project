#pragma once

#include "Server_Defines.h"

BEGIN(Server)

class CLoader_Server : public CBase
{
private:
	CLoader_Server();
	virtual~CLoader_Server() = default;

public:
	const wstring Get_LoadingText() const {
		return m_strLoading;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

public:
	/* eNextLevel에 대한 로딩을 하기위해 스레드를 생성한다. */
	HRESULT Initialize(LEVELID eNextLevel);
	_int	Loading();


private:
	LEVELID					m_eNextLevel = { LEVEL_END };
	wstring					m_strLoading = { TEXT("") };
	_bool					m_isFinished = { false };

private:
	HANDLE					m_hThread = { 0 };
	CRITICAL_SECTION		m_Critical_Section;



private:
	HRESULT Loading_For_Level_Bern();

	HRESULT Loading_Model_For_Level_Bern();
public:
	static CLoader_Server* Create(LEVELID eNextLevel);
	virtual void Free() override;
};

END