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

class CPlayer_Select : public CGameObject
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

protected:
	CPlayer_Select(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Select(const CPlayer_Select& rhs);
	virtual ~CPlayer_Select() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

public:
	virtual const _bool&	Is_Selected() { return m_bSelected; }

	virtual _bool			Intersect_Mouse();
	virtual void			Clicked();
	virtual void			Unclicked();
	virtual void			Set_Entrance();

public:
	CShader* Get_ShaderCom() { return m_pShaderCom; }

	void					Set_Camera(class CCamera_Player* pCamera) { m_pCamera = pCamera; }
	class CCamera_Player*	Get_Camera() { return m_pCamera; }

public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bReverse = false);
	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

protected:
	virtual HRESULT			Ready_Components();

protected:
	class CCamera_Player* m_pCamera = nullptr;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer* m_pRendererCom = nullptr;

	wstring m_szNickName;

	/* 장비 모델컴 */
	CModel* m_pModelPartCom[(_uint)PART::_END] = { nullptr };

protected:
	class CGameObject* m_pPart = { nullptr };

	/* 플레이어 변수 설정 */
	Vec4	m_vHairColor_1 = { 0.f, 0.f, 0.f, 0.f };
	Vec4	m_vHairColor_2 = { 0.f, 0.f, 0.f, 0.f };

	Vec3	m_vSelectPos;

	_uint	m_iSelectAnim = { 0 };
	_uint	m_iSelectAnim_Start = { 0 };
	_uint	m_iSelectAnim_Loop = { 0 };
	_uint	m_iSelectAnim_End = { 0 };
	_uint	m_iSelectAnim_Normal = { 0 };
	_bool	m_bSelected = false;

protected:
	class CUI* m_pEntranceButton = { nullptr };
	class CUI* m_pEntranceNameChanger = { nullptr };
	_bool	m_bSetEntrance = { false };

public:
	virtual void Free();

};

END

