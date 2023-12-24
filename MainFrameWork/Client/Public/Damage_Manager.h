#pragma once
#include "Base.h"
#include "Lock.h"

BEGIN(Engine)
class CTextBox;
END


BEGIN(Client)

class CUI_DamageFont;

class CDamage_Manager final : public CBase
{
	DECLARE_SINGLETON(CDamage_Manager);
public:
	typedef struct DamageDesc
	{
		CGameObject* pMonster;
		_float fScale;
		_float fOffsetY;
		_float fLength;
		_bool bCritical;
		_uint iDamage;
	}DAMAGEDESC;


public:
	CDamage_Manager();
	virtual ~CDamage_Manager() = default;

public:
	HRESULT Reserve_Manager(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimedelta);
	void	Reset();
	void	Update_DamageFonts();

	void	Set_Stop(_bool bStop) { m_bStop = bStop; }
	_bool	Is_Stop() { return m_bStop; }

	void	Print_DamageFont(CGameObject* pMonster, _float TextBoxScale, _float fOffsetY, _float fLength, _bool IsCritical, _uint iDamage);

private:
	vector<CUI_DamageFont*> m_DamageFonts;
	vector<DamageDesc> m_ReserveList;
	atomic<_uint>	m_iDamageIndex = 0;
	_uint m_iFontCount;

	_bool m_bStop = false;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	USE_LOCK
public:
	virtual void Free() override;
};

END
