#pragma once

#include "Client_Defines.h"
#include "Item.h"

BEGIN(Client)

class CIT_MG_Body_Legend final : public CItem
{
private:
	CIT_MG_Body_Legend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIT_MG_Body_Legend(const CIT_MG_Body_Legend& rhs);
	virtual ~CIT_MG_Body_Legend() = default;

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
	static CIT_MG_Body_Legend* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

