#pragma once

#include "Client_Defines.h"
#include "BindShaderDesc.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Point;

END

BEGIN(Client)
class CVoidEffect : public CGameObject
{
	using Super = CGameObject;
public:
	struct tagVoidEffectDesc
	{
		wstring protoModel				= TEXT("");
		wstring protoDiffuseTexture		= TEXT("");
		wstring protoNoiseTexture		= TEXT("");
		wstring protoMaskTexture		= TEXT("");
		wstring protoEmissiveTexture	= TEXT("");
		wstring protoDissolveTexture	= TEXT("");

		//Vec2	vUV_Position		= Vec2(0.f, 0.f);
		//Vec2	vUV_Rotation		= Vec2(0.f, 0.f);
		//Vec2	vUV_Scaling			= Vec2(1.f, 1.f);
		//Vec2	vUV_Tiles			= Vec2(1.f, 1.f);
		//Vec2	vUV_Speed			= Vec2(0.f, 0.f);

		//Vec3	vTransform_Position = Vec3(0.f, 0.f, 0.f);
		//Vec3	vTransform_Rotation = Vec3(0.f, 0.f, 0.f);
		//Vec3	vTransform_Scaling	= Vec3(1.f, 1.f, 1.f);

		//_float	fLifeTime			= 0.f;
		//_float	fBloomIntensity		= 0.f;
		//_float	fRadialIntensity	= 0.f;
		//_bool	bBlendNonBlend		= 0.f;
	};
private:
	CVoidEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVoidEffect(const CVoidEffect& rhs);
	virtual ~CVoidEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(tagVoidEffectDesc* pDesc);
	virtual HRESULT Ready_Components() override { return E_FAIL; };

private:
	CVIBuffer_Point* m_pBuffer		= nullptr;
	CTexture*	m_pDiffuseTexture	= nullptr;
	CTexture*	m_pNoiseTexture		= nullptr;
	CTexture*	m_pMaskTexture		= nullptr;
	CTexture*	m_pEmissiveTexture	= nullptr;
	CTexture*	m_pDissolveTexture	= nullptr;

	EffectMaterialFlag m_tNoisMaskEmisDslv;

	//friend class CEffectTool;

public:
	static CVoidEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END