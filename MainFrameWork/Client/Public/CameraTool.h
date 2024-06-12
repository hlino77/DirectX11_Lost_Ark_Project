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
class CSphereCollider;
END

BEGIN(Client)
class CCamera_Free;

class CCameraTool : public CToolBase
{
	using Super = CToolBase;

private:
	
	struct CameraPos
	{
		Vec3 vPos;
		Vec3 vLook;
		_float fSpeed = 1.0f;
	};

	struct CameraScene
	{
		vector<CameraPos> PosList;
	};

private:
	CCameraTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCameraTool() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr)	override;
	virtual HRESULT Tick(_float fTimeDelta)		override;
	virtual HRESULT LateTick(_float fTimeDelta)	override;
	virtual HRESULT	DebugRender()						override;


	void	Update_CameraTool(_float fTimeDelta);
	void	Update_CameraPosTool(_float fTimeDelta);
	void	Update_Tool(_float fTimeDelta);
	void	Update_CutScene(_float fTimeDelta);

	void	Update_SaveLoad();

	void	Save();
	void	Load();
private:
	void	Set_Camera(Vec3 vPos, Vec3 vLook);

	void	Render_DummyCollider();
private:
	CCamera_Free* m_pCamera = nullptr;

	unordered_map<wstring, CameraScene> m_CutList;

	CameraScene* m_pCurrCameraPos = nullptr;
	CameraPos* m_pCurrPos = nullptr;
	_uint m_iCurrPosIndex = 0;

	char m_szText[MAX_PATH];

	char m_szDataText[MAX_PATH];

	_bool m_bTestStart = false;
	wstring m_szCurrTestCut;
	_uint m_iCurrTestPosIndex = 0;

	_float m_fCameraSpeed = 0.0f;
	_float m_fTimeRatio = 0.0f;

	_int m_iSelectedDataIndex = 0;
	wstring m_szSelectedFileName;

	CSphereCollider* m_pDummyCollider;
public:
	static class CCameraTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END