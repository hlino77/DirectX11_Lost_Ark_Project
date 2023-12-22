#pragma once
#include "Base.h"
#include "Client_Defines.h"

#include "../Public/ImGUI/imgui.h"
#include "../Public/ImGUI/imgui_impl_dx11.h"
#include "../Public/ImGUI/imgui_impl_win32.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

//class CToolMediator;
class CToolBase : public CBase
{
	using Super = CBase;

protected:
	CToolBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CToolBase() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual HRESULT Tick()			PURE;
	virtual HRESULT LateTick()		PURE;
	virtual HRESULT	DebugRender()	PURE;

	void	DeactivatePicking() { m_IsPickingActivated = false; }

	//void	SetMediator(CToolMediator* pMediator);

protected:
	//const _char* s2c(string str);
	const _char* s2c(const string& str);
	void	s2cPushBack(vector<_char*>& vecChar, string str);
	void	s2cPushBack(vector<const _char*>& vecChar, string str);

	void	s2cPushBackRef(vector<_char*>& vecChar, string& str);
	void	s2cPushBackRef(vector<const _char*>& vecChar, string& str);

protected:
	CGameInstance*			m_pGameInstance = nullptr;
	//CToolMediator*			m_pMediator = nullptr;

	_bool					m_IsPickingActivated = false;

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END