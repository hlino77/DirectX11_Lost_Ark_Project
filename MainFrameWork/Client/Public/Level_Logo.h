#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
	enum class LOGOSTATE {LOGO, ENTERGAME, TYPEEND};

private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;
	virtual HRESULT Exit();

	HRESULT Ready_SoundTrack();

	void Tick_LOGO(_float fTimeDelta);
	void Tick_ENTERGAME(_float fTimeDelta);
private:
	HRESULT Ready_Layer_BackGround();

	HRESULT Ready_Layer_UI();

public:
	static class CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


private:
	_bool m_bConnect = false;

	LOGOSTATE m_eState;
};

END