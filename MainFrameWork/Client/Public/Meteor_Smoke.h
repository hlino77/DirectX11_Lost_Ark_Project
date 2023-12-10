#pragma once
#include "Effect.h"
BEGIN(Engine)
END


BEGIN(Client)

class CMeteor_Smoke : public CEffect
{
	// CGameObject을(를) 통해 상속됨

private:
	/* 원형을 생성할 때 */
	CMeteor_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 */
	CMeteor_Smoke(const CMeteor_Smoke& rhs); /* 복사 생성자. */
	virtual ~CMeteor_Smoke() = default;

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

	_bool m_bEnd = false;

	_float m_fUV = 0.0f;

	CGameObject* m_pOwner = nullptr;


	Vec3 m_vScale;

public:
	virtual void Free() override;
	virtual CGameObject* Clone(void* pArg) override;
	static CMeteor_Smoke* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

};

END