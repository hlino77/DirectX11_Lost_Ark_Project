#pragma once
#include "Boss.h"

BEGIN(Engine)
class CPartObject;
END

BEGIN(Client)

class CBoss_Valtan_Corpse :
    public CBoss
{
public:
	enum class PARTS
	{
		BODY,
		PART1,
		PART2,
		GHOST,
		PARTS_END

	};
	CBoss_Valtan_Corpse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Valtan_Corpse(const CBoss_Valtan_Corpse& rhs);
	virtual ~CBoss_Valtan_Corpse() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Reserve_WeaponAnimation(wstring strAnimName, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fAnimspeed);
	void Set_Weapon_Render(_bool IsRender);

private:
	virtual HRESULT		Ready_Components();
	virtual HRESULT		Ready_Coliders();
	virtual	HRESULT		Ready_BehaviourTree();

private:
	CModel* m_pModelPartCom[(_uint)PARTS::PARTS_END] = {};
private:
	CPartObject* m_pWeapon = nullptr;

public:
	static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END