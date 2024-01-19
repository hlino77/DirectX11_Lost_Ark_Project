#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"
#include "Model.h"

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
class CParty;
class CUI_SpeechBubble;
class CUI_InGame_NamePlate;
class CEffect;
class CItem;

class CPlayer : public CGameObject
{
public:
	enum class PART { FACE, HELMET, SHOULDER, BODY, ARM, LEG, _END };

public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_bool	bControl;
		_int	iObjectID;
		_uint	iLayer;
		Matrix	matWorld;
		Vec3	vTargetPos;
		wstring szState;
		wstring szNickName;
		_int	iWeaponIndex;
		_uint	iCurrLevel;
	}MODELDESC;

public:
	typedef struct tagStatDesc
	{
		uint64	iMaxHp;
		uint64	iCurHp;

		uint64	iMaxMp;
		uint64	iCurMp;

		_uint	iAtkPower;

		_uint	iCrit;
		_uint	iSpecialization;
		_uint	iDomination;
		_uint	iSwiftness;
		_uint	iEndurance;
		_uint	iExpertise;

	}STATDESC;

public:
	typedef struct SkillInfoTag
	{
		_float m_fCoolTime;
		_float m_fCurrCoolTime;
		_bool m_bReady;
	}SKILLINFO;

protected:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

	virtual void			Set_SlowMotion(_bool bSlow) override;


	virtual	void			OnCollisionEnter_NoneControl(const _uint iColLayer, class CCollider* pOther) PURE;
	virtual	void			OnCollisionExit_NoneControl(const _uint iColLayer, class CCollider* pOther) PURE;
public:
	CShader*				Get_ShaderCom() { return m_pShaderCom; }

	void					Set_Camera(class CCamera_Player* pCamera) { m_pCamera = pCamera; }
	class CCamera_Player*	Get_Camera() { return m_pCamera; }

	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }

	void					Set_AttackMoveSpeed(_float fSpeed) { m_fAttackMoveSpeed = fSpeed; }
	_float					Get_AttackMoveSpeed() { return m_fAttackMoveSpeed; }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

	void					Store_Part();
	void					UnStore_Part();
	void					Stop_Update_Part(_bool bUpdate);

public:
	//Send Packet
	void					Send_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	void					Send_State(const wstring& szName);
	void					Send_ColliderState(const _uint& iLayer);
	void					Send_SlowMotion(_bool bSlow);
	void					Send_Hp();


	void					Set_State(const wstring& szName);
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false ,_bool bReverse = false, _bool bUseY = false);
	const	wstring&		Get_State() { return m_pStateMachine->Get_CurState(); }
	const	wstring&		Get_PreState() { return m_pStateMachine->Get_PreState(); }

	void					Find_NearTarget();
	void					Send_NearTarget();

	void					Go_Straight(_float fSpeed, _float fTimeDelta);
	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);


	void					Follow_ServerPos(_float fDistance, _float fLerpSpeed);

	void					Set_Picking(_bool bPicking) { m_bPicking = bPicking; }

	void					Set_PlayerUp(_float fTimeDelta);

	void					Body_Collision(CGameObject* pObject);
	void					Hit_Attack(CCollider* pCollider);

	void					Set_HitEffect(_bool bHitEffect) { m_bHitEffect = bHitEffect; }


	void					Set_EnemyBodyHit(_bool bEnemyBodyHit) { m_bEnemyBodyHit = bEnemyBodyHit; }
	_bool					Is_EnemyBodyHit() { return m_bEnemyBodyHit; }



	const SKILLINFO&		Get_SkillInfo(_uint iSkill) { return m_SkillInfo[iSkill]; }
	_bool					Get_SkillReady(_uint iSkill) { return m_SkillInfo[iSkill].m_bReady; }
	void					Set_SkillReady(_uint iSkill, _bool bReady) { m_SkillInfo[iSkill].m_bReady = bReady; }
	

	const wstring&			Get_NickName() { return m_szNickName; }
	void					Set_NickName(const wstring& szNickName);


	const wstring&			Get_VoiceSoundKey() { return m_VoiceSoundKey; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound) { m_VoiceSoundKey = VoiceSound; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound, _float fDelay) { m_VoiceSoundKey = VoiceSound; m_fVoiceSoundDelay = fDelay; }
	_bool					Stop_VoiceSound();

	void					Reset_SlowMotion() { m_iSlowMotionCount = 0; Set_SlowMotion(false); }


	virtual _bool					Get_CellPickingPos(Vec3& vPickPos);

	CGameObject*					Get_Parts(const CPartObject::PARTS& ePart) { return m_Parts[ePart]; }
	void							Set_Part_RenderState(CPartObject::PARTS iInex, _bool IsRender) { m_Parts[iInex]->Set_Render(IsRender); }

	void							Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

	CParty*							Get_Party() { return m_pParty; }
	void							Set_Party(CParty* pParty) { m_pParty = pParty; }

	const _bool&					Is_ClickNpc() { return m_IsClickNpc; }

	/* 아이템관련 함수들 */
	CModel*							 Get_ModelPart(_uint iPartIndex) { return m_pModelPartCom[iPartIndex]; }
	void							 Set_ModelPart(_uint iPartIndex, CModel* pModel) {
		m_pModelPartCom[iPartIndex] = pModel;
		if ((_uint)PART::HELMET == iPartIndex)
			m_IsHair = m_pModelPartCom[iPartIndex]->Is_HairTexture();
	}

	STATDESC						Get_PlayerStat_Desc() { return m_tPCStatDesc; }
	void							Set_PlayerStat_Desc(STATDESC tStatDesc) { m_tPCStatDesc = tStatDesc; }

	CModel*							Get_DefaultPart(_uint iPartIndex) { return m_pDefaultModel[iPartIndex]; }
	//

	unordered_map<CItem*, _uint>	Get_Items() { return m_mapItems; }
	CItem*							Get_EquipItem(_uint iPartIndex) { return m_pEqupis[iPartIndex]; }
	void							Set_EquipItem(_uint iPartIndex, CItem* pEquipItem) { m_pEqupis[iPartIndex] = pEquipItem; }

	void							Add_Effect(const wstring& szEffectName, CEffect* pEffect);
	void							Delete_Effect(const wstring& szEffectName);
	CEffect*						Get_Effect(const wstring& szEffectName);
