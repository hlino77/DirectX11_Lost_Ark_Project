#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Lobby final : public CCamera
{
private:
	CCamera_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Lobby(const CCamera_Lobby& rhs, CTransform::TRANSFORMDESC* pArg);
	virtual ~CCamera_Lobby() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;
	
public:
	void		Set_FadeInOut(_float fSpeed, _bool bInOut, Vec4 vColor = Vec4());
	void		Set_FadeIntensity(_float fIntensity) { m_fFadeIntensity = fIntensity; }
	
private:
	HRESULT		Render_FadeInOut();

	void		Ready_FadeInOut();
	void		Update_FadeInOut(_float fTimeDelta);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureFade = { nullptr };

	_bool m_bFade = false;
	_float m_fFadeIntensity = 0.0f;
	_float m_fFadeSpeed = 0.0f;
	Vec4 m_vFadeColor;
	_bool m_bFadeInOut = false;

	Matrix m_FadeWorld, m_FadeVeiw, m_FadeProj;

public:
	static CCamera_Lobby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END