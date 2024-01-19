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
	enum class PART { FACE, HELMET, SHOULDER, BODY, ARM, LEG, _END };
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

public:
	virtual void Free();
};

END

