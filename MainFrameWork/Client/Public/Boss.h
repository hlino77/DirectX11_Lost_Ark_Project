#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END


BEGIN(Client)
class CBoss : public CMonster
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		_uint	iLevel;
		Vec3	vPos;
	}MODELDESC;

public:
	enum AttackColliders
	{
		SKILL1 = 9,
		SKILL2,
		SKILL3,
		ATTACK_END
	};


protected:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;
	virtual HRESULT Render_ShadowDepth();

public:
	CShader*				Get_ShaderCom() { return m_pShaderCom; }



	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }


	void					Set_AttackMoveSpeed(_float fSpeed) { m_fAttackMoveSpeed = fSpeed; }
	_float					Get_AttackMoveSpeed() { return m_fAttackMoveSpeed; }


	_bool					Is_Control() { return m_bControl; }
public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);


	virtual	void OnCollisionEnter(const _uint iColLayer, CCollider* pOther);

	void					Follow_ServerPos(_float fDistance, _float fLerpSpeed);


	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void Add_Colider(_int iColIndex, CSphereCollider* pCollider);

	_bool						Is_CounterSkill() { return m_IsCounterSkill; }
	void						Set_CounterSkill(_bool IsCounterSkill) { m_IsCounterSkill = IsCounterSkill; }

	virtual void			Set_Die();

	Vec3						Get_SpawnPosition() { return m_vSpawnPosition; }
	void						Set_SpawnPosition(Vec3 vPosition) { m_vSpawnPosition = vPosition; }
	void						Move_to_SpawnPosition();

	_int						Get_Armor() { return m_iArmor; }
	void						Set_Armor(_int iArmor) { m_iArmor = iArmor; }

	_uint						Get_Phase() { return m_iPhase; }
	void						Set_Phase(_uint iPhase) { m_iPhase = iPhase; }
protected:
	virtual HRESULT Ready_Components();
	HRESULT Ready_HP_UI(_uint iTextureIndex);


protected:
	void					CullingObject();
	virtual	HRESULT			Ready_Coliders() { return S_OK; };
	virtual HRESULT			Ready_BehaviourTree();

protected:
	_bool m_IsCounterSkill = false;
	Vec3  m_vSpawnPosition = {};
	_int m_iArmor = 0;
	_uint m_iPhase = 1;
	_int m_iGroggyGauge = 0;
	_uint m_iMaxGroggyGauge = 0;

public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

