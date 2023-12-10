#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point final : public CVIBuffer
{
private:
	CVIBuffer_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point(const CVIBuffer_Point& rhs);
	virtual ~CVIBuffer_Point() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render_Instance(ID3D11Buffer * pInstanceBuffer, _uint iSize);
	virtual HRESULT Render_Instance(ID3D11Buffer * pInstanceBuffer, _uint iSize, _uint iStride);
public:



public:
	static CVIBuffer_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject * pObject, void* pArg) override;
	virtual void Free() override;
};

END