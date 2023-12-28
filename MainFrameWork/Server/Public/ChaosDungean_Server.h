#pragma once

#include "Server_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
END


BEGIN(Server)

class CGameSession;
class CPlayer_Server;
class CMonster_Server;
class CBoss_Server;

class CChaosDungean_Server final : public CGameObject
{
public:
	typedef struct DungeanDesc
	{
		CHAOSDUNGEANLEVEL eLevel;
		vector<CPlayer_Server*> Players;
	}DUNGEANDESC;


private:
	CChaosDungean_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChaosDungean_Server(const CChaosDungean_Server& rhs);
	virtual ~CChaosDungean_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	virtual HRESULT Ready_Components() override;

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	HRESULT Ready_ChaosDungean(CHAOSDUNGEANLEVEL eLevel);

	void	Spawn_Monster();
	void	Broadcast_Monster(Vec3 vPos, wstring ModelName);
	void	Broadcast_Boss(Vec3 vPos, wstring ModelName);

	void	Send_OpenLevel();
	void	Exit_Dungean();
	HRESULT	Broadcast_PlayerInfo();
private:
	_float m_fStartDelay = 0.0f;

	_float m_fCurrSpawn = 0.0f;
	_float m_fSpawnDelay = 0.0f;

	_float m_fBroadcastTime = 0.0f;

	CHAOSDUNGEANSTATE m_eDungeanState;
	CHAOSDUNGEANLEVEL m_eDungeanLevel;


	_uint m_iMonsterMaxSpawnCount = 0;
	_uint m_iMonsterCount = 0;
	_uint m_iBossCount = 0;

	vector<CMonster_Server*> m_Monsters;
	vector<CBoss_Server*> m_Bosses;
	vector<wstring> m_MonsterSpawnList;
	vector<wstring> m_BossSpawnList;
	vector<CPlayer_Server*> m_Players;
public:
	static CChaosDungean_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

