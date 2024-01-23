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

	void Update_TrailVertices(Matrix & TransformMatrix);
	void Update_TrailBuffer();
	virtual HRESULT Render() override;

public:
	void SetUp_Position(Vec3 vHighPosition, Vec3 vLowPosition);
	void Start_Trail(Matrix TransformMatrix);
	void Stop_Trail();

public:
	void Set_VtxCount(_uint iCount) { m_iVtxCount = iCount; m_BufferData.resize(m_iVtxCount); }

private:
	Vec3 m_vHighPosition = { 0.f, 0.5f, 0.f};
	Vec3 m_vLowPosition= { 0.f, -0.5f, 0.f};

	std::deque<VTXTRAIL> m_TrailVertices;
	std::vector<VTXTRAIL> m_BufferData;

	_uint m_iVtxCount = 4;
	_uint m_iCatmullRomCount = 10;

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject * pObject, void* pArg);
	virtual void Free();
};

END