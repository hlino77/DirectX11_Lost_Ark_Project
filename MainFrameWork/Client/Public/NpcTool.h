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

class CMannequin;
class CLevel_Tool_Npc;
class CNpc;
class CNpc_Part;

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
	virtual HRESULT	DebugRender()						override;

private:
	void	Input();
	_bool	Get_CellPos(Vec3& vPos);
	void	Pick_Npc();
	void	Set_DebugRender_Npc();

private:
	HRESULT	InfoView(const _float& fTimeDelta);
	/* NPC 가져오기 */
	void	Npc_List();
	HRESULT	Load_Npc();
	HRESULT	Start_Load_Npc(const wstring& strPath);
	void	Delete_Npc();

	HRESULT ModelView(const _float& fTimeDelta);
	/* 셀렉트 */
	void	Select_Npc(const _float& fTimeDelta);
	void	Start_Pos(const _float& fTimeDelta);
	void	Name(const _float& fTimeDelta);
	void	Type(const _float& fTimeDelta);
	void	Shape(const _float& fTimeDelta);
	/* 에딧 */
	void	Edit_Npc(const _float& fTimeDelta);
	void	Transform_Matrix(const _float& fTimeDelta);
	void	Move(const _float& fTimeDelta);
	void	Talk(const _float& fTimeDelta);
	void	Animaition(const _float& fTimeDelta);
	void	Weapon(const _float& fTimeDelta);
	void	LeftWeapon(const _float& fTimeDelta);
	void	RightWeapon(const _float& fTimeDelta);
	void	HeadBody(const _float& fTimeDelta);
	void	Head(const _float& fTimeDelta);
	void	Body(const _float& fTimeDelta);

	/* 생성 및 저장 */
	void	Create_Npc(const _float& fTimeDelta);
	HRESULT	Save_Npc(const _float& fTimeDelta);

	void	Clear_Info();
private:
	HRESULT	Ready_Sort_Part();

private:
	CUtils* m_pUtils = nullptr;
	CMannequin* m_pMannequin = nullptr;
	CTransform* m_pTransformCom = { nullptr };
	CTransform* m_pTransformLeftPartCom = { nullptr };
	CTransform* m_pTransformRIghtPartCom = { nullptr };

	CGameObject* m_pCurrentPlayer = nullptr;
	CPartObject* m_pCurrentWeapon = nullptr;

private:
	CNpc::NPCDESC	m_NpcCreateDesc;
	vector<CNpc::NPCDESC> m_vecNpcDesc;

private:
	vector<CGameObject*>	m_vecNpcs;
	_int					m_iCurNpc;
	_bool					m_bDebugRender = { false };


private: /* 셀럭트 NPC 변수 */
	Vec3	m_vStartPos;
	_int	m_iSelectNpType = -1;
	_bool	m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::_END] = { false };
	_int	m_iLastCheck = -1;
	CNpc::NPCSHAPE m_eNpcShape;
	_char	m_szNpcTag[MAX_PATH];
	_char	m_szNpcName[MAX_PATH];
	wstring	m_strGroup;

	_int	m_iSelectSol = -1;
	_int	m_iSelectSP = -1;

	_bool	m_bSelected = { false };

private: /* NPC 트랜스폼 변수 */
	Vec3	m_vNpcScale;
	Vec3	m_vNpcRot;
	Vec3	m_vNpcPos;
	_bool	m_bTransformChange = { false };

private: /* NPC 이동 변수*/
	_bool	m_IsMove = { false };
	_bool	m_IsMovePatrol = { false };
	Vec3	m_vMovePos;
	vector<Vec3> m_vecMovePos;
	_int	m_iCurMove;
	vector<string> m_vecforMoveList;

private: /* NPC 파츠 변수 */
	unordered_map<wstring, CComponent*> m_mapHead;
	unordered_map<wstring, CComponent*> m_mapBody;
	_char	m_szHeadPartName[MAX_PATH];
	_char	m_szBodyPartName[MAX_PATH];
	_int	m_iCurHeadIndex = { 0 };
	_int	m_iCurBodyIndex = { 0 };
	_int	m_iSelectModelPart = { (_uint)CNpc::PART::_END };


private: /* NPC 무기파츠 변수*/
	unordered_map<wstring, CComponent*> m_mapWp;
	_int	m_iCurWpLeftIndex = { -1 };
	_int	m_iCurWpRightIndex = { -1 };
	_char	m_szLeftWpName[MAX_PATH];
	_char	m_szRightWpName[MAX_PATH];
	Matrix	m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::_END];
	_bool	m_bSetWpLeftPart = { false };
	_bool	m_bSetWpRightPart = { false };

	_int	m_iSelectPart = { (_uint)CNpc::WEAPON_PART::_END };
	_bool	m_Check_NpcPart[(_uint)CNpc::WEAPON_PART::_END] = { false };
	_int	m_iWPLeftLastCheck = -1;
	_int	m_iWPRightLastCheck = -1;

	Vec3	m_vWpLeftScale = { 1.f, 1.f, 1.f };
	Vec3	m_vWpLeftRot;
	Vec3	m_vWpLeftPos;

	Vec3	m_vWpRightScale = { 1.f, 1.f, 1.f };
	Vec3	m_vWpRightRot;
	Vec3	m_vWpRightPos;

private: /* NPC 애니메이션 변수*/
	_char	m_szAnimationName[MAX_PATH];
	_char	m_szCurrAnimName[MAX_PATH];
	_uint	m_iCurrAnimation = 0;
	_bool	m_bAnimationPlay = true;
	_float	m_fDuration = { 0.f };

private: /* Npc 토크버블 변수 */
	_bool			m_IsTalk = { false };
	_float			m_fTalkStartTime = { 0.f };
	_int			m_iTalkSequence = { -1 };
	vector<wstring> m_vecTalks;
	vector<string>	m_vecSelectTalk;

	_char			m_szTalk[MAX_PATH];
	_int			m_iCurTalk;

public:
	static class CNpcTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool_Npc* pLevel_Tool);
	virtual void Free() override;
};

END