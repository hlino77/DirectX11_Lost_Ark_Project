#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

class CVIBuffer_Trail;

END

BEGIN(Client)

class CEffect_Custom_SwordTrail : public CEffect
{
	using Super = CEffect;
private:
	CEffect_Custom_SwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_SwordTrail(const CEffect_Custom_SwordTrail& rhs);
	virtual ~CEffect_Custom_SwordTrail() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

public:
	virtual void Reset(CEffect_Manager::EFFECTPIVOTDESC& tEffectDesc) override;

private:
	CVIBuffer_Trail* m_pBuffer = nullptr;

	_int	m_iTrailVtxCount = 4;

private:
	void Run_Sequence(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;

public:
	static CEffect_Custom_SwordTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END