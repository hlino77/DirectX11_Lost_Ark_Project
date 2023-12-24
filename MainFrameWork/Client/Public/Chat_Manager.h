#pragma once
#include "Base.h"
#include "Lock.h"

BEGIN(Engine)
class CTextBox;
END


BEGIN(Client)
class CUI_TextBox;

class CChat_Manager final : public CBase
{
	DECLARE_SINGLETON(CChat_Manager);

public:
	CChat_Manager();
	virtual ~CChat_Manager() = default;

public:
	HRESULT Reserve_Manager(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void	ClearText();
	void	PopText();
	void	ClearCombineText();
	void	Add_Text(const wstring& szText);
	void	Set_CombineText(const wstring& szText);

	LRESULT Chat_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT Render();

	void	CursurTick(_float fTimeDelta);

	_bool	Is_Active() { return m_bActive; }
	void	Set_Active(_bool bActive);
	void	OnOff();
	void	ResetBlink();
	_uint	Get_Language() { return m_iLanguage; }
	_bool	Is_Chat() { return m_bChat; }

	//Chating


	void	Add_Chat(wstring& szChat);
	void	Send_Chat(const wstring& szChat);

	static wstring S2W(const string& strValue);
	static string W2S(const wstring& szValue);


	HRESULT Ready_ChatWindows();
	void	Release_ChatWindows();
private:
	void	StartChat();
	void	EndChat();

	void	Update_InputChat();
	void	Update_ChatWindow();

private:

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;


	wstring m_szInputText;
	wstring m_szCombiningText;
	wstring m_szSendText;

	_uint m_iLanguage = 0;

	_bool m_bChat = false;
	_bool m_bSend = false;
	_bool m_bActive = false;

	_float m_fBlinkDelay = 0.0f;
	_float m_fCurrDelay = 0.0f;
	_bool m_bCursur = true;


	deque<wstring> m_ChatList;

	_uint m_iMaxChat = 0;



	Vec2 m_vTextScale;

	_float m_fChatWinSizeX = 0.0f;
	_float m_fChatWinSizeY = 0.0f;
	_float m_fInputWindowSizeX = 0.0f;


	wstring m_szFont;

	//TextBox
	CTextBox* m_pInputWindow = nullptr;
	CTextBox* m_pChatWindow = nullptr;

	USE_LOCK
public:
	virtual void Free() override;
};

END
