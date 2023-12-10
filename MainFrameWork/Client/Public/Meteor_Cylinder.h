#pragma once
#include "Effect.h"
BEGIN(Engine)
END


BEGIN(Client)

class CMeteor_Cylinder : public CEffect
{
	// CGameObject을(를) 통해 상속됨

private:
	/* 원형을 생성할 때 */
	CMeteor_Cylinder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 */
	CMeteor_Cylinder(const CMeteor_Cylinder& rhs); /* 복사 생성자. */
	virtual ~CMeteor_Cylinder() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	void		Appear();
	void		DisAppear();

	void		Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }


protected:
	virtual HRESULT Ready_Components();


private:



private:
	Vec4 m_vColor;
	Vec4 m_vBlur;


	Vec3 m_vTargetScale;

	_float m_fAlphaWeight = 0.0f;

	_bool m_bEnd = false;



	CGameObject* m_pOwner = nullptr;


	_float m_fEffectTime = 0.0f;

public:
	virtual void Free() override;
	virtual CGameObject* Clone(void* pArg) override;
	static CMeteor_Cylinder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

};

END