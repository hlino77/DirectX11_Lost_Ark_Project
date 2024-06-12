#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CEsther_Bahuntur_Cut;
class CEsther_Silian_Cut;
class CEsther_Way_Cut;

class CLevel_Studio final : public CLevel
{
public:
	enum ESTHER { SA, WY, BT, _END };

private:
	CLevel_Studio(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Studio() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Debug();
	virtual HRESULT Exit();

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Esther();

	HRESULT Start_Esther_Cut();

private:
	CEsther_Bahuntur_Cut*	m_pBahuntur = { nullptr };
	CEsther_Silian_Cut*		m_pSilian = { nullptr };
	CEsther_Way_Cut*		m_pWay = { nullptr };

	_bool					m_bCutStart[ESTHER::_END];

public:
	static class CLevel_Studio* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END