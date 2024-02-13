#include "stdafx.h"
#include "UI_OptionWnd.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Sound_Manager.h"

CUI_OptionWnd::CUI_OptionWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_OptionWnd::CUI_OptionWnd(const CUI& rhs)
	: CUI(rhs)
{
}


HRESULT CUI_OptionWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_OptionWnd::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_OptionWnd");

	m_fSizeX = 633.f;
	m_fSizeY = 450.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_QuitBox
	m_pTransform_QuitBox->Set_Scale(Vec3(18.f, 16.f, 1.f));
	m_pTransform_QuitBox->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 300.f) - g_iWinSizeX * 0.5f, -(m_fY - 205.f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_ApplyBox
	m_pTransform_ApplyBox->Set_Scale(Vec3(101.f, 34.f, 1.f));
	m_pTransform_ApplyBox->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 150.f) - g_iWinSizeX * 0.5f, -(m_fY + 200.f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_CancleBox
	m_pTransform_CancleBox->Set_Scale(Vec3(101.f, 34.f, 1.f));
	m_pTransform_CancleBox->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 258.f) - g_iWinSizeX * 0.5f, -(m_fY + 200.f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_OptionTag
	for (size_t i = 0; i < 2; i++)
	{
		m_pTransform_OptionTag[i]->Set_Scale(Vec3(125.f, 30.f, 1.f));
		m_pTransform_OptionTag[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX - 241.f) - g_iWinSizeX * 0.5f, -(m_fY - 132.f + (35.f * i)) + g_iWinSizeY * 0.5f, 0.f));
	}
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_iOptionMode = 0;
	m_iTextureIndex_OptionTag[0] = 2;
	m_iTextureIndex_OptionTag[1] = 0;
	m_strNowOption = TEXT("ºñµð¿À");

	if (FAILED(Initialize_TextBox()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_OptionWnd::Initialize_TextBox()
{

	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	m_strOptionTag[0] = TEXT("ºñµð¿À");
	m_strOptionTag[1] = TEXT("¿Àµð¿À");
	if (FAILED(Ready_TextBox()))
		return E_FAIL;

	Print_OptionText();
	m_pOptionTextWnd->Set_Active(true);

	return S_OK;
}

void CUI_OptionWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_OptionWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CUI_OptionWnd::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ApplyBox()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_CancleBox()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_QuitBox()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_OptionTagVedio()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_OptionTagSound()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if(nullptr != m_pOptionTextWnd)
		m_pOptionTextWnd->Render();

	return S_OK;
}

void CUI_OptionWnd::Update_Buttons(_float fTimeDelta, POINT pt)
{
	Create_Rect();
	Picking_UI();
	Set_Player_Control();

	Create_ButtonRect();
	Is_Picking_MovingWnd(pt);
	Is_Picking_ApplyButton(pt);
	Is_Picking_CancleButton(pt);
	Is_Picking_QuitButton(pt);
	Is_Picking_ObjectTag(pt);
	Moving_Wnd(pt);
}

void CUI_OptionWnd::Moving_Wnd(POINT pt)
{
	_float	MouseMoveX;
	MouseMoveX = (_float)CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);

	if (m_bMovingWnd)
	{
		m_fX += MouseMoveX * 0.8f;
		m_fY = pt.y + 204.f;

		//m_pTransformCom
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
		//m_pTransform_QuitBox
		m_pTransform_QuitBox->Set_Scale(Vec3(18.f, 16.f, 1.f));
		m_pTransform_QuitBox->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 300.f) - g_iWinSizeX * 0.5f, -(m_fY - 205.f) + g_iWinSizeY * 0.5f, 0.f));
		//m_pTransform_ApplyBox
		m_pTransform_ApplyBox->Set_Scale(Vec3(101.f, 34.f, 1.f));
		m_pTransform_ApplyBox->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 150.f) - g_iWinSizeX * 0.5f, -(m_fY + 200.f) + g_iWinSizeY * 0.5f, 0.f));
		//m_pTransform_CancleBox
		m_pTransform_CancleBox->Set_Scale(Vec3(101.f, 34.f, 1.f));
		m_pTransform_CancleBox->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 258.f) - g_iWinSizeX * 0.5f, -(m_fY + 200.f) + g_iWinSizeY * 0.5f, 0.f));
		//m_pTransform_OptionTag
		for (size_t i = 0; i < 2; i++)
		{
			m_pTransform_OptionTag[i]->Set_Scale(Vec3(125.f, 30.f, 1.f));
			m_pTransform_OptionTag[i]->Set_State(CTransform::STATE_POSITION,
				Vec3((m_fX - 241.f) - g_iWinSizeX * 0.5f, -(m_fY - 132.f + (35.f * i)) + g_iWinSizeY * 0.5f, 0.f));
		}

		m_pOptionTextWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

		if (KEY_AWAY(KEY::LBTN))
		{
			m_bMovingWnd = false;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
}

