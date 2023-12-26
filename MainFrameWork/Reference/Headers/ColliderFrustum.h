#pragma once
#include "ColliderBase.h"

BEGIN(Engine)

class ENGINE_DLL CFrustumCollider : public CCollider
{
	using Super = CCollider;
private:
	CFrustumCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFrustumCollider(const CFrustumCollider& rhs);
	virtual ~CFrustumCollider() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void	Update_Collider()						override;
	virtual void	DebugRender()							override;

	virtual _bool	Intersects(SimpleMath::Ray& ray, OUT _float& distance)	override;
	virtual _bool	Intersects(Super* other)								override;
	virtual _bool	Intersects_Bounding(Super* other)						override;
	virtual _bool	Intersects_Box(const BoundingBox& Collier)				override;




	BoundingFrustum& GetBoundingFrustum()		{ return m_tBoundingFrustum; }


	void				Set_BoundingFrustum();


private:
	BoundingFrustum	m_tBoundingFrustum;

public:
	static	CFrustumCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free()						override;
};

END