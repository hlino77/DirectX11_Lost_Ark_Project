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
		_bool  bIsLoop = false;
		_float fMaxLoopTime = 0.f;
		_bool  IsEndInstant = false;
		_bool	IsRootRot = false;
	}ANIMATION_DESC;

	typedef struct tagAction_Desc : public NODE_DESC
	{
		wstring					strActionName;
		vector<ANIMATION_DESC>	vecAnimations;
	}ACTION_DESC;

protected:
	CBT_Action();
	CBT_Action(const CBT_Action& rhs);
	virtual ~CBT_Action() = default;

public:
	virtual HRESULT				Initialize(void* pArg);

	virtual void		OnStart(_int iAnimIndex = 0);

	virtual CBT_Node::BT_RETURN OnUpdate(const _float& fTimeDelta);

	virtual void On_FirstAnimStart();

	virtual void On_LastAnimEnd();

	void Add_Sound(_int iAnimIndex, _int iSoundOnIndex, const wstring& szChannelGroup, const wstring& strSoundKey, _int iAnimFrame = 0, _float fVolume = 0.5f, FMOD_CHANNEL** pChannel = nullptr);
	void Add_Sound_Channel(_int iAnimIndex, _int iSoundOnIndex, const wstring& szChannelGroup, const wstring& strSoundKey, _int iAnimFrame = 0, _float fVolume = 0.5f);

	void Add_Sound(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume = 0.5f, FMOD_CHANNEL** pChannel = nullptr);
	void Add_Sound_Channel(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume = 0.5f);


	virtual    void		OnEnd();


	virtual void		Reset()		override { m_eReturn = RETURN_END; }
	
	void Reset_Sound();
	ANIMATION_DESC Get_CurrentAnimDesc() { return m_vecAnimDesc[m_iCurrAnimation]; }

protected:

	_int						m_iMaxSound = 20;
	_int						m_iCurrAnimation = 0;
	vector<ANIMATION_DESC>		m_vecAnimDesc;
	_float						m_fFrame = 0.f;
	_bool*						m_bSoundOn;
	wstring						m_strActionName;
	_float						m_fLoopTime = 0.f;
	_uint						m_iMaxAnimation = 0;
	_bool						m_bStart = false;
	_bool						m_bEnd = false;
public:
	virtual void Free() override;
};


END