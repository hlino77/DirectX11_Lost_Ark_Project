#include "stdafx.h"
#include "UI_Option_Video.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Renderer.h"
#include "Sound_Manager.h"

CUI_Option_Video::CUI_Option_Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_Option_Video::CUI_Option_Video(const CUI& rhs)
	: CUI(rhs)
{
}


HRESULT CUI_Option_Video::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Option_Video::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_VideoOption");

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

	for (size_t i = 0; i < 4; i++)
	{
		m_pTransform_OptionCheckBox[i]->Set_Scale(Vec3(21.f, 23.f, 1.f));
	}
	m_pTransform_OptionCheckBox[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 78.f) - g_iWinSizeX * 0.5f, -(m_fY - 105.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionCheckBox[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 184.f) - g_iWinSizeX * 0.5f, -(m_fY - 105.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionCheckBox[2]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX -  78.f) - g_iWinSizeX * 0.5f, -(m_fY - 64.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionCheckBox[3]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 184.f) - g_iWinSizeX * 0.5f, -(m_fY - 64.f) + g_iWinSizeY * 0.5f, 0.f));

	for (size_t i = 0; i < 2; i++)
	{
		m_pTransform_OptionLine[i]->Set_Scale(Vec3(282.f * 1.62f, 7.f, 1.f));
	}
	m_pTransform_OptionLine[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 70.f) - g_iWinSizeX * 0.5f, -(m_fY - 42.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionLine[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 70.f) - g_iWinSizeX * 0.5f, -(m_fY + 62.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragLineSizeX = 138.f * 2.f;
	m_fDragLineMinX = (m_fX + 62.f) - 138.f;
	m_fDragLineMaxX = (m_fX + 62.f) + 138.f;
	for (size_t i = 0; i < 5; i++)
	{
		m_pTransform_OptionDragLine[i]->Set_Scale(Vec3(m_fDragLineSizeX, 7.f, 1.f));
		if(2 > i)
			m_pTransform_OptionDragBar[i]->Set_Scale(Vec3(17.f, 32.f, 1.f));
		else
			m_pTransform_OptionDragBar[i]->Set_Scale(Vec3(17.f * 0.8f, 32.f * 0.8f, 1.f));
	}
	m_pTransform_OptionDragLine[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 14.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 33.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[2]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 93.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[3]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 119.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[4]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 145.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragBarX_IBL = (m_fX + 62.f) - (m_fDragLineSizeX * 0.5f);
	m_pTransform_OptionDragBar[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_IBL) - g_iWinSizeX * 0.5f, -(m_fY - 14.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragBarX_SSR = (m_fX + 62.f) + (m_fDragLineSizeX * 0.5f);
	m_pTransform_OptionDragBar[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_SSR)-g_iWinSizeX * 0.5f, -(m_fY - 33.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragBarX_ScreenTone_Grayscale = (m_fX + 62.f) + (m_fDragLineSizeX * 0.5f);
	m_pTransform_OptionDragBar[2]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_ScreenTone_Grayscale)-g_iWinSizeX * 0.5f, -(m_fY + 93.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragBarX_ScreenTone_Contrast = (m_fX + 62.f) + (m_fDragLineSizeX * 0.5f);
	m_pTransform_OptionDragBar[3]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_ScreenTone_Contrast)-g_iWinSizeX * 0.5f, -(m_fY + 119.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragBarX_ScreenTone_Saturation = (m_fX + 62.f) + (m_fDragLineSizeX * 0.5f);
	m_pTransform_OptionDragBar[4]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_ScreenTone_Saturation)-g_iWinSizeX * 0.5f, -(m_fY + 145.f) + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_TextBox()))
		return E_FAIL;

	m_fRatioX[0] = (m_fDragBarX_IBL - m_fDragLineMinX) / m_fDragLineSizeX;
	m_fRatioX[1] = (m_fDragBarX_SSR - m_fDragLineMinX) / m_fDragLineSizeX;
	m_fRatioX[2] = (m_fDragBarX_ScreenTone_Grayscale - m_fDragLineMinX) / m_fDragLineSizeX;
	m_fRatioX[3] = (m_fDragBarX_ScreenTone_Contrast - m_fDragLineMinX) / m_fDragLineSizeX;
	m_fRatioX[4] = (m_fDragBarX_ScreenTone_Saturation - m_fDragLineMinX) / m_fDragLineSizeX;

	return S_OK;
}

HRESULT CUI_Option_Video::Initialize_TextBox()
{
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	if (FAILED(Ready_TextBox()))
		return E_FAIL;
	m_pOptionTextWnd->Set_Active(false);
	return S_OK;
}

void CUI_Option_Video::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Option_Video::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Option_Video::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_PBR()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SSAO()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Fxaa3_11()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_HBAO()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResourcess_LineT()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IBL()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IBLDrag()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SSR()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SSRDrag()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResourcess_LineB()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ScreenTone_G()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ScreenTone_GBar()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ScreenTone_C()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ScreenTone_CBar()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ScreenTone_S()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ScreenTone_SBar()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pOptionTextWnd->Render();

	return S_OK;
}

