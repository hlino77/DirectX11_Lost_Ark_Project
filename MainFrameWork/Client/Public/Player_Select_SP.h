#pragma once
#include "Player_Select.h"

BEGIN(Client)

class CLineCircle;

class CPlayer_Select_SP : public CPlayer_Select
{
public:

private:
	CPlayer_Select_SP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Select_SP(const CPlayer_Select_SP& rhs);
	virtual ~CPlayer_Select_SP() = default;

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
	_int			m_IsHair = { -1 };

public:
	static CPlayer_Select_SP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END