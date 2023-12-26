#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CGameInstance;
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
private:
	HRESULT Ready_Layer_BackGround();

	HRESULT Ready_Layer_UI();

	HRESULT	Ready_Camera();

	HRESULT	Ready_Tools();

private:
	class CEffectTool* m_pEffectTool = nullptr;

	_bool	m_IsImGUIReady = false;
	Vec4	clear_color = Vec4(0.f);

	CGameInstance* m_pGameInstance = nullptr;
	CCamera_Free* m_pCamera = nullptr;

public:
	static class CLevel_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};

END