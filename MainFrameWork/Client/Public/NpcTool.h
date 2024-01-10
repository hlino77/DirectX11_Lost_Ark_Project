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

private:
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
	void	HeadBody(const _float& fTimeDelta);
	/* 생성 및 저장 */
	void	Create_Npc(const _float& fTimeDelta);
	void	Save_Npc(const _float& fTimeDelta);

private:
	HRESULT	Ready_Sort_Part();
	void	InfoView();

private:
	CUtils* m_pUtils = nullptr;
	CMannequin* m_pMannequin = nullptr;
	CTransform* m_pTransformCom = { nullptr };
	CTransform* m_pTransformPartCom = { nullptr };

	CGameObject* m_pCurrentPlayer = nullptr;
	CPartObject* m_pCurrentWeapon = nullptr;

private:
	CNpc::NPCDESC	m_NpcCreateDesc;
	vector<CNpc::NPCDESC> m_vecNpcDesc;

private: /* 셀럭트 NPC 변수 */
	Vec3	m_vStartPos;
	_int	m_iSelectNpType = -1;
	_bool	m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::_END] = { false };
	_int	m_iLastCheck = -1;
	CNpc::NPCSHAPE m_eNpcShape;
	_char	m_szNpcTag[MAX_PATH];
	_char	m_szNpcName[MAX_PATH];

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
	Vec3	m_vMovePos;

private: /* NPC 파츠 변수 */
	unordered_map<wstring, CComponent*> m_mapHead;
	unordered_map<wstring, CComponent*> m_mapBody;
	_int	m_iCurHeadIndex = { 0 };
	_int	m_iCurBodyIndex = { 0 };

private: /* NPC 무기파츠 변수*/
	unordered_map<wstring, CComponent*> m_mapWp;
	_int	m_iCurWpIndex = { 0 };
	_char	m_szLeftWpName[MAX_PATH];
	_char	m_szRightWpName[MAX_PATH];
	Matrix	m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::_END];
	_bool	m_bSetWpPart = { false };

	_int	m_iSelectPart = { (_uint)CNpc::WEAPON_PART::_END };
	_bool	m_Check_NpcPart[(_uint)CNpc::WEAPON_PART::_END] = { false };
	_int	m_iWPLastCheck = -1;

	Vec3	m_vWpScale = { 100.f, 100.f, 100.f };
	Vec3	m_vWpRot;
	Vec3	m_vWpPos;

private: /* NPC 애니메이션 변수*/
	_char	m_szAnimationName[MAX_PATH];
	_char	m_szCurrAnimName[MAX_PATH];
	_uint	m_iCurrAnimation = 0;
	_bool	m_bAnimationPlay = true;
	_float	m_fDuration = { 0.f };

private: /* Npc 토크버블 변수 */
	_bool			IsTalk = { false };
	_float			m_fTalkStartTime = { 0.f };
	vector<wstring> m_vecTalks;
	vector<string>	m_vecSelectTalk;

	_char			m_szTalk[MAX_PATH];
	_int			m_iCurTalk;

public:
	static class CNpcTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool_Npc* pLevel_Tool);
	virtual void Free() override;
};

END