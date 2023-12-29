#pragma once
#include "Base.h"
#include "Effect.h"
#include "hasher.h"

BEGIN(Engine)

class CTransform;
class CGameInstance;

END

BEGIN(Client)

class CEffect_Manager final : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager);

public:
	
public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimedelta);

	HRESULT Effect_Start(wstring strEffectBundle, CTransform* pTransform);

private:
	unordered_map<wstring, vector<wstring>, djb2Hasher> m_hashEffectBundles;

	CGameInstance* m_pGameInstance = nullptr;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};

END
