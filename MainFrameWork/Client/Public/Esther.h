#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CPlayer;
class CEsther_Skill;
class CEsther_Cut;
class CEsther_Scene;

class CEsther abstract : public CGameObject
{
public:
	typedef struct tagEstherDesc
	{
		CPlayer* pLeaderPlayer = { nullptr };
	}ESTHERDESC;

public:
	enum class ESTHERTYPE { SA, WY, BT, _END };

protected:
	CEsther(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther(const CEsther& rhs);
	virtual ~CEsther() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Tick(_float fTimeDelta);
	virtual void					LateTick(_float fTimeDelta);
	virtual HRESULT					Render();

public:
	CEsther_Cut*					Get_Esther_Cut() { return m_pEsther_Cut; }
	CEsther_Skill*					Get_Esther_Skill() { return m_pEsther_Skill; }
	CEsther_Scene*					Get_Esther_Scene() { return m_pEsther_Scene; }

	void							Set_LeaderPlayer(CPlayer* pPlayer) { m_pLeaderPlayer = pPlayer; }

public:
	void							Leader_Active_Esther();
	void							NonLeader_Active_Esther();
	void							Check_DeActive_Esther();
	_uint							Get_EstherType() { return m_iEstherType; }


protected:
	virtual HRESULT					Ready_Components();

protected:
	/* Esther 정보 */
	class CEsther_Cut*				m_pEsther_Cut = { nullptr };
	class CEsther_Skill*			m_pEsther_Skill = { nullptr };
	class CEsther_Scene*			m_pEsther_Scene = { nullptr };

	_uint							m_iEstherType = { (_uint)ESTHERTYPE::_END };

	/* 플레이어 정보*/
	CPlayer*						m_pLeaderPlayer = { nullptr };

public:
	virtual void Free();

};

END

