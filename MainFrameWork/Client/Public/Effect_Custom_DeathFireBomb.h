#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)

class CEffect_Custom_DeathFireBomb : public CEffect
{
	using Super = CEffect;
public:
	typedef struct DeathFireBombDesc
	{
		Vec3 vPos;
		Vec3 vTargetPos;
		class CPlayer_Gunslinger* pPlayer;
		_uint iSkillKey;
		_bool bFirst = false;
	};

private:
	CEffect_Custom_DeathFireBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_DeathFireBomb(const CEffect_Custom_DeathFireBomb& rhs);
	virtual ~CEffect_Custom_DeathFireBomb() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;


private:
	virtual HRESULT Ready_Components() override;

	void	Init_Projectile_Motion();

	void	Update_GrenadePos(_float fTimedelta);
	void	Effect_Explosion();

private:
	_bool m_bAttackStart = false;

	class CPlayer_Gunslinger* m_pPlayer = nullptr;
	_uint m_iSkillKey = 0;



	Vec3 m_vStartPos;
	_float m_fVelocityX, m_fVelocityY, m_fVelocityZ;
	_float m_fGravity;
	_float m_fEndTime;

	_bool m_bFirst = false;
public:
	static CEffect_Custom_DeathFireBomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END