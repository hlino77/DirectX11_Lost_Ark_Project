#pragma once

#include "Base.h"

/* 게임내에 사용될 객체들을 만들어내기위한 원형객체들을 보관한다.(원본) */
/* 게임내에 사용될 객체들을 보관한다.(사본) */


BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels, _uint iNumLayerTypes);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	CGameObject* Add_GameObject(_uint iLevelIndex, const _uint iLayerType, const wstring& strPrototypeTag, void* pArg);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg = nullptr);
	
public:
	class CGameObject* Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, const wstring& strObjectTag);
	class CGameObject* Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, _int iObjectID);
	list<CGameObject*>& Find_GameObjects(_uint iLevelIndex, const _uint iLayerType);
	class CGameObject* Find_NearGameObject(_uint iLevelIndex, const _uint iLayerType, CGameObject* pCallObject);
	HRESULT Delete_GameObject(_uint iLevelIndex, const _uint iLayerType, const CGameObject* pGameObject);

	vector<class CLayer*>* Get_Layers(_uint iLayer) { return &m_pLayers[iLayer]; }
public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

private:
	/* 원형객체들을 레벨별로 보관할까?! */
	map<const wstring, class CGameObject*>				m_Prototypes;

private:
	/* 사본객체들을 레벨별로 그룹(CLayer)지어서 보관한다. */
	_uint								m_iNumLevels = { 0 };
	vector<class CLayer*>*				m_pLayers = { nullptr };
	typedef vector<class CLayer*>*		LAYERS;

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, _uint iLayerType);

public:
	virtual void Free() override;
};

END