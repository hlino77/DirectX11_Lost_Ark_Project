#pragma once
#include "Effect.h"
#include "Delegate.h"

BEGIN(Engine)

class CVIBuffer_Trail;

END

BEGIN(Client)

class CEffect_Trail : public CEffect
{
	using Super = CEffect;
private:
	CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Trail(const CEffect_Trail& rhs);
	virtual ~CEffect_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* desc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

public:
	void Reset();
	void	TrailEnd(_float fRemainTime);

private:
	CVIBuffer_Trail* m_pBuffer = nullptr;

	_int	m_iTrailVtxCount = 4;
private:
	void Run_Sequence(const _float& fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;

public:
	static CEffect_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END