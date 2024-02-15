#pragma once

#include "Base.h"
#include "AsTypes.h"
#include "Lock.h"
#include "InstanceData.h"

/* 클라이엉ㄴ트에서 제작할 다양한 게임오브젝트들의 부모가된다. */
BEGIN(Engine)

class CCollider;
class CGameInstance;

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct CollisionStayDesc
	{
		CCollider* pCollider;
		_uint iColLayer;
	}COLLISIONSTAY;

public:
	typedef struct tagStatDesc
	{
		int64	iMaxHp;
		int64	iCurHp;

		int64	iMaxMp;
		int64	iCurMp;

		int64   iDefHP;

		_uint	iAtkPower;

		_uint	iCrit;
		_uint	iSpecialization;
		_uint	iDomination;
		_uint	iSwiftness;
		_uint	iEndurance;
		_uint	iExpertise;

	}STATDESC;

protected:
	/* 원형을 생성할 때 */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjectTag, _int iObjType);

	/* 사본을 생성할 때 */
	CGameObject(const CGameObject& rhs); /* 복사 생성자. */
	virtual ~CGameObject() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Tick(_float fTimeDelta);
	virtual void				LateTick(_float fTimeDelta);
	virtual HRESULT				Render();
	virtual HRESULT				Render_Instance(_uint iSize) { return S_OK; }
	virtual HRESULT				Render_ShadowDepth() { return S_OK; }
	virtual HRESULT				Render_CascadeShadowDepth(_uint iIndex) { return S_OK; }
	virtual HRESULT				Render_Outline() { return S_OK; }
	virtual HRESULT				Render_ShadowDepth_Instance(_uint iSize) { return S_OK; }
	virtual HRESULT				Render_MakeSRV() { return S_OK; }
	virtual HRESULT				Render_Debug() { return S_OK; }



	virtual	void				OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) {};
	virtual	void				OnCollisionStay(const _uint iColLayer, class CCollider* pOther) {};
	virtual	void				OnCollisionExit(const _uint iColLayer, class CCollider* pOther) {};
	virtual void				Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy) {};


	virtual void				Set_Skill(CGameObject * pGameObject) {};
	virtual void				Set_SlowMotion(_bool bSlow) {};

	//ID3D11Buffer*				Get_InstanceBuffer() { return m_pInstaceData->pInstanceBuffer; }
	virtual void				Add_InstanceData(_uint iSize, _uint& iIndex) {};
public:
	class CComponent*			Get_Component(const wstring & strComponentTag);

	class CTransform*			Get_TransformCom()	{ return m_pTransformCom; }
	class CShader*				Get_ShaderCom()		{ return m_pShaderCom; }
	class CRenderer*			Get_RendererCom()	{ return m_pRendererCom; }
	class CModel*				Get_ModelCom()		{ return m_pModelCom; }
	class CRigidBody*			Get_RigidBody()		{ return m_pRigidBody; }

public:
	virtual HRESULT				SetUp_State(Matrix StateMatrix) { return S_OK; }
	void						Set_NoneControlState(const wstring & szName);

