#pragma once
#include "Engine_Defines.h"
#include "Base.h"
#include "Lock.h"

BEGIN(Engine)

class CGameObject;
class CQuadTreeNode;
class CSphereCollider;

class ENGINE_DLL CQuadTreeMgr : public CBase
{
	DECLARE_SINGLETON(CQuadTreeMgr);

public:
	CQuadTreeMgr();
	virtual ~CQuadTreeMgr() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Make_QaudTree(Vec3 vPos, Vec3 vScale, _uint iMaxDepth);
	_uint	Add_Node(CQuadTreeNode* pNode);
	HRESULT Reset_QaudTree();
	void	Set_Object_NodeIndex(CGameObject* pObject);
	HRESULT Add_Object(CGameObject* pObject, _uint iIndex);

	void	Tick(_float fTimeDelta);

public: // Get Set
	CQuadTreeNode* Get_SuperNode() {return m_pSuperNode;}


private:
	USE_LOCK

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	CQuadTreeNode* m_pSuperNode = nullptr;
	vector<CQuadTreeNode*> m_Nodes;
public:
	virtual void Free() override;
};

END
