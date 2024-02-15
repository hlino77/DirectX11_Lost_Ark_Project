#pragma once
#include "Esther.h"

BEGIN(Client)

class CEsther_Way final : public CEsther
{
private:
	CEsther_Way(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Way(const CEsther_Way& rhs);
	virtual ~CEsther_Way() = default;

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
	static CEsther_Way* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

