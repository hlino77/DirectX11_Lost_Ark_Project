#pragma once
#include "Effect.h"
#include <random>

BEGIN(Engine)

END

BEGIN(Client)

class CEffect_Custom_CrossHair : public CEffect
{
	using Super = CEffect;

	enum class CrossHairState { START, IDLE, SHAKE, EFFECTEND, STATEEND };

private:
	CEffect_Custom_CrossHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Custom_CrossHair(const CEffect_Custom_CrossHair& rhs);
	virtual ~CEffect_Custom_CrossHair() = default;

public:
	virtual HRESULT Initialize_Prototype(EFFECTDESC* pDesc)	override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(_float fTimeDelta)		override;
	virtual void	LateTick(_float fTimeDelta)	override;
	virtual HRESULT Render()					override;

	virtual void EffectEnd();

	void	EffectShot();
private:
	void		Tick_Start(_float fTimeDelta);
	void		Tick_Idle(_float fTimeDelta);
	void		Tick_Shake(_float fTimeDelta);
	void		Tick_EffectEnd(_float fTimeDelta);


	virtual HRESULT Ready_Components() override;

	void		Update_Pos();
	void		Update_Scale_Start(_float fTimeDelta);
	void		Update_Scale_End(_float fTimeDelta);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCrossHair = { nullptr };
	CTexture* m_pTextureOutCircle = { nullptr };


	_float					m_fX, m_fY, m_fDefaultSize, m_fStartSize;
	Matrix					m_ViewMatrix, m_ProjMatrix;
	_float					m_fAlpha;

	_float m_fShakeAcc = 0.0f;
	_float m_fShakeTime = 0.0f;

	Vec4					m_vColor;

	CrossHairState m_eState;
public:
	static CEffect_Custom_CrossHair* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END