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
	
private:
	void	UI_Tool();
	void	Find_PickedUI();
	void	UI_Hierachy();
	void	UI_Inspector();
	void	Reset_CurrentUI();
	void	Change_Mode();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	HWND m_hWnd = { nullptr };

	CUI*	m_pCurrentUI = {nullptr};
	_bool	m_bToolMode = { false };
	list<CUI*>	m_UIList_pCurrentLevel;

public:
	virtual void Free()override;
};

END