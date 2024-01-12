#include "AsUtils.h"

bool CAsUtils::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool CAsUtils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

void CAsUtils::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void CAsUtils::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

wstring CAsUtils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

string CAsUtils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

wstring CAsUtils::S2W(const string& strValue)
{
	_int iLength = MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, NULL, 0);

	wchar_t* szString = new wchar_t[iLength];

	MultiByteToWideChar(CP_ACP, 0, strValue.c_str(), -1, szString, iLength);

	wstring szResult = szString;

	Safe_Delete_Array(szString);

	return szResult;
}

string CAsUtils::W2S(const wstring& szValue)
{
	_int iLength = WideCharToMultiByte(CP_ACP, 0, szValue.c_str(), -1, NULL, 0, NULL, NULL);

	char* strString = new char[iLength];

	WideCharToMultiByte(CP_ACP, 0, szValue.c_str(), -1, strString, iLength, NULL, NULL);

	string strResult = strString;

	Safe_Delete_Array(strString);

	return strResult;
}

_float CAsUtils::Lerpf(_float fStart, _float fEnd, _float fTime)
{
	return fStart * (1.0f - fTime) + fEnd * fTime;
}
