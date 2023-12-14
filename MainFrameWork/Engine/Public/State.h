#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState abstract : public CBase
{
public:
	CState(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController);
	virtual ~CState() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void	Enter_State() PURE;
	virtual void	Tick_State(_float fTimeDelta) PURE;
	virtual void	Exit_State() PURE;

public:
	void Set_StateMachine(class CStateMachine* pStateMachine) { m_pStateMachine = pStateMachine; }

public:
	const wstring& Get_StateName() { return m_strStateName; }
	void Set_StateName(const wstring& strName) { m_strStateName = strName; }

protected:
	wstring						m_strStateName = L"";
	wstring						m_strChangeName = L"";

	class CStateMachine*		m_pStateMachine = { nullptr };
	class CPlayer_Controller*	m_pController = { nullptr };

public:
	virtual void Free() override;
};

END

