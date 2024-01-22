#pragma once
#include "Skill.h"
#include "Client_Defines.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Client)

class CSkill_Golem_Charge_Punch :	public CSkill
{
private:
	CSkill_Golem_Charge_Punch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Golem_Charge_Punch(const CSkill_Golem_Charge_Punch& rhs);
	virtual ~CSkill_Golem_Charge_Punch() = default;

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



private:
	virtual HRESULT			Ready_Components();



public:
	static	CSkill_Golem_Charge_Punch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg);
	virtual void Free();

};

END