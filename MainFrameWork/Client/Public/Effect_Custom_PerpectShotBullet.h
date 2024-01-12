#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)

class CEffect_Custom_PerpectShotBullet : public CEffect
{
	using Super = CEffect;
public:
	typedef struct PerpectShotBulletDesc
	{
		Vec3 vPos;
		Vec3 vLook;
	};

private:
	CEffect_Custom_PerpectShotBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_PerpectShotBullet(const CEffect_Custom_PerpectShotBullet& rhs);
	virtual ~CEffect_Custom_PerpectShotBullet() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;


private:
	virtual HRESULT Ready_Components() override;

private:
	CEffect* m_pBulletEffect = nullptr;
public:
	static CEffect_Custom_PerpectShotBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END