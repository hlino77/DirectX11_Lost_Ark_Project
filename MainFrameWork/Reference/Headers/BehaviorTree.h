#pragma once
#include "Component.h"
#include "Hasher.h"

BEGIN(Engine)

class ENGINE_DLL CBehaviorTree final:    public CComponent
{
protected:
    CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBehaviorTree(const CComponent& rhs);
    ~CBehaviorTree() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual void	DebugRender()						override;

	HRESULT Add_Action(wstring strActionName, class CBT_Action* pAction);

	HRESULT	 Tick_Action(wstring strActionName, _float fTimeDelta);

	HRESULT Change_Action(wstring strActionName);

	CBT_Action* Get_CurrentAction();

	void			SetRoot(class CBT_Composite * pComposite) { m_pRootNode = pComposite; }
	HRESULT			Init_PreviousAction(wstring strAction, _int iAnimIndex = 0);
	void			ResetAll();

protected:
	class CBT_Composite* m_pRootNode = nullptr;
	using Actions = unordered_map<wstring, class CBT_Node*, djb2Hasher>;
	Actions m_hashActions;
	Actions::iterator		m_PreviousAction;

public:
	static CBehaviorTree* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(CGameObject * pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END