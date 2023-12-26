#pragma once

#include "Server_Defines.h"
#include "Monster_Server.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
class CBehaviorTree;
END


BEGIN(Server)
class CBoss_Server : public CMonster_Server
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
	}MODELDESC;


	typedef struct SkillInfoTag
	{
		_float m_fCoolTime;
		_float m_fCurrCoolTime;
		_bool m_bReady;
	}SKILLINFO;



protected:
	CBoss_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Server(const CBoss_Server& rhs);
	virtual ~CBoss_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;



protected:
	virtual HRESULT		Ready_Components();
	virtual	HRESULT		Ready_BehaviourTree();
	void Set_Colliders(_float fTimeDelta);
	HRESULT Ready_Coliders();

public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

