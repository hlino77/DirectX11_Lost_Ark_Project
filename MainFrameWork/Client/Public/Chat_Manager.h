#pragma once
#include "Base.h"
#include "Lock.h"

BEGIN(Client)

class CChat_Manager final : public CBase
{
	DECLARE_SINGLETON(CChat_Manager);

public:
	typedef struct ChatDesc
	{
		wstring szNickName;
		wstring szChat;
	}CHAT;

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

	_bool	Is_Active() { return m_bChat; }
	void	OnOff();
	void	ResetBlink();



	//Chating


	void	Add_Chat(const CHAT& tChat);
	void	Send_Chat(const wstring& szChat);

	static wstring S2W(const string& strValue);
	static string W2S(const wstring& szValue);

private:
	void	StartChat();
	void	EndChat();



private:

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;


	wstring m_szInputText;
	wstring m_szCombiningText;
	wstring m_szSendText;

	_uint m_iLanguage = 0;

	_bool m_bChat = false;
	_bool m_bSend = false;

	_float m_fBlinkDelay = 0.0f;
	_float m_fCurrDelay = 0.0f;
	_bool m_bCursur = true;


	deque<CHAT> m_ChatList;

	_uint m_iMaxChat = 0;

	USE_LOCK
public:
	virtual void Free() override;
};

END
