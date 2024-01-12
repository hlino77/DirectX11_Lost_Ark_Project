#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;

END

BEGIN(Client)

class CCamera_Free;
class CLevel_Tool final : public CLevel
{
	using Super = CLevel;
private:
	CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tool() = default;

public:
	virtual HRESULT Initialize()				override;
	virtual HRESULT Tick(const _float& fTimeDelta)		override;
	virtual HRESULT LateTick(const _float& fTimeDelta)	override;
	virtual HRESULT Exit();
	virtual HRESULT Render_Debug()				override;

	HRESULT Ready_SoundTrack();

public:
	CGameObject*	GetPivotObject();
	void			SetPivotObject(CGameObject* pPartObject);

private:
	HRESULT Ready_Layer_SkyBox();
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_UI();
	HRESULT	Ready_Camera();
	HRESULT	Ready_Tools();
	HRESULT Load_MapData(LEVELID eLevel, const wstring& szFullPath);

private:
	class CEffectTool*		m_pEffectTool = nullptr;
	class CEffect_PcModel*	m_pEffectModel = nullptr;

	CGameObject*			m_pPivotObject = nullptr;

	_bool					m_IsImGUIReady = false;
	Vec4	clear_color = Vec4(0.f);

	CGameInstance*			m_pGameInstance = nullptr;
	CCamera_Free*			m_pCamera = nullptr;

private:


public:
	static class CLevel_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};

END