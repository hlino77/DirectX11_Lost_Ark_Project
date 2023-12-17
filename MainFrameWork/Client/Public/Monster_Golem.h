#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END


BEGIN(Client)
class CMonster_Golem : public CMonster
{

protected:
	CMonster_Golem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Golem(const CMonster_Golem& rhs);
	virtual ~CMonster_Golem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
	virtual void Set_SlowMotion(_bool bSlow) override;



protected:
	_float	m_fPositionTimeAcc = 0.f;
protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_BehaviourTree();
public:
	static CMonster_Golem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

