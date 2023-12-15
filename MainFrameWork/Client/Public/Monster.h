#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CBehaviorTree;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END


BEGIN(Client)
class CMonster : public CGameObject
{
public:
	typedef struct MODELDESC
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		Vec3	vPos;
	}MODELDESC;

public:


protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();




	virtual void Set_SlowMotion(_bool bSlow) override;



public:
	void					Find_NearTarget();
	CShader*				Get_ShaderCom() { return m_pShaderCom; }
	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }
	_bool					Is_Control() { return m_bControl; }
public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	void					Follow_ServerPos(_float fDistance, _float fLerpSpeed);
	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);
	_float					Get_Target_Distance();
	void					LookAt_Target_Direction(_float fTimeDelta);
	Vec3					Get_Target_Direction();
	void					Set_RandomPosition();
	void					Move_to_RandomPosition(_float fTimeDelta);
	_bool					Is_Close_To_RandomPosition();
	virtual void			Set_Die();
public:
	_bool						Is_Hit() { return m_IsHit; }
	void						Set_Hit(_bool bHit) { m_IsHit = bHit; }

	_bool						Is_Left() { return m_IsLeft; }
	void						Set_Left(_bool IsLeft) { m_IsLeft = IsLeft; }

	_bool						Is_Spawn() { return m_IsSpawn; }
	void						Set_Spawn(_bool IsSpawn) { m_IsSpawn = IsSpawn; }

	void					Set_AnimationSpeed(_float fAnimationSpeed) { m_fAnimationSpeed = fAnimationSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

	void					Set_Action(wstring strAction) { m_strAction = strAction; }
	wstring					Get_Action() { return m_strAction; }

protected:
	virtual HRESULT Ready_Components();
	HRESULT Ready_HP_UI();

protected:
	void					CullingObject();


protected:
	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;
	_float							m_fScanCoolDown = 0.f;
	_bool							m_IsHit = false;
	_bool							m_IsLeft = false;
	_bool							m_IsSpawn = true;
	Vec3							m_vRandomPosition = {};
	wstring							m_strAction = L"";
	unordered_map<wstring, _uint>	m_BoneIndex;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CBehaviorTree* m_pBehaviorTree = nullptr;
	std::future<HRESULT>			m_PlayAnimation;


	//Culling
	BoundingSphere m_tCullingSphere;

	atomic<_int>					m_iSlowMotionCount = 0;
public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

