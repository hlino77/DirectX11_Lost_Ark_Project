#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"

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

class CUI_SpeechBubble;

class CNpc : public CGameObject
{
public:
	enum class PART { FACE, BODY, _END };
	enum class WEAPON_PART { RIGHT, LEFT, _END };

public:
	typedef struct Npc_Desc
	{
		wstring strNpcName;
		_int	iObjectID;
		_uint	iLayer;
		
		wstring szState;
		wstring szNpcName;

		Vec3	vStartPos;
		_int	iWeaponIndex;
		_uint	iCurrLevel;

	}NPCDESC;

protected:
	CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

public:
	CShader*				Get_ShaderCom() { return m_pShaderCom; }

	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

	void					Store_Part();
	void					UnStore_Part();
	void					Stop_Update_Part(_bool bUpdate);

public:
	void					Set_State(const wstring& szName);
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false, _bool bReverse = false, _bool bUseY = false);
	const	wstring&		Get_State() { return m_pStateMachine->Get_CurState(); }
	const	wstring&		Get_PreState() { return m_pStateMachine->Get_PreState(); }

	void					Go_Straight(_float fSpeed, _float fTimeDelta);
	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void					Set_Picking(_bool bPicking) { m_bPicking = bPicking; }

	void					Body_Collision(CGameObject* pObject);

	const wstring&			Get_NpcName() { return m_szNpcName; }
	void					Set_NpckName(const wstring& szNpcName) { m_szNpcName = szNpcName; }


	const wstring&			Get_VoiceSoundKey() { return m_VoiceSoundKey; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound) { m_VoiceSoundKey = VoiceSound; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound, _float fDelay) { m_VoiceSoundKey = VoiceSound; m_fVoiceSoundDelay = fDelay; }
	_bool					Stop_VoiceSound();

	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

public:
	void					Set_Several_Weapon_RenderState(WEAPON_PART ePart, _bool Is_Render);

	virtual HRESULT			Ready_Coliders() { return S_OK; }

	void					Show_SpeechBuble(const wstring& szChat);

protected:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_Parts() { return S_OK; }
	HRESULT					Ready_SpeechBuble();

	void					CullingObject();

protected:
	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

protected: 

	/* 파츠 */
	CModel*			m_pNpcPartCom[(_uint)PART::_END] = { nullptr };
	CPartObject*	m_pWeaponPart[(_uint)WEAPON_PART::_END] = { nullptr };

	/* 컬링 절두체 */
	BoundingSphere	m_tCullingSphere;

	wstring			m_szNpcName;
	wstring			m_VoiceSoundKey;
	_float			m_fVoiceSoundDelay;

protected:
	/* 플레이어 변수 설정 */
	Vec4	m_vHairColor_1 = { 0.f, 0.f, 0.f, 0.f };
	Vec4	m_vHairColor_2 = { 0.f, 0.f, 0.f, 0.f };
	_int	m_IsHair = { -1 };

	CUI_SpeechBubble* m_pSpeechBuble = nullptr;

public:
	virtual void Free();

};

END

