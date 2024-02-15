#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;

END

BEGIN(Client)

class CSkyFloor : public CGameObject
{
	using Super = CGameObject;
public:
	struct SkyFloorDescription
	{
		Vec2 vTileCount = Vec2::One;
		Vec2 vFloorUVoffset = Vec2::Zero;	// 여기엔 Speed를 전달해주자
		_float fAlpha = FLT_MAX;
		Vec3 vFloorColorMul = Vec3::One;
	};

private:
	CSkyFloor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkyFloor(const CSkyFloor& rhs);
	virtual ~CSkyFloor() = default;

public:
	virtual HRESULT Initialize_Prototype()		override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render();

private:
	SkyFloorDescription m_tSKyFloorDesc;

	Vec2 m_vFloorSpeed = Vec2::Zero;

	CTexture* m_pTexture = nullptr;

private:
	virtual HRESULT Ready_Components();

public:
	virtual void Free() override;
	virtual CGameObject* Clone(void* pArg) override;
	static CSkyFloor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
};

END