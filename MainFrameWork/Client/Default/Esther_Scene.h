#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEsther_Scene final : public CGameObject
{
public:
	typedef struct tagEsSecneDesc
	{
		wstring strTexutre;
		_float fStartFrame;
		_float fFinalFrame;
		_float fFrameSpeed;
	}ESTHERSCENEDESC;

private:
	CEsther_Scene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Scene(const CGameObject& rhs); 

	virtual ~CEsther_Scene() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool	Is_PlayFrame() { return m_bPlayFrame; }

	void	Play_Frame();
	void	Stop_Frame() { m_bPlayFrame = false; }

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	Matrix					m_ViewMatrix, m_ProjMatrix;
	_float					m_fFrame = { 0.f };
	_bool					m_IsFrameEnd = { false };

private:
	_bool					m_bPlayFrame = { false };

	wstring					m_strTexutre;
	_float					m_fStartFrame;
	_float					m_fFinalFrame;
	_float					m_fFrameSpeed;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CEsther_Scene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* 사본객체를 생성할때 원본데이터로부터 복제해올 데이터외에 더 추가해 줘야할 데이터가 있다라면 받아오겠다. */
	virtual void Free() override;
};

END