#pragma once
#include "Component.h"
#include "Lock.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine final : public CComponent
{
private:
	CStateMachine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CStateMachine(const CStateMachine& rhs);
	virtual ~CStateMachine() = default;


	USE_LOCK
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Tick_State(_float fTimeDelta);

public:
	HRESULT Add_State(const wstring& strStateTag, class CState* pState);
	HRESULT Change_State(const wstring& strStateTag);
		
	const wstring& Get_CurrState();
private:
	class CState* m_pCurrState = nullptr;
	map<wstring, class CState*> m_States;

private:
	class CState* Find_State(const wstring & strStateTag);


public:
	static CStateMachine* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(CGameObject * pObject, void* pArg) override;
	virtual void Free();
};

END

