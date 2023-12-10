#pragma once
#include "Engine_Defines.h"
#include "Base.h"
#include "Lock.h"

BEGIN(Engine)

class CGameObject;
class CQuadTreeNode;
class CSphereCollider;

class CQuadTreeMgr : public CBase
{
	DECLARE_SINGLETON(CQuadTreeMgr);

public:
	CQuadTreeMgr();
	virtual ~CQuadTreeMgr() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Make_QaudTree(Vec3 vPos, Vec3 vScale, _uint iMaxDepth);
	HRESULT Reset_QaudTree();
	_bool	Add_Object(CSphereCollider* pCollider);


	void	Tick(_float fTimeDelta);
private:
	USE_LOCK

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	CQuadTreeNode* m_pSuperNode = nullptr;
public:
	virtual void Free() override;
};

END
