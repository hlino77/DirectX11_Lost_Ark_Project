#pragma once
#include "BT_Node.h"
BEGIN(Engine)

class ENGINE_DLL CBT_Action : public CBT_Node
{
public:

	typedef struct tagAnimation_Desc
	{
		wstring strAnimName;
		_int iAnimIndex;
		_float fChangeTime;
		_uint iStartFrame; 
		_uint iChangeFrame;
	}ANIMATION_DESC;

	typedef struct tagAction_Desc : public NODE_DESC
	{
		vector<ANIMATION_DESC>	vecAnimations;
	}ACTION_DESC;

protected:
	CBT_Action();
	CBT_Action(const CBT_Action& rhs);
	virtual ~CBT_Action() = default;

public:
	virtual HRESULT				Initialize(void* pArg);

	virtual void		OnStart(_int iAnimIndex = 0);

	virtual void		Reset()		override { m_eReturn = RETURN_END; }

protected:
	_int						m_iCurrAnimation = 0;
	vector<pair<ANIMATION_DESC, _uint>>	m_vecAnimIndexFrame;
	_float						m_fFrame = 0.f;
	_bool						m_bSoundOn = false;

public:
	virtual void Free() override;
};


END