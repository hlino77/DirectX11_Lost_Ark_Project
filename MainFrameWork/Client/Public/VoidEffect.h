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
	};
private:
	CVoidEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVoidEffect(const CVoidEffect& rhs);
	virtual ~CVoidEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(tagVoidEffectDesc* pDesc);
	virtual HRESULT Ready_Components() override { return E_FAIL; };

public:
	void	Set_UV_Position(Vec2 vUV_Position) {};
	void	Set_UV_Rotation(Vec2 vUV_Rotation) {};
	void	Set_UV_Scaling(Vec2 vUV_Scaling) {};
	void	Set_UV_Speed(Vec2 vUV_Speed) {};
	void	Set_UV_Tiles(Vec2 vTiles) {};

	void	Set_Transform_Position(Vec3 vTransform_Position) {};
	void	Set_Transform_Rotation(Vec3 vTransform_Rotation) {};
	void	Set_Transform_Scaling(Vec3 vTransform_Scaling) {};

	void	Set_LifeTime(Vec3 vTransform_Position) {};
	void	Set_BloomIntensity(Vec3 vTransform_Rotation) {};
	void	Set_BlendNonBlend(Vec3 vTransform_Scaling) {};

private:
	CVIBuffer_Point* m_pBuffer		= nullptr;
	CTexture*	m_pDiffuseTexture	= nullptr;
	CTexture*	m_pNoiseTexture		= nullptr;
	CTexture*	m_pMaskTexture		= nullptr;
	CTexture*	m_pEmissiveTexture	= nullptr;
	CTexture*	m_pDissolveTexture	= nullptr;

	EffectMaterialFlag m_tNoisMaskEmisDslv;

private:
	Vec2	m_vUV_Position = Vec2(0.f, 0.f);
	Vec2	m_vUV_Rotation = Vec2(0.f, 0.f);
	Vec2	m_vUV_Speed = Vec2(0.f, 0.f);
	Vec2	m_vUV_Tiles = Vec2(1.f, 1.f);
	Vec2	m_vUV_Index = Vec2(0.f, 0.f);

	Vec3	m_vTransform_Position_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vTransform_Position_End = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vTransform_Rotation_Start = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vTransform_Rotation_End = Vec3(0.f, 0.f, 0.f);
	Vec3	m_vTransform_Scaling_Start = Vec3(1.f, 1.f, 1.f);
	Vec3	m_vTransform_Scaling_End = Vec3(1.f, 1.f, 1.f);

	_float	m_fLifeTime = FLT_MAX;
	_float	m_fBloomIntensity = 0.f;
	_float	m_fRadialIntensity = 0.f;
	_bool	m_bBlendNonBlend = 0.f;

	//friend class CEffectTool;

public:
	static CVoidEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END