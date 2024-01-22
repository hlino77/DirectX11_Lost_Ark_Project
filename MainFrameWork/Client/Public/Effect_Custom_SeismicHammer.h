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
	struct EffectDesc
	{
		Vec3 vPos;
		Vec3 vLook;
	};

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


private:
	virtual HRESULT Ready_Components() override;

public:
	static CEffect_Custom_SeismicHammer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END