public:
	void						Set_TargetPos(Vec3 vTargetPos) { m_vTargetPos.store(vTargetPos); }
	Vec3						Get_TargetPos() { return m_vTargetPos.load(); }

	void						Set_TargetMatrix(Matrix matTargetWorld) { m_matTargetWorld.store(matTargetWorld); }
	Matrix						Get_TargetMatrix() { return m_matTargetWorld.load(); }

	void						Set_ObjectTag(const wstring & strName) { m_strObjectTag = strName; }
	const wstring&				Get_ObjectTag() { return m_strObjectTag; }

	_uint						Get_ObjectID() { return m_iObjectID; }
	_uint						Get_ObjectLayer() { return m_iLayer; }

	class CSphereCollider*		Get_Colider(const _uint & iLayer) { return m_Coliders[iLayer]; }


	_bool						Is_Control() { return m_bControl; }

	_bool						Is_Active() { return m_bActive; }
	void						Set_Active(_bool bActive) { m_bActive = bActive; }

	_bool						Is_Dead() { return m_bDead; }
	virtual void				Set_Dead(_bool bDead) { m_bDead = bDead; }

	void						Set_Instance(_bool bInstace) { m_bInstance = bInstace; }

	/* 멀티스레드 */
	CGameObject* Get_NearTarget()
	{
		READ_LOCK
		return m_pNearTarget;
	}

	void						Reset_NearTarget()
	{
		WRITE_LOCK
		m_pNearTarget = nullptr;
	}

	void						Set_NearTarget(CGameObject * pObject)
	{
		WRITE_LOCK
		m_pNearTarget = pObject;
	}

	CGameObject*				Get_HitObject() { return m_pHitObject; }
	void						Reset_HitObject() { m_pHitObject = nullptr; }
	void						Set_HitObject(CGameObject * pObject) { m_pHitObject = pObject; }

	void						Set_ModelName(const wstring & strName) { m_szModelName = strName; }
	const wstring&				Get_ModelName() { return m_szModelName; }

	void						Set_Render(_bool bRender) { m_bRender = bRender; }
	_bool						Is_Render() { return m_bRender; }
	void						Set_RenderMarking(_bool bRenderMarking) { m_bRenderMarking = bRenderMarking; }
	_bool						Is_RenderMarking() { return m_bRenderMarking; }

	_uint						Get_ObjectType() { return m_iObjType; }

	void						Set_Pick(TRIAGLEDESC tTriangle) { m_tTriangle = tTriangle; };
	_bool						Is_Picking() { return m_bPicking; }

	TRIAGLEDESC					Get_Triangle() { return m_tTriangle; }
	void						Reset_Triangle() { ZeroMemory(&m_tTriangle, sizeof(TRIAGLEDESC)); m_tTriangle.fDist = -1.0f; }

	void						Set_CurrCell(_int iIndex) { m_iCurrCell = iIndex; }
	_int						Get_CurrCell() { return m_iCurrCell; }

	void						Add_CollisionStay(_uint iColLayer, CCollider * pCollider);
	void						Delete_CollisionStay(_uint iColLayer, CCollider * pCollider);

	void						Set_Navi(_bool bNavi) { m_bNavi = bNavi; }
	_bool						Is_Navi() { return m_bNavi; }

	void						Set_Invincible(_bool bInvincible) { m_IsInvincible = bInvincible; }
	_bool						Is_Invincible() { return m_IsInvincible; }

	int64						Get_Hp() { return m_iHp; }
	void						Set_Hp(_int iHp) { m_iHp = iHp; }
	uint64						Get_MaxHp() { return m_iMaxHp; }
	void						Set_MaxHp(_int iMaxHp) { m_iMaxHp = iMaxHp; }
	int64						Get_Mp() { return m_iMp; }
	uint64						Get_MaxMp() { return m_iMaxMp; }
	void						Set_Mp(_int iMp) { m_iMp = iMp; }

	void						Set_ServerState(const wstring& szState) { m_szState = szState; }
	const wstring&				Get_ServerState() { return m_szState; }

	_int						Get_WeaponIndex() { return m_iWeaponIndex; }
	void						Set_WeaponIndex(_int iWeaponIndex) { m_iWeaponIndex = iWeaponIndex; }

	_int						Get_CurrLevel() { return m_iCurrLevel; }
	void						Set_CurrLevel(_uint iIndex) { m_iCurrLevel = iIndex; }

	_int						Get_Layer() { return m_iLayer; }

	_bool						Get_IsMapObject() { return m_IsMapObject; }
	void						Set_IsMapObject() { m_IsMapObject = true; }

	void						Add_QuadTreeIndex(_uint Index) { m_vecQuadTreeIndex.push_back(Index); }
	void						Set_QuadTreeIndices(vector<_uint> Indices) { m_vecQuadTreeIndex = Indices; }
	vector<_uint>				Get_QuadTreeIndices() { return m_vecQuadTreeIndex; }


	void						Set_OriginScale(Vec3 vScale) { m_vOriginScale = vScale; }
	Vec3						Get_OriginScale() {  return m_vOriginScale; }

	Vec3						Get_EffectPos() { return m_vEffectPos; }

	const STATDESC&				Get_PlayerStat_Desc() { return m_tPCStatDesc; }
	void						Set_PlayerStat_Desc(STATDESC tStatDesc) { m_tPCStatDesc = tStatDesc; }

	void						Set_CascadeRender();
	void						Set_CascadeRenderMark(_uint iIndex, _bool bRender) { m_bCascadeRenderMark[iIndex] = bRender; }
	void						Reset_CascadeRenderMark();
