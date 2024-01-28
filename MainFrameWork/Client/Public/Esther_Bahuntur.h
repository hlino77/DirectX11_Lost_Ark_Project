#pragma once
#include "Esther.h"

BEGIN(Client)

class CEsther_Bahuntur final : public CEsther
{
private:
	CEsther_Bahuntur(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Bahuntur(const CEsther_Bahuntur& rhs);
	virtual ~CEsther_Bahuntur() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Tick(_float fTimeDelta);
	virtual void					LateTick(_float fTimeDelta);
	virtual HRESULT					Render();

public:
	void							Leader_Active_Esther();

private:
	virtual HRESULT					Ready_Components();

public:
	static CEsther_Bahuntur* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

