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
    
    m_fChatWinSizeX = 397.0f;
    m_fInputWindowSizeX = 357.0f;

    m_szFont = L"던파연마된칼날";

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
    if (m_InputTextObject.empty() == false)
        InputTextObject_WndProcHandler(hwnd, uMsg, wParam, lParam);

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

LRESULT CChat_Manager::InputTextObject_WndProcHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HIMC hIMC = ImmGetContext(hwnd);
    _uint iLanguage = ImmGetOpenStatus(hIMC);

    for (auto& InputTextObject : m_InputTextObject)
    {
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
                    InputTextObject.second.szText += szInput;
                }
                else if (szInput == L"\b")
                {
                    if (InputTextObject.second.szCombineText.length() == 0)
                    {
                        if(InputTextObject.second.szText.length() > 0)
                            InputTextObject.second.szText.pop_back();
                    }
                }
                else if (szInput == L"\t")
                {
                }
                else
                {
                    InputTextObject.second.szText += szInput;
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
                    InputTextObject.second.szCombineText = wstring(resultString, resultLength - 1);
            }
            break;
        }
        case WM_IME_ENDCOMPOSITION:
        {
            InputTextObject.second.szCombineText.clear();
            break;
        }
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

void CChat_Manager::Add_Chat(_uint iPlayerID, _uint iLevel, wstring& szChat)
{
    WRITE_LOCK
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if(CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID() == iPlayerID)
        CServerSessionManager::GetInstance()->Get_Player()->Show_SpeechBuble(szChat);
    else
    {
        CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_GameObejct(iLevel, (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID));
        if (pPlayer == nullptr)
        {
            Safe_Release(pGameInstance);
            return;
        }

        pPlayer->Show_SpeechBuble(szChat);
    }

    wstring szLine;

    for (_uint i = 0; i < szChat.length(); ++i)
    {
        szLine += szChat[i];

        Vec2 vMeasure = pGameInstance->MeasureString(m_szFont, szLine);
        if (vMeasure.x * m_vTextScale.x > m_fChatWinSizeX)
        {
            szLine.pop_back();
            m_ChatList.push_front(szLine);
            szLine.clear();
            --i;
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

    Update_ChatWindow();

    Safe_Release(pGameInstance);
}

void CChat_Manager::Send_Chat(const wstring& szChat)
{
    Protocol::S_CHAT pkt;

    wstring szSendChatW = CServerSessionManager::GetInstance()->Get_Player()->Get_NickName() + L" : " + szChat;

    string szSendChat = CAsUtils::W2S(szSendChatW);

    pkt.set_szchat(szSendChat);
    pkt.set_iobjectid(CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID());
    pkt.set_ilevel(CServerSessionManager::GetInstance()->Get_Player()->Get_CurrLevel());

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

    Vec2 vTextPos(0.f, 20.0f);
    

    wstring szDrawText = m_szInputText + m_szCombiningText;
    wstring szMeasureText = szDrawText + L".";
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, szMeasureText);
    Vec2 vOrigin(0.0f, vMeasure.y * 0.5f);
    vMeasure *= m_vTextScale;


    {
        if (vMeasure.x > m_fInputWindowSizeX)
        {
            vTextPos.x -= vMeasure.x - m_fInputWindowSizeX;
        }

        m_pInputWindow->Set_Text(L"InputText", m_szFont, szDrawText, vTextPos, m_vTextScale, vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }


    {
        Vec2 vCursurPos;

        vCursurPos.x = vTextPos.x + vMeasure.x - 7.0f;
        vCursurPos.y = vTextPos.y;

        if(m_bCursur)
            m_pInputWindow->Set_Text(L"Cursur", m_szFont, L"|", vCursurPos, Vec2(m_vTextScale.x * 1.0f, m_vTextScale.y * 1.05f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        else
            m_pInputWindow->Set_Text(L"Cursur", m_szFont, L"", vCursurPos, m_vTextScale, vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void CChat_Manager::Update_ChatWindow()
{
    Vec2 vTextPos(0.f, 192.0f);

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
        vCursurPos.y -= vMeasure.y * m_vTextScale.y;

        m_pChatWindow->Set_Text(L"ChatWindow", m_szFont, szResult, vCursurPos, m_vTextScale, Vec2(0.0f, 0.0f), 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
}


HRESULT CChat_Manager::Ready_ChatWindows()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

    if (m_pInputWindow == nullptr)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = L"InputWindow";
        tTextDesc.vSize = Vec2(359.0f, 41.0f);

        m_pInputWindow = dynamic_cast<CTextBox*>(pGameInstance->Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        if (m_pInputWindow == nullptr)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pInputWindow->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pInputWindow->Set_Pos(189.5f, 693.f);
    }
    
    
    if (m_pChatWindow == nullptr)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = L"ChatWindow";
        tTextDesc.vSize = Vec2(399.0f, 192.0f);


        m_pChatWindow = dynamic_cast<CTextBox*>(pGameInstance->Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
        if (m_pChatWindow == nullptr)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }


        m_pChatWindow->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pChatWindow->Set_Pos(210.5f, 571.f);
    }



    Safe_Release(pGameInstance);
    return S_OK;
}

void CChat_Manager::Add_InputTextObject(CGameObject* pObject)
{
    if (m_InputTextObject.find(pObject) == m_InputTextObject.end())
        return;

    m_InputTextObject.emplace(pObject, InputTextObjectDesc());
}

wstring CChat_Manager::Get_InputText(CGameObject* pObject)
{
    return m_InputTextObject[pObject].szText + m_InputTextObject[pObject].szCombineText;
}

void CChat_Manager::Delete_InputTextObject(CGameObject* pObject)
{
    if (m_InputTextObject.find(pObject) == m_InputTextObject.end())
        return;

    m_InputTextObject.erase(pObject);
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