void CUI_Option_Video::Print_OptionText()
{
	if (nullptr == m_pOptionTextWnd)
		return;

	m_pOptionTextWnd->Clear_Text();
	m_pOptionTextWnd->Set_Alpha(1.f);

	m_pOptionTextWnd->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("PBR"));
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("PBR"), m_strFont, TEXT("PBR"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) - 110.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));
	
	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("SSAO"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("SSAO"), m_strFont, TEXT("SSAO"), Vec2(((m_fSizeX * 0.5f) + 140.f), ((m_fSizeY * 0.5f) - 110.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));
	
	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("Fxaa"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Fxaa"), m_strFont, TEXT("Fxaa"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) - 64.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("HBAO+"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("HBAO"), m_strFont, TEXT("HBAO+"), Vec2(((m_fSizeX * 0.5f) + 144.f), ((m_fSizeY * 0.5f) - 64.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("IBL"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("IBL"), m_strFont, TEXT("IBL"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) - 14.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(m_iIndex_IBL));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("IBL_Index"), m_strFont, to_wstring(m_iIndex_IBL), Vec2(((m_fSizeX * 0.5f) + 220.f), ((m_fSizeY * 0.5f))), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("SSR"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("SSR"), m_strFont, TEXT("SSR"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) + 33.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring(m_iIndex_SSR));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("SSR_Index"), m_strFont, to_wstring(m_iIndex_SSR), Vec2(((m_fSizeX * 0.5f) + 220.f), ((m_fSizeY * 0.5f) + 37.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

//screentone 
	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("½ºÅ©¸°Åæ"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("ScreenTone"), m_strFont, TEXT("½ºÅ©¸°Åæ"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) + 75.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("È¸»öÁ¶"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Grayscale"), m_strFont, TEXT("È¸»öÁ¶"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) + 93.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring((_uint)m_fScreenTone_Grayscale));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Grayscale_Value"), m_strFont, to_wstring((_uint)m_fScreenTone_Grayscale), Vec2(((m_fSizeX * 0.5f) + 220.f), ((m_fSizeY * 0.5f) + 107.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("´ëºñ"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Contrast"), m_strFont, TEXT("´ëºñ"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) + 119.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring((_uint)m_fScreenTone_Contrast));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Contrast_Value"), m_strFont, to_wstring((_uint)m_fScreenTone_Contrast), Vec2(((m_fSizeX * 0.5f) + 220.f), ((m_fSizeY * 0.5f) + 133.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", TEXT("Ã¤µµ"));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Saturation"), m_strFont, TEXT("Ã¤µµ"), Vec2(((m_fSizeX * 0.5f) - 140.f), ((m_fSizeY * 0.5f) + 145.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.85f, 0.85f, 0.82f, 1.0f));

	vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", to_wstring((_uint)m_fScreenTone_Saturation));
	vOrigin = vMeasure * 0.5f;
	m_pOptionTextWnd->Set_Text(m_strWndTag + TEXT("Saturation_Value"), m_strFont, to_wstring((_uint)m_fScreenTone_Saturation), Vec2(((m_fSizeX * 0.5f) + 220.f), ((m_fSizeY * 0.5f) + 159.f)), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.37f, 0.37f, 0.37f, 1.0f));

}

void CUI_Option_Video::Is_VideoOptionMode(_uint iMode)
{
	if (0 == iMode)
	{
		Print_OptionText();
		if(nullptr != m_pOptionTextWnd)
			m_pOptionTextWnd->Set_Active(true);
		m_bActive = true;
	}
	if (1 == iMode)
	{
		if (nullptr != m_pOptionTextWnd)
			m_pOptionTextWnd->Set_Active(false);
		m_bActive = false;
	}
}

void CUI_Option_Video::Update_OptionVideo(_float fTimeDelta, class CUI* pUI, POINT pt)
{
	MovingWnd(pUI, pt.x);
	Create_CheckButton();
	Create_DragBar();
	Is_Picking_CheckButton(pt);
	Is_Picking_DragBar(pt);
	Update_DragBar();
}

void CUI_Option_Video::Set_Active_Option(_bool bOption)
{
	if (!bOption)
		return;

	m_bPre_PBR = m_bOption_PBR;
	m_bPre_SSAO = m_bOption_SSAO;
	m_bPre_Fxaa3_11 = m_bOption_Fxaa3_11;
	m_bPre_HBAO = m_bOption_HBAO;
	m_fPreX_IBL = m_fDragBarX_IBL;
	m_fPreX_SSR = m_fDragBarX_SSR;
	m_fPreX_ScreenTone_Grayscale = m_fDragBarX_ScreenTone_Grayscale;
	m_fPreX_ScreenTone_Contrast = m_fDragBarX_ScreenTone_Contrast;
	m_fPreX_ScreenTone_Saturation = m_fDragBarX_ScreenTone_Saturation;
	for (size_t i = 0; i < 5; i++)
		m_fPreRatioX[i] = m_fRatioX[i];

	m_iIndex_IBL = (_uint)(22.f * m_fRatioX[0]);
	m_iIndex_SSR = (_uint)(4.f *  m_fRatioX[1]);
	m_fScreenTone_Grayscale = (_uint)(255.f * m_fRatioX[2]);
	m_fScreenTone_Contrast = (_uint)(255.f * m_fRatioX[3]);
	m_fScreenTone_Saturation = (_uint)(255.f * m_fRatioX[4]);
	CRenderer::Set_IBLTexture(m_iIndex_IBL);
	CRenderer::Set_PBR_Switch(m_bOption_PBR);
	CRenderer::Set_SSAO_Switch(m_bOption_SSAO);
	CRenderer::Set_Fxaa_Switch(m_bOption_Fxaa3_11);
	//CRenderer::Render_HBAOPLUS(m_bOption_HBAO);
	CRenderer::Set_GrayScale(m_fRatioX[2]);
	CRenderer::Set_Contrast(m_fRatioX[3]);
	CRenderer::Set_Saturation(m_fRatioX[4]);
}

void CUI_Option_Video::Apply_Option()
{
	m_bPre_PBR = m_bOption_PBR;
	m_bPre_SSAO = m_bOption_SSAO;
	m_bPre_Fxaa3_11 = m_bOption_Fxaa3_11;
	m_bPre_HBAO	 =	m_bOption_HBAO;
	m_fPreX_IBL = m_fDragBarX_IBL;
	m_fPreX_SSR = m_fDragBarX_SSR;
	m_fPreX_ScreenTone_Grayscale = m_fDragBarX_ScreenTone_Grayscale;
	m_fPreX_ScreenTone_Contrast = m_fDragBarX_ScreenTone_Contrast;
	m_fPreX_ScreenTone_Saturation = m_fDragBarX_ScreenTone_Saturation;
	for (size_t i = 0; i < 5; i++)
		m_fPreRatioX[i] = m_fRatioX[i];

	m_iIndex_IBL = (_uint)(22.f * m_fRatioX[0]);
	m_iIndex_SSR = (_uint)(4.f * m_fRatioX[1]);
	m_fScreenTone_Grayscale = (_uint)(255.f * m_fRatioX[2]);
	m_fScreenTone_Contrast = (_uint)(255.f *m_fRatioX[3]);
	m_fScreenTone_Saturation = (_uint)(255.f * m_fRatioX[4]);
	CRenderer::Set_IBLTexture(m_iIndex_IBL);
	CRenderer::Set_PBR_Switch(m_bOption_PBR);
	CRenderer::Set_SSAO_Switch(m_bOption_SSAO);
	CRenderer::Set_Fxaa_Switch(m_bOption_Fxaa3_11);
	//CRenderer::Render_HBAOPLUS(m_bOption_HBAO);
	CRenderer::Set_GrayScale(m_fRatioX[2]);
	CRenderer::Set_Contrast(m_fRatioX[3]);
	CRenderer::Set_Saturation(m_fRatioX[4]);
}

void CUI_Option_Video::Cancle_Option()
{
	m_bOption_PBR = m_bPre_PBR;
	m_bOption_SSAO = m_bPre_SSAO;
	m_bOption_Fxaa3_11 = m_bPre_Fxaa3_11;
	m_bOption_HBAO = m_bPre_HBAO;
	m_fDragBarX_IBL = m_fPreX_IBL;
	m_fDragBarX_SSR = m_fPreX_SSR;
	m_fDragBarX_ScreenTone_Grayscale = m_fPreX_ScreenTone_Grayscale;
	m_fDragBarX_ScreenTone_Contrast = m_fPreX_ScreenTone_Contrast;
	m_fDragBarX_ScreenTone_Saturation = m_fPreX_ScreenTone_Saturation;
	for (size_t i = 0; i < 5; i++)
		m_fRatioX[i] = m_fPreRatioX[i];

	m_iIndex_IBL = (_uint)(22.f * m_fRatioX[0]);
	m_iIndex_SSR = (_uint)(4.f * m_fRatioX[1]);
	m_fScreenTone_Grayscale = (_uint)(255.f * m_fRatioX[2]);
	m_fScreenTone_Contrast = (_uint)(255.f * m_fRatioX[3]);
	m_fScreenTone_Saturation = (_uint)(255.f * m_fRatioX[4]);
	CRenderer::Set_IBLTexture(m_iIndex_IBL);
	CRenderer::Set_PBR_Switch(m_bOption_PBR);
	CRenderer::Set_SSAO_Switch(m_bOption_SSAO);
	CRenderer::Set_Fxaa_Switch(m_bOption_Fxaa3_11);
	//CRenderer::Render_HBAOPLUS(m_bOption_HBAO);
	CRenderer::Set_GrayScale(m_fRatioX[2]);
	CRenderer::Set_Contrast(m_fRatioX[3]);
	CRenderer::Set_Saturation(m_fRatioX[4]);
}

void CUI_Option_Video::Update_DragBar()
{
	_float	MouseMoveX;
	MouseMoveX = (_float)CGameInstance::GetInstance()->Get_DIMMoveState(DIMM::DIMM_X);

	if (m_bHolding[0])
	{
		if ((m_fDragLineMinX <= m_fDragBarX_IBL) && (m_fDragLineMaxX >= m_fDragBarX_IBL))
			m_fDragBarX_IBL += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX_IBL)
			m_fDragBarX_IBL = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX_IBL)
			m_fDragBarX_IBL = m_fDragLineMaxX;

		m_fRatioX[0] = (m_fDragBarX_IBL - m_fDragLineMinX) / m_fDragLineSizeX;
		m_iIndex_IBL = (_uint)(22.f * m_fRatioX[0]);
		CRenderer::Set_IBLTexture(m_iIndex_IBL);
		Print_OptionText();
	}

	else if (m_bHolding[1])
	{
		if ((m_fDragLineMinX <= m_fDragBarX_SSR) && (m_fDragLineMaxX >= m_fDragBarX_SSR))
			m_fDragBarX_SSR += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX_SSR)
			m_fDragBarX_SSR = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX_SSR)
			m_fDragBarX_SSR = m_fDragLineMaxX;

		m_fRatioX[1] = (m_fDragBarX_SSR - m_fDragLineMinX) / m_fDragLineSizeX;
		m_iIndex_SSR = (_uint)(4.f * m_fRatioX[1]);

		CRenderer::Set_SSRLevel(m_iIndex_SSR);
		Print_OptionText();
	}

	else if (m_bHolding[2])
	{
		if ((m_fDragLineMinX <= m_fDragBarX_ScreenTone_Grayscale) && (m_fDragLineMaxX >= m_fDragBarX_ScreenTone_Grayscale))
			m_fDragBarX_ScreenTone_Grayscale += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX_ScreenTone_Grayscale)
			m_fDragBarX_ScreenTone_Grayscale = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX_ScreenTone_Grayscale)
			m_fDragBarX_ScreenTone_Grayscale = m_fDragLineMaxX;

		m_fRatioX[2] = (m_fDragBarX_ScreenTone_Grayscale - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fScreenTone_Grayscale = (_uint)(255.f * m_fRatioX[2]);

		CRenderer::Set_GrayScale(m_fRatioX[2]);
		Print_OptionText();
	}
	
	else if (m_bHolding[3])
	{
		if ((m_fDragLineMinX <= m_fDragBarX_ScreenTone_Contrast) && (m_fDragLineMaxX >= m_fDragBarX_ScreenTone_Contrast))
			m_fDragBarX_ScreenTone_Contrast += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX_ScreenTone_Contrast)
			m_fDragBarX_ScreenTone_Contrast = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX_ScreenTone_Contrast)
			m_fDragBarX_ScreenTone_Contrast = m_fDragLineMaxX;

		m_fRatioX[3] = (m_fDragBarX_ScreenTone_Contrast - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fScreenTone_Contrast = (_uint)(255.f * m_fRatioX[3]);

		CRenderer::Set_Contrast(m_fRatioX[3]);
		Print_OptionText();
	}

	else if (m_bHolding[4])
	{
		if ((m_fDragLineMinX <= m_fDragBarX_ScreenTone_Saturation) && (m_fDragLineMaxX >= m_fDragBarX_ScreenTone_Saturation))
			m_fDragBarX_ScreenTone_Saturation += MouseMoveX * 0.5f;
		if (m_fDragLineMinX > m_fDragBarX_ScreenTone_Saturation)
			m_fDragBarX_ScreenTone_Saturation = m_fDragLineMinX;
		if (m_fDragLineMaxX < m_fDragBarX_ScreenTone_Saturation)
			m_fDragBarX_ScreenTone_Saturation = m_fDragLineMaxX;

		m_fRatioX[4] = (m_fDragBarX_ScreenTone_Saturation - m_fDragLineMinX) / m_fDragLineSizeX;
		m_fScreenTone_Saturation = (_uint)(255.f * m_fRatioX[4]);

		CRenderer::Set_Saturation(m_fRatioX[4]);
		Print_OptionText();
	}
	if (KEY_AWAY(KEY::LBTN))
	{
		for (size_t i = 0; i < 5; i++)
		{
			m_bHolding[i] = false;
			m_bSound[i + 3] = false;
			m_bDragSound[i] = false;
		}
	}
}

