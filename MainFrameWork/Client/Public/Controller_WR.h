#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CController_WR final : public CPlayer_Controller
{
public:

private:
	CController_WR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController_WR(const CController_WR& rhs);
	virtual ~CController_WR() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:

public:
	virtual void	Get_AttackMessage() { Attack(); }

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack() override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Hit(CGameObject* pHitObject) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;

private:

public:
	static CController_WR* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END