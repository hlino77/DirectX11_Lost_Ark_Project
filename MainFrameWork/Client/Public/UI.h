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

class CUI : public CGameObject
{
public:
	enum class UISTATE
	{
		READY,
		APPEAR,
		TICK,
		DISAPPEAR,
		STATE_END
	};

	struct UIDESC
	{
		_float		fX;
		_float		fY;
		_float		fSizeX;
		_float		fSizeY;
	};

protected:
	/* 원형을 생성할 때 */
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 */
	CUI(const CGameObject& rhs); /* 복사 생성자. */
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Appear();
	virtual void UI_Tick(_float fTimeDelta) PURE;
	virtual void UI_AppearTick(_float fTimeDelta);
	virtual void UI_DisappearTick(_float fTimeDelta);
	virtual void Disappear();

public:
	void		Set_State(UISTATE eState) { m_eState = eState; }
	UISTATE		Get_UIState() { return m_eState; }
	_bool Get_Pick() { return m_bPick; }
	_bool Set_Pick(_bool Pick) { m_bPick = Pick; }
	virtual void	Picking_UI();
	virtual void	Create_Rect(void* pArg = nullptr);
	virtual void	Create_Rect(UIDESC UIDesc);
	virtual void	RemoveDeadItem_And_ReOrder(vector<CGameObject*>& vec);  // 벡터 정렬

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

protected:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	Matrix					m_ViewMatrix, m_ProjMatrix;

	UISTATE					m_eState;
	Vec3					m_vUITargetPos;
	Vec2					m_vUITargetSize;

	_float					m_fAlpha = 1.0f;
	Vec4					m_vColor;

	_bool				m_bPick = false;            // 피킹 확인
	RECT				m_rcUI = {};
protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;
};

END