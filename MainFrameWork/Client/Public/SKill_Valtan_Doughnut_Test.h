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

class CSKill_Valtan_Doughnut_Test : public CSkill
{
private:
	CSKill_Valtan_Doughnut_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSKill_Valtan_Doughnut_Test(const CSKill_Valtan_Doughnut_Test& rhs);
	virtual ~CSKill_Valtan_Doughnut_Test() = default;

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


private:
	_float m_fBlinkTime = 0.f;

public:
	static	CSKill_Valtan_Doughnut_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg);
	virtual void Free();

};

END