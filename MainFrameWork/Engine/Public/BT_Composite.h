#pragma once
#include "BT_Node.h"
BEGIN(Engine)

class ENGINE_DLL CBT_Composite :  public CBT_Node
{
public:
	enum class CompositeType { SELECTOR, SEQUENCE, TYPE_END };
	typedef struct tag_Composite_Desc: public NODE_DESC
	{
		CompositeType	eCompositeType;
	}COMPOSITE_DESC;
protected:
	CBT_Composite();
	CBT_Composite(const CBT_Composite& rhs);
	virtual ~CBT_Composite() = default;

public:
	virtual HRESULT				Initialize(void* pArg);

	virtual void		OnStart()	override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd()		override;

	virtual void		Reset()		override;

protected:
	CompositeType		m_eCompositeType = CompositeType::TYPE_END;
	vector<CBT_Node*>::iterator	m_RunningChild;


public:
	static	CBT_Composite* Create(void* pArg);
	virtual void Free() override;
};


END