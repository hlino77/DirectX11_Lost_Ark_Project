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
#include "UI_TextBox.h"
#include "TextBox.h"

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

    m_iMaxChat = 20;

    m_vTextScale = Vec2(0.4f, 0.4f);

    m_bActive = false;
    
    m_fChatWinSizeX = 1000.0f;
    m_fInputWindowSizeX = 800.0f;

    m_szFont = L"���Ŀ�����Į��";

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
    if (m_szCombiningText != szText)
    {
        m_szCombiningText = szText;
        Update_InputChat();
    }
       
}



LRESULT CChat_Manager::Chat_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_bActive == false)
        return 0;


    if (m_bChat == false)
    {
        if (uMsg == WM_CHAR)
        {
            wchar_t charInput = static_cast<wchar_t>(wParam);
            wstring szInput(&charInput, 1);
            if (szInput == L"\r")
            {
                OnOff();
            }
        }
        return 0;
    }
      
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

            // ���� �Ϸ�� ���ڿ��� ���� ���ڿ��� �߰�
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
    return S_OK;
}

void CChat_Manager::CursurTick(_float fTimeDelta)
{
    if (m_bActive == false)
        return;

    m_fCurrDelay += fTimeDelta;
    if (m_fCurrDelay >= m_fBlinkDelay)
    {
        m_bCursur = !m_bCursur;
        m_fCurrDelay = 0.0f;
        Update_InputChat();
    }


    if (m_bSend)
    {
        if (m_szSendText == m_szInputText)
        {
          
            //Add_Chat(tChat);
            Send_Chat(m_szInputText + m_szCombiningText);

            m_szInputText.clear();
            m_szCombiningText.clear();
            m_szSendText.clear();
            m_bSend = false;
            Update_InputChat();
        }
    }

}

void CChat_Manager::Set_Active(_bool bActive)
{
    m_bActive = bActive;
    if (m_bActive)
    {
        if (m_pChatWindow == nullptr || m_pInputWindow == nullptr)
        {
            Ready_ChatWindows();
        }

        //m_pChatWindow->Appear();
        m_pChatWindow->Set_Active(true);
    }
    else
    {
        EndChat();
        //m_pChatWindow->Disappear();
        m_pChatWindow->Set_Active(false);
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
    Update_InputChat();
}

void CChat_Manager::Add_Chat(wstring& szChat)
{
    //WRITE_LOCK

    wstring szCurrChat;

    for (auto& Chat : m_ChatList)
    {
        szCurrChat = Chat + szCurrChat;
    }

 
    wstring szLine;

    for (_uint i = 0; i < szChat.length(); ++i)
    {
        szLine += szChat[i];

        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szLine);
        if (vMeasure.x * m_vTextScale.x > m_fChatWinSizeX * m_vTextScale.x)
        {
            szCurrChat = szLine + szCurrChat;
            m_ChatList.push_front(szLine);
            szLine.clear();
        }
    }

    if(szLine.length() > 0)
       m_ChatList.push_front(szLine);

    _int iPopSize = m_ChatList.size() - m_iMaxChat;

    if (iPopSize > 0)
    {
        for (_uint i = 0; i < iPopSize; ++i)
            m_ChatList.pop_back();
    }

    Vec2 vTextPos(0.f, 300.f);

    _int iChatSize = m_ChatList.size();

    if (iChatSize > 0)
    {
        Vec2 vStartPos = vTextPos;

        wstring szResult;

        for (_int i = iChatSize - 1; i >= 0; --i)
        {
            szResult += m_ChatList[i] + L"\n";
        }

        Vec2 vCursurPos = vStartPos;
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szResult);
        vCursurPos.y -= (vMeasure.y * m_vTextScale.y) + 15.0f;

        m_pChatWindow->Set_Text(L"ChatWindow", m_szFont, szResult, vCursurPos, m_vTextScale, Vec2(0.0f, 0.0f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
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
    m_pInputWindow->Set_Active(true);
}

void CChat_Manager::EndChat()
{
    m_szInputText.clear();
    m_szCombiningText.clear();
    m_bChat = false;
    m_bCursur = false;
    m_bSend = false;
    m_pInputWindow->Set_Active(false);
}

void CChat_Manager::Update_InputChat()
{
    if (m_bChat == false)
        return;

    Vec2 vTextPos(0.f, 300.f);

    wstring szDrawText = m_szInputText + m_szCombiningText;
    wstring szMeasureText = szDrawText + L".";

    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szMeasureText);
        vMeasure *= m_vTextScale;

        if (vMeasure.x > m_fInputWindowSizeX * m_vTextScale.x)
        {

            vTextPos.x -= vMeasure.x - (m_fInputWindowSizeX * m_vTextScale.x);
        }

        m_pInputWindow->Set_Text(L"InputText", m_szFont, szDrawText, vTextPos, m_vTextScale, Vec2(0.0f, 0.0f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }


    {
        Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szMeasureText);

        Vec2 vCursurPos;

        vCursurPos.x = vTextPos.x + (vMeasure.x * m_vTextScale.x) - 7.0f;
        vCursurPos.y = vTextPos.y;


        if(m_bCursur)
            m_pInputWindow->Set_Text(L"Cursur", m_szFont, L"|", vCursurPos, Vec2(m_vTextScale.x * 1.0f, m_vTextScale.y * 1.05f), Vec2(0.0f, 0.0f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        else
            m_pInputWindow->Set_Text(L"Cursur", m_szFont, L"", vCursurPos, m_vTextScale, Vec2(0.0f, 0.0f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
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

HRESULT CChat_Manager::Ready_ChatWindows()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (m_pInputWindow == nullptr)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = L"InputWindow";
        tTextDesc.vSize = Vec2(900.0f, 900.0f);

        m_pInputWindow = dynamic_cast<CTextBox*>(pGameInstance->Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        if (m_pInputWindow == nullptr)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pInputWindow->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pInputWindow->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f + 100.f);
    }

    if (m_pChatWindow == nullptr)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = L"ChatWindow";
        tTextDesc.vSize = Vec2(900.0f, 900.0f);


        m_pChatWindow = dynamic_cast<CTextBox*>(pGameInstance->Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        if (m_pChatWindow == nullptr)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }


        m_pChatWindow->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pChatWindow->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
    }



    Safe_Release(pGameInstance);
    return S_OK;
}

void CChat_Manager::Release_ChatWindows()
{
    Safe_Release(m_pChatWindow);
    Safe_Release(m_pInputWindow);
}



void CChat_Manager::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