void CUI_Option_Video::Create_DragBar()
{
	//IBL
	m_rcDragBar[0].left = LONG((m_fDragBarX_IBL) - (17.f / 2));
	m_rcDragBar[0].top = LONG((m_fY - 14.f) - (34.f / 2));
	m_rcDragBar[0].right = LONG((m_fDragBarX_IBL) + (17.f / 2));
	m_rcDragBar[0].bottom = LONG((m_fY - 14.f) + (34.f / 2));
	//SSR
	m_rcDragBar[1].left = LONG((m_fDragBarX_SSR) - (17.f / 2));
	m_rcDragBar[1].top = LONG((m_fY + 33.f) - (34.f / 2));
	m_rcDragBar[1].right = LONG((m_fDragBarX_SSR) + (17.f / 2));
	m_rcDragBar[1].bottom = LONG((m_fY + 33.f) + (34.f / 2));
	//ScreenTone_GrayScale
	m_rcDragBar[2].left = LONG((m_fDragBarX_ScreenTone_Grayscale) - (17.f * 0.8f / 2));
	m_rcDragBar[2].top = LONG((m_fY + 93.f) - (34.f * 0.8f / 2));
	m_rcDragBar[2].right = LONG((m_fDragBarX_ScreenTone_Grayscale) + (17.f * 0.8f / 2));
	m_rcDragBar[2].bottom = LONG((m_fY + 93.f) + (34.f * 0.8f / 2));
	//ScreenTone_Contrast
	m_rcDragBar[3].left = LONG((m_fDragBarX_ScreenTone_Contrast) - (17.f * 0.8f / 2));
	m_rcDragBar[3].top = LONG((m_fY + 119.f) - (34.f * 0.8f / 2));
	m_rcDragBar[3].right = LONG((m_fDragBarX_ScreenTone_Contrast) + (17.f * 0.8f / 2));
	m_rcDragBar[3].bottom = LONG((m_fY + 119.f) + (34.f * 0.8f / 2));
	//ScreenTone_Saturation
	m_rcDragBar[4].left = LONG((m_fDragBarX_ScreenTone_Saturation) - (17.f * 0.8f / 2));
	m_rcDragBar[4].top = LONG((m_fY + 145.f) - (34.f * 0.8f / 2));
	m_rcDragBar[4].right = LONG((m_fDragBarX_ScreenTone_Saturation) + (17.f * 0.8f / 2));
	m_rcDragBar[4].bottom = LONG((m_fY + 145.f) + (34.f * 0.8f / 2));
}

