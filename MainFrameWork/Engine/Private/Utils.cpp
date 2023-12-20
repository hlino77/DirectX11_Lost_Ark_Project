#include "../Public/Utils.h"

IMPLEMENT_SINGLETON(CUtils)

CUtils::CUtils()
{

}

string CUtils::wstring_to_string(const wstring& strW)
{
	return string(strW.begin(), strW.end());
}

wstring CUtils::string_to_wstring(const string& strS)
{
	
	return wstring(strS.begin(), strS.end());
}

bool CUtils::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool CUtils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

void CUtils::Replace(OUT string& str, string comp, string rep)
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

void CUtils::Replace(OUT wstring& str, wstring comp, wstring rep)
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

wstring CUtils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

string CUtils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

void CUtils::CreateRandomTexture1DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV)
{
	random_device		RandomDevice;

	mt19937_64							RandomNumber(RandomDevice());
	uniform_real_distribution<_float>	RandomX(-1.0f, 1.0f);
	uniform_real_distribution<_float>	RandomY(-1.0f, 1.0f);
	uniform_real_distribution<_float>	RandomZ(-1.0f, 1.0f);
	uniform_real_distribution<_float>	RandomW(-1.0f, 1.0f);

	vector<XMFLOAT4> randomValues(1024);

	for (int32 i = 0; i < 1024; ++i)
	{
		randomValues[i].x = RandomX(RandomNumber);
		randomValues[i].y = RandomY(RandomNumber);
		randomValues[i].z = RandomZ(RandomNumber);
		randomValues[i].w = RandomW(RandomNumber);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues.data();
	initData.SysMemPitch = 1024 * sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	//
	// Create the texture.
	//
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ComPtr<ID3D11Texture1D> randomTex;
	if (FAILED(device->CreateTexture1D(&texDesc, &initData, randomTex.GetAddressOf())))
		__debugbreak();

	//
	// Create the resource view.
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	if (FAILED(device->CreateShaderResourceView(randomTex.Get(), &viewDesc, ppSRV)))
		__debugbreak();
}

void CUtils::Free(void)
{
	__super::Free();
}
