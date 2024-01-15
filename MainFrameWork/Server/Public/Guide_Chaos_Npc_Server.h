#pragma once
#include "Npc_Server.h"

BEGIN(Server)

class CGuide_Chaos_Npc_Server final : public CNpc_Server
{
private:
	CGuide_Chaos_Npc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuide_Chaos_Npc_Server(const CGuide_Chaos_Npc_Server& rhs);
	virtual ~CGuide_Chaos_Npc_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Debug();


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

public:
	virtual HRESULT	Actice_Npc_Function(int32 iLevel, int32 iPlayerID) override;

private:
	virtual HRESULT		Ready_Components();

private:
	_uint				m_iGuide_Level = { 0 };
	_uint				m_iCheck_Cnt = { 0 };


public:
	static CGuide_Chaos_Npc_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END