void CUI_Option_Video::Is_Picking_DragBar(POINT pt)
{
	Is_Picking_DragBar_IBL(pt);
	Is_Picking_DragBar_SSR(pt);
	Is_Picking_DragBar_ScreenTone_GBar(pt);
	Is_Picking_DragBar_ScreenTone_CBar(pt);
	Is_Picking_DragBar_ScreenTone_SBar(pt);
}

void CUI_Option_Video::Is_Picking_DragBar_IBL(POINT pt)
{
	if (PtInRect(&m_rcDragBar[0], pt))
	{
		if (!m_bSound[3])
		{
			m_bSound[3] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[0] = true;
			if (!m_bDragSound[0])
			{
				m_bDragSound[0] = true;
				CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			}
		}
	}
	else
		m_bSound[3] = false;
}

void CUI_Option_Video::Is_Picking_DragBar_SSR(POINT pt)
{
	if (PtInRect(&m_rcDragBar[1], pt))
	{
		if (!m_bSound[4])
		{
			m_bSound[4] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[1] = true;
			if (!m_bDragSound[1])
			{
				m_bDragSound[1] = true;
				CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			}
		}
	}
	else
		m_bSound[4] = false;
}

void CUI_Option_Video::Is_Picking_DragBar_ScreenTone_GBar(POINT pt)
{
	if (PtInRect(&m_rcDragBar[2], pt))
	{
		if (!m_bSound[5])
		{
			m_bSound[5] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[2] = true;
			if (!m_bDragSound[2])
			{
				m_bDragSound[2] = true;
				CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			}
		}
	}
	else
		m_bSound[5] = false;
}

