#pragma once

#include "Engine_Defines.h"

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* 레퍼런스 카운트를 증가시킨다. */
	/* 	unsigned long : 증가시키고 난 이후의 레퍼런스카운트를 리턴한다. */
	unsigned long AddRef();
	
	/* 레퍼런스 카운트를 감소시킨다. or 삭제한다.  */
	/* 	unsigned long : 감소시키고 난 이전의 레퍼런스카운트를 리턴한다. */
	unsigned long Release();

private:
	unsigned long		m_dwRefCnt = 0;

public:
	virtual void Free();

};

