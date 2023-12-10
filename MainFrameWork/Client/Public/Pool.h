#pragma once

#include "Base.h"
#include "GameObject.h"
#include <queue>

BEGIN(Client)

template <typename T>
class CPool : public CBase
{
public:
	CPool() {}
	~CPool() 
	{ 
		Free();  
		__super::Free(); 
	}

public:

	static T* Get_Obj()
	{
		if (g_objQueue.empty()) return nullptr;

		T* pObj = g_objQueue.front();
		if (pObj == nullptr)
			return nullptr;

		g_objQueue.pop();

		return pObj;
	}

	static _bool Return_Obj(T* _pObj)
	{
		if (_pObj == nullptr)
			return nullptr;
		g_objQueue.push(_pObj);

		return TRUE;
	}

	void Free()
	{
		if (g_objQueue.empty()) 
			return;

		_int iSize = g_objQueue.size();

		for (_int i = 0; i < iSize; ++i)
		{
			T* pObj = g_objQueue.front();
			Safe_Release(pObj);
			g_objQueue.pop();
		}
	}

	static queue<T*>& Get_Que() { return g_objQueue; }

private:
	static queue<T*> g_objQueue;     // 비활성화된 오브젝트들만 갖고있다.
};

template <typename T>
queue<T*> CPool<T>::g_objQueue;

END