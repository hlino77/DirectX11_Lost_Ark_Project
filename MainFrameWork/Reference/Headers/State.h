#pragma once
#include "Base.h"



BEGIN(Engine)

class ENGINE_DLL CState abstract : CBase
{
public:
	CState(const wstring& strStateName);
	CState(const CState& rhs, class CStateMachine* pMachine);
	virtual ~CState() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void Enter_State() PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

public:
	void Set_StateMachine(class CStateMachine* pStateMachine) { m_pStateMachine = pStateMachine; }

public:
	const wstring& Get_StateName() { return m_strStateName; }
	void Set_StateName(const wstring& strName) { m_strStateName = strName; }

protected:
	wstring m_strStateName = L"";

private:
	class CStateMachine* m_pStateMachine = nullptr;

public:
	virtual void Free() override;
};

END

