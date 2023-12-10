#pragma once
#include "Base.h"
#include "MurmurHash3.h"
#include "Lock.h"


BEGIN(Engine)

class CCollider;
class CSphereCollider;

union COLLIDER_ID
{
	struct
	{
		_uint Left_id;
		_uint Right_id;
	};
	uint64 ID;
};

class ENGINE_DLL CCollisionManager : public CBase
{
    DECLARE_SINGLETON(CCollisionManager);


public:
	struct IDInfo
	{
		_bool bCollsion = false;
		_bool bCheck = true;
	};
private:
	CCollisionManager();
	virtual ~CCollisionManager() = default;


	USE_LOCK
public:
	void	LateTick_Collision(const _float& fTimeDelta);

	

public:
	HRESULT Reserve_Manager(_uint iNumColTypes);
	//map<uint64, _bool>& GetMapColInfo() { return m_mapColInfo; }
	//void SetMapColInfo(ULONGLONG ID, _bool bCollision) { m_mapColInfo[ID] = bCollision; }
	void	Add_Colider(CSphereCollider* pCollider);
	void	Out_Colider(CSphereCollider* pCollider);
	void	Set_Stop(_bool bStop) { m_bStop = bStop; }
	_bool	Is_Stop() { return m_bStop; }


	void	Reset();
	void	CheckGroup(_uint iLeft, _uint iRight);
private:
	
	
	_bool	IsCollided(CSphereCollider* pLeft, CSphereCollider* pRight);

	void	CheckDynamicCollision(_uint iObjectLeft, _uint iObjectRight, const _float& fTimeDelta);
	_bool	CompareMaxMinZ(_float& fLeftMaxZ, _float& fRightMinZ);

private:
	struct MurmurHash3Hasher
	{
		size_t operator() (const uint64& key) const
		{
			uint64 hashResult[2];
			MurmurHash3_x64_128(&key, sizeof(key), 0, hashResult);
			size_t combinedHash = static_cast<size_t>(hashResult[0]) ^ static_cast<size_t>(hashResult[1]);
			return combinedHash;
		}
	};	// HashFunctionObject (MurmurHash3)

	using CollisionHash = unordered_map<uint64, IDInfo, MurmurHash3Hasher>;
	CollisionHash m_hashColInfo;

	vector<_uint> m_arrCheck;
	vector<_bool> m_arrSorted;


	_uint m_iNumColLayers = 0;

	vector<vector<CSphereCollider*>> m_Colliders;

	_bool m_bStop = false;
public:
	virtual void Free() override;
};

END