void CUI_OptionWnd::Print_OptionText()
{
	if (nullptr == m_pOptionTextWnd)
		return;

	m_pOptionTextWnd->Clear_Text();
	m_pOptionTextWnd->Set_Alpha(1.f);

	m_pOptionTextWnd->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pOptionTextWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", m_strOptionTag[0]);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Video"), m_strFont, m_strOptionTag[0], Vec2(((m_fSizeX * 0.5f) - 270.f), ((m_fSizeY * 0.5f) - 132.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", m_strOptionTag[1]);
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Audio"), m_strFont, m_strOptionTag[1], Vec2(((m_fSizeX * 0.5f) - 270.f), ((m_fSizeY * 0.5f) - 97.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold",TEXT("Àû¿ë"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Apply"), m_strFont, TEXT("Àû¿ë"), Vec2(((m_fSizeX * 0.5f) + 150.f), ((m_fSizeY * 0.5f) + 200.f)), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("Ãë¼Ò"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Cancle"), m_strFont, TEXT("Ãë¼Ò"), Vec2(((m_fSizeX * 0.5f) + 258.f), ((m_fSizeY * 0.5f) + 200.f)), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"´øÆÄ¿¬¸¶µÈÄ®³¯", m_strNowOption);
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("NowOption"), TEXT("´øÆÄ¿¬¸¶µÈÄ®³¯"), m_strNowOption, Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) - 160.f)), Vec2(0.5f, 0.5f), vOrigin, 0.f, Vec4(0.84f, 0.68f, 0.35f, 1.0f));

}

void CUI_OptionWnd::Is_Picking_MovingWnd(POINT pt)
{
	if (PtInRect(&m_rcMovingRect, pt))
	{
		if (KEY_HOLD(KEY::LBTN))
			m_bMovingWnd = true;
	}
	else
	{
		m_bMovingWnd = false;
	}
}

