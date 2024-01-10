#include "stdafx.h"
#include "Damage_Manager.h"
#include "Client_Defines.h"
#include "UI_DamageFont.h"

IMPLEMENT_SINGLETON(CDamage_Manager);


CDamage_Manager::CDamage_Manager()
{
}

HRESULT CDamage_Manager::Reserve_Manager(HWND hwnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_iFontCount = 70;

	/*for (_uint i = 0; i < 200; ++i)
	{
		CProjectile* pProjectile = dynamic_cast<CProjectile*>(m_pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Projectile"));
		CPool<CProjectile>::Return_Obj(pProjectile);
	}*/

	for (_uint i = 0; i < m_iFontCount; ++i)
	{
		CUI_DamageFont* pDamageFont = CUI_DamageFont::Create(m_pDevice, m_pContext);
		if (pDamageFont)
		{
			pDamageFont->Initialize_DamageFont(i);
			m_DamageFonts.push_back(pDamageFont);
		}
	}

	return S_OK;
}

void CDamage_Manager::Tick(_float fTimedelta)
{
	Update_DamageFonts();

	for (auto& Damage : m_DamageFonts)
	{
		if (Damage->Is_Active())
			Damage->LateTick(fTimedelta);
	}
}

void CDamage_Manager::Reset()
{
	for (auto& Damage : m_DamageFonts)
		Damage->Set_Active(false);

	m_iDamageIndex = 0;

	m_bStop = false;
}

void CDamage_Manager::Update_DamageFonts()
{
	WRITE_LOCK
	for (auto& Damage : m_ReserveList)
	{
		m_DamageFonts[m_iDamageIndex]->Print_DamageFont(Damage.pMonster, Damage.fScale, Damage.fOffsetY, Damage.fLength, Damage.bCritical, Damage.iDamage);

		m_iDamageIndex = (m_iDamageIndex + 1) % m_iFontCount;
	}
	m_ReserveList.clear();
}

void CDamage_Manager::Print_DamageFont(CGameObject* pMonster, _float fTextBoxScale, _float fOffsetY, _float fLength, _bool IsCritical, _uint iDamage)
{
	WRITE_LOCK
	DamageDesc tDamage;
	tDamage.pMonster = pMonster;
	tDamage.fScale = fTextBoxScale;
	tDamage.fOffsetY = fOffsetY;
	tDamage.fLength = fLength;
	tDamage.bCritical = IsCritical;
	tDamage.iDamage = iDamage;
	m_ReserveList.push_back(tDamage);
}


void CDamage_Manager::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

