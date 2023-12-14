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
	virtual void	Tick(const _float & fTimeDelta)		override;
	virtual void	LateTick(const _float & fTimeDelta)	override;
	virtual void	DebugRender()						override;

	HRESULT Add_Action(wstring strActionName, class CBT_Action* pAction);

	HRESULT	 Tick_Action(wstring strActionName, const _float & fTimeDelta);

	void			SetRoot(class CBT_Composite * pComposite) { m_pRootNode = pComposite; }
	HRESULT			Init_PreviousAction(wstring strAction);

protected:
	class CBT_Composite* m_pRootNode = nullptr;
	using Actions = unordered_map<wstring, class CBT_Action*, djb2Hasher>;
	Actions m_hashActions;
	Actions::iterator		m_PreviousAction;

public:
	static CBehaviorTree* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(CGameObject * pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END