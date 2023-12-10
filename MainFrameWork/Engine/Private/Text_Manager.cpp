#include "Text_Manager.h"
#include "Engine_Macro.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CText_Manager);


CText_Manager::CText_Manager()
{
}

HRESULT CText_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;


	m_pBatch = new SpriteBatch(m_pContext);

	return S_OK;
}

void CText_Manager::AddFont(const wstring& szTextName, const wstring& szFontPath)
{
	m_Fonts.emplace(szTextName, new SpriteFont(m_pDevice, szFontPath.c_str()));
}

void CText_Manager::DrawFont(const wstring& szTextName, const wstring& szString, const Vec2& vPosition, const Vec4& vColor, const _float& fRotation, const Vec2& vOrigin, const Vec2& vScale)
{
	m_pBatch->Begin();
	m_Fonts[szTextName]->DrawString(m_pBatch, szString.c_str(), vPosition, vColor, fRotation, vOrigin, vScale);
	m_pBatch->End();
}

Vec2 CText_Manager::MeasureString(const wstring& szTextName, const wstring& szString)
{
	return m_Fonts[szTextName]->MeasureString(szString.c_str());
}

void CText_Manager::InputText(wstring& szInputText)
{
	if (KEY_TAP(KEY::Q))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"Q";
		else
			szInputText += L"q";
	}

	if (KEY_TAP(KEY::W))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"W";
		else
			szInputText += L"w";
	}

	if (KEY_TAP(KEY::E))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"E";
		else
			szInputText += L"e";
	}

	if (KEY_TAP(KEY::R))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"R";
		else
			szInputText += L"r";
	}

	if (KEY_TAP(KEY::T))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"T";
		else
			szInputText += L"t";
	}

	if (KEY_TAP(KEY::Y))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"Y";
		else
			szInputText += L"y";
	}

	if (KEY_TAP(KEY::U))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"U";
		else
			szInputText += L"u";
	}

	if (KEY_TAP(KEY::I))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"I";
		else
			szInputText += L"i";
	}

	if (KEY_TAP(KEY::O))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"O";
		else
			szInputText += L"o";
	}

	if (KEY_TAP(KEY::P))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"P";
		else
			szInputText += L"p";
	}

	if (KEY_TAP(KEY::A))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"A";
		else
			szInputText += L"a";
	}

	if (KEY_TAP(KEY::S))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"S";
		else
			szInputText += L"s";
	}

	if (KEY_TAP(KEY::D))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"D";
		else
			szInputText += L"d";
	}

	if (KEY_TAP(KEY::F))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"F";
		else
			szInputText += L"f";
	}

	if (KEY_TAP(KEY::G))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"G";
		else
			szInputText += L"g";
	}

	if (KEY_TAP(KEY::H))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"H";
		else
			szInputText += L"h";
	}

	if (KEY_TAP(KEY::J))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"J";
		else
			szInputText += L"j";
	}

	if (KEY_TAP(KEY::K))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"K";
		else
			szInputText += L"k";
	}

	if (KEY_TAP(KEY::L))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"L";
		else
			szInputText += L"l";
	}

	if (KEY_TAP(KEY::Z))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"Z";
		else
			szInputText += L"z";
	}

	if (KEY_TAP(KEY::X))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"X";
		else
			szInputText += L"x";
	}

	if (KEY_TAP(KEY::C))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"C";
		else
			szInputText += L"c";
	}

	if (KEY_TAP(KEY::V))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"V";
		else
			szInputText += L"v";
	}

	if (KEY_TAP(KEY::B))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"B";
		else
			szInputText += L"b";
	}

	if (KEY_TAP(KEY::N))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"N";
		else
			szInputText += L"n";
	}

	if (KEY_TAP(KEY::M))
	{
		if (KEY_HOLD(KEY::SHIFT))
			szInputText += L"M";
		else
			szInputText += L"m";
	}


	if (KEY_TAP(KEY::BACK))
	{
		if (!szInputText.empty())
		{
			szInputText.erase(szInputText.size() - 1);
		}
	}
}


void CText_Manager::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
