#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CMannequinPart final : public CPartObject
{
protected:
	CMannequinPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMannequinPart(const CMannequinPart& rhs);
	virtual ~CMannequinPart() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

public:
	void	Set_AnimState(_bool IsAnim) { m_IsAnim = IsAnim; }
	_bool	Is_AnimState() { return m_IsAnim; }
	void			Set_AnimationPlay() { m_bAnimationPlay = !m_bAnimationPlay; }

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Render_Static();
	HRESULT Render_Anim();

private:
	class CShader*					m_pAnimShaderCom = { nullptr };

	_bool							m_IsAnim = { false };
	_float							m_fAnimationSpeed = 1.0f;
	std::future<HRESULT>			m_PlayAnimation;
	_bool							m_bAnimationPlay = true;

public:
	static CMannequinPart* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END