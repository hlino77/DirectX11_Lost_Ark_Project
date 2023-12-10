#pragma once
#include "Base.h"

BEGIN(Server)

class CLobbyUser_Server
{
public:
	CLobbyUser_Server(const wstring& szNickName);
	~CLobbyUser_Server() = default;

public:
	const wstring&	Get_NickName() { return m_szNickName; }

	const wstring&	Get_Character() { return m_szCharacter; }
	void			Set_Character(const wstring& szCharacter) { m_szCharacter = szCharacter; }

	void			Set_Ready() { m_bReady = true; }
	_bool			Is_Ready() { return m_bReady; }
private:
	wstring m_szNickName;
	wstring m_szCharacter;

	_bool	m_bReady = false;
};

END