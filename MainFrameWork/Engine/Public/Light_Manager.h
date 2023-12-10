#pragma once

#include "Base.h"

BEGIN(Engine)

/* 빛들을 보관한다. */

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);

public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc, class CTexture* pTexture);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
public:
	HRESULT Reset_Lights();
	void Ready_LightMatrix(Vec3 vOffset,  Vec3 vLook);
	void Ready_StaticLightMatrix(Vec3 vPos, Vec3 vLook);
	void	Update_LightMatrix(Vec3 vPos);
	Matrix Get_DirectionLightMatrix() { return m_DirectionLightMatrix; }
	Matrix Get_StaticLightMatrix() { return m_StaticLightMatrix; }
private:
	

private:
	list<class CLight*>			m_Lights;
	typedef list<class CLight*>	LIGHTS;

	Matrix m_DirectionLightMatrix;
	Matrix m_StaticLightMatrix;

	Vec3 m_vLightOffset;
	Vec3 m_vLightLook;

public:
	virtual void Free() override;
};

END