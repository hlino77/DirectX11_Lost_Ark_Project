#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
END


BEGIN(Client)

class CPlayer;

class CItem abstract : public CGameObject
{
public:
	enum class GRADE { WHITE, GREEN, BLUE, PURPLE, ORANGE, RED, IVORY, _END };
	enum class PART { FACE, HELMET, SHOULDER, BODY, ARM, LEG, WEAPON, _END };
	enum class TYPE { EQUIP, CONSUM, _END};

public:
	typedef struct tagChangeStatDesc
	{
		uint64	iHp;

		uint64	iMp;

		_uint	iAtkPower;

		_uint	iCrit;
		_uint	iSpecialization;
		_uint	iDomination;
		_uint	iSwiftness;
		_uint	iEndurance;
		_uint	iExpertise;

	}CHANGESTATDESC;

protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Tick(_float fTimeDelta);
	virtual void				LateTick(_float fTimeDelta);
	virtual HRESULT				Render();
	
public:
	virtual HRESULT				Use_Item(CPlayer* pOwner) PURE;
	virtual HRESULT				Disuse_Item(CPlayer* pOwner, _bool bUseDefault) PURE;

	virtual void				Upgrade_Item();

public:
	CHANGESTATDESC				Get_StatChangeDesc() { return m_tStatChangeDesc; }

	wstring						Get_ItemName() { return m_strItemName; }
	wstring						Get_ItemDescript() { return m_strItemDescript; }

	_uint						Get_ItemGrade() { return m_iItemGrade; }
	_uint						Get_ItemType() { return m_iItemType; }

	_uint						Get_EquipType() { return m_iEquipType; }

	_float						Get_UpgradeGauge() { return m_fUpgaradeGauge; }
	void						Set_UpgradeGauge(_float fGauge) { m_fUpgaradeGauge = fGauge; }
	CTexture*					Get_ItemTexture() { if(nullptr != m_pItemTextureCom)return m_pItemTextureCom; }
protected:
	virtual HRESULT				Ready_Components();

protected:
	class	CTexture*			m_pItemTextureCom = { nullptr };

protected:
	CHANGESTATDESC				m_tStatChangeDesc;

	wstring						m_strItemName;
	wstring						m_strItemDescript;

	_uint						m_iItemGrade = { (_uint)GRADE::_END };
	_uint						m_iItemType = { (_uint)TYPE::_END };

	_uint						m_iEquipType = { (_uint)PART::_END };
	vector<_uint>				m_vecUseEquipSlot;

	_float						m_fUpgaradeGauge = { 0.f };

public:
	virtual void Free();
};

END

