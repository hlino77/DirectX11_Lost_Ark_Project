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
class CMonster_Plant : public CMonster
{

protected:
	CMonster_Plant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Plant(const CMonster_Plant& rhs);
	virtual ~CMonster_Plant() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(const _float& fTimeDelta);
	virtual void LateTick(const _float& fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
	virtual void Set_SlowMotion(_bool bSlow) override;


protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_BehaviourTree();
public:
	static CMonster_Plant* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

