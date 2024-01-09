#pragma once

#include "Client_Defines.h"
#include "Level.h"



BEGIN(Engine)

class CRenderer;
END



BEGIN(Client)


class CLevel_ChaosLevel3 final : public CLevel
{
private:
	CLevel_ChaosLevel3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_ChaosLevel3() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(const _float& fTimeDelta) override;
	virtual HRESULT LateTick(const _float& fTimeDelta) override;
	virtual HRESULT Render_Debug();
	virtual HRESULT Exit() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Player(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_BackGround(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Monster(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_UI(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Effect(const LAYER_TYPE eLayerType);
	HRESULT Ready_Player_Camera(const LAYER_TYPE eLayerType);

	HRESULT Send_UserInfo();
private:
	void		Send_LevelState(LEVELSTATE eState);
	void		Wait_ServerLevelState(LEVELSTATE eState);



	HRESULT Load_MapData(LEVELID eLevel, const wstring& szFullPath);

	HRESULT Ready_Events();

	HRESULT	Ready_Renderer();


	void			Set_CheckGruop();
	void			Start_Collision();
	void			Start_Damage();
	void			Start_QuadTree();

	void			End_Damage();
	void			End_Picking();
	void			End_Collision();
	void			End_QuadTree();

private:
	thread* m_pCollisionThread = nullptr;
	thread* m_pPickingThread = nullptr;
	thread* m_pDamageThread = nullptr;
	thread* m_pQuadTreeThread = nullptr;

	CRenderer* m_pRendererCom = nullptr;
	//_bool m_bStaticShadow = false;

public:
	static class CLevel_ChaosLevel3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END