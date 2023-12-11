#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Tool final : public CLevel
{

private:
	CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;
	virtual HRESULT Exit();

	HRESULT Ready_SoundTrack();
private:
	HRESULT Ready_Layer_BackGround();

	HRESULT Ready_Layer_UI();

public:
	static class CLevel_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};

END