void CUI_Option_Video::Is_Picking_DragBar_ScreenTone_CBar(POINT pt)
{
	if (PtInRect(&m_rcDragBar[3], pt))
	{
		if (!m_bSound[6])
		{
			m_bSound[6] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[3] = true;
			if (!m_bDragSound[3])
			{
				m_bDragSound[3] = true;
				CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			}
		}
	}
	else
		m_bSound[6] = false;
}

void CUI_Option_Video::Is_Picking_DragBar_ScreenTone_SBar(POINT pt)
{
	if (PtInRect(&m_rcDragBar[4], pt))
	{
		if (!m_bSound[7])
		{
			m_bSound[7] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (KEY_HOLD(KEY::LBTN))
		{
			m_bHolding[4] = true;
			if (!m_bDragSound[4])
			{
				m_bDragSound[4] = true;
				CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
			}
		}
	}
	else
		m_bSound[7] = false;
}

void CUI_Option_Video::MovingWnd(CUI* pUI, _float fMoveX)
{
	m_fX = pUI->Get_UIDesc().fX;
	m_fY = pUI->Get_UIDesc().fY;

	m_pTransform_OptionCheckBox[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 78.f) - g_iWinSizeX * 0.5f, -(m_fY - 105.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionCheckBox[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 184.f) - g_iWinSizeX * 0.5f, -(m_fY - 105.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionCheckBox[2]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 78.f) - g_iWinSizeX * 0.5f, -(m_fY - 64.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionCheckBox[3]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 184.f) - g_iWinSizeX * 0.5f, -(m_fY - 64.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragLineMinX = (m_fX + 62.f) - 138.f;
	m_fDragLineMaxX = (m_fX + 62.f) + 138.f;

	m_pTransform_OptionLine[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 70.f) - g_iWinSizeX * 0.5f, -(m_fY - 42.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionLine[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 70.f) - g_iWinSizeX * 0.5f, -(m_fY + 62.f) + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_OptionDragLine[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY - 14.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 33.f) + g_iWinSizeY * 0.5f, 0.f));

	//µå·¡±×¹Ù ±æÀÌÀ§Ä¡
	m_fDragBarX_IBL = (m_fDragLineMinX + (m_fDragLineSizeX * m_fRatioX[0]));
	m_pTransform_OptionDragBar[0]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_IBL) - g_iWinSizeX * 0.5f, -(m_fY - 14.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragBarX_SSR = ((m_fX + 62.f) - (m_fDragLineSizeX * 0.5f)) + m_fDragLineSizeX * m_fRatioX[1];
	m_pTransform_OptionDragBar[1]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_SSR)-g_iWinSizeX * 0.5f, -(m_fY + 33.f) + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_OptionDragLine[2]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 93.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[3]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 119.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pTransform_OptionDragLine[4]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 62.f) - g_iWinSizeX * 0.5f, -(m_fY + 145.f) + g_iWinSizeY * 0.5f, 0.f));

	m_fDragBarX_ScreenTone_Grayscale = ((m_fX + 62.f) - (m_fDragLineSizeX * 0.5f)) + m_fDragLineSizeX * m_fRatioX[2];
	m_pTransform_OptionDragBar[2]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_ScreenTone_Grayscale)-g_iWinSizeX * 0.5f, -(m_fY + 93.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragBarX_ScreenTone_Contrast = ((m_fX + 62.f) - (m_fDragLineSizeX * 0.5f)) + m_fDragLineSizeX * m_fRatioX[3];
	m_pTransform_OptionDragBar[3]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_ScreenTone_Contrast)-g_iWinSizeX * 0.5f, -(m_fY + 119.f) + g_iWinSizeY * 0.5f, 0.f));
	m_fDragBarX_ScreenTone_Saturation = ((m_fX + 62.f) - (m_fDragLineSizeX * 0.5f)) + m_fDragLineSizeX * m_fRatioX[4];
	m_pTransform_OptionDragBar[4]->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fDragBarX_ScreenTone_Saturation)-g_iWinSizeX * 0.5f, -(m_fY + 145.f) + g_iWinSizeY * 0.5f, 0.f));

	if (nullptr != m_pOptionTextWnd)
	{
		m_pOptionTextWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	}
}

