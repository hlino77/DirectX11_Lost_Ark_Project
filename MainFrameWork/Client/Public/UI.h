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
		_float		fAlpha;
		Vec4		vColor;
	};

	enum BUTTON_STATE
	{
		BUTTON_NORMAL, BUTTON_PICKING, BUTTON_PICKED, BUTTON_END
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
	void		Set_UIDesc(UIDESC UIDesc);
	UIDESC		Get_UIDesc();
	_bool		Get_Pick() { return m_bPick; }
	_bool		Set_Pick(_bool Pick) { m_bPick = Pick; }
	virtual void	Picking_UI();
	virtual void	Create_Rect(void* pArg = nullptr);
	virtual void	Moving_Rect();
	virtual void	RemoveDeadItem_And_ReOrder(vector<CGameObject*>& vec);  // 벡터 정렬
	CUI*			Get_UIPart(const wstring & strPartTag);
	vector<CUI*>	Get_UIParts();
	_uint			Get_ButtonState() { return m_eButtonState; }
	_uint			Set_ButtonState(_uint iButtonState) { m_eButtonState = (BUTTON_STATE)iButtonState; }
	void			Load_UIData(const wstring& _FilePath);

public://IMGUI
	void	Change_SizeX(_float MMX);
	void	Change_SizeY(_float MMY);
	void	Change_SizeR(_float MMX);
	void	Change_SizeL(_float MMX);
	void	Change_SizeT(_float MMY);
	void	Change_SizeB(_float MMY);

	void	Set_Size(_float fSizeX, _float fSizeY);
	void	Set_Z(_float fZ);
	void	Set_Pos(Vec2 vPos);
	void	Move_UI(POINT pt);

	void	Change_Alpha(_float fAlpha) { m_fAlpha += fAlpha; }
	void	Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
	_float	Get_Alpha() { return m_fAlpha; }
	void	Decrease_Alpha(_float fTimeDelta) { if (0 <= m_fAlpha) m_fAlpha -= fTimeDelta; }
	void	Set_Color(Vec4 vColor) { m_vColor = vColor; }
	Vec4	Get_Color() { return m_vColor; }

public:
	const wstring	Get_UITag() { return m_strUITag; }
	void	Set_ToolMode(_bool IsToolMode) { m_bTool = IsToolMode; }
	_uint	Get_TextureIndex() { return m_iTextureIndex; }
	void	Set_TextureIndex(_uint iIndex) { m_iTextureIndex = iIndex; }
	void    Set_IsChange(_bool  IsChange) { m_bChange = IsChange; }

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer*				m_pRendererCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

protected:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	Matrix					m_ViewMatrix, m_ProjMatrix;
	wstring					m_strUITag;
	wstring					m_strTextureProtoTag;

	UISTATE					m_eState;
	Vec3					m_vUITargetPos;
	Vec2					m_vUITargetSize;

	_float					m_fAlpha = 1.0f;
	Vec4					m_vColor = { 1.f, 1.f, 1.f ,1.f };

	BUTTON_STATE		m_eButtonState = { BUTTON_NORMAL };
	_bool				m_bPick = { false };
	_bool				m_bTool = { false };
	_bool				m_bChange = { false };
	RECT				m_rcUI = {};
	vector<CUI*>		m_vecUIParts;
	_uint				m_iTextureIndex = { 0 };

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;
};

END