#pragma once
#include "BT_Action.h"
BEGIN(Engine)

class ENGINE_DLL CBT_MultiAnimationAction : public CBT_Action
{

protected:
	CBT_MultiAnimationAction();
	CBT_MultiAnimationAction(const CBT_MultiAnimationAction& rhs);
	virtual ~CBT_MultiAnimationAction() = default;

public:
	virtual HRESULT				Initialize(void* pArg);

	virtual void		OnStart(_int iAnimIndex = 0);
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		Reset()		override { m_eReturn = RETURN_END; }

protected:
	_float                m_fLoopTime = 0.f;
	_uint	m_iMaxAnimation = 0;
public:
	virtual void Free() override;
};


END