public:
	/* 플레이어 상태 세팅 */
	const _bool&			Is_SuperArmor() { return  m_bInvincible; }
	void					Set_SuperArmorState(_bool IsSuperArmor) { m_bInvincible = IsSuperArmor; }

	const _bool&			Is_SuperiorArmor() { return  m_IsSuperiorArmor; }
	void					Set_SuperiorArmorState(_bool IsSuperiorArmor) { m_IsSuperiorArmor = IsSuperiorArmor; }

	void					Set_Several_Weapon_RenderState(CPartObject::PARTS ePart, _bool Is_Render);

	virtual HRESULT			Ready_PhysxBoneBranch() { return S_OK; };
	virtual HRESULT			Ready_Coliders() { return S_OK; }

	void					Show_SpeechBuble(const wstring& szChat);

	void					Reset_Level();
protected:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_Parts() { return S_OK; }
	HRESULT					Ready_SpeechBuble();
	HRESULT					Ready_NamePlate();

	void					CullingObject();
	void					Update_Skill(SKILLINFO& tSkill, _float fTimeDelta);
	virtual void			Set_EffectPos() override;
	
protected:
	class CCamera_Player*			m_pCamera = nullptr;
	

	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;
	
	_bool							m_bHitEffect = false;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bEnemyBodyHit = false;

	atomic<_int>					m_iSlowMotionCount = 0;


	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer* m_pRendererCom = nullptr;

	/* 파츠 맵 */
	unordered_map<CPartObject::PARTS, CGameObject*>	m_Parts;

	/* 장비 모델컴 */
	CModel* m_pModelPartCom[(_uint)PART::_END] = { nullptr };
	_int	m_IsHair = { -1 };

	/* 컬링 절두체 */
	BoundingSphere m_tCullingSphere;

	vector<SKILLINFO> m_SkillInfo;


	wstring m_szNickName;
	wstring m_VoiceSoundKey;
	_float m_fVoiceSoundDelay;

protected:
	/* 플레이어 변수 설정 */
	STATDESC	m_tPCStatDesc;

	Vec4	m_vHairColor_1 = { 0.f, 0.f, 0.f, 0.f };
	Vec4	m_vHairColor_2 = { 0.f, 0.f, 0.f, 0.f };

	_bool	m_IsSuperiorArmor = false;


	/* 플레이어 아이템 변수 */
	unordered_map<CItem*, _uint> m_mapItems;
	CItem* m_pEqupis[(_uint)PART::_END] = { nullptr };

	CModel* m_pDefaultModel[(_uint)PART::_END] = { nullptr };

	/* NPC 정보 변수 */
	vector<CGameObject*> m_vecNpcs;
	_bool				 m_IsClickNpc = { false };

	/* UI */
	CParty* m_pParty = nullptr;
	CUI_SpeechBubble* m_pSpeechBuble = nullptr;
	CUI_InGame_NamePlate* m_pNamePlate = { nullptr };

	unordered_map<wstring, CEffect*> m_Effects;
public:
	virtual void Free();

};

END

