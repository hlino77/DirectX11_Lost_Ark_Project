#pragma once
#include "BT_Node.h"
BEGIN(Engine)

class ENGINE_DLL CBT_Decorator :    public CBT_Node
{
public:
	enum class DecoratorType { IF, WHILE, REPEAT, TYPE_END };
	typedef struct tag_Decorator_Desc : public NODE_DESC
	{
		DecoratorType eDecoratorType;
		_int iRepeatCount = 0;
	}DECORATOR_DESC;
protected:
	CBT_Decorator();
	CBT_Decorator(const CBT_Decorator& rhs);
	virtual ~CBT_Decorator() = default;

public:
	virtual HRESULT				Initialize(void* pArg);

	virtual void		OnStart()	override;
	virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
	virtual void		OnEnd()		override;

	virtual void		Reset()		override { m_vecChildren[0]->Reset(); }

	virtual	void Check_Condition() {};

protected:
	DecoratorType		m_eDecoratorType = DecoratorType::TYPE_END;
	_bool m_bCondition = false;
	_int m_iRepeatCount = 0;
	_int m_iCounter = 0;
public:
	static	CBT_Decorator* Create(void* pArg);
	virtual void Free() override;
};


END