#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CGameInstance;
class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void	Tick(const _float & fTimeDelta) {};
	virtual void	LateTick(const _float & fTimeDelta) {};
	virtual HRESULT Render() { return S_OK; }
	virtual void	DebugRender() {};

public:
	CGameObject*	Get_GameObject() { return m_pOwner; }
	CTransform*		Get_TransformCom() const;

protected:
	CGameObject*			m_pOwner = nullptr;

protected:
	CGameInstance*			m_pGameInstance = nullptr;

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	_bool					m_isCloned = { false };

public:
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) = 0;
	virtual void Free() override;
};

END