void CUI_OptionWnd::Is_Picking_ApplyButton(POINT pt)
{
	if (PtInRect(&m_rcApplyButton, pt))
	{
		m_iTextureIndex_Button[0] = 1;
		if (!m_bSound[0])
		{
			m_bSound[0] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickngSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_TAP(KEY::LBTN))
		{
			m_iTextureIndex_Button[0] = 2;
			m_bApplyButton = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		m_iTextureIndex_Button[0]= 0;
		m_bSound[0] = false;
	}
}

void CUI_OptionWnd::Is_Picking_CancleButton(POINT pt)
{
	if (PtInRect(&m_rcCancleButton, pt))
	{
		if (!m_bSound[1])
		{
			m_bSound[1] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickngSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		m_iTextureIndex_Button[1] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_iTextureIndex_Button[1] = 2;
			m_bCancleButton = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		m_iTextureIndex_Button[1] = 0;
		m_bSound[1] = false;
	}
}

void CUI_OptionWnd::Is_Picking_QuitButton(POINT pt)
{
	if (PtInRect(&m_rcQuitButton, pt))
	{
		if (!m_bSound[2])
		{
			m_bSound[2] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickngSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		m_iTextureIndex_Button[2] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bQuitButton = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		m_iTextureIndex_Button[2] = 0;
		m_bSound[2] = false;
	}
}

void CUI_OptionWnd::Is_Picking_ObjectTag(POINT pt)
{
	if (PtInRect(&m_rcObjectTag[0], pt))
	{
		if (0 != m_iOptionMode)
			m_iTextureIndex_OptionTag[0] = 1;
		if (!m_bSound[3])
		{
			m_bSound[3] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickngSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_TAP(KEY::LBTN))
		{
			m_iOptionMode = 0;
			m_iTextureIndex_OptionTag[0] = 2;
			m_iTextureIndex_OptionTag[1] = 0;
			m_strNowOption = TEXT("ºñµð¿À");
			Print_OptionText();
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		if (0 != m_iOptionMode)
			m_iTextureIndex_OptionTag[0] = 0;
		m_bSound[3] = false;
	}
	if (PtInRect(&m_rcObjectTag[1], pt))
	{
		if (1 != m_iOptionMode)
			m_iTextureIndex_OptionTag[1] = 1;
		if (!m_bSound[4])
		{
			m_bSound[4] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickngSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_TAP(KEY::LBTN))
		{
			m_iOptionMode = 1;
			m_iTextureIndex_OptionTag[0] = 0;
			m_iTextureIndex_OptionTag[1] = 2;
			m_strNowOption = TEXT("¿Àµð¿À");
			Print_OptionText();
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		if (1 != m_iOptionMode)
			m_iTextureIndex_OptionTag[1] = 0;
		m_bSound[4] = false;
	}
}

void CUI_OptionWnd::Create_ButtonRect()
{
	//m_rcMovingRect
	m_rcMovingRect.left = LONG((m_fX - 12.f) - (583.f / 2));
	m_rcMovingRect.top = LONG((m_fY - 204.f) - (34.f / 2));
	m_rcMovingRect.right = LONG((m_fX - 12.f) + (583.f / 2));
	m_rcMovingRect.bottom = LONG((m_fY - 204.f) + (34.f / 2));
	//m_rcApplyButton
	m_rcApplyButton.left = LONG((m_fX + 150.f) - (101.f / 2));
	m_rcApplyButton.top = LONG((m_fY + 200.f) - (34.f / 2));
	m_rcApplyButton.right = LONG((m_fX + 150.f) + (101.f / 2));
	m_rcApplyButton.bottom = LONG((m_fY + 200.f) + (34.f / 2));
	//m_rcCancleButton
	m_rcCancleButton.left = LONG((m_fX + 258.f) - (101.f / 2));
	m_rcCancleButton.top = LONG((m_fY + 200.f) - (34.f / 2));
	m_rcCancleButton.right = LONG((m_fX + 258.f) + (101.f / 2));
	m_rcCancleButton.bottom = LONG((m_fY + 200.f) + (34.f / 2));
	//m_rcQuitButton
	m_rcQuitButton.left = LONG((m_fX + 300.f) - (18.f / 2));
	m_rcQuitButton.top = LONG((m_fY - 205.f) - (16.f / 2));
	m_rcQuitButton.right = LONG((m_fX + 300.f) + (18.f / 2));
	m_rcQuitButton.bottom = LONG((m_fY - 205.f) + (16.f / 2));
	//m_rcObjectTag
	for (size_t i = 0; i < 2; i++)
	{
		m_rcObjectTag[i].left = LONG((m_fX - 241.f) - (125.f / 2));
		m_rcObjectTag[i].top = LONG((m_fY - 132.f + (35.f * i)) - (30.f / 2));
		m_rcObjectTag[i].right = LONG((m_fX - 241.f) + (125.f / 2));
		m_rcObjectTag[i].bottom = LONG((m_fY - 132.f + (35.f * i)) + (30.f / 2));
	}
}

HRESULT CUI_OptionWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/*m_pTextureCom*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_OptionWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/*m_pTexture_Button*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Button"),
		TEXT("Com_Texture_Button"), (CComponent**)&m_pTexture_Button)))
		return E_FAIL;
	/*m_pTexture_QuitBox*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Quit_Option"),
		TEXT("Com_Texture_QuitBox"), (CComponent**)&m_pTexture_QuitBox)))
		return E_FAIL;
	/*m_pTexture_OptionTag*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_OptionTag"),
		TEXT("Com_Texture_OptionTag"), (CComponent**)&m_pTexture_OptionTag)))
		return E_FAIL;

	/*m_pTransform_QuitBox*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_QuitkBox"), (CComponent**)&m_pTransform_QuitBox)))
		return E_FAIL;
	/*m_pTransform_ApplyBox*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ApplyBox"), (CComponent**)&m_pTransform_ApplyBox)))
		return E_FAIL;
	/*m_pTransform_CancleBox*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_CancleBox"), (CComponent**)&m_pTransform_CancleBox)))
		return E_FAIL;
	/*m_pTransform_OptionTag*/
	for (size_t i = 0; i < 2; i++)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_OptionTag") + to_wstring(i), (CComponent**)&m_pTransform_OptionTag[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_OptionWnd::Ready_TextBox()
{
	if (nullptr == m_pOptionTextWnd)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("s_Option");
		m_strWndTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(m_fSizeX, m_fSizeY);
		m_pOptionTextWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pOptionTextWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pOptionTextWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pOptionTextWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		m_pOptionTextWnd->Set_Render(false);

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CUI_OptionWnd::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_OptionWnd::Bind_ShaderResources_ApplyBox()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ApplyBox->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_Button[0]);

	return S_OK;
}

HRESULT CUI_OptionWnd::Bind_ShaderResources_CancleBox()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_CancleBox->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_Button[1]);
	return S_OK;
}

HRESULT CUI_OptionWnd::Bind_ShaderResources_QuitBox()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_QuitBox->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_QuitBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_Button[2]);
	return S_OK;
}

HRESULT CUI_OptionWnd::Bind_ShaderResources_OptionTagVedio()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionTag[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_OptionTag->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_OptionTag[0]);
	return S_OK;
}

HRESULT CUI_OptionWnd::Bind_ShaderResources_OptionTagSound()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionTag[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_OptionTag->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex_OptionTag[1]);
	return S_OK;
}

CUI_OptionWnd* CUI_OptionWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_OptionWnd* pInstance = new CUI_OptionWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_OptionWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_OptionWnd::Clone(void* pArg)
{
	CUI_OptionWnd* pInstance = new CUI_OptionWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_OptionWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_OptionWnd::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_Button);
	Safe_Release(m_pTexture_QuitBox);
	Safe_Release(m_pTexture_OptionTag);

	Safe_Release(m_pTransform_QuitBox);
	Safe_Release(m_pTransform_ApplyBox);
	Safe_Release(m_pTransform_CancleBox);

	for (size_t i = 0; i < 2; i++)
	{
		Safe_Release(m_pTransform_OptionTag[i]);
	}

	if(nullptr != m_pOptionTextWnd)
		m_pOptionTextWnd->Set_Dead(true);
}
