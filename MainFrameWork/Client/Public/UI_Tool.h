#pragma once
#include "../Public/ImGUI/imgui.h"
#include "../Public/ImGUI/imgui_impl_dx11.h"
#include "../Public/ImGUI/imgui_impl_win32.h"

#include "Client_Defines.h"
#include "Base.h"

#pragma region IMGUI_MACRO
#define IMGUI_NEW_LINE ImGui::NewLine()
#define IMGUI_SAME_LINE ImGui::SameLine()
#pragma endregion

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUI;

class CUI_Tool final:
    public CBase
{
	DECLARE_SINGLETON(CUI_Tool)
	enum UI_ChANGE_MODE
	{
		UIMODE_X, UIMODE_Y, UIMODE_L, UIMODE_R, UIMODE_T, UIMODE_B, UIMODE_MOVE, UIMODE_END
	};
private:
	CUI_Tool();
    virtual ~CUI_Tool() = default;

public:
	HRESULT	Reserve_Manager(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Initialize(void* pArg = nullptr);
	HRESULT Tick();
	HRESULT LateTick();

public:
	HRESULT Add_UI_CurrentLevel();
	_bool	Is_ToolMode() { return m_bToolMode; }
	void	Set_ToolMode(_bool	IsToolMode) { m_bToolMode = IsToolMode; }

private:
	void	UI_Tool();
	void	Find_PickedUI();
	void	UI_Hierachy();
	void	UI_Inspector();
	void	Reset_CurrentUI();
	void	Change_Mode();

private:
	//내부기능
	void	Modify_UIPannel();
	void	Modify_UIController();
	void	Change_SizeX(_float MMX);
	void	Change_SizeY(_float MMY);
	void	Change_SizeR(_float MMX);
	void	Change_SizeL(_float MMX);
	void	Change_SizeT(_float MMY);
	void	Change_SizeB(_float MMY);

	void	Set_Size(_float fSizeX, _float fSizeY);
	void	Set_Pos(Vec2 vPos);
	void	Move_UI(POINT pt);
	void	Set_Alpha(_float fAlpha);
	void	Set_Color(Vec4 vColor);

	void	Save_UIData(wstring _strFolderPath);
	void	Load_UIData(wstring _strFolderPath);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	HWND m_hWnd = { nullptr };

	CUI*	m_pCurrentUI = {nullptr};//현 레벨의 UI리스트중 선택된 UI.
	CUI*	m_pPartUI	= { nullptr };//선택된 UI의 UI파츠.
	CUI*	m_pBeforePartUI = { nullptr };
	UI_ChANGE_MODE	m_eMode = { UIMODE_END };
	wstring	m_strModeName;
	_bool	m_bToolMode = { false };//툴모드 On/Off
	_bool	m_bHierachy = { false };//저절로 켜지지말게
	_uint	m_iCurrentLevel = { LEVELID::LEVEL_END };
	_uint	m_iCurrIndex = 0;
	_uint	m_iCurrentPartIndex = 0;
	_float	m_fAlpha = { 1.f };
	_float	m_fColor[4] = { 1.f, 1.f, 1.f, 1.f };
	_float	m_fPos[2] = { g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f };
	_float	m_fSize[2] = { 200.f, 200.f };
	_float	m_fAngle = { 0.f };

	list<CUI*>	m_UIList_CurrentLevel;
	vector<CUI*> m_vecUIPart_CurrentLevel;

	//파일저장
	char m_szFileName[MAX_PATH] = {};
	wstring m_strFolderPath;
	
public:
	virtual void Free()override;
};

END