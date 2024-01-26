#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"
#include "Projectile.h"

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

class CPlayer;
class CEsther;

class CEsther_Skill abstract : public CGameObject
{
public:
	enum class ESTHERTYPE { SA, WY, BT, _END };
	enum class MODEL_PART { FACE, FACE_DEFAULT, FACE_S_ANGRY, FACE_ANGRY, BODY, _END };

protected:
	CEsther_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Skill(const CEsther_Skill& rhs);
	virtual ~CEsther_Skill() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(CPlayer* pLeader, void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();

public:
	void					Set_OwnerEsther(CEsther* pEsther) { m_pOwnerEsther = pEsther; }
	void					Set_LeaderPlayer(CPlayer* pPlayer) { m_pLeaderPlayer = pPlayer; }

	CShader*				Get_ShaderCom() { return m_pShaderCom; }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

public:
	void					Set_DebugRender_State(_bool bRender) { m_bDebugRender = bRender; }

	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false, _bool bReverse = false, _bool bUseY = false);

	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

	_bool					Is_Finished() { return m_IsFinished; }

	virtual void			Reset() PURE;
	virtual void			Ready() PURE;
	virtual void			Check_Finish() PURE;

public:
	virtual HRESULT			Ready_Coliders() { return S_OK; }

protected:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_ModelPart();
	virtual HRESULT			Ready_Parts();
	virtual HRESULT			Ready_Projectile();

	void					CullingObject();

protected:
	_bool							m_IsFinished = { false };

	_bool							m_bDebugRender = { false };

	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

protected:
	/* 플레이어 정보*/
	CPlayer*						m_pLeaderPlayer = { nullptr };

	/* Esther 정보 */
	CEsther*						m_pOwnerEsther = { nullptr };
	_uint							m_iAnimIndex = { 0 };

	vector<PROJECTILE_DESC>			m_vecSkillProjDesces;

	/* 파츠 */
	vector<CGameObject*>			m_vecSkillModel;
	CModel*							m_pModelPartCom[(_uint)MODEL_PART::_END] = { nullptr };
	CPartObject*					m_pPart = { nullptr };

	/* 컬링 절두체 */
	BoundingSphere	m_tCullingSphere;

public:
	virtual void Free();

};

END

