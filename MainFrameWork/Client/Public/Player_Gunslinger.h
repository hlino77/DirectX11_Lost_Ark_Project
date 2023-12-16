#pragma once
#include "Player.h"

BEGIN(Client)

class CLineCircle;

class CPlayer_Gunslinger : public CPlayer
{
public:
	enum GUNSLINGER_SKILL { SKILLEND };

private:
	CPlayer_Gunslinger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Gunslinger(const CPlayer_Gunslinger& rhs);
	virtual ~CPlayer_Gunslinger() = default;

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

	void			Set_Weapon_RenderState(_uint iIndex, _bool Is_Shot2 = false);
	void			Set_Several_Weapon_RenderState(CPartObject::PARTS ePart, _bool Is_Render);

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Parts() override;
	HRESULT			Ready_State();
	HRESULT			Ready_Coliders();


private:
	HRESULT			Ready_SkillUI();


private:
	_float			m_fSendInfoTime = 0.0f;

	class CPlayer_Controller_GN* m_pController = { nullptr };


public:
	static CPlayer_Gunslinger* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END