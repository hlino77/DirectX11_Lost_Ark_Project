#pragma once
#include "Npc_Server.h"

BEGIN(Server)

class CGuide_Valtan_Npc_Server final : public CNpc_Server
{
private:
	CGuide_Valtan_Npc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuide_Valtan_Npc_Server(const CGuide_Valtan_Npc_Server& rhs);
	virtual ~CGuide_Valtan_Npc_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Debug();

public:
	virtual HRESULT	Actice_Npc_Function(int32 iLevel, int32 iPlayerID) override;

private:
	virtual HRESULT		Ready_Components();

private:
	_uint				m_iGuide_Level = { 0 };
	_uint				m_iCheck_Cnt = { 0 };


public:
	static CGuide_Valtan_Npc_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END