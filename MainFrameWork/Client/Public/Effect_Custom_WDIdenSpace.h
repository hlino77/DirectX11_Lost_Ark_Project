#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)
class CPlayer_Destroyer;

class CEffect_Custom_WDIdenSpace : public CEffect
{
	using Super = CEffect;
public:
	typedef struct CustomEffectDesc
	{
		CPlayer_Destroyer* pOwner = nullptr;
	};

	enum class WDIDENSTATE { START, IDLE, IDENEND, STATEEND };

private:
	CEffect_Custom_WDIdenSpace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_WDIdenSpace(const CEffect_Custom_WDIdenSpace& rhs);
	virtual ~CEffect_Custom_WDIdenSpace() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

private:
	virtual HRESULT Ready_Components() override;
	
	void	Tick_Start(_float fTimeDelta);
	void	Tick_Idle(_float fTimeDelta);
	void	Tick_End(_float fTimeDelta);
private:
	CPlayer_Destroyer* m_pOwner = nullptr;

	WDIDENSTATE m_eState;

	Vec3 m_vTargetScale;
	_float m_fUVSpeed;
public:
	static CEffect_Custom_WDIdenSpace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END