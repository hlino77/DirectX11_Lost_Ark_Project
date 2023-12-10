#include "Key_Manager.h"


IMPLEMENT_SINGLETON(CKey_Manager);


int g_arrVK[(int)KEY::TYPE_END] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_TAB,
	VK_OEM_4,
	VK_OEM_6,
	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9,
	VK_INSERT, VK_DELETE, VK_HOME, VK_END, VK_PRIOR, VK_NEXT,
	'1', '2', '3', '4', '5', '6', '7', '8', '9',
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,
	VK_BACK,

	VK_LBUTTON,
	VK_RBUTTON
};


CKey_Manager::CKey_Manager()
{
}


HRESULT CKey_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pContext = pContext;
	Safe_AddRef(m_pContext);

	m_hWnd = hWnd;

	m_vecKey.reserve(_uint(KEY::TYPE_END));
	for (_uint i = 0; i < (_uint)KEY::TYPE_END; ++i)
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });


	ZeroMemory(&m_tMousePos, sizeof(POINT));


	return S_OK;
}

void CKey_Manager::Tick(_float fTimeDelta)
{
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::TYPE_END; ++i)
		{
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPushed)
					m_vecKey[i].eState = KEY_STATE::HOLD;
				else
					m_vecKey[i].eState = KEY_STATE::TAP;

				m_vecKey[i].bPrevPushed = true;
			}
			else
			{
				if (m_vecKey[i].bPrevPushed)
					m_vecKey[i].eState = KEY_STATE::AWAY;
				else
					m_vecKey[i].eState = KEY_STATE::NONE;

				m_vecKey[i].bPrevPushed = false;
			}
		}
	}

	else
	{
		for (int i = 0; i < (int)KEY::TYPE_END; ++i)
		{
			m_vecKey[i].bPrevPushed = false;

			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
		}
	}

	GetCursorPos(&m_tMousePos);
	ScreenToClient(m_hWnd, &m_tMousePos);
}




void CKey_Manager::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
