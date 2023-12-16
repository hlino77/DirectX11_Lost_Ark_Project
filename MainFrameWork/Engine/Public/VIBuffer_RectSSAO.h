#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectSSAO final : public CVIBuffer
{
private:
	CVIBuffer_RectSSAO(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_RectSSAO(const CVIBuffer_RectSSAO& rhs);
	virtual ~CVIBuffer_RectSSAO() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	static CVIBuffer_RectSSAO* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject * pObject, void* pArg);
	virtual void Free();
};

END