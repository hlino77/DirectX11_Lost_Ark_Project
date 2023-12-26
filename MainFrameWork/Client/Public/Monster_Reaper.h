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
class CMonster_Reaper : public CMonster
{

protected:
	CMonster_Reaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Reaper(const CMonster_Reaper& rhs);
	virtual ~CMonster_Reaper() = default;

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
protected:
	CPartObject* m_pWeapon = nullptr;
public:
	static CMonster_Reaper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

