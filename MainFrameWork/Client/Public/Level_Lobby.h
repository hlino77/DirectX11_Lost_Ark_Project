#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	CLevel_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Lobby() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(const _float& fTimeDelta) override;
	virtual HRESULT LateTick(const _float& fTimeDelta) override;
	virtual HRESULT Render_Debug();
	virtual HRESULT Exit();
	
private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const LAYER_TYPE eLayerType);
	HRESULT Ready_CameraFree(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Player(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_UI();
	HRESULT Load_MapData(LEVELID eLevel, const wstring& szFullPath);
	HRESULT Ready_Mannequin();

	void	Start_QuadTree();
	void	End_QuadTree();

	void	Select_Player();

private:
	CGameObject*	m_pCamera = { nullptr };

	CGameObject*	m_pPC_Select[(_uint)CHR_CLASS::CLASSEND] = { nullptr };
	CHR_CLASS		m_eSelectClass = { CHR_CLASS::CLASSEND };

	_bool			m_bConnect = false;
	thread*			m_pQuadTreeThread = nullptr;
	Matrix			m_CameraMatrix = {};


	wstring m_strObjectTag = TEXT("");
	_bool m_bConnect = false;
	thread* m_pQuadTreeThread = nullptr;
	Matrix m_CameraMatrix = {};

public:
	static class CLevel_Lobby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END