#pragma once
#include "Component.h"
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

	void			SetRoot(class CBT_Composite * pComposite) { m_pRootNode = pComposite; }

	

protected:
	class CBT_Composite* m_pRootNode = nullptr;


public:
	static CBehaviorTree* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(CGameObject * pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END