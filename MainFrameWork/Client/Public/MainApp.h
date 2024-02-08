#pragma once

#include "Client_Defines.h"

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Client)
class CCamera_Free;


class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	/* 내 게임의 시작. */
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = { nullptr };

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CRenderer*				m_pRenderer_Com = { nullptr };

private:
	HRESULT Open_Level(LEVELID eLevelID);

private:
	HRESULT Initialize_Client();
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Prototype_Font();
	HRESULT Ready_Prototype_Effect();
	HRESULT Ready_Camera_Free();
	HRESULT Loading_OptionUI();

	HRESULT Ready_SoundManager();
	HRESULT Ready_Event();
	void	Active_Camera_Free();



	_bool m_bMouse = false;
	
	CCamera_Free* m_pCamera = nullptr;

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END

