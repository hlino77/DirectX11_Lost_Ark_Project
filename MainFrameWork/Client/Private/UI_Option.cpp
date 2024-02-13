#include "stdafx.h"
#include "UI_Option.h"
#include "GameInstance.h"
#include "UI_OptionWnd.h"
#include "UI_Option_Video.h"
#include "UI_Option_Sound.h"
#include "UI_OptionWnd.h"

CUI_Option::CUI_Option(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_Option::CUI_Option(const CUI& rhs)
	: CUI(rhs)
{
}


HRESULT CUI_Option::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Option::Initialize(void* pArg)
{
	m_strUITag = TEXT("UI_Option");

	if (FAILED(UI_Set()))
		return E_FAIL;

	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Active(false);
	}

	return S_OK;
}

void CUI_Option::Tick(_float fTimeDelta)
{
	if (!m_bOption)
		Set_Player_Control(true);
	Option_OnOff();
	if(m_bOption)
		Update_OptionWnd(fTimeDelta);
}

void CUI_Option::LateTick(_float fTimeDelta)
{
	if(m_bOption)
		LateUpdate_OptionWnd(fTimeDelta);
}

HRESULT CUI_Option::Render()
{
	return S_OK;
}

HRESULT CUI_Option::UI_Set()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CUI* pUI = nullptr;

	pUI = static_cast<CUI*>(pInstance->Add_GameObject(LEVEL_STATIC
		, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_UI_OptionWnd")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_pOptionWnd = static_cast<CUI_OptionWnd*>(pUI);
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(pInstance->Add_GameObject(LEVEL_STATIC
		, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_UI_OptionVideo"), m_pOptionWnd));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_pOptionVideo = static_cast<CUI_Option_Video*>(pUI);
		m_pOptionVideo->Set_Active_Option(true);
		m_vecUIParts.push_back(m_pOptionVideo);
	}

	pUI = static_cast<CUI*>(pInstance->Add_GameObject(LEVEL_STATIC
		, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_UI_OptionSound"), m_pOptionWnd));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_pOptionSound = static_cast<CUI_Option_Sound*>(pUI);
		m_pOptionSound->Set_Active_Option(true);
		m_vecUIParts.push_back(m_pOptionSound);
	}

	return S_OK;
}

void CUI_Option::Update_OptionWnd(_float fTimeDelta)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;

	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort);

	m_pOptionWnd->Update_Buttons(fTimeDelta, pt);

	if (0 == m_pOptionWnd->Get_OptionMode())
	{
		m_pOptionVideo->Is_VideoOptionMode(0);
		m_pOptionSound->Is_SoundOptionMode(0);
		m_pOptionVideo->Update_OptionVideo(fTimeDelta, m_pOptionWnd, pt);
	}
	else if (1 == m_pOptionWnd->Get_OptionMode())
	{
		m_pOptionVideo->Is_VideoOptionMode(1);
		m_pOptionSound->Is_SoundOptionMode(1);
		m_pOptionSound->Update_OptionSound(fTimeDelta, m_pOptionWnd, pt);
	}		
}

void CUI_Option::LateUpdate_OptionWnd(_float fTimeDelta)
{
	if (m_pOptionWnd->Get_ApplyButton())
	{
		m_pOptionVideo->Apply_Option();
		m_pOptionSound->Apply_Option();
		m_pOptionWnd->Set_ApplyButton(false);
	}

	if (m_pOptionWnd->Get_CancleButton())
	{
		m_pOptionVideo->Cancle_Option();
		m_pOptionSound->Cancle_Option();
		m_pOptionWnd->Set_CancleButton(false);
	}

	if (m_pOptionWnd->Get_QuitButton())
	{
		m_pOptionVideo->Cancle_Option();
		m_pOptionSound->Cancle_Option();
		for (auto & iter : m_vecUIParts)
		{
			iter->Set_Active(false);
		}
		m_pOptionWnd->Set_QuitButton(false);
		m_bOption = false;
	}
}

_float CUI_Option::Get_ChannelSound(_uint iChannel)
{
	return m_pOptionSound->Get_ChannelSound(iChannel);
}

void CUI_Option::Set_LevelCustomOption()
{
	m_pOptionVideo->Set_LevelCustomOption();
}

HRESULT CUI_Option::Ready_Components()
{
	return S_OK;
}

HRESULT CUI_Option::Bind_ShaderResources()
{
	return S_OK;
}

void CUI_Option::Option_OnOff()
{
	if (KEY_TAP(KEY::ESC))
	{
		m_bOption = !m_bOption;
		if (m_bOption)
		{
			m_pOptionWnd->Set_Active(true);
			m_pOptionVideo->Set_Active_Option(true);
		}
		if (!m_bOption)
		{
			m_pOptionVideo->Cancle_Option();
			m_pOptionSound->Cancle_Option();
			for (auto& iter : m_vecUIParts)
			{
				iter->Set_Active(false);
			}
			m_pOptionWnd->Set_QuitButton(false);
			m_pOptionWnd->Set_CancleButton(false);
		}
	}
}

CUI_Option* CUI_Option::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Option* pInstance = new CUI_Option(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Option");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Option::Clone(void* pArg)
{
	CUI_Option* pInstance = new CUI_Option(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Option");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Option::Free()
{
	__super::Free();

	m_pOptionWnd->Set_Dead(true);
	m_pOptionVideo->Set_Dead(true);

}
