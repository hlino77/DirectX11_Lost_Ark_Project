#pragma once

#include "Server_Defines.h"


BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Server)

class CMainApp_Server : public CBase
{
private:
	CMainApp_Server();
	virtual ~CMainApp_Server() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();


private:
	HRESULT Initiailize_Server();
	HRESULT Ready_Prototype_Component();

private:
	CGameInstance* m_pGameInstance = { nullptr };

	CRenderer* m_pRenderer_Com = { nullptr };
private:
	HRESULT Open_Level(LEVELID eLevelID);

public:
	static CMainApp_Server* Create();
	virtual void Free() override;
};

END