#pragma once
#include "Player.h"

BEGIN(Client)

class CLineCircle;

class CPlayer_Destroyer : public CPlayer
{
public:

private:
	CPlayer_Destroyer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Destroyer(const CPlayer_Destroyer& rhs);
	virtual ~CPlayer_Destroyer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT	Render_ShadowDepth();

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	virtual	void	OnCollisionEnter_NoneControl(const _uint iColLayer, class CCollider* pOther);
	virtual	void	OnCollisionExit_NoneControl(const _uint iColLayer, class CCollider* pOther);
	virtual void	Set_Skill(CGameObject* pGameObject) override;

public:
	void			Send_PlayerInfo();
	void			Set_Colliders(_float fTimeDelta);

public:
	class CController_WDR* Get_WDR_Controller() { return m_pController; }

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Parts() override;
	HRESULT			Ready_State();
	HRESULT			Ready_Skill();
	HRESULT			Ready_Coliders();
	HRESULT			Ready_PhysxBoneBranch();

private:
	HRESULT			Ready_SkillUI();


private:
	_float					m_fSendInfoTime = 0.0f;

	class CController_WDR* m_pController = { nullptr };


public:
	static CPlayer_Destroyer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END