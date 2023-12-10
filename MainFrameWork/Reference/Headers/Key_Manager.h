#pragma once
#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)
struct tKeyInfo
{
	KEY_STATE	eState;
	bool		bPrevPushed;
};

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager);

public:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	void	Tick(_float fTimeDelta);




public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	const POINT& GetMousePos() { return m_tMousePos; }

private:
	vector<tKeyInfo> m_vecKey;

	HWND	m_hWnd = nullptr;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	POINT m_tMousePos;
public:
	virtual void Free() override;
};

END