protected:
	virtual HRESULT Ready_Components() PURE;
	HRESULT Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Compute_CamZ(Vec4 vWorldPos);


	
	virtual HRESULT				Ready_Proto_InstanceBuffer() { return S_OK; }
	virtual HRESULT				Ready_Instance_For_Render(_uint iSize) { return S_OK; }

	virtual void				Set_EffectPos();
protected:
	USE_LOCK
	CGameInstance*					m_pGameInstance = nullptr;

	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

protected:
	map<wstring, class CComponent*> m_Components;

	class CTransform*				m_pTransformCom = nullptr;
	class CShader*					m_pShaderCom = nullptr;
	class CModel*					m_pModelCom = nullptr;
	class CStateMachine*			m_pStateMachine = nullptr;
	class CRigidBody*				m_pRigidBody = nullptr;
	class CRenderer*				m_pRendererCom = nullptr;


	unordered_map<_uint, class CSphereCollider*> m_Coliders;

protected:
	_float				m_fCamDistance = 0.f;
	wstring				m_strObjectTag = L"";
	wstring				m_szModelName = L"";
	wstring				m_szState = L"";

	_int				m_iObjType = -1;
	_int				m_iObjectID = -1;
	_bool				m_bControl = false;
	_int				m_iLayer = -1;


	_bool				m_bActive = true;
	_bool				m_bDead = false;
	_bool				m_bNavi = true;
	_bool				m_IsInvincible = false;
	atomic<Vec3>		m_vTargetPos;
	atomic<Matrix>		m_matTargetWorld;

	CGameObject*		m_pNearTarget = nullptr;
	CGameObject*		m_pHitObject = nullptr;

	//QuadTree
	_bool				m_bRenderMarking = false;
	_bool				m_bRender = true;

	//WallPicking
	_bool						m_bPicking = false;
	TRIAGLEDESC					m_tTriangle;


	//Navigation
	_int						m_iCurrCell = -1;
	_int						m_iPrevCell = -1;

	vector<COLLISIONSTAY>			m_CollisionList;

	int64							m_iHp;
	uint64							m_iMaxHp;
	int64							m_iMp;
	uint64							m_iMaxMp;

	_int						m_iWeaponIndex = -1;

	//Level
	_int						m_iCurrLevel = -1;

	//Instancing
	_bool						m_bInstance = false;
	shared_ptr<unordered_map<wstring, INSTANCEDATA>>	m_pInstaceData = nullptr;

	// MapObject
	_bool						m_IsMapObject = false;

	// QuadTreeIndex 
	vector<_uint>       m_vecQuadTreeIndex;

	// For.Model
	Vec3	m_vOriginScale;

	// EffectPos
	Vec3							m_vEffectPos;

	// 스탯변수
	STATDESC			m_tPCStatDesc;


	//Cascade
	_bool m_bCascadeRender[3] = { false, false, false };
	_bool m_bCascadeRenderMark[3] = { false, false, false };
private:
	CComponent* Find_Component(const wstring & strComponentTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END