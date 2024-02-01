#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CPartObject;
END

BEGIN(Client)

class CUI_SpeechBubble;

class CMannequin final : public CGameObject
{
public:
	enum STATE { STATE_IDLE, STATE_WALK, STATE_RUN, STATE_JUMP, STATE_END };
	enum PARTTYPE { PART_R, PART_L, PART_END };
	enum MODELTYPE { HEAD, BODY, MODEL_END };

private:
	CMannequin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMannequin(const CMannequin& rhs);
	virtual ~CMannequin() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	CShader*	Get_ShaderCom() { return m_pShaderCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CModel*		Get_ModelCom() { return m_pModelCom; }



public:
	void			Clear_MQ();

	void			Set_ModelCom(CModel* pModel);
	CPartObject*	Set_Part(_uint PartType, CModel* pModel, Matrix LocalMatrix);
	void			Set_ModelPart(_uint iIndex, CModel* pModel);

	void			Set_HairColor(Vec3 rgb1, Vec3 rgb2);

public:
	void			Set_AnimationPlay() { m_bAnimationPlay = !m_bAnimationPlay; }
	void			Set_Move_State(_bool IsMove, _bool IsPatrol);
	void			Set_StartPos(Vec3 vPos) { m_vStartPos = vPos; }

	void			Set_Talk_State(_bool IsTalk, vector<wstring> TalkScript, _float fTalkTime);

	void			Push_MovePos(Vec3 vPos) { m_vecMovePos.push_back(vPos); }
	void			Pop_MovePos() { m_vecMovePos.pop_back(); }
	void			Clear_MovePos() { m_vecMovePos.clear(); m_iMoveCnt = 0; }

private:
	virtual HRESULT Ready_Components() override;
	HRESULT			Ready_PlayerPart();
	HRESULT			Ready_SpeechBuble();

private:
	virtual void	Set_EffectPos() override;

	void			Move(const _float& fTimeDelta);
	void			Move_Patrol(const _float& fTimeDelta);
	void			Talk(const _float& fTimeDelta);

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CGameObject*	m_pPart[PART_END] = { nullptr };
	CModel*			m_pModelPart[MODEL_END] = { nullptr };

private:
	_bool			m_bAnimationPlay = true;

	_bool			m_bRimLight = false;
	_float			m_fRimLightTime = 0.0f;

	std::future<HRESULT>			m_PlayAnimation;

	_int			m_IsHair = { -1 };

	Vec4			m_vHairColor_1;
	Vec4			m_vHairColor_2;

	/* Npc용 변수 */
	_bool			m_IsMove = { false };
	_bool			m_IsMovePatrol = { false };
	_bool			m_bReach = { false };

	_int			m_iMoveCnt = { 0 };
	vector<Vec3>	m_vecMovePos;

	Vec3			m_vStartPos;

	_bool			  m_IsTalk = { false };
	CUI_SpeechBubble* m_pSpeechBuble = nullptr;
	vector<wstring>	  m_vecTalkScript;
	_float			  m_fTalkStartAcc = { 0.f };
	_float			  m_fTalkTime = { 0.f };
	_uint			  m_iCurrTalk = { 0 };
	_bool			  m_IsTalkStart = { false };

public:
	static CMannequin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

