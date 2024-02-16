#pragma once
#include "Boss.h"

BEGIN(Engine)
class CPartObject;
END

BEGIN(Client)
class CEffect;

class CBoss_Valtan_RunningGhost :
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
	CBoss_Valtan_RunningGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Valtan_RunningGhost(const CBoss_Valtan_RunningGhost& rhs);
	virtual ~CBoss_Valtan_RunningGhost() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT		Ready_Components();
	virtual HRESULT		Ready_Coliders();
	virtual	HRESULT		Ready_BehaviourTree();

private:
	CModel* m_pModelPartCom[(_uint)PARTS::PARTS_END] = {};

	_bool m_bEffect = false;

	vector<CEffect*> m_Effects;
	vector<CEffect*> m_Particles;

	vector<CEffect*> m_RushEffects;
public:
	static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};


END