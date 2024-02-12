#include "stdafx.h"
#include "UI_Option_Sound.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Sound_Manager.h"

CUI_Option_Sound::CUI_Option_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_Option_Sound::CUI_Option_Sound(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Option_Sound::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Option_Sound::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_SoundOption");

	if (nullptr != pArg)
	{
		m_fSizeX = static_cast<CUI*>(pArg)->Get_UIDesc().fSizeX;
		m_fSizeY = static_cast<CUI*>(pArg)->Get_UIDesc().fSizeY;
		m_fX = static_cast<CUI*>(pArg)->Get_UIDesc().fX;
		m_fY = static_cast<CUI*>(pArg)->Get_UIDesc().fY;
	}
	else
	{
		m_fSizeX = 633.f;
		m_fSizeY = 450.f;
		m_fX = g_iWinSizeX * 0.5f;
		m_fY = g_iWinSizeY * 0.5f;
	}

	m_pTransform_CheckBox[0]->Set_Scale(Vec3(21.f, 23.f, 1.f));
	m_pTransform_CheckBox[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f + (238.f * 0.9f)) - g_iWinSizeX * 0.5f, -(m_fY - 87.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragLineSizeX = 238.f;
	m_fDragLineMinX = (m_fX + 62.f) - 119.f;
	m_fDragLineMaxX = (m_fX + 62.f) + 119.f;

	m_pTransform_DragLine[0]->Set_Scale(Vec3(m_fDragLineSizeX, 7.f, 1.f));
	m_pTransform_DragLine[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 87.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragBarX[0] = (m_fX + 62.f) - (m_fDragLineSizeX * 0.5f);
	m_pTransform_DragBar[0]->Set_Scale(Vec3(17.f * 0.8f, 32.f * 0.8f, 1.f));
	m_pTransform_DragBar[0]->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fDragBarX[0] - g_iWinSizeX * 0.5f, -(m_fY - 87.f) + g_iWinSizeY * 0.5f, 0.f));

	//m_pTransform_Line
	m_pTransform_Line->Set_Scale(Vec3(282.f * 1.62f, 7.f, 1.f));
	m_pTransform_Line->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 87.f + 38.f) + g_iWinSizeY * 0.5f, 0.f));

	for (size_t i = 1; i < 5; i++)
	{
		m_pTransform_CheckBox[i]->Set_Scale(Vec3(21.f, 23.f, 1.f));
		m_pTransform_CheckBox[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 62.f + (m_fDragLineSizeX * 0.6f)) - g_iWinSizeX * 0.5f, -(m_fY - 87.f + ((i + 1) * 38.f)) + g_iWinSizeY * 0.5f, 0.f));
		m_pTransform_DragLine[i]->Set_Scale(Vec3(m_fDragLineSizeX, 7.f, 1.f));
		m_pTransform_DragLine[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 87.f + ((i + 1) * 38.f)) + g_iWinSizeY * 0.5f, 0.f));
		m_fDragBarX[i] = (m_fX + 62.f) - (m_fDragLineSizeX * 0.5f);
		m_pTransform_DragBar[i]->Set_Scale(Vec3(17.f * 0.8f, 32.f * 0.8f, 1.f));
		m_pTransform_DragBar[i]->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fDragBarX[i] - g_iWinSizeX * 0.5f, -(m_fY - 87.f + ((i + 1) * 38.f)) + g_iWinSizeY * 0.5f, 0.f));
	}
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_TextBox()))
		return E_FAIL;

	m_fSoundRatio[0] = 0.5f;
	m_fSoundRatio[1] = CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("BGM"));
	m_fSoundRatio[2] = CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI"));
	m_fSoundRatio[3] = CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("Ambient"));
	m_fSoundRatio[4] = CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("Effect"));
	for (size_t i = 0; i < 5; i++)
	{
		m_fVolume[i] = m_fSoundRatio[i];
		m_fDragBarX[i] = m_fDragLineMinX + (m_fDragLineSizeX* m_fSoundRatio[i]);
	}

	return S_OK;
}

HRESULT CUI_Option_Sound::Initialize_TextBox()
{
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	if (FAILED(Ready_TextBox()))
		return E_FAIL;
	m_pTextBox->Set_Active(false);

	return S_OK;
}

