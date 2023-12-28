#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Lobby() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(const _float& fTimeDelta) override;
	virtual HRESULT LateTick(const _float& fTimeDelta) override;
	virtual HRESULT Exit();

private:
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_CameraFree(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_UI();

public:
	static class CLevel_Lobby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


private:
	_bool m_bConnect = false;
};

END