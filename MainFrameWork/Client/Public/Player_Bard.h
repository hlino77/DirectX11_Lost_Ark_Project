#pragma once
#include "Player.h"

BEGIN(Client)

class CLineCircle;

class CPlayer_Bard : public CPlayer
{
public:

private:
	CPlayer_Bard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Bard(const CPlayer_Bard& rhs);
	virtual ~CPlayer_Bard() = default;

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
	class CController_MG* Get_MG_Controller() { return m_pController; }

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Parts() override;
	virtual HRESULT Ready_Coliders() override;

	HRESULT			Ready_State();
	HRESULT			Ready_Skill();
	HRESULT			Ready_PhysxBoneBranch();

private:
	HRESULT			Ready_SkillUI();


private:
	_float					m_fSendInfoTime = 0.0f;

	class CController_MG* m_pController = { nullptr };


public:
	static CPlayer_Bard* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END