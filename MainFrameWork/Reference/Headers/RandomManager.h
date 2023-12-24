#pragma once
#include "Base.h"

BEGIN(Engine)

class CRandomManager final : public CBase
{
	DECLARE_SINGLETON(CRandomManager)

private:
	CRandomManager();
	virtual ~CRandomManager() = default;

public:
	const _float&	Random_Float(_float fMin, _float fMax);
	const _int&		Random_Int(_int iMin, _int iMax);
	const _bool&	Random_Coin(_float fProbality);

private:
	random_device m_Random;

	_float m_fRandom_float = { 0.f };
	_int   m_iRandom_int = { 0 };
	_bool m_bRandom_Bool = { false };

public:
	virtual void Free() override;
};

END