#pragma once
#include "Base.h"
#include "hasher.h"

BEGIN(Engine)

class CTransform;
class CGameInstance;

END

BEGIN(Client)
class CEffect;

class CEffect_Manager final : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager);

public:
	typedef struct tagEffectPivot
	{
		CTransform* pPivotTransform = nullptr;
		Matrix*		pPivotMatrix = nullptr;
	}EFFECTPIVOTDESC;

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimedelta);

	HRESULT Effect_Start(wstring strEffectBundle, EFFECTPIVOTDESC* pDesc);
	HRESULT Return_Effect(CEffect* pEffect);
	HRESULT Ready_EffectPool(_uint iDefaultSize);

private:
	unordered_map<wstring, vector<wstring>, djb2Hasher> m_hashEffectBundles;
	unordered_map<wstring, queue<CEffect*>, djb2Hasher> m_hashEffectPools;

	CGameInstance* m_pGameInstance = nullptr;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};

END
