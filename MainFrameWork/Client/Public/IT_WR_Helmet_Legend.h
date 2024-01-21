#pragma once

#include "Client_Defines.h"
#include "Item.h"

BEGIN(Client)

class CIT_WR_Helmet_Legend final : public CItem
{
private:
	CIT_WR_Helmet_Legend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIT_WR_Helmet_Legend(const CIT_WR_Helmet_Legend& rhs);
	virtual ~CIT_WR_Helmet_Legend() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Tick(_float fTimeDelta);
	virtual void				LateTick(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	virtual HRESULT				Use_Item(CPlayer* pOwner) override;
	virtual HRESULT				Disuse_Item(CPlayer* pOwner, _bool bUseDefault) override;

private:
	virtual HRESULT				Ready_Components();

public:
	static CIT_WR_Helmet_Legend* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

