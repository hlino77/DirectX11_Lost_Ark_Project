#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)
END


BEGIN(Client)

class CEffect_Custom_BreakObject : public CEffect
{
	using Super = CEffect;

public:
	struct BreakObjectDesc
	{
		Vec3 vPos = {};
		Vec3 vTargetPos = {};
		class CGameObject* pOwner = nullptr;
		wstring strModelName = {};
	};


private:
	CEffect_Custom_BreakObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_BreakObject(const CEffect_Custom_BreakObject& rhs);
	virtual ~CEffect_Custom_BreakObject() = default;


public:

	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;


private:

	virtual HRESULT Ready_Components() override;
	HRESULT Ready_Components(void* pArg);
	void	Init_Projectile_Motion();
	void	Update_BreakPos(_float fTimedelta);
	void	Effect_BreakObject();


private:
	_bool				m_bBreakStart = false;
	class CGameObject*	m_pOwner = nullptr;
	wstring				m_strModelName = {};

	Vec3	 m_vStartPos = {};
	_float	 m_fVelocityX, m_fVelocityY, m_fVelocityZ = 0.f;
	_float	 m_fGravity = {};
	_float	 m_fEndTime = {};



public:
	static CEffect_Custom_BreakObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END
