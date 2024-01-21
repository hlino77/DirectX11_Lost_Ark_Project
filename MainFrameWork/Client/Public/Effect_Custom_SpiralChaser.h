#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)

class CEffect_Custom_SpiralChaser : public CEffect
{
	using Super = CEffect;
public:
	struct SpiralChaserDesc
	{
		Vec3 vPos;
		Vec3 vTargetPos;
		class CPlayer_Gunslinger* pPlayer;
		_uint iSkillKey;
	};

private:
	CEffect_Custom_SpiralChaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_SpiralChaser(const CEffect_Custom_SpiralChaser& rhs);
	virtual ~CEffect_Custom_SpiralChaser() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

private:
	void	Effect_Bullet();

private:
	virtual HRESULT Ready_Components() override;

private:
	_float m_fBulletAcc = 0.0f;
	_float m_fBulletTime = 0.0f;

	Vec3 m_vOffsetLook;

	_bool m_bAttackStart = false;

	class CPlayer_Gunslinger* m_pPlayer = nullptr;
	_uint m_iSkillKey = 0;
public:
	static CEffect_Custom_SpiralChaser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END