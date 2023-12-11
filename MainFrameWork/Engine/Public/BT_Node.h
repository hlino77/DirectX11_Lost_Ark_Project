#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CBT_Node :public CBase
{
public:
	enum BT_RETURN { BT_FAIL = -1, BT_RUNNING, BT_SUCCESS, RETURN_END };
	enum class BT_NODETYPE { COMPOSITE, DECORATOR, ACTION, TYPE_END };
public:
	typedef struct tag_Node_Desc
	{
		class CGameObject* pGameObject;
		class CBehaviorTree* pBehaviorTree;
	}NODE_DESC;

protected:
	CBT_Node();
	CBT_Node(const CBT_Node& rhs);
	virtual ~CBT_Node() = default;

public:
	virtual HRESULT			Initialize(void* pArg);

	virtual	BT_RETURN		Tick(const _float& fTimeDelta);
	virtual void			OnStart() {}
	virtual BT_RETURN		OnUpdate(const _float& fTimeDelta)	PURE;
	virtual void			OnEnd() {}

	virtual void			Reset()								PURE;

	virtual BT_NODETYPE		NodeType() {	return m_eNodeType;}
	HRESULT					AddChild(CBT_Node* pChild);

protected:
	class CGameObject*				m_pGameObject = nullptr;
	class CBehaviorTree*				m_pBehaviorTree = nullptr;

	vector<CBT_Node*>			m_vecChildren;
	BT_RETURN					m_eReturn = BT_RETURN::RETURN_END;
	BT_NODETYPE					m_eNodeType= BT_NODETYPE::TYPE_END;
public:
	virtual void Free() override;
};

END