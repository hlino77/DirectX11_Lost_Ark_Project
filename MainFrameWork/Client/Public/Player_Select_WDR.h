#pragma once
#include "Player_Select.h"

BEGIN(Client)

class CLineCircle;

class CPlayer_Select_WDR : public CPlayer_Select
{
public:

private:
	CPlayer_Select_WDR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Select_WDR(const CPlayer_Select_WDR& rhs);
	virtual ~CPlayer_Select_WDR() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT	Render_ShadowDepth();

public:
	void			Set_Colliders(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;
	HRESULT			Ready_Coliders();
	HRESULT			Ready_Part();

private:

public:
	static CPlayer_Select_WDR* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END