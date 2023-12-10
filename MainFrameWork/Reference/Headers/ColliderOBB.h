#pragma once
#include "ColliderBase.h"

BEGIN(Engine)

class ENGINE_DLL COBBCollider : public CCollider
{
	using Super = CCollider;
private:
	COBBCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	COBBCollider(const COBBCollider& rhs);
	virtual ~COBBCollider() = default;

public:
	virtual HRESULT Initialize_Prototype()					override;
	virtual HRESULT Initialize(void* pArg)					override;
	virtual void	Tick(const _float& fTimeDelta)			override;
	virtual void	LateTick(const _float& fTimeDelta)		override;
	virtual void	DebugRender()							override;

	virtual _bool	Intersects(SimpleMath::Ray& ray, OUT _float& distance)	override;
	virtual _bool	Intersects(Super* other)					override;
	virtual _bool			Intersects_Box(const BoundingBox& Collier) override;

	void			Set_Scale(Vec3 vScale) { m_vScale = vScale; }
	Vec3			Get_Scale() { return m_vScale; }



	BoundingOrientedBox& GetBoundingBox()		{ return m_tBoundingBox; }

	void				Set_Orientation(Quaternion vQuat) { m_vOrientation = vQuat; }
	Quaternion			Get_Orientation() { return m_vOrientation; }

	void				Set_StaticBoundingBox();
private:
	void				Set_BoundingBox();

private:
	BoundingOrientedBox	m_tBoundingBox;



	Vec3				m_vScale;
	Quaternion			m_vOrientation;
public:
	static	COBBCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free()						override;
};

END