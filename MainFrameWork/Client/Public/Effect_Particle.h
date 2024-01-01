#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

class CVIBuffer_Particle;

END

BEGIN(Client)

class CEffect_Particle : public CEffect
{
	using Super = CEffect;
private:
	CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Particle(const CEffect_Particle& rhs);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* desc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;
	
private:
	struct tagFX_Billboard
	{
		_int	iBillboard = true;
		Vec3	padding;
	} m_Billboard;
	
	struct tagFX_Particle
	{
		_float	fGameTime = 0.f;
		Vec3	vEmitPosition = Vec3(0.f, 0.f, 0.f);
		Vec3	vEmitDirection = Vec3(0.f, 0.f, 0.f);
		_float	fTimeStep = 0.f;
	} m_Particle;

	string m_strParticlePassName = "Smoke";

private:
	CVIBuffer_Particle* m_pBuffer = nullptr;
	ID3D11ShaderResourceView* m_pRandomTextureSRV = nullptr;

private:
	virtual HRESULT Ready_Components() override;

public:
	static CEffect_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END