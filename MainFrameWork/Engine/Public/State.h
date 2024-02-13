#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState abstract : public CBase
{
public:
	typedef struct tagSoundDesc
	{
		tagSoundDesc() {}
		tagSoundDesc(_int iFrameIndex, wstring strGroup, wstring strName, _float fVolume = 1.f, _bool bAddChannel = false)
			: iFrame(iFrameIndex), strGroup(strGroup), strName(strName), fVolume(fVolume), bAddChannel(bAddChannel) {}

		_int iFrame = -1;
		_float fVolume = 1.f;
		_bool bAddChannel = false;
		wstring strGroup;
		wstring strName;
		
	}SOUNDDESC;

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

	_uint m_iSoundCnt = 0;
	vector<SOUNDDESC> m_SoundFrames;

public:
	virtual void Free() override;
};

END

