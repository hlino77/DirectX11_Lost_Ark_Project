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
class CMonster_Ghoul : public CMonster
{

protected:
	CMonster_Ghoul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Ghoul(const CMonster_Ghoul& rhs);
	virtual ~CMonster_Ghoul() = default;

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


private:
	HRESULT			Ready_Coliders();
	void			Set_Colliders(_float fTimeDelta);
public:
	static CMonster_Ghoul* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

	
};

END

