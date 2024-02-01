#pragma once
#include "Skill.h"

BEGIN(Client)

class CSkill_TeleportDoor : public CSkill
{
public:
	struct RisingSunDesc
	{
		CGameObject* pSkillOwner = nullptr;
		_int	iObjectID = -1;
		Matrix* matWorld = nullptr;
	};

private:
	CSkill_TeleportDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_TeleportDoor(const CSkill_TeleportDoor& rhs);
	virtual ~CSkill_TeleportDoor() = default;

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
	virtual void	Buff_Player(_uint iPlayerID);

	virtual HRESULT					Ready_Coliders()override;



private:
	virtual HRESULT			Ready_Components();

	void	Send_Buff();
	void	Effect_End();
private:
	_bool m_bEnter = false;

	_bool m_bSend = false;
	_bool m_bEnd = false;
	_float m_fDelay = 0.0f;
	_float m_fDeadTime = 10.f;

	vector<CEffect*> m_Effects;
public:
	static	CSkill_TeleportDoor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg);
	virtual void Free();

};

END