void CUI_Option_Sound::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Option_Sound::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Option_Sound::Render()
{
	if (FAILED(Bind_ShaderResources_ChckBoxMaster()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ChckBoxBGM()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ChckBoxUI()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ChckBoxBACKGROUND()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ChckBoxEFFECT()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragLineMaster()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragLineBGM()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragLineUI()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragLineBACKGROUND()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragLineEFFECT()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Line()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragBarMaster()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragBarBGM()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragBarUI()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragBarBACKGROUND()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DragBarEFFECT()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pTextBox->Render();

	return S_OK;
}

_float CUI_Option_Sound::Get_ChannelSound(_uint iChannel)
{
	if (false == m_bCheckVolume[0])
	{
		return 0.f;
	}
	else
	{
		if (true == m_bCheckVolume[iChannel + 1])
			return m_fVolume[iChannel + 1];
		else
			return 0.f;
	}
}

void CUI_Option_Sound::Is_SoundOptionMode(_uint iMode)
{
	if (0 == iMode)
	{
		if (nullptr != m_pTextBox)
			m_pTextBox->Set_Active(false);
		m_bActive = false;
	}
	if (1 == iMode)
	{
		Print_OptionText();
		if (nullptr != m_pTextBox)
			m_pTextBox->Set_Active(true);
		m_bActive = true;
	}
}

void CUI_Option_Sound::Update_OptionSound(_float fTimeDelta, CUI* pUI, POINT pt)
{
	MovingWnd(pUI, pt.x);
	Create_CheckButton();
	Create_DragBar();
	Is_Picking_CheckButton(pt);
	Is_Picking_DragBar(pt);
	Update_DragBar();
}

void CUI_Option_Sound::Set_Active_Option(_bool bOption)
{
	if (false == bOption)
		return;

	for (size_t i = 0; i < 5; i++)
	{
		m_bPreCheckVolume[i] = m_bCheckVolume[i];
		m_fPreSoundRatio[i] = m_fSoundRatio[i];
		m_fPreDragBarX[i] = m_fDragBarX[i];

		m_fVolume[i] = m_fPreSoundRatio[i];
	}
	if (true == m_bCheckVolume[0])
	{
		if (true == m_bCheckVolume[1])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		if (true == m_bCheckVolume[2])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		if (true == m_bCheckVolume[3])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		if (true == m_bCheckVolume[4])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	else
	{
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	Print_OptionText();
	m_pTextBox->Set_Active(true);
}

void CUI_Option_Sound::Apply_Option()
{
	for (size_t i = 0; i < 5; i++)
	{
		m_bPreCheckVolume[i] = m_bCheckVolume[i];
		m_fPreSoundRatio[i] = m_fSoundRatio[i];
		m_fPreDragBarX[i] = m_fDragBarX[i];

		m_fVolume[i] = m_fPreSoundRatio[i];
	}
	if (true == m_bCheckVolume[0])
	{
		if (true == m_bCheckVolume[1])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		if (true == m_bCheckVolume[2])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		if (true == m_bCheckVolume[3])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		if (true == m_bCheckVolume[4])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	else
	{
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	Print_OptionText();
}

void CUI_Option_Sound::Cancle_Option()
{
	for (size_t i = 0; i < 5; i++)
	{
		m_bCheckVolume[i] = m_bPreCheckVolume[i];
		m_fSoundRatio[i] = m_fPreSoundRatio[i];
		m_fDragBarX[i] = m_fPreDragBarX[i];

		m_fDragBarX[i] = m_fDragLineMinX + (m_fDragLineSizeX * m_fSoundRatio[i]);
		m_fVolume[i] = m_fPreSoundRatio[i];
	}
	if (true == m_bCheckVolume[0])
	{
		if (true == m_bCheckVolume[1])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		if (true == m_bCheckVolume[2])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		if (true == m_bCheckVolume[3])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		if (true == m_bCheckVolume[4])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	else
	{
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	Print_OptionText();
}

void CUI_Option_Sound::Print_OptionText()
{
	if (nullptr == m_pTextBox)
		return;

	m_pTextBox->Clear_Text();
	m_pTextBox->Set_Alpha(1.f);

	m_pTextBox->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("| ¿Àµð¿À ¿É¼Ç"));
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pTextBox->Set_Text(m_strTextWnd + TEXT("Audio_Option"), m_strFont, TEXT("| ¿Àµð¿À ¿É¼Ç"), Vec2(((m_fSizeX * 0.5f) - 128.f), ((m_fSizeY * 0.5f) - 129.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

	{
		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("ÀüÃ¼ À½·®"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("Master"), m_strFont, TEXT("ÀüÃ¼ À½·®"), Vec2(((m_fSizeX * 0.5f) - 128.f), ((m_fSizeY * 0.5f) - 95.f)), Vec2(0.3f, 0.3f), Vec2(0.f,0.f), 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));

		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(_uint(m_fVolume[0] * 100.f)) + TEXT("%"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("Master_Value"), m_strFont, to_wstring(_uint(m_fVolume[0] * 100.f)) + TEXT("%"), Vec2(((m_fSizeX * 0.5f) + 200.f), ((m_fSizeY * 0.5f) - 95.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

	}

	{	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("¹è°æ À½¾Ç"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("BGM"), m_strFont, TEXT("¹è°æ À½¾Ç"), Vec2(((m_fSizeX * 0.5f) - 128.f), ((m_fSizeY * 0.5f) - 20.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));

		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(_uint(m_fVolume[1] * 100.f)) + TEXT("%"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("BGM_Value"), m_strFont, to_wstring(_uint(m_fVolume[1] * 100.f)) + TEXT("%"), Vec2(((m_fSizeX * 0.5f) + 200.f), ((m_fSizeY * 0.5f) - 20.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));
	}

	{
		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("ÀÎÅÍÆäÀÌ½º"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("UI"), m_strFont, TEXT("ÀÎÅÍÆäÀÌ½º"), Vec2(((m_fSizeX * 0.5f) - 128.f), ((m_fSizeY * 0.5f) + 20.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));

		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(_uint(m_fVolume[2] * 100.f)) + TEXT("%"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("UI_Value"), m_strFont, to_wstring(_uint(m_fVolume[2] * 100.f)) + TEXT("%"), Vec2(((m_fSizeX * 0.5f) + 200.f), ((m_fSizeY * 0.5f) + 20.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));
	}

	{
		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("È¯°æÀ½"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("BackGround"), m_strFont, TEXT("È¯°æÀ½"), Vec2(((m_fSizeX * 0.5f) - 128.f), ((m_fSizeY * 0.5f) + 60.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));

		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(_uint(m_fVolume[3] * 100.f)) + TEXT("%"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("BackGround_Value"), m_strFont, to_wstring(_uint(m_fVolume[3] * 100.f)) + TEXT("%"), Vec2(((m_fSizeX * 0.5f) + 200.f), ((m_fSizeY * 0.5f) + 60.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));
	}

	{
		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("È¿°úÀ½"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("Effect"), m_strFont, TEXT("È¿°úÀ½"), Vec2(((m_fSizeX * 0.5f) - 128.f), ((m_fSizeY * 0.5f) + 100.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(1.f, 1.f, 1.f, 1.0f));

		vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(_uint(m_fVolume[4] * 100.f)) + TEXT("%"));
		vOrigin = vMeasure * 0.5f;
		m_pTextBox->Set_Text(m_strTextWnd + TEXT("Effect_Value"), m_strFont, to_wstring(_uint(m_fVolume[4] * 100.f)) + TEXT("%"), Vec2(((m_fSizeX * 0.5f) + 200.f), ((m_fSizeY * 0.5f) + 100.f)), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));
	}
}

HRESULT CUI_Option_Sound::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTexture_OptionCheckBox
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_CheckBox"),
		TEXT("Com_Texture_OptionCheckBox"), (CComponent**)&m_pTexture_OptionCheckBox)))
		return E_FAIL;
	//m_pTexture_OptionDetailButton
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DetailButton"),
		TEXT("Com_Texture_OptionDetailButton"), (CComponent**)&m_pTexture_OptionDetailButton)))
		return E_FAIL;
	//m_pTexture_Line
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Line"),
		TEXT("Com_Texture_Line"), (CComponent**)&m_pTexture_Line)))
		return E_FAIL;
	//m_pTexture_OptionDragLine
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DragLine"),
		TEXT("Com_Texture_OptionDragLine"), (CComponent**)&m_pTexture_OptionDragLine)))
		return E_FAIL;
	//m_pTexture_ValueTextWnd
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_ValueTextWnd"),
		TEXT("Com_Texture_ValueTextWnd"), (CComponent**)&m_pTexture_ValueTextWnd)))
		return E_FAIL;
	//m_pTexture_DragBar
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DragBar"),
		TEXT("Com_Texture_DragBar"), (CComponent**)&m_pTexture_DragBar)))
		return E_FAIL;

	//m_pTransform_Line
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Line"), (CComponent**)&m_pTransform_Line)))
		return E_FAIL;

	for (size_t i = 0; i < 5; i++)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Texture_CheckBox") + to_wstring(i), (CComponent**)&m_pTransform_CheckBox[i])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Texture_DragLine") + to_wstring(i), (CComponent**)&m_pTransform_DragLine[i])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Texture_DragBar") + to_wstring(i), (CComponent**)&m_pTransform_DragBar[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Option_Sound::Ready_TextBox()
{
	if (nullptr == m_pTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("s_SoundOption");
		m_strTextWnd = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(m_fSizeX, m_fSizeY);
		m_pTextBox = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pTextBox)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pTextBox->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pTextBox->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		m_pTextBox->Set_Render(false);

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_ChckBoxMaster()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_CheckBox[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex[0]);
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_Line()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Line->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Line->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_ChckBoxBGM()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_CheckBox[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex[1]);
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_ChckBoxUI()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_CheckBox[2]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex[2]);
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_ChckBoxBACKGROUND()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_CheckBox[3]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex[3]);
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_ChckBoxEFFECT()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_CheckBox[4]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex[4]);
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragLineMaster()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragLine[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionDragLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragLineBGM()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragLine[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionDragLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragLineUI()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragLine[2]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionDragLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragLineBACKGROUND()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragLine[3]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionDragLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragLineEFFECT()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragLine[4]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionDragLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragBarMaster()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragBar[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_DragBar->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragBarBGM()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragBar[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_DragBar->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragBarUI()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragBar[2]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_DragBar->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragBarBACKGROUND()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragBar[3]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_DragBar->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources_DragBarEFFECT()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DragBar[4]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_DragBar->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}


void CUI_Option_Sound::Create_CheckButton()
{
	m_rcCheckBox[0].left = LONG((m_fX + 62.f + (238.f * 0.9f)) - (21.f / 2));
	m_rcCheckBox[0].top = LONG((m_fY - 87.f) - (23.f / 2));
	m_rcCheckBox[0].right = LONG((m_fX + 62.f + (238.f * 0.9f)) + (21.f / 2));
	m_rcCheckBox[0].bottom = LONG((m_fY - 87.f) + (23.f / 2));

	for (size_t i = 1; i < 5; i++)
	{
		m_rcCheckBox[i].left = LONG((m_fX + 62.f + (238.f * 0.9f)) - (21.f / 2));
		m_rcCheckBox[i].top = LONG((m_fY - 87.f + ((i + 1) * 38.f)) - (23.f / 2));
		m_rcCheckBox[i].right = LONG((m_fX + 62.f + (238.f * 0.9f)) + (21.f / 2));
		m_rcCheckBox[i].bottom = LONG((m_fY - 87.f + ((i + 1) * 38.f)) + (23.f / 2));
	}
}

void CUI_Option_Sound::Is_Picking_CheckButton(POINT pt)
{
	Is_Picking_CheckButton_Master(pt);
	Is_Picking_CheckButton_BGM(pt);
	Is_Picking_CheckButton_UI(pt);
	Is_Picking_CheckButton_BACKGROUND(pt);
	Is_Picking_CheckButton_EFFECT(pt);
}

void CUI_Option_Sound::Is_Picking_CheckButton_Master(POINT pt)
{
	if (m_bCheckVolume[0])
		m_iTextureIndex[0] = 2;
	else
		m_iTextureIndex[0] = 0;

	if (PtInRect(&m_rcCheckBox[0], pt))
	{

		if (2 != m_iTextureIndex[0])
			m_iTextureIndex[0] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bCheckVolume[0] = !m_bCheckVolume[0];
			if (false == m_bCheckVolume[0])
			{
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
			}
			else
			{
				if (true == m_bCheckVolume[1])
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
				else
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
				if (true == m_bCheckVolume[2])
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
				else
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
				if (true == m_bCheckVolume[3])
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
				else
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
				if (true == m_bCheckVolume[4])
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
				else
					CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
			}
		}
	}
	else
	{
		if (2 != m_iTextureIndex[0])
			m_iTextureIndex[0] = 0;
	}
}

void CUI_Option_Sound::Is_Picking_CheckButton_BGM(POINT pt)
{
	if (m_bCheckVolume[1])
		m_iTextureIndex[1] = 2;
	else
		m_iTextureIndex[1] = 0;

	if (PtInRect(&m_rcCheckBox[1], pt))
	{

		if (2 != m_iTextureIndex[1])
			m_iTextureIndex[1] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bCheckVolume[1] = !m_bCheckVolume[1];
			if (false == m_bCheckVolume[1])
			{
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
			}
			else
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
		}
	}
	else
	{
		if (2 != m_iTextureIndex[1])
			m_iTextureIndex[1] = 0;
	}
}

void CUI_Option_Sound::Is_Picking_CheckButton_UI(POINT pt)
{
	if (m_bCheckVolume[2])
		m_iTextureIndex[2] = 2;
	else
		m_iTextureIndex[2] = 0;

	if (PtInRect(&m_rcCheckBox[2], pt))
	{

		if (2 != m_iTextureIndex[2])
			m_iTextureIndex[2] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bCheckVolume[2] = !m_bCheckVolume[2];
			if (false == m_bCheckVolume[2])
			{
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
			}
			else
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
		}
	}
	else
	{
		if (2 != m_iTextureIndex[2])
			m_iTextureIndex[2] = 0;
	}
}

