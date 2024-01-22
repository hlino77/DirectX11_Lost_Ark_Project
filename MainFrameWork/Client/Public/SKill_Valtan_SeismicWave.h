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

class CSkill_Valtan_SeismicWave : public CSkill
{
private:
	CSkill_Valtan_SeismicWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Valtan_SeismicWave(const CSkill_Valtan_SeismicWave& rhs);
	virtual ~CSkill_Valtan_SeismicWave() = default;

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
	static	CSkill_Valtan_SeismicWave* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg);
	virtual void Free();

};

END