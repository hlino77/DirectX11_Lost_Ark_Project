#pragma once
#include "Engine_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CText_Manager final : public CBase
{
	DECLARE_SINGLETON(CText_Manager);

public:
	CText_Manager();
	virtual ~CText_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);


	void	AddFont(const wstring& szTextName, const wstring& szFontPath);
	void	DrawFont(const wstring& szTextName, const wstring& szString, const Vec2& vPosition, const Vec4& vColor, const _float& fRotation, const Vec2& vOrigin, const Vec2& vScale);

	Vec2	MeasureString(const wstring& szTextName, const wstring& szString);


	void	InputText(wstring& szInputText);
private:

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	SpriteBatch* m_pBatch;
	unordered_map<wstring , SpriteFont*> m_Fonts;
public:
	virtual void Free() override;
};

END
