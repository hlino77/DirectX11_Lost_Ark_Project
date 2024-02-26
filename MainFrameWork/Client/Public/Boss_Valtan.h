#pragma once
#include "Boss.h"

BEGIN(Engine)
class CPartObject;
END

BEGIN(Client)

class CBoss_Valtan :
    public CBoss
{
public:
	enum class PARTS
	{
		BODY,
		PART1,
		PART2,
		GHOST,
		POSTDEATH,
		PARTS_END
	};
	CBoss_Valtan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Valtan(const CBoss_Valtan& rhs);
	virtual ~CBoss_Valtan() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy) override;

public:
	_int						Get_Shield() { return m_iShield; }
	void						Set_Shield(_int iShield) { m_iShield = iShield; }

	_bool	Get_RenderPostValtan() { return m_bRenderPostValtan; }

	void	Load_GrabMatrix(Matrix& matWorld);

private:
	virtual HRESULT		Ready_Components();
	virtual HRESULT		Ready_Coliders();
	virtual	HRESULT		Ready_BehaviourTree();

	virtual	void		Update_Dissolve(_float fTimeDelta) override;

private:
	CModel* m_pModelPartCom[(_uint)PARTS::PARTS_END] = {};

private:
	Vec3							m_vSummonPositions[3] = {};
	_float							m_fSummonTime = 0.f;
	_bool							m_bRenderPostValtan = true;
	_int		m_iShield = 0;

public:
	static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END