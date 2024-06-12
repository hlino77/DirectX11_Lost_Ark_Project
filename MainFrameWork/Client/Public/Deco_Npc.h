#pragma once
#include "Npc.h"

BEGIN(Client)

class CDeco_Npc : public CNpc
{
protected:
	CDeco_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDeco_Npc(const CDeco_Npc& rhs);
	virtual ~CDeco_Npc() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

private:
	virtual HRESULT			Ready_Components() override;
	virtual HRESULT			Ready_Parts();

	HRESULT					Render_Model();
	HRESULT					Render_Model_Shadow();
	HRESULT					Render_PartModel();
	HRESULT					Render_PartModel_Shadow();

	void					Set_Colliders(_float fTimeDelta);

	_bool					Check_True_All_Sequence_Npc();
	_bool					Check_False_All_Sequence_Npc();

private:
	void	Move(_float fTimeDelta);
	void	Move_Patrol(_float fTimeDelta);
	void	Talk(_float fTimeDelta);

private:
	_float			m_fTalkStartAcc = { 0.f };
	_uint			m_iCurrTalk = { 0 };
	_int			m_iMoveCnt = { 0 };
	_bool			m_bReach = { false };

	_bool			m_bTalkStart = false;

public:
	static CDeco_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

