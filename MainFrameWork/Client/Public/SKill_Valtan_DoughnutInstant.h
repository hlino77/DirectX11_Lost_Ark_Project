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

class CSkill_Valtan_DoughnutInstant : public CSkill
{
private:
	CSkill_Valtan_DoughnutInstant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Valtan_DoughnutInstant(const CSkill_Valtan_DoughnutInstant& rhs);
	virtual ~CSkill_Valtan_DoughnutInstant() = default;

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

	void	Set_DoughnutRadii(_float OutsideRadius, _float InsideRadius);

private:
	virtual HRESULT			Ready_Components();



public:
	static	CSkill_Valtan_DoughnutInstant* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg);
	virtual void Free();

};

END