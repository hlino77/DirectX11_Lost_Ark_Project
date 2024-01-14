#pragma once
#include "Function_Npc.h"

BEGIN(Client)

class CGuide_Npc final : public CFunction_Npc
{
protected:
	CGuide_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuide_Npc(const CGuide_Npc& rhs);
	virtual ~CGuide_Npc() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			LateTick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;
	virtual HRESULT			Render_ShadowDepth() override;
	virtual HRESULT			Render_Debug() override;

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Ready_Parts() override;

	virtual HRESULT			Render_Model() override;
	virtual HRESULT			Render_Model_Shadow() override;
	virtual HRESULT			Render_PartModel() override;
	virtual HRESULT			Render_PartModel_Shadow() override;

private:

public:
	static CGuide_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

