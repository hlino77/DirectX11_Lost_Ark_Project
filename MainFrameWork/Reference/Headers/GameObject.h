#pragma once

#include "Base.h"
#include "AsTypes.h"
#include "Lock.h"

/* 클라이엉ㄴ트에서 제작할 다양한 게임오브젝트들의 부모가된다. */

BEGIN(Engine)

class CCollider;

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct CollisionStayDesc
	{
		CCollider* pCollider;
		_uint iColLayer;
	}COLLISIONSTAY;

protected:
	/* 원형을 생성할 때 */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjectTag, _int iObjType);

	/* 사본을 생성할 때 */
	CGameObject(const CGameObject& rhs); /* 복사 생성자. */
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize) { return S_OK; }
	virtual HRESULT Render_ShadowDepth() { return S_OK; }
	virtual HRESULT Render_MakeSRV() { return S_OK; }


	virtual void	Add_InstanceData(vector<Vec4>& BufferData) {};

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) {};
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider * pOther) {};
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider * pOther) {};

	virtual void	Set_Skill(CGameObject* pGameObject) {};
	virtual void	Set_SlowMotion(_bool bSlow) {};

public:
	class CComponent*				Get_Component(const wstring & strComponentTag);

	class CTransform*	Get_TransformCom() { return m_pTransformCom; }
	class CModel*		Get_ModelCom() { return m_pModelCom; }
	class CRigidBody*	Get_RigidBody() { return m_pRigidBody; }
public:
	virtual HRESULT				SetUp_State(Matrix StateMatrix) { return S_OK; }
	void						Set_NoneControlState(const wstring& szName);

public:
	void						Set_TargetPos(Vec3 vTargetPos) { m_vTargetPos.store(vTargetPos); }
	Vec3						Get_TargetPos() { return m_vTargetPos.load(); }

	void						Set_TargetMatrix(Matrix matTargetWorld) { m_matTargetWorld.store(matTargetWorld); }
	Matrix						Get_TargetMatrix() { return m_matTargetWorld.load(); }

	void						Set_ObjectTag(const wstring& strName) { m_strObjectTag = strName; }
	const wstring&				Get_ObjectTag() { return m_strObjectTag; }

	_uint						Get_ObjectID() { return m_iObjectID; }
	_uint						Get_ObjectLayer() { return m_iLayer; }

	class CSphereCollider*		Get_Colider(const _uint& iLayer) { return m_Coliders[iLayer]; }

	_bool						Is_Control() { return m_bControl; }

	_bool						Is_Active() { return m_bActive; }
	void						Set_Active(_bool bActive) { m_bActive = bActive; }

	_bool						Is_Die() { return m_bDie; }
	void						Set_Die(_bool bDie) { m_bDie = bDie; }

	CGameObject*				Get_NearTarget()
	{
		READ_LOCK
		return m_pNearTarget;
	}

	void						Reset_NearTarget()
	{
		WRITE_LOCK
		m_pNearTarget = nullptr;
	}

	void						Set_NearTarget(CGameObject* pObject)
	{
		WRITE_LOCK
		m_pNearTarget = pObject;
	}




	CGameObject*				Get_HitObject() { return m_pHitObject; }
	void						Reset_HitObject() { m_pHitObject = nullptr; }
	void						Set_HitObject(CGameObject* pObject) { m_pHitObject = pObject; }


	void						Set_ModelName(const wstring& strName) { m_szModelName = strName; }
	const wstring&				Get_ModelName() { return m_szModelName; }


	void						Set_Render(_bool bRender) { m_bRender = bRender; }
	_bool						Is_Render() { return m_bRender; }


	_uint						Get_ObjectType() { return m_iObjType; }

	void						Set_Pick(TRIAGLEDESC tTriangle) { m_tTriangle = tTriangle; };
	_bool						Is_Picking() { return m_bPicking; }

	TRIAGLEDESC					Get_Triangle() { return m_tTriangle; }
	void						Reset_Triangle() { ZeroMemory(&m_tTriangle, sizeof(TRIAGLEDESC)); m_tTriangle.fDist = -1.0f; }


	void						Set_CurrCell(_int iIndex) { m_iCurrCell = iIndex; }
	_int						Get_CurrCell() { return m_iCurrCell; }


	void						Add_CollisionStay(_uint iColLayer, CCollider* pCollider);
	void						Delete_CollisionStay(_uint iColLayer, CCollider* pCollider);


	void						Set_Navi(_bool bNavi) { m_bNavi = bNavi; }
	_bool						Is_Navi() { return m_bNavi; }


	void						Set_Gravity(_bool bGravity) { m_bGravity = bGravity; }
	_bool						Is_Gravity() { return m_bGravity; }
	

	void			Set_Invincible(_bool bInvincible) { m_bInvincible = bInvincible; }
	_bool			Is_Invincible() { return m_bInvincible; }


	_int			Get_Hp() { return m_iHp; }
	_uint			Get_MaxHp() { return m_iMaxHp; }
	void			Set_Hp(_int iHp) { m_iHp = iHp; }

protected:
	virtual HRESULT Ready_Components() PURE;
	HRESULT Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Compute_CamZ(Vec4 vWorldPos);

protected:
	USE_LOCK

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	map<wstring, class CComponent*> m_Components;

	class CTransform* m_pTransformCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CStateMachine* m_pStateMachine = nullptr;
	class CRigidBody* m_pRigidBody = nullptr;


	unordered_map<_uint, class CSphereCollider*> m_Coliders;
protected:
	_float				m_fCamDistance = 0.f;
	wstring				m_strObjectTag = L"";
	wstring				m_szModelName = L"";

	_int				m_iObjType = -1;
	_int				m_iObjectID = -1;
	_bool				m_bControl = false;
	_uint				m_iLayer = 0;



	_bool				m_bRender = true;
	_bool				m_bActive = true;
	_bool				m_bDie = false;
	_bool				m_bNavi = true;
	_bool				m_bGravity = false;
	_bool				m_bInvincible = false;

	atomic<Vec3>		m_vTargetPos;
	atomic<Matrix>		m_matTargetWorld;

	CGameObject*		m_pNearTarget = nullptr;
	CGameObject*		m_pHitObject = nullptr;


	//WallPicking
	_bool						m_bPicking = false;
	TRIAGLEDESC					m_tTriangle;


	//Navigation
	_int						m_iCurrCell = -1;
	_int						m_iPrevCell = -1;

	list<COLLISIONSTAY>			m_CollisionList;


	_int							m_iHp;
	_uint							m_iMaxHp;
private:
	CComponent* Find_Component(const wstring & strComponentTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END