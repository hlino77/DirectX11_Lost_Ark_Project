#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

class CVIBuffer;

END

BEGIN(Client)

class CEffect_Custom_EarthEaterDecal : public CEffect
{
	using Super = CEffect;

public:
	struct CustomEffectDesc
	{
		Vec3 vPos;
		Vec3 vLook;
	};

private:
	CEffect_Custom_EarthEaterDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_EarthEaterDecal(const CEffect_Custom_EarthEaterDecal& rhs);
	virtual ~CEffect_Custom_EarthEaterDecal() = default;

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

private:
	CTexture* m_pDecalTexture1 = nullptr;
	CTexture* m_pDecalTexture2 = nullptr;

	vector<Matrix> m_matSubDecal;

public:
	static CEffect_Custom_EarthEaterDecal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END