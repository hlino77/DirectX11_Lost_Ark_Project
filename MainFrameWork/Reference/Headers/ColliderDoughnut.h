#pragma once
#include "ColliderBase.h"

BEGIN(Engine)

class ENGINE_DLL CDoughnutCollider : public CCollider
{
	using Super = CCollider;
private:
	CDoughnutCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDoughnutCollider(const CDoughnutCollider& rhs);
	virtual ~CDoughnutCollider() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void	Update_Collider()						override;
	virtual void	DebugRender()							override;

	virtual _bool	Intersects(SimpleMath::Ray& ray, OUT _float& distance)	override;
	virtual _bool	Intersects(Super* other)					override;
	virtual _bool	Intersects_Bounding(Super* other)						override;
	virtual _bool	Intersects_Box(const BoundingBox& Collider) override;
	
	void			Set_Center(Vec3 vPos) { m_tBoundingSphere.Center = vPos; }
	void			Set_Center_ToBone();

	Vec3			Get_Center() { return m_tBoundingSphere.Center; }
	void			Set_Radius(_float fRadius) { m_tBoundingSphere.Radius = fRadius; }
	_float			Get_Radius() { return m_tBoundingSphere.Radius; }

	

	BoundingSphere&	GetBoundingSphere()		{ return m_tBoundingSphere; }

public:
	void			Set_Center();


private:
	BoundingSphere	m_tBoundingSphere;


public:
	static	CDoughnutCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free()						override;
};

END