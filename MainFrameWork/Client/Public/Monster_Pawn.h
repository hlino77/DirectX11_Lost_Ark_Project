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
class CPartObject;
END


BEGIN(Client)
class CMonster_Pawn : public CMonster
{

protected:
	CMonster_Pawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Pawn(const CMonster_Pawn& rhs);
	virtual ~CMonster_Pawn() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
	virtual void Set_SlowMotion(_bool bSlow) override;


protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_BehaviourTree();
protected:
	CPartObject* m_pSword = nullptr;
	CPartObject* m_pShield = nullptr;
public:
	static CMonster_Pawn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

