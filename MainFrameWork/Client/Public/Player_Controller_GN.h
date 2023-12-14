#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CPlayer_Controller_GN final : public CPlayer_Controller
{
private:
	CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs);
	virtual ~CPlayer_Controller_GN() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;


private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack() override;
	virtual void	Skill() override;
	virtual void	Hit() override;

public:
	static CPlayer_Controller_GN* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END