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
		Matrix PivotMatrix = {};
		Matrix WorldMatrix = {};
		Vec3   vPos = {};

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


	void	Spread_Random_Dir(float fTimeDelta);
	void	Exponential_Lerp(float fTimeDelta);


private:

	class CGameObject*	m_pOwner  = nullptr;  // StaticModel 
	class CGameObject*  m_pTarget = nullptr;  // Valtan
	wstring				m_strModelName = {};  // Object Name

	Matrix	 m_WorldMatrix = {};
	Vec3	 m_vStartPos = {};

	_float	 m_fVelocityX, m_fVelocityY, m_fVelocityZ = 0.f;
	_float	 m_fGravity = {};
	_float	 m_fEndTime = {};
	_uint	 m_AttackStyle = {};

	_bool	m_bBreakStart = false;


	// 	RandomDir
	_bool  m_bSetDir = false;
	Vec3   m_RandomMoveDirection = {};

	// RimLight
	_float	m_fRimLightTime = {};
	_bool   m_bRimLight = false;

public:
	static CEffect_Custom_BreakObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END
