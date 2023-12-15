#pragma once
#include "ColliderBase.h"

BEGIN(Engine)


class ENGINE_DLL CSphereColliderGroup : public CCollider
{
	using Super = CCollider;
private:
	CSphereColliderGroup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSphereColliderGroup(const CSphereColliderGroup& rhs);
	virtual ~CSphereColliderGroup() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void	Update_Collider()						override;
	virtual void	DebugRender()							override;

	virtual _bool	Intersects(SimpleMath::Ray& ray, OUT _float& distance)	override;
	virtual _bool	Intersects(Super* other)					override;
	virtual _bool	Intersects_Bounding(Super* other)						override;
	virtual _bool	Intersects_Box(const BoundingBox& Collider) override;
	

	void				Add_Collider(CCollider* pCollider);
	CCollider*			Get_Collider(_uint iIndex);
	vector<CCollider*>& Get_Colliders() { return m_Colliders; }

	_uint		Get_CollisionIndex() { return m_iCollisionIndex; }

private:
	vector<CCollider*> m_Colliders;



	_uint m_iCollisionIndex;
public:
	static	CSphereColliderGroup* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free()						override;
};

END