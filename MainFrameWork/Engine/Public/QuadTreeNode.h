#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CSphereCollider;

class CQuadTreeNode
{
public:
	CQuadTreeNode(_uint iDepth, Vec3 vPos, Vec3 vScale);
	~CQuadTreeNode();


public:
	HRESULT Make_Child(_uint iMaxDepth);
	_bool Add_Object(CSphereCollider* pCollider);

	void	Tick(_float fTimeDelta);
	void	Set_ObjectRender(const BoundingFrustum& tFrustum);

private:
	vector<CQuadTreeNode*> m_Childs;
	_uint m_iDepth;
	BoundingBox m_tBoudingBox;

	vector<CGameObject*> m_Objects;
};

END