void CUI_Option_Sound::Is_Picking_CheckButton_BACKGROUND(POINT pt)
{
	if (m_bCheckVolume[3])
		m_iTextureIndex[3] = 2;
	else
		m_iTextureIndex[3] = 0;

	if (PtInRect(&m_rcCheckBox[3], pt))
	{

		if (2 != m_iTextureIndex[3])
			m_iTextureIndex[3] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bCheckVolume[3] = !m_bCheckVolume[3];
			if (false == m_bCheckVolume[3])
			{
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
			}
			else
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
		}
	}
	else
	{
		if (2 != m_iTextureIndex[3])
			m_iTextureIndex[3] = 0;
	}
}

void CUI_Option_Sound::Is_Picking_CheckButton_EFFECT(POINT pt)
{
	if (m_bCheckVolume[4])
		m_iTextureIndex[4] = 2;
	else
		m_iTextureIndex[4] = 0;

	if (PtInRect(&m_rcCheckBox[4], pt))
	{
		if (2 != m_iTextureIndex[4])
			m_iTextureIndex[4] = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bCheckVolume[4] = !m_bCheckVolume[4];
			if (false == m_bCheckVolume[4])
			{
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
			}
			else
				CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
		}
	}
	else
	{
		if (2 != m_iTextureIndex[4])
			m_iTextureIndex[4] = 0;
	}
}

