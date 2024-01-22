#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)

class CEffect_Custom_EarthEaterSmallParticle : public CEffect
{
	using Super = CEffect;
public:
	typedef struct CustomEffectDesc
	{
		Vec3 vPos;
		Vec3 vLook;
		Vec3 vPlayerPos;
	};

private:
	enum class EarthEaterState { START, TURN, SWING, SHOT, DISAPPEAR, STATEEND };

private:
	CEffect_Custom_EarthEaterSmallParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_EarthEaterSmallParticle(const CEffect_Custom_EarthEaterSmallParticle& rhs);
	virtual ~CEffect_Custom_EarthEaterSmallParticle() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

	void	Reset(CustomEffectDesc& tDesc);

	void	Tick_Start(_float fTimeDelta);
	void	Tick_Turn(_float fTimeDelta);
	void	Tick_Swing(_float fTimeDelta);
	void	Tick_Shot(_float fTimeDelta);
	void	Tick_Disappear(_float fTimeDelta);

	void	Set_Swing();
	
private:
	virtual HRESULT Ready_Components() override;
	void	Update_Rotation(_float fTimeDelta);

	void	Init_TurnPos();
	void	Init_SwingPos();
	void	Init_Shot();

	void	Update_RockPos(_float fTimedelta);
private:
	CTexture* m_pDissolveTexture;

	EarthEaterState m_eState;

	Vec3 m_vAxisSpeed;

	deque<Vec3> m_TurnPos;
	_float m_fTurnRatio = 0.0f;
	_float m_fTurnSpeed = 0.0f;

	deque<Vec3> m_SwingPos;
	_float m_fSwingRatio = 0.0f;
	_float m_fSwingSpeed = 0.0f;

	CustomEffectDesc m_tDesc;

	_float m_fVelocityX, m_fVelocityY, m_fVelocityZ;
	_float m_fGravity;
	_float m_fEndTime;
	Vec3 m_vStartPos;

public:
	static CEffect_Custom_EarthEaterSmallParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END