void CUI_Option_Video::Set_LevelCustomOption()
{
	m_bOption_PBR = m_pRendererCom->Get_PBR_Switch();
	m_bOption_SSAO = m_pRendererCom->Get_SSAO_Switch();
	m_bOption_Fxaa3_11 = m_pRendererCom->Get_Fxaa_Switch();
	//m_bOption_HBAO = !m_bOption_SSAO;
	
	m_iIndex_IBL = m_pRendererCom->Get_IBLTexture();
	m_fRatioX[0] = m_iIndex_IBL / 22.f;

	m_fRatioX[1] = m_pRendererCom->Get_SSRLevel();
	m_iIndex_SSR = (_uint)(4.f * m_fRatioX[1]);

	m_fRatioX[2] = m_pRendererCom->Get_GrayScale();
	m_fRatioX[3] = m_pRendererCom->Get_Contrast();
	m_fRatioX[4] = m_pRendererCom->Get_Saturation();
	m_fScreenTone_Grayscale =  255.f * m_fRatioX[2];
	m_fScreenTone_Contrast = 255.f * m_fRatioX[3];
	m_fScreenTone_Saturation = 255.f * m_fRatioX[4];
}

void CUI_Option_Video::Create_CheckButton()
{
	m_rcCheckButton[0].left = LONG((m_fX - 78.f) - (21.f / 2));
	m_rcCheckButton[0].top = LONG((m_fY - 105.f) - (23.f / 2));
	m_rcCheckButton[0].right = LONG((m_fX - 78.f) + (21.f / 2));
	m_rcCheckButton[0].bottom = LONG((m_fY - 105.f) + (23.f / 2));

	m_rcCheckButton[1].left = LONG((m_fX + 184.f) - (21.f / 2));
	m_rcCheckButton[1].top = LONG((m_fY - 105.f) - (23.f / 2));
	m_rcCheckButton[1].right = LONG((m_fX + 184.f) + (21.f / 2));
	m_rcCheckButton[1].bottom = LONG((m_fY - 105.f) + (23.f / 2));

	m_rcCheckButton[2].left = LONG((m_fX - 78.f) - (21.f / 2));
	m_rcCheckButton[2].top = LONG((m_fY - 64.f) - (23.f / 2));
	m_rcCheckButton[2].right = LONG((m_fX - 78.f) + (21.f / 2));
	m_rcCheckButton[2].bottom = LONG((m_fY - 64.f) + (23.f / 2));

	m_rcCheckButton[3].left = LONG((m_fX + 184.f) - (21.f / 2));
	m_rcCheckButton[3].top = LONG((m_fY - 64.f) - (23.f / 2));
	m_rcCheckButton[3].right = LONG((m_fX + 184.f) + (21.f / 2));
	m_rcCheckButton[3].bottom = LONG((m_fY - 64.f) + (23.f / 2));
}

