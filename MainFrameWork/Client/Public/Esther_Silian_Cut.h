#pragma once
#include "Esther_Cut.h"

BEGIN(Client)

class CEsther_Silian_Cut final : public CEsther_Cut
{
private:
	CEsther_Silian_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Silian_Cut(const CEsther_Silian_Cut& rhs);
	virtual ~CEsther_Silian_Cut() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(CPlayer* pPlayer, void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();

public:
	virtual void			Reset() override;
	virtual void			Ready() override;
	virtual void			Check_Finish() override;

private:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_ModelPart();
	virtual HRESULT			Ready_Parts();

private:
	void					Act1(_float fTimeDelta);
	void					Act2(_float fTimeDelta);

private:
	_uint					m_iAnimIndex;


public:
	static CEsther_Silian_Cut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer* pPlayer, void* pArg);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

