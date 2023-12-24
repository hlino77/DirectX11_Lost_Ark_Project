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
class CPartObject;
END


BEGIN(Client)
class CBoss_King : public CBoss
{

protected:
	CBoss_King(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_King(const CBoss_King& rhs);
	virtual ~CBoss_King() = default;

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
	HRESULT Ready_Coliders();
	virtual HRESULT Ready_BehaviourTree();
protected:
	CPartObject* m_pWeapon = nullptr;
public:
	static CBoss_King* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

