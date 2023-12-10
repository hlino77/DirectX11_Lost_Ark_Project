#pragma once
#include "Engine_Defines.h"


BEGIN(Engine)

/*-------------------
	BaseAllocator
-------------------*/

class ENGINE_DLL BaseAllocator
{
public:
	static void*	Alloc(int32 size);
	static void		Release(void* ptr);
};



/*-------------------
	StompAllocator
-------------------*/

class ENGINE_DLL StompAllocator
{
	enum { PAGE_SIZE = 0x1000 };

public:
	static void* Alloc(int32 size);
	static void		Release(void* ptr);
};

/*-------------------
	PoolAllocator
-------------------*/

class ENGINE_DLL PoolAllocator
{
public:
	static void* Alloc(int32 size);
	static void		Release(void* ptr);
};

/*-------------------
	STL Allocator
-------------------*/

template<typename T>
class ENGINE_DLL StlAllocator
{
public:
	using value_type = T;

	StlAllocator() { }

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(PoolAllocator::Alloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		PoolAllocator::Release(ptr);
	}
};

END