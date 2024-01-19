#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)

class CEffect_Custom_SeismicHammer : public CEffect
{
	using Super = CEffect;
public:
	typedef struct EffectDesc
	{
		Vec3 vPos;
		Vec3 vLook;
	};

private:
	enum class SeismicHammerState { START, IDLE, DISAPPEAR, STATEEND };

private:
	CEffect_Custom_SeismicHammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_SeismicHammer(const CEffect_Custom_SeismicHammer& rhs);
	virtual ~CEffect_Custom_SeismicHammer() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;


	void	Tick_Start(_float fTimeDelta);
	void	Tick_Idle(_float fTimeDelta);
	void	Tick_Disappear(_float fTimeDelta);


private:
	virtual HRESULT Ready_Components() override;

private:
	CTexture* m_pDissolveTexture;

	SeismicHammerState m_eState;
public:
	static CEffect_Custom_SeismicHammer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END