void CUI_Option_Video::Is_Picking_CheckButton(POINT pt)
{
	Is_Picking_CheckButton_PBR(pt);
	Is_Picking_CheckButton_SSAO(pt);
	Is_Picking_CheckButton_Fxaa3_11(pt);
	Is_Picking_CheckButton_HBAO(pt);
}

void CUI_Option_Video::Is_Picking_CheckButton_PBR(POINT pt)
{
	if (m_bOption_PBR)
		m_iTextureIndex_PBR = 2;
	else
		m_iTextureIndex_PBR = 0;
	if (PtInRect(&m_rcCheckButton[0], pt))
	{
		if (!m_bSound[0])
		{
			m_bSound[0] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (2 != m_iTextureIndex_PBR)
			m_iTextureIndex_PBR = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bOption_PBR = !m_bOption_PBR;
			CRenderer::Set_PBR_Switch(m_bOption_PBR);
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		if (2 != m_iTextureIndex_PBR)
			m_iTextureIndex_PBR = 0;
		m_bSound[0] = false;
	}
}

void CUI_Option_Video::Is_Picking_CheckButton_SSAO(POINT pt)
{
	if (m_bOption_SSAO)
		m_iTextureIndex_SSAO = 2;
	else
		m_iTextureIndex_SSAO = 0;

	if (PtInRect(&m_rcCheckButton[1], pt))
	{
		if (!m_bSound[1])
		{
			m_bSound[1] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (2 != m_iTextureIndex_SSAO)
			m_iTextureIndex_SSAO = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bOption_SSAO = !m_bOption_SSAO;
			m_bOption_HBAO = !m_bOption_SSAO;
			CRenderer::Set_SSAO_Switch(m_bOption_SSAO);
			//CRenderer::Render_HBAOPLUS(m_bOption_HBAO);
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		if (2 != m_iTextureIndex_SSAO)
			m_iTextureIndex_SSAO = 0;
		m_bSound[1] = false;
	}
}

void CUI_Option_Video::Is_Picking_CheckButton_Fxaa3_11(POINT pt)
{
	if (m_bOption_Fxaa3_11)
		m_iTextureIndex_Fxaa3_11 = 2;
	else
		m_iTextureIndex_Fxaa3_11 = 0;

	if (PtInRect(&m_rcCheckButton[2], pt))
	{
		if (!m_bSound[2])
		{
			m_bSound[2] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (2 != m_iTextureIndex_Fxaa3_11)
			m_iTextureIndex_Fxaa3_11 = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bOption_Fxaa3_11 = !m_bOption_Fxaa3_11;
			CRenderer::Set_Fxaa_Switch(m_bOption_Fxaa3_11);
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		if (2 != m_iTextureIndex_Fxaa3_11)
			m_iTextureIndex_Fxaa3_11 = 0;
		m_bSound[2] = false;
	}
}

void CUI_Option_Video::Is_Picking_CheckButton_HBAO(POINT pt)
{
	if (m_bOption_HBAO)
		m_iTextureIndex_HBAO = 2;
	else
		m_iTextureIndex_HBAO = 0;

	if (PtInRect(&m_rcCheckButton[3], pt))
	{
		if (!m_bSound[8])
		{
			m_bSound[8] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		if (2 != m_iTextureIndex_HBAO)
			m_iTextureIndex_HBAO = 1;
		if (KEY_TAP(KEY::LBTN))
		{
			m_bOption_HBAO = !m_bOption_HBAO;
			m_bOption_SSAO = !m_bOption_HBAO;
			//CRenderer::Render_HBAOPLUS(m_bOption_HBAO);
			CRenderer::Set_SSAO_Switch(m_bOption_SSAO);
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
	else
	{
		if (2 != m_iTextureIndex_HBAO)
			m_iTextureIndex_HBAO = 0;
		m_bSound[8] = false;
	}
}

HRESULT CUI_Option_Video::Ready_Components()
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

	//m_pTransform_OptionCheckBox
	for (size_t i = 0; i < 4; i++)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_OptionCheckBox") + to_wstring(i), (CComponent**)&m_pTransform_OptionCheckBox[i])))
			return E_FAIL;
	}
	//m_pTransform_OptionLine
	for (size_t i = 0; i < 3; i++)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_OptionLine") + to_wstring(i), (CComponent**)&m_pTransform_OptionLine[i])))
			return E_FAIL;
	}
	for (size_t i = 0; i < 5; i++)
	{	
		//m_pTransform_OptionDragLine
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_OptionDragLine") + to_wstring(i), (CComponent**)&m_pTransform_OptionDragLine[i])))
			return E_FAIL;
		//m_pTransform_OptionDragBar
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_OptionDragBar") + to_wstring(i), (CComponent**)&m_pTransform_OptionDragBar[i])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Option_Video::Ready_TextBox()
{
	if (nullptr == m_pOptionTextWnd)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = CServerSessionManager::GetInstance()->Get_NickName() + TEXT("s_VideoOption");
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

HRESULT CUI_Option_Video::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	//m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_Option_Video::Bind_ShaderResources_PBR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionCheckBox[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex_PBR);

	return S_OK;
}

HRESULT CUI_Option_Video::Bind_ShaderResources_SSAO()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionCheckBox[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex_SSAO);

	return S_OK;
}

HRESULT CUI_Option_Video::Bind_ShaderResources_Fxaa3_11()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionCheckBox[2]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex_Fxaa3_11);

	return S_OK;
}

HRESULT CUI_Option_Video::Bind_ShaderResources_HBAO()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionCheckBox[3]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_OptionCheckBox->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex_HBAO);

	return S_OK;
}

