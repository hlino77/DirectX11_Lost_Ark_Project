#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
	using Super = CVIBuffer;
private:
	CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	void Update_TrailBuffer(Matrix& TransformMatrix);
	virtual HRESULT Render() override;

public:
	void SetUp_Position(Vec4 vHighPosition, Vec4 vLowPosition);
	void Start_Trail(Matrix TransformMatrix);
	void Stop_Trail();

public:
	void Set_VtxCount(_uint iCount) { m_iVtxCount = iCount; }

private:
	Vec4 m_vHighPosition = { 0.f, 0.5f, 0.f, 1.f };
	Vec4 m_vLowPosition= { 0.f, -0.5f, 0.f, 1.f };

	std::deque<VTXTRAIL> m_TrailVertices;

	_uint m_iVtxCount = 4;
	_uint m_iCatmullRomCount = 10;

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject * pObject, void* pArg);
	virtual void Free();
};

END