void CUI_Option_Sound::Update_DragBar()
{
	_float	MouseMoveX;
	MouseMoveX = (_float)CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);

	Create_DragBar();
	if (m_bHolding[0])
	{
		for (size_t i = 0; i < 5; i++)
		{
			if ((m_fDragLineMinX <= m_fDragBarX[i]) && (m_fDragLineMaxX >= m_fDragBarX[i]))
			{
				m_fDragBarX[i] += MouseMoveX * 0.5f;
			}
			if (m_fDragLineMinX > m_fDragBarX[i])
			{
				m_fDragBarX[i] = m_fDragLineMinX;
			}
			if (m_fDragLineMaxX < m_fDragBarX[i])
			{
				m_fDragBarX[i] = m_fDragLineMaxX;
			}
		}
		
		Print_OptionText();
		for (size_t i = 0; i < 5; i++)
		{
			m_fSoundRatio[i] = (m_fDragBarX[i] - m_fDragLineMinX) / m_fDragLineSizeX;
			m_fVolume[i] = m_fSoundRatio[i];
		}
		if (true == m_bCheckVolume[1])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
		if (true == m_bCheckVolume[2])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
		if (true == m_bCheckVolume[3])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
		if (true == m_bCheckVolume[4])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}
	else if (m_bHolding[1])
	{
		if ((m_fDragLineMinX <= m_fDragBarX[1]) && (m_fDragLineMaxX >= m_fDragBarX[1]))
			m_fDragBarX[1] += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX[1])
			m_fDragBarX[1] = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX[1])
			m_fDragBarX[1] = m_fDragLineMaxX;

		m_fSoundRatio[1] = (m_fDragBarX[1] - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fVolume[1] = m_fSoundRatio[1];
		Print_OptionText();
		if (true == m_bCheckVolume[1])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), m_fVolume[1]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("BGM"), 0.f);
	}
	else if (m_bHolding[2])
	{
		if ((m_fDragLineMinX <= m_fDragBarX[2]) && (m_fDragLineMaxX >= m_fDragBarX[2]))
			m_fDragBarX[2] += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX[2])
			m_fDragBarX[2] = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX[2])
			m_fDragBarX[2] = m_fDragLineMaxX;

		m_fSoundRatio[2] = (m_fDragBarX[2] - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fVolume[2] = m_fSoundRatio[2];
		Print_OptionText();
		if (true == m_bCheckVolume[2])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), m_fVolume[2]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("UI"), 0.f);
	}
	else if (m_bHolding[3])
	{
		if ((m_fDragLineMinX <= m_fDragBarX[3]) && (m_fDragLineMaxX >= m_fDragBarX[3]))
			m_fDragBarX[3] += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX[3])
			m_fDragBarX[3] = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX[3])
			m_fDragBarX[3] = m_fDragLineMaxX;

		m_fSoundRatio[3] = (m_fDragBarX[3] - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fVolume[3] = m_fSoundRatio[3];
		Print_OptionText();
		if (true == m_bCheckVolume[3])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), m_fVolume[3]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Ambient"), 0.f);
	}
	else if (m_bHolding[4])
	{
		if ((m_fDragLineMinX <= m_fDragBarX[4]) && (m_fDragLineMaxX >= m_fDragBarX[4]))
			m_fDragBarX[4] += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX[4])
			m_fDragBarX[4] = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX[4])
			m_fDragBarX[4] = m_fDragLineMaxX;

		m_fSoundRatio[4] = (m_fDragBarX[4] - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fVolume[4] = m_fSoundRatio[4];
		Print_OptionText();
		if (true == m_bCheckVolume[4])
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), m_fVolume[4]);
		else
			CSound_Manager::GetInstance()->Set_ChannelGroupVolume(TEXT("Effect"), 0.f);
	}

	if (KEY_AWAY(KEY::LBTN))
	{
		for (size_t i = 0; i < 5; i++)
		{
			m_bHolding[i] = false;
		}
	}
}

void CUI_Option_Sound::Create_DragBar()
{
	m_rcDragBar[0].left = LONG((m_fDragBarX[0]) - (17.f * 0.8f / 2));
	m_rcDragBar[0].top = LONG((m_fY - 87.f) - (32.f * 0.8f / 2));
	m_rcDragBar[0].right = LONG((m_fDragBarX[0]) + (17.f * 0.8f / 2));
	m_rcDragBar[0].bottom = LONG((m_fY - 87.f) + (32.f * 0.8f / 2));

	for (size_t i = 1; i < 5; i++)
	{
		m_rcDragBar[i].left = LONG((m_fDragBarX[i]) - (17.f * 0.8f / 2));
		m_rcDragBar[i].top = LONG((m_fY - 87.f + ((i + 1) * 38.f)) - (32.f * 0.8f / 2));
		m_rcDragBar[i].right = LONG((m_fDragBarX[i]) + (17.f * 0.8f / 2));
		m_rcDragBar[i].bottom = LONG((m_fY - 87.f + ((i + 1) * 38.f)) + (32.f * 0.8f / 2));
	}
}

void CUI_Option_Sound::Is_Picking_DragBar(POINT pt)
{
	Is_Picking_DragBar_Master(pt);
	Is_Picking_DragBar_BGM(pt);
	Is_Picking_DragBar_UI(pt);
	Is_Picking_DragBar_BACKGROUND(pt);
	Is_Picking_DragBar_EFFECT(pt);
}

