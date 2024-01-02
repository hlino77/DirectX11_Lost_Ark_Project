#include "QuadTreeNode.h"
#include "ColliderSphere.h"
#include "PipeLine.h"
#include "QuadTreeMgr.h"

CQuadTreeNode::CQuadTreeNode(_uint iDepth, Vec3 vPos, Vec3 vScale)
	: m_iDepth(iDepth)
{
	m_tBoudingBox = BoundingBox(vPos, vScale);
	m_iIndex = CQuadTreeMgr::GetInstance()->Add_Node(this);
}


HRESULT CQuadTreeNode::Make_Child(_uint iMaxDepth)
{
	if (m_iDepth == iMaxDepth)
		return S_OK;


	for (_int i = -1; i < 2; i += 2)
	{
		for (_int k = -1; k < 2; k += 2)
		{
			Vec3 vScale = m_tBoudingBox.Extents;
			vScale.x *= 0.5f;
			vScale.z *= 0.5f;

			Vec3 vPos = m_tBoudingBox.Center;
			vPos.x += i * vScale.x;
			vPos.z += k * vScale.z;

			_uint iDepth = m_iDepth + 1;

			CQuadTreeNode* pQuadTree = new CQuadTreeNode(iDepth, vPos, vScale);
			m_Childs.push_back(pQuadTree);
		}
	}

	for (auto& Child : m_Childs)
	{
		Child->Make_Child(iMaxDepth);
	}


	return S_OK;
}

void CQuadTreeNode::Set_Object_NodeIndex(CGameObject* pObject)
{

}

HRESULT CQuadTreeNode::Add_Object(CGameObject* pObject)
{	
	m_Objects.push_back(pObject);
	return S_OK;
}

void CQuadTreeNode::Tick(_float fTimeDelta)
{
	const BoundingFrustum& tCamFrustum = CPipeLine::GetInstance()->Get_CamFrustum();

	for (auto& Object : m_Objects)
	{
		Object->Set_RenderMarking(false);
	}

	Set_ObjectRender(tCamFrustum);

	for (auto& Object : m_Objects)
	{
		if (Object->Is_RenderMarking() == true)
			Object->Set_Render(true);
		else
			Object->Set_Render(false);
	}
}

void CQuadTreeNode::Set_ObjectRender(const BoundingFrustum& tFrustum)
{
	ContainmentType eContain = tFrustum.Contains(m_tBoudingBox);

	switch (eContain)
	{
	case ContainmentType::DISJOINT:
		return;
	case ContainmentType::CONTAINS:
	{
		for (auto& Object : m_Objects)
		{
			Object->Set_RenderMarking(true);
		}
		
	}
	return;
	case ContainmentType::INTERSECTS:
	{
		if (m_Childs.empty())
		{
			for (auto& Object : m_Objects)
			{
				Object->Set_RenderMarking(true);
			}
			return;
		}

		for (auto& Child : m_Childs)
		{
			Child->Set_ObjectRender(tFrustum);
		}
	}
	return;
	}

}

void CQuadTreeNode::Render_DeBug()
{
}

CQuadTreeNode::~CQuadTreeNode()
{
}
