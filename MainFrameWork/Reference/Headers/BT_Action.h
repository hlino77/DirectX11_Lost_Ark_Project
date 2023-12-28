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
		_float fAnimSpeed = 1.2f;
		_float fRootDist = 1.5f;
	}ANIMATION_DESC;

	typedef struct tagAction_Desc : public NODE_DESC
	{
		wstring					strActionName;
		vector<ANIMATION_DESC>	vecAnimations;
		_int				  iLoopAnimationIndex = -1;
		_float                fMaxLoopTime = 0.f;
	}ACTION_DESC;

protected:
	CBT_Action();
	CBT_Action(const CBT_Action& rhs);
	virtual ~CBT_Action() = default;

public:
	virtual HRESULT				Initialize(void* pArg);

	virtual void		OnStart(_int iAnimIndex = 0);

	CBT_Node::BT_RETURN OnUpdate(const _float& fTimeDelta);

	virtual void		Reset()		override { m_eReturn = RETURN_END; }

protected:
	_int						m_iCurrAnimation = 0;
	vector<ANIMATION_DESC>	m_vecAnimDesc;
	_float						m_fFrame = 0.f;
	_bool						m_bSoundOn = false;
	wstring						m_strActionName;
	_int						m_iLoopAnimationIndex = -1;
	_float						m_fMaxLoopTime = 0.f;
	_float						m_fLoopTime = 0.f;
	_uint						m_iMaxAnimation = 0;
public:
	virtual void Free() override;
};


END