#pragma once
#include "ToolBase.h"
#include "Hasher.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CUtils;
class CModel;
class CTransform;
class CPartObject;
class CGameObject;
END

BEGIN(Client)

class CVoidEffect;
class CMannequin;
class CMannequinPart;
class CLevel_Tool;

class CEffect_PcModel : public CToolBase
{
	using Super = CToolBase;

private:
	enum CLASS { SLAYER, GUNSLINGER, DESTROYER, BARD, DOAGA, GOLEM, KING, VALTAN, ESSA, ESWY, ESBT, CLASS_END };
	enum WEAPON { SWORD, HAND, SHOT, LONG, HAMMER, MUSE, BRUSH, KINGSWORD, VALTANAXE, ESSASWORD, ESBTHAMMER, WEAPON_END };
	enum PARTTYPE { PART_R, PART_L, PART_END };

private:
	CEffect_PcModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffect_PcModel() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr)	override;
	virtual HRESULT Tick(const _float& fTimeDelta)		override;
	virtual HRESULT LateTick(const _float& fTimeDelta)	override;
	virtual HRESULT	DebugRender()			override;

public:

private:
	void	Input();

private:
	HRESULT ModelView(const _float& fTimeDelta);

	void	Class(const _float& fTimeDelta);
	void	Weapon(const _float& fTimeDelta);
	void	Animaition(const _float& fTimeDelta);
private:
	
	void	InfoView();

	void	SelectClass(CLASS eClass);
	void	SelectWeapon(PARTTYPE ePart, WEAPON eWeapon);
	void	SelectAnim();

private:
	HRESULT	Ready_PcModels();
	HRESULT Ready_PartsModels();

private:
	CUtils* m_pUtils = nullptr;
	CMannequin* m_pMannequin = nullptr;

	CModel* m_pPcModels[CLASS_END] = { nullptr };
	CModel* m_pWpModels[WEAPON_END] = { nullptr };

	CTransform* m_pTransformCom = { nullptr };

	CGameObject* m_pCurrentPlayer = nullptr;
	CPartObject* m_pCurrentWeapon = nullptr;

private:
	CLASS m_eSelectClass = { CLASS_END };
	CLASS m_ePreSelectClass = { CLASS_END };
	WEAPON m_eSelectWeapon = { WEAPON_END };
	WEAPON m_ePreSelectWeapon = { WEAPON_END };
	

	_bool	m_CheckBoxStates[CLASS_END] = { false };
	_int	m_iLastCheck = -1;

	_int	m_iSelectPart = { PART_END };
	_bool	m_WPCheckBoxStates[WEAPON_END] = { false };
	_int	m_iWPLastCheck = -1;

	_char	m_szAnimationName[MAX_PATH];
	_char	m_szCurrAnimName[MAX_PATH];
	_uint	m_iCurrAnimation = 0;
	_bool	m_bAnimationPlay = true;

	_char	m_szWPAnimationName[MAX_PATH];
	_char	m_szWPCurrAnimName[MAX_PATH];
	_uint	m_iWPCurrAnimation = 0;

	_bool	m_bActiveRoot = { false };

public:
	static class CEffect_PcModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool* pLevel_Tool);
	virtual void Free() override;
};

END