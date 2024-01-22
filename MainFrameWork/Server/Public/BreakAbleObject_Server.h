#pragma once
#include "Server_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
class CPipeLine;
END


BEGIN(Server)

class CGameSession;

class CBreakAbleObject_Server : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		int iObjectID;
		_uint	iLayer;
		_uint iCurrentLevel;


	}MODELDESC;

private:
	CBreakAbleObject_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBreakAbleObject_Server(const CBreakAbleObject_Server& rhs);
	virtual	~CBreakAbleObject_Server() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	// Collider
	CSphereCollider* Get_StaticCollider(_uint iIndex) { return m_StaticColliders[iIndex]; }
	void Add_Collider();
	void Add_ChildCollider(_uint iIndex);
	void Add_CollidersToManager();

	// NaviCell Info 
	void Add_NaviCellIndex(_uint CellIndex) { m_NaviCellIndex.push_back(CellIndex); }


protected:
	virtual HRESULT Ready_Components() override;


private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;


	vector<class CSphereCollider*> m_StaticColliders;
	vector<_uint>				   m_NaviCellIndex;


public:
	static CBreakAbleObject_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END