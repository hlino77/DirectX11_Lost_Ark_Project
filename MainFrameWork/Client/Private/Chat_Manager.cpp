#include "stdafx.h"
#include "Chat_Manager.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include <iostream>
#include "AsUtils.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include <locale>
#include <atlstr.h>

IMPLEMENT_SINGLETON(CChat_Manager);


CChat_Manager::CChat_Manager()
{
}

HRESULT CChat_Manager::Reserve_Manager(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;


    HIMC hIMC = ImmGetContext(hwnd);
    m_iLanguage = ImmGetOpenStatus(hIMC);

    m_fBlinkDelay = 0.7f;

    m_iMaxChat = 10;

	return S_OK;
}

void CChat_Manager::ClearText()
{
    m_szInputText.clear();
}

void CChat_Manager::PopText()
{
    if (m_szCombiningText.length() > 0)
        return;

    if(m_szInputText.length() > 0)
        m_szInputText.pop_back();

    ResetBlink();
}

void CChat_Manager::ClearCombineText()
{
    m_szCombiningText.clear();
}

void CChat_Manager::Add_Text(const wstring& szText)
{
    m_szInputText += szText;
    ResetBlink();
}

void CChat_Manager::Set_CombineText(const wstring& szText)
{
    m_szCombiningText = szText;
}



LRESULT CChat_Manager::Chat_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_bChat == false)
        return 0;

    HIMC hIMC = ImmGetContext(hwnd);
    _uint iLanguage = ImmGetOpenStatus(hIMC);

    if (m_iLanguage != iLanguage && iLanguage == 0)
        ClearCombineText();

    m_iLanguage = iLanguage;


    switch (uMsg)
    {
    case WM_CHAR:
    {
        wchar_t charInput = static_cast<wchar_t>(wParam);
        wstring szInput(&charInput, 1);
        if (szInput.length() > 0)
        {
            if (szInput == L" ")
            {
                Add_Text(szInput);
            }
            else if (szInput == L"\b")
            {
                PopText();
            }
            else if (szInput == L"\t")
            {
            }
            else if (szInput == L"\r")
            {
                if (m_bChat)
                    OnOff();
            }
            else
            {
                Add_Text(szInput);
            }
        }
        break;
    }
    case WM_IME_COMPOSITION:
    {
        if (lParam & GCS_COMPSTR)
        {
            wchar_t resultString[2];
            UINT resultLength = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, resultString, sizeof(resultString) / sizeof(wchar_t));

            // 조합 완료된 문자열을 현재 문자열에 추가
            if (resultLength > 1)
                Set_CombineText(wstring(resultString, resultLength - 1));
        }
        break;
    }
    case WM_IME_ENDCOMPOSITION:
    {
        ClearCombineText();
        break;
    }
    }

    return 0;
}

HRESULT CChat_Manager::Render()
{
    if (m_bChat == false)
        return S_OK;
    
   

    wstring szDrawText = m_szInputText + m_szCombiningText;

    Vec2 vTextPos(0.f, 500.f);
    Vec2 vScale(0.5f, 0.5f);


    CGameInstance::GetInstance()->DrawFont(L"125", szDrawText, vTextPos, Vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.f, Vec2(0.0f, 0.0f), vScale);


    if (m_bCursur)
    {
        szDrawText += L".";

       
        Vec2 vCursurPos;
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"125", szDrawText);

        vCursurPos.x = vTextPos.x + (vMeasure.x * vScale.x) - 12.0f;
        vCursurPos.y = vTextPos.y;

        CGameInstance::GetInstance()->DrawFont(L"125", L"|", vCursurPos, Vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, Vec2(0.0f, 0.0f), vScale);
    }


    _int iChatSize = m_ChatList.size();

    if (iChatSize > 0)
    {
        Vec2 vStartPos = vTextPos;

        wstring szResult;

        for (_int i = iChatSize - 1; i >= 0; --i)
        {
            szResult += m_ChatList[i].szChat + L"\n";
        }

        Vec2 vCursurPos = vStartPos;
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"125", szResult);
        vCursurPos.y -= (vMeasure.y * vScale.y) + 15.0f;

        CGameInstance::GetInstance()->DrawFont(L"125", szResult, vCursurPos, Vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.f, Vec2(0.0f, 0.0f), vScale);
    }

    return S_OK;
}

void CChat_Manager::CursurTick(_float fTimeDelta)
{
    m_fCurrDelay += fTimeDelta;
    if (m_fCurrDelay >= m_fBlinkDelay)
    {
        m_bCursur = !m_bCursur;
        m_fCurrDelay = 0.0f;
    }


    if (m_bSend)
    {
        if (m_szSendText == m_szInputText)
        {
            CHAT tChat;
            tChat.szNickName = CServerSessionManager::GetInstance()->Get_NickName();
            tChat.szChat = m_szInputText + m_szCombiningText;

            //Add_Chat(tChat);
            Send_Chat(tChat.szChat);

            m_szInputText.clear();
            m_szCombiningText.clear();
            m_szSendText.clear();
            m_bSend = false;
        }
    }

}

void CChat_Manager::OnOff()
{
    if (m_bChat)
    {
        wstring szInputText = m_szInputText + m_szCombiningText;
        if (szInputText.length() > 0)
        {
            m_szSendText = szInputText;
            m_bSend = true;
        }
        else
            EndChat();
    }
    else
    {
        StartChat();
    }
}

void CChat_Manager::ResetBlink()
{
    m_bCursur = true;
    m_fCurrDelay = 0.0f;
}

void CChat_Manager::Add_Chat(const CHAT& tChat)
{
    WRITE_LOCK
    m_ChatList.push_front(tChat);

    _int iPopSize = m_ChatList.size() - m_iMaxChat;

    if (iPopSize > 0)
    {
        for (_uint i = 0; i < iPopSize; ++i)
            m_ChatList.pop_back();
    }
}

void CChat_Manager::Send_Chat(const wstring& szChat)
{
    Protocol::S_CHAT pkt;

    wstring szSendChatW = CServerSessionManager::GetInstance()->Get_Player()->Get_NickName() + L" : " + szChat;

    string szSendChat = W2S(szSendChatW);

    pkt.set_szchat(szSendChat);

    SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
    CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CChat_Manager::StartChat()
{
    m_szInputText.clear();
    m_szCombiningText.clear();
    m_bChat = true;
    ResetBlink();
    m_bSend = false;
}

void CChat_Manager::EndChat()
{
    m_szInputText.clear();
    m_szCombiningText.clear();
    m_bChat = false;
    m_bCursur = false;
    m_bSend = false;
}

wstring CChat_Manager::S2W(const string& strValue)
{
    _int iLength = MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, NULL, 0);

    wchar_t* szString = new wchar_t[iLength];

    MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, szString, iLength);

    wstring szResult = szString;
    
    Safe_Delete_Array(szString);
    
    return szResult;
}

string CChat_Manager::W2S(const wstring& szValue)
{
    _int iLength = WideCharToMultiByte(CP_ACP, 0, szValue.c_str(), -1, NULL, 0, NULL, NULL);

    char* strString = new char[iLength];
    
    WideCharToMultiByte(CP_ACP, 0, szValue.c_str(), -1, strString, iLength, NULL, NULL);

    string strResult = strString;

    Safe_Delete_Array(strString);

    return strResult;
}



void CChat_Manager::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

