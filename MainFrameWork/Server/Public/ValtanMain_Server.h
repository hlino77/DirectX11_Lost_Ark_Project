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

class CValtanMain_Server final : public CGameObject
{
public:
	typedef struct DungeanDesc
	{
		vector<CPlayer_Server*> Players;
	}DUNGEANDESC;


private:
	CValtanMain_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CValtanMain_Server(const CValtanMain_Server& rhs);
	virtual ~CValtanMain_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	virtual HRESULT Ready_Components() override;

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	HRESULT Ready_Dungean();

	void	Broadcast_Boss(Vec3 vPos, wstring ModelName);

	void	Send_OpenLevel();
	void	Exit_Dungean();
	HRESULT	Broadcast_PlayerInfo();
private:
	_float m_fStartDelay = 0.0f;

	_float m_fBroadcastTime = 0.0f;
	_uint m_iBossCount = 0;
	
	_bool m_bEnd = false;
	_float m_fEndDelay = 0.0f;

	vector<CBoss_Server*> m_Bosses;
	vector<CPlayer_Server*> m_Players;
public:
	static CValtanMain_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