HRESULT CUI_Option_Video::Bind_ShaderResourcess_LineT()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionLine[0]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_IBL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragLine[0]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_IBLDrag()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragBar[0]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_SSR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragLine[1]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_SSRDrag()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragBar[1]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResourcess_LineB()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionLine[1]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_ScreenTone_G()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragLine[2]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_ScreenTone_GBar()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragBar[2]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_ScreenTone_C()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragLine[3]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_ScreenTone_CBar()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragBar[3]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_ScreenTone_S()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragLine[4]->Get_WorldMatrix())))
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

HRESULT CUI_Option_Video::Bind_ShaderResources_ScreenTone_SBar()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_OptionDragBar[4]->Get_WorldMatrix())))
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

CUI_Option_Video* CUI_Option_Video::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Option_Video* pInstance = new CUI_Option_Video(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Option_Video");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Option_Video::Clone(void* pArg)
{
	CUI_Option_Video* pInstance = new CUI_Option_Video(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Option_Video");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Option_Video::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_OptionCheckBox);
	Safe_Release(m_pTexture_OptionDetailButton);
	Safe_Release(m_pTexture_Line);
	Safe_Release(m_pTexture_OptionDragLine);
	Safe_Release(m_pTexture_ValueTextWnd);
	Safe_Release(m_pTexture_DragBar);

	for (size_t i = 0; i < 4; i++)
	{
		Safe_Release(m_pTransform_OptionCheckBox[i]);
	}

	for (size_t i = 0; i < 2; i++)
	{
		Safe_Release(m_pTransform_OptionLine[i]);
	}

	for (size_t i = 0; i < 5; i++)
	{
		Safe_Release(m_pTransform_OptionDragLine[i]);
		Safe_Release(m_pTransform_OptionDragBar[i]);
	}

	if (nullptr != m_pOptionTextWnd)
		Safe_Release(m_pOptionTextWnd);
}
