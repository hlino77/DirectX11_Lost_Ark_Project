#pragma once
#include "ToolBase.h"
#include "Hasher.h"
#include "Client_Defines.h"
#include "Npc.h"
#include "Npc_Part.h"

BEGIN(Engine)
class CUtils;
class CModel;
class CTransform;
class CPartObject;
class CGameObject;
END

BEGIN(Client)

class CLevel_Tool_Npc;
class CNpc;

class CNpcTool : public CToolBase
{
	using Super = CToolBase;

private:

private:
	CNpcTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNpcTool() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr)	override;
	virtual HRESULT Tick(const _float& fTimeDelta)		override;
	virtual HRESULT LateTick(const _float& fTimeDelta)	override;
	virtual HRESULT	DebugRender()			override;

public:

private:
	void	Input();
	_bool	Get_CellPos(Vec3& vPos);

private:
	HRESULT ModelView(const _float& fTimeDelta);

	void	Select_Npc(const _float& fTimeDelta);
	void	Start_Pos(const _float& fTimeDelta);
	void	Name(const _float& fTimeDelta);
	void	Type(const _float& fTimeDelta);
	void	Shape(const _float& fTimeDelta);

	void	Edit_Npc(const _float& fTimeDelta);
	void	Transform_Matrix(const _float& fTimeDelta);
	void	Move(const _float& fTimeDelta);
	void	Talk(const _float& fTimeDelta);
	void	Animaition(const _float& fTimeDelta);
	void	Weapon(const _float& fTimeDelta);

	void	Create_Npc(const _float& fTimeDelta);

	void	Save_Npc(const _float& fTimeDelta);
	
	//typedef struct Npc_Desc
	//{
	//	_uint			iNpcType;
	//	wstring			strNpcTag = { TEXT("") };
	//	wstring			strNpcName = { TEXT("") };
	//	Matrix			matStart;

	//	_uint			iNpcShape;

	//	wstring			strNpcMq;
	//	wstring			strNpcHead;
	//	wstring			strNpcBody;

	//	wstring			strIdleAnim = { TEXT("") };
	//	wstring			strActAnim = { TEXT("") };
	//	_float			fChangeAnimTime = { 0.f };

	//	_bool			IsMove = { false };
	//	_float			fMoveLength = { 0.f };

	//	_bool			IsTalk = { false };
	//	_float			fTalkStartTime = { 0.f };
	//	vector<wstring> vecTalks;

	//	_bool			bUseWeaponPart = { false };
	//	wstring			strLeftPart = { TEXT("") };
	//	Vec3			vLeftOffset[(_uint)WEAPON_OFFSET::_END];
	//	wstring			strRightPart = { TEXT("") };
	//	Vec3			vRightOffset[(_uint)WEAPON_OFFSET::_END];

	//}NPCDESC;

private:

	void	InfoView();
	void	SelectAnim();

private:

private:
	CUtils* m_pUtils = nullptr;
	CMannequin* m_pMannequin = nullptr;
	CTransform* m_pTransformCom = { nullptr };

	CGameObject* m_pCurrentPlayer = nullptr;
	CPartObject* m_pCurrentWeapon = nullptr;

	unordered_map<wstring, CModel*> m_mapHead;
	unordered_map<wstring, CModel*> m_mapBody;

private:
	CNpc::NPCDESC	m_NpcCreateDesc;
	vector<CNpc::NPCDESC> m_vecNpcDesc;

	Vec3	m_vStartPos;
	_int	m_iSelectNpType = -1;
	_bool	m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::_END] = { false };
	_int	m_iLastCheck = -1;
	CNpc::NPCSHAPE m_eNpcShape;
	_char	m_szNpcTag[MAX_PATH];
	_char	m_szNpcName[MAX_PATH];

	
	_int	m_iSelectSol = -1;
	_int	m_iSelectSP = -1;

	_int	m_iSelectPart = { (_uint)CNpc::WEAPON_PART::_END };
	_bool	m_Check_NpcPart[(_uint)CNpc::WEAPON_PART::_END] = { false };
	_int	m_iWPLastCheck = -1;

	_char	m_szAnimationName[MAX_PATH];
	_char	m_szCurrAnimName[MAX_PATH];
	_uint	m_iCurrAnimation = 0;
	_bool	m_bAnimationPlay = true;

public:
	static class CNpcTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool_Npc* pLevel_Tool);
	virtual void Free() override;
};

END