#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

class CVIBuffer;

END

BEGIN(Client)

class CEffect_Decal : public CEffect
{
	using Super = CEffect;
private:
	CEffect_Decal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Decal(const CEffect_Decal& rhs);
	virtual ~CEffect_Decal() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;
	
private:
	virtual HRESULT Ready_Components() override;

private:
	CVIBuffer*	m_pBuffer		= nullptr;

public:
	static CEffect_Decal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END