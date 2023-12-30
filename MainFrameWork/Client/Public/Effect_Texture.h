#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

class CVIBuffer_Point;

END

BEGIN(Client)

class CEffect_Texture : public CEffect
{
	using Super = CEffect;
private:
	CEffect_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Texture(const CEffect_Texture& rhs);
	virtual ~CEffect_Texture() = default;

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
		Vec3	vPadding;
	} m_Billboard;

private:
	CVIBuffer_Point* m_pBuffer = nullptr;

private:
	virtual HRESULT Ready_Components() override;

public:
	static CEffect_Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END