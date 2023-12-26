#pragma once

#include "Client_Defines.h"
#include "Boss.h"
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
class CBoss_Golem : public CBoss
{
protected:
	CBoss_Golem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Golem(const CBoss_Golem& rhs);
	virtual ~CBoss_Golem() = default;

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
public:
	static CBoss_Golem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

