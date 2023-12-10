#pragma once

#include "Component_Manager.h"

/* 클라이언트개발자가 엔진의 기능을 이용하고자할 때 접촉하는 객체.  */
/* 클라이언트에 보여줘야할 함수들을 모두 정의하고 있는다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(_uint iNumLevels, _uint iNumLayerType, 
		const GRAPHIC_DESC& GraphicDesc, 
		_Inout_ ID3D11Device** ppDevice, 
		_Inout_ ID3D11DeviceContext** ppContext, 
		_In_ HWND hWnd,
		_In_ HINSTANCE hInst);

	HRESULT Initialize_Engine_Server(_uint iNumLevels, _uint iNumLayerType);

	void Tick(_float fTimeDelta);
	void Tick_Server(_float fTimeDelta);


	void Clear(_uint iLevelIndex);

public: /* For.Timer_Manager */
	_float Compute_TimeDelta(const wstring& strTimerTag);
	HRESULT	Add_Timer(const wstring& strTimerTag);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(Vec4 vClearColor);	
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();
	ID3D11Device* Get_Device();
	ID3D11DeviceContext* Get_Context();


public: /* For.Input_Device */
	_char Get_DIKState(_uchar eKeyID);
	_char Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);


public: /* For.Level_Manager */
	HRESULT			Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	HRESULT			Render_Debug();
	_uint			Get_CurrLevelIndex();
	void			Set_Loading(_bool bLoading);
	void			Set_LoadingNext(_uint iNextLevel);
	class CLevel*	Get_CurrLevel();
public: /* For.Object_Manager */
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	CGameObject* Add_GameObject(_uint iLevelIndex, _uint iNumLayerType, const wstring& strPrototypeTag, void* pArg = nullptr);
	CGameObject* Clone_GameObject(const wstring & strPrototypeTag, void* pArg = nullptr);
	CGameObject* Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, const wstring & strObjectTag);
	CGameObject* Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, _int iObjectID);
	CGameObject* Find_NearGameObject(_uint iLevelIndex, const _uint iLayerType, CGameObject * pObject);
	list<CGameObject*>& Find_GameObjects(_uint iLevelIndex, const _uint iLayerType);
	HRESULT Delete_GameObject(_uint iLevelIndex, const _uint iLayerType, const CGameObject* pGameObject);

public: /* For.Utilities */
	string wstring_to_string(const wstring & strW);
	wstring string_to_wstring(const string & strS);

public: /* For. Componenet_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring & strProtoTypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(CGameObject * pObject, _uint iLevelIndex, const wstring & strProtoTypeTag, void* pArg = nullptr);
	HRESULT Check_Prototype(_uint iLevelIndex, const wstring & strProtoTypeTag);

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc);
	HRESULT Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc, class CTexture* pTexture);
	void Ready_LightMatrix(Vec3 vOffset, Vec3 vLook);
	void Ready_StaticLightMatrix(Vec3 vPos, Vec3 vLook);

	Matrix Get_DirectionLightMatrix();
	Matrix Get_StaticLightMatrix();

	void	Update_LightMatrix(Vec3 vPos);

	HRESULT Reset_Lights();


public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, Matrix TransformMatrix);
	Matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eTransformState) const;
	Matrix Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eTransformState) const;
	Vec4 Get_CamPosition();
	const BoundingFrustum& Get_CamFrustum();
/* For. KeyManager */
public:
	KEY_STATE GetKeyState(KEY _eKey);
	const POINT& GetMousePos();


	/* For. NavigationManager */
public:


	/* For. QuadTreeManager */
public:
	HRESULT Make_QaudTree(Vec3 vPos, Vec3 vScale, _uint iMaxDepth);
	HRESULT Reset_QaudTree();
	_bool	Add_Object_To_QuadTree(CSphereCollider * pCollider);


	/* For. FontManager */
public:
	void			AddFont(const wstring & szTextName, const wstring & szFontPath);
	void			DrawFont(const wstring & szTextName, const wstring & szString, const Vec2 & vPosition, const Vec4 & vColor, const _float & fRotation, const Vec2 & vOrigin, const Vec2 & vScale);
	Vec2			MeasureString(const wstring & szTextName, const wstring & szString);
	void			InputText(wstring& szInputText);

	/* For. SoundManager */

	HRESULT PlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume);
	HRESULT PlayBGM(const wstring& strSoundKey, _uint iChannel, _float fVolume);
	HRESULT StopSound(_uint iChannel);
	HRESULT StopSoundAll();
	HRESULT SetChannelVolume(_uint iChannel, _float fVolume);
	HRESULT CheckPlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume);

	HRESULT	Initialize_LoopChannel(_uint iStart, _uint iEnd);
	HRESULT	PlaySound_Distance(const wstring& strSoundKey, _uint iChannel, _float fVolume, Vec3 vPos, _float fRange);
	HRESULT PlaySoundFile_LoopChannel(const wstring& strSoundKey, _float fVolume);
	HRESULT	PlaySound_Distance_LoopChannel(const wstring& strSoundKey, _float fVolume, Vec3 vPos, _float fRange);
	HRESULT Find_Stop_Sound(const wstring& strSoundKey);

	HRESULT Add_SoundTrack(const wstring& strSoundTrack, const wstring& strSoundKey);
	const wstring& Get_RandomSoundKey(const wstring& strSoundTrack);
public:



private:
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CText_Manager*			m_pText_Manager = { nullptr };

	class CKey_Manager*				m_pKey_Manager = { nullptr };
	class CUtils*					m_pUtilities = { nullptr };

	class CNavigationMgr*			m_pNavigationMgr = { nullptr };
	class CEventMgr*				m_pEventMgr = { nullptr };
	class CPhysXMgr*				m_pPhysXMgr = { nullptr };
	class CQuadTreeMgr*				m_pQuadTreeMgr = { nullptr };
	class CSound_Manager*			m_pSoundMgr = { nullptr };

public:
	static void Release_Engine();
	virtual void Free() override;
};

END