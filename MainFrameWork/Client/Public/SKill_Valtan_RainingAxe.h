#pragma once
#include "Skill.h"

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Client)
class CEffect;

class CSKill_Valtan_RainingAxe : public CSkill
{
public:
	struct RainAxeDesc
	{
		CSkill::ModelDesc tSkillDesc;
		Vec3 vTargetPos;
	};

private:
	CSKill_Valtan_RainingAxe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSKill_Valtan_RainingAxe(const CSKill_Valtan_RainingAxe& rhs);
	virtual ~CSKill_Valtan_RainingAxe() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	virtual void	Explosion() {};


	virtual HRESULT					Ready_Coliders()override;

	void	Load_EffectPivotMatrix(Matrix& matWorld);
private:
	virtual HRESULT			Ready_Components();

private:
	Matrix m_OriginWorldMatrix = XMMatrixIdentity();

	_bool m_bAttack = false;

	_float m_fWaiting = 0.0f;

	vector<CEffect*> m_Trails;
	Vec3 m_vOffsetTargetPos;
	Vec3 m_vStartPos;

	_float m_fMaxRadian;
public:
	static	CSKill_Valtan_RainingAxe* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg);
	virtual void Free();

};

END