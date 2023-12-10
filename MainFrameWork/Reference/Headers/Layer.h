#pragma once

#include "Base.h"
#include "Lock.h"

/* 사본 객체들을 리스트에 보관한다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;


	USE_LOCK
public:
	HRESULT Initialize();
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	
	void Clear();
public:
	list<class CGameObject*>& Find_GameObjects() { return m_GameObjects; }
	CGameObject* Find_GameObject(const wstring& strObjectTag);
	CGameObject* Find_GameObject(_int iObjectID);
	HRESULT Delete_GameObject(const CGameObject* pGameObject);
private:
	list<class CGameObject*>	m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END