void CUI_Option_Sound::Is_Picking_DragBar_Master(POINT pt)
{
	if (PtInRect(&m_rcDragBar[0], pt))
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[0] = true;
		}
	}
}

void CUI_Option_Sound::Is_Picking_DragBar_BGM(POINT pt)
{
	if (PtInRect(&m_rcDragBar[1], pt))
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[1] = true;
		}
	}
}

void CUI_Option_Sound::Is_Picking_DragBar_UI(POINT pt)
{
	if (PtInRect(&m_rcDragBar[2], pt))
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[2] = true;
		}
	}
}

void CUI_Option_Sound::Is_Picking_DragBar_BACKGROUND(POINT pt)
{
	if (PtInRect(&m_rcDragBar[3], pt))
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[3] = true;
		}
	}
}

void CUI_Option_Sound::Is_Picking_DragBar_EFFECT(POINT pt)
{
	if (PtInRect(&m_rcDragBar[4], pt))
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[4] = true;
		}
	}
}

void CUI_Option_Sound::MovingWnd(CUI* pUI, _float fMoveX)
{
	m_fX = pUI->Get_UIDesc().fX;
	m_fY = pUI->Get_UIDesc().fY;

	//m_pTransform_Line
	m_pTransform_Line->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 87.f + 38.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragLineMinX = (m_fX + 62.f) - 119.f;
	m_fDragLineMaxX = (m_fX + 62.f) + 119.f;

	m_fDragBarX[0] = (m_fDragLineMinX + (m_fDragLineSizeX * m_fSoundRatio[0]));
	m_pTransform_CheckBox[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f + (238.f * 0.9f)) - g_iWinSizeX * 0.5f, -(m_fY - 87.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_DragLine[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 87.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_DragBar[0]->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fDragBarX[0] - g_iWinSizeX * 0.5f, -(m_fY - 87.f) + g_iWinSizeY * 0.5f, 0.f));
	
	for (size_t i = 1; i < 5; i++)
	{
		m_fDragBarX[i] = (m_fDragLineMinX + (m_fDragLineSizeX * m_fSoundRatio[i]));
		m_pTransform_CheckBox[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 62.f + (238.f * 0.9f)) - g_iWinSizeX * 0.5f, -(m_fY - 87.f + ((i + 1) * 38.f)) + g_iWinSizeY * 0.5f, 0.f));
		m_pTransform_DragLine[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 87.f + ((i + 1) * 38.f)) + g_iWinSizeY * 0.5f, 0.f));
		m_pTransform_DragBar[i]->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fDragBarX[i] - g_iWinSizeX * 0.5f, -(m_fY - 87.f + ((i + 1) * 38.f)) + g_iWinSizeY * 0.5f, 0.f));
	}

	m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

CUI_Option_Sound* CUI_Option_Sound::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Option_Sound* pInstance = new CUI_Option_Sound(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Option_Sound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Option_Sound::Clone(void* pArg)
{
	CUI_Option_Sound* pInstance = new CUI_Option_Sound(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Option_Sound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Option_Sound::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_OptionCheckBox);
	Safe_Release(m_pTexture_OptionDetailButton);
	Safe_Release(m_pTexture_Line);
	Safe_Release(m_pTexture_OptionDragLine);
	Safe_Release(m_pTexture_ValueTextWnd);
	Safe_Release(m_pTexture_DragBar);

	Safe_Release(m_pTransform_Line);

	for (size_t i = 0; i < 5; i++)
	{
		Safe_Release(m_pTransform_CheckBox[i]);
		Safe_Release(m_pTransform_DragLine[i]);
		Safe_Release(m_pTransform_DragBar[i]);
	}

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
}
