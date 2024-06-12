#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_ServerSelect final : public CLevel
{
private:
	CLevel_ServerSelect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_ServerSelect() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;
	virtual HRESULT Exit();

private:
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_UI();

public:
	static class CLevel_ServerSelect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


private:
	_bool m_bConnect = false;
};

END