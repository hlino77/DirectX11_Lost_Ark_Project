#pragma once
#include "Server_Defines.h"
#include "Level.h"

BEGIN(Server)

class CLevel_Loading_Server : public CLevel
{
private:
	CLevel_Loading_Server();
	virtual ~CLevel_Loading_Server() = default;

public:
	virtual HRESULT Initialize(LEVELID eNextLevel);
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;

private:
	/* 로딩 다음레벨. */
	LEVELID			m_eNextLevel = { LEVEL_END };
	class CLoader_Server* m_pLoader = { nullptr };

public:
	static class CLevel_Loading_Server* Create(LEVELID eNextLevel);
	virtual void Free() override;
};

END