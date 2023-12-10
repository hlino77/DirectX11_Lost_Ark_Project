#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
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
class CMonster : public CGameObject
{
public:
	typedef struct ModelDesc
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
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
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


	virtual void			Set_Die();

	void	Effect_Die();
protected:
	virtual HRESULT Ready_Components();
	HRESULT Ready_HP_UI();

protected:
	void					CullingObject();


protected:
	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;


	unordered_map<wstring, _uint>	m_BoneIndex;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;



	std::future<HRESULT>			m_PlayAnimation;



	//Culling
	BoundingSphere m_tCullingSphere;

	atomic<_int>					m_iSlowMotionCount = 0;
public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

