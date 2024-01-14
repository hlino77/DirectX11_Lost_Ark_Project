#pragma once
#include "Npc.h"

BEGIN(Client)

class CFunction_Npc : public CNpc
{
protected:
	CFunction_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFunction_Npc(const CFunction_Npc& rhs);
	virtual ~CFunction_Npc() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Ready_Parts();

	HRESULT					Render_Model();
	HRESULT					Render_Model_Shadow();
	HRESULT					Render_PartModel();
	HRESULT					Render_PartModel_Shadow();

	void					Set_Colliders(_float fTimeDelta);

private:

public:
	static CFunction_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

