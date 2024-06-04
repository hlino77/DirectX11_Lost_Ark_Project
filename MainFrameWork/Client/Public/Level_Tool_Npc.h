#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;

END

BEGIN(Client)

class CCamera_Free;
class CNpcTool;
class CCameraTool;

class CLevel_Tool_Npc final : public CLevel
{
	using Super = CLevel;
private:
	CLevel_Tool_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tool_Npc() = default;

public:
	virtual HRESULT Initialize()				override;
	virtual HRESULT Tick(_float fTimeDelta)		override;
	virtual HRESULT LateTick(_float fTimeDelta)	override;
	virtual HRESULT Exit();
	virtual HRESULT Render_Debug()				override;

	HRESULT Ready_SoundTrack();

public:

private:
	HRESULT	Ready_Light();

	HRESULT Ready_Layer_BackGround();

	HRESULT Ready_Layer_UI();

	HRESULT	Ready_Camera();

	HRESULT	Ready_Tools();

	HRESULT Load_MapData(LEVELID eLevel, const wstring& szFullPath);

private:
	CNpcTool* m_pNpcTool = { nullptr };
	CCameraTool* m_pCameraTool = { nullptr };

	_bool	m_IsImGUIReady = false;
	Vec4	clear_color = Vec4(0.f);

	CGameInstance* m_pGameInstance = nullptr;
	CCamera_Free* m_pCamera = nullptr;

private:


public:
	static class CLevel_Tool_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};

END