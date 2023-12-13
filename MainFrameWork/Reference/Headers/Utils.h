#pragma once

#include "Base.h"

BEGIN(Engine)

class CUtils : public CBase
{
	DECLARE_SINGLETON(CUtils)

private:
	CUtils();
	virtual ~CUtils() = default;

public:
	string wstring_to_string(const wstring& strW);
	wstring string_to_wstring(const string& strS);

public:
	bool StartsWith(string str, string comp);
	bool StartsWith(wstring str, wstring comp);

	void Replace(OUT string& str, string comp, string rep);
	void Replace(OUT wstring& str, wstring comp, wstring rep);

	wstring ToWString(string value);
	string ToString(wstring value);

	void CreateRandomTexture1DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV);

public:
	virtual void	Free();
};

END