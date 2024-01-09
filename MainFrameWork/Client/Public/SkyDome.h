#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSkyDome : public CGameObject
{
	using Super = CGameObject;
private:
	CSkyDome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkyDome(const CSkyDome& rhs);
	virtual ~CSkyDome() = default;

public:
	virtual HRESULT Initialize_Prototype()		override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components();

public:
	virtual void Free() override;
	virtual CGameObject* Clone(void* pArg) override;
	static